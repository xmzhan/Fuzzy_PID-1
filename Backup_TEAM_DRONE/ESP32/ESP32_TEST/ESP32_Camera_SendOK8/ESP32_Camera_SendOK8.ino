#include "Arduino.h"
#include <WiFi.h>
#include <WiFiUdp.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"


#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"

#include "esp_camera.h"

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const char* ssid = "room201";
const char* password =  "room201201";

/*
const char * serverAddress = "192.168.0.23";
const int serverPort = 8000;

const char * okAddress = "192.168.0.23";
const int okPort = 8008;
*/
///*
const char * serverAddress = "192.168.0.21";
const int serverPort = 9000;

const char * okAddress = "192.168.0.21";
const int okPort = 8011;
//*/
WiFiUDP udp;
WiFiUDP okudp;

void data_lost()
{
  if(udp.endData()<0)
  {
    Serial.println("data lost");
    data_lost();
  }
}

void size_lost(size_t filesize)
{
  if(udp.endSize(filesize)<0)
  {
    Serial.println("size lost");
    size_lost(filesize);
  }
}

camera_fb_t * fb = NULL;
FILE* fp = NULL;

static int flag = 0;
static int Reflag = 0;
static int oneflag = 0;

#define BUFSIZE 65507
char message[BUFSIZE];
size_t fsize = 0, fpsize  = 0;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);

  delay(1000);
reboot:
  WiFi.begin(ssid, password);
  delay(2000);
  if(WiFi.status() != WL_CONNECTED)  
  {
    goto reboot;
  }

  SPI.begin(14, 2, 15, 13);
  if(!SD.begin(13)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  
  Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
  
  Serial.println(WiFi.localIP()); 


  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count = 2;

  // Camera reboot fix. Powers down the camera for 10 ms to reset it
  // since there is no reset pin on the OV2640
  gpio_set_level((gpio_num_t)config.pin_pwdn, 1);
  delay(10);
  
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  udp.begin(serverPort);
  okudp.begin(okPort);
}

int k=0;
char cbuff[50];

void loop() {
  
  if(!(flag == 0))
  {
    //Serial.println("START");
    
    udp.begin(serverPort);
    if(flag==1)
    {
      flag = okudp.receviePacket(); // OK 받는 부분
    }
ReSend:    
    fb = esp_camera_fb_get();  
    if(!fb) {
      goto ReSend;
    }
    fp = fopen("/sd/1.jpg","wb");
    fwrite((char*)fb->buf,sizeof(char),fb->len,fp);
    fclose(fp);

    fp = fopen("/sd/1.jpg","rb");
    fseek(fp,0,SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    memset(message,'\0',sizeof(message));
    fpsize = fread(message,1,65507,fp);
    char data_check[5];
    for(int k=6;k<10;k++)
    {
      data_check[k-6] = fb->buf[k];
    }
    
    if(strstr(data_check,"JFIF"))
    {    
      if(flag == 2 || flag ==3)
      {         
      //fsize = fb->len;
      udp.beginSize(serverAddress,serverPort); 
      size_lost(fpsize);

      static uint8_t buf[1460];
      int i = 0;
      while(fpsize)
      {
        size_t toRead = fpsize;
        if(toRead > 1460)
        {
          toRead = 1460;
        }
        memset(buf,'\0',sizeof(buf));
        for(int j=0;j<toRead;j++)
        {
          buf[j] = (message[i++]);
        }
        udp.beginData(serverAddress,serverPort);
        udp.image_write((char*)buf,toRead);
        data_lost();
        fpsize -= toRead;
        flag = okudp.resendPacket();
        if(flag == 3)
        { 
          Serial.println("ReSend");
          flag = 2;
          i= 0;
          fclose(fp);
          udp.endPacket();
          goto ReSend;
        }
      }
      udp.endPacket();
      i=0;
      fclose(fp);
      esp_camera_fb_return(fb);
      delay(9); 
      }
    }
    else
    {
       Serial.println(k++);
       esp_camera_fb_return(fb);
       delay(9); 
    }
   }
  else if(flag == 0)
  {
    flag = okudp.StartPacket();
    delay(9);
  }
}
