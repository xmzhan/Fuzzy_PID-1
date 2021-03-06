#include <pthread.h>
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems


void *t_function(void *arg);
void *u_function(void *arg);

pthread_mutex_t u_mutex;//= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t t_mutex;//= PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  u_cond;//= PTHREAD_COND_INITIALIZER;
pthread_cond_t  t_cond;//= PTHREAD_COND_INITIALIZER;

int flag = 0;
static int socket_flag=1;
static int thread_flag=1;
void *t_function(void *arg)
{
  while(1)
  {
    pthread_mutex_lock(&t_mutex);
    if(!(thread_flag ==1))//thread_flag!
        pthread_cond_wait(&t_cond,&t_mutex);
    else
        pthread_cond_signal(&t_cond);
    printf("t_Thread Start\n");
    delay(1000);
     pthread_cond_signal(&u_cond);
        thread_flag=2;
    pthread_mutex_unlock(&t_mutex);
  }
}   

void *u_function(void *arg)
{
  while(1)
  {
    pthread_mutex_lock(&u_mutex);
    if(!(thread_flag ==2))
        pthread_cond_wait(&u_cond,&u_mutex);
    else
        pthread_cond_signal(&u_cond);
    printf("u_Thread Start\n");
    delay(1000);
     pthread_cond_signal(&t_cond);
        thread_flag=1;
    pthread_mutex_unlock(&u_mutex);
  }
}   



void setup() 
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  delay(1000);

  pthread_t u_thread;  
  pthread_t t_thread;
  pthread_attr_t attr;   //쓰레드 속성값
 
  int status;
  int a = 100;

  // '쓰레드 분리' 속성값 지정 : PTHREAD_CREATE_DETACHED
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_mutex_init(&u_mutex, NULL);
  pthread_mutex_init(&t_mutex, NULL);
  pthread_cond_init(&u_cond, NULL);
  pthread_cond_init(&t_cond, NULL);
  
  pthread_create(&u_thread, &attr, u_function, (void *)&a);
  pthread_create(&t_thread, &attr, t_function, (void *)&a);
  
  pthread_attr_destroy(&attr);
}


void loop()
{    
}
