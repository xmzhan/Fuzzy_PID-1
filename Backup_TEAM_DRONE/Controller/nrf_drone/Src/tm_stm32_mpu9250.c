/**	
 * |----------------------------------------------------------------------
 * | Copyright (c) 2016 Tilen Majerle
 * |  
 * | Permission is hereby granted, free of charge, to any person
 * | obtaining a copy of this software and associated documentation
 * | files (the "Software"), to deal in the Software without restriction,
 * | including without limitation the rights to use, copy, modify, merge,
 * | publish, distribute, sublicense, and/or sell copies of the Software, 
 * | and to permit persons to whom the Software is furnished to do so, 
 * | subject to the following conditions:
 * | 
 * | The above copyright notice and this permission notice shall be
 * | included in all copies or substantial portions of the Software.
 * | 
 * | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * | OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * | AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * | HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * | WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * | FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * | OTHER DEALINGS IN THE SOFTWARE.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32_mpu9250.h"

/* Magnetometer */
#define WHO_AM_I_AK8963     0x00 // Result = 0x48
#define INFO                0x01
#define AK8963_ST1          0x02
#define AK8963_XOUT_L	    0x03
#define AK8963_XOUT_H	    0x04
#define AK8963_YOUT_L	    0x05
#define AK8963_YOUT_H	    0x06
#define AK8963_ZOUT_L	    0x07
#define AK8963_ZOUT_H	    0x08
#define AK8963_ST2          0x09
#define AK8963_CNTL         0x0A
#define AK8963_ASTC         0x0C
#define AK8963_I2CDIS       0x0F
#define AK8963_ASAX         0x10
#define AK8963_ASAY         0x11
#define AK8963_ASAZ         0x12

/* MPU9250 data */
#define SELF_TEST_X_GYRO    0x00                  
#define SELF_TEST_Y_GYRO    0x01                                                                          
#define SELF_TEST_Z_GYRO    0x02

#define SELF_TEST_X_ACCEL   0x0D
#define SELF_TEST_Y_ACCEL   0x0E    
#define SELF_TEST_Z_ACCEL   0x0F

#define SELF_TEST_A         0x10

#define XG_OFFSET_H         0x13
#define XG_OFFSET_L         0x14
#define YG_OFFSET_H         0x15
#define YG_OFFSET_L         0x16
#define ZG_OFFSET_H         0x17
#define ZG_OFFSET_L         0x18
#define SMPLRT_DIV          0x19
#define CONFIG              0x1A
#define GYRO_CONFIG         0x1B
#define ACCEL_CONFIG        0x1C
#define ACCEL_CONFIG2       0x1D
#define LP_ACCEL_ODR        0x1E   
#define WOM_THR             0x1F   

#define MOT_DUR             0x20
#define ZMOT_THR            0x21
#define ZRMOT_DUR           0x22

#define FIFO_EN             0x23
#define I2C_MST_CTRL        0x24   
#define I2C_SLV0_ADDR       0x25
#define I2C_SLV0_REG        0x26
#define I2C_SLV0_CTRL       0x27
#define I2C_SLV1_ADDR       0x28
#define I2C_SLV1_REG        0x29
#define I2C_SLV1_CTRL       0x2A
#define I2C_SLV2_ADDR       0x2B
#define I2C_SLV2_REG        0x2C
#define I2C_SLV2_CTRL       0x2D
#define I2C_SLV3_ADDR       0x2E
#define I2C_SLV3_REG        0x2F
#define I2C_SLV3_CTRL       0x30
#define I2C_SLV4_ADDR       0x31
#define I2C_SLV4_REG        0x32
#define I2C_SLV4_DO         0x33
#define I2C_SLV4_CTRL       0x34
#define I2C_SLV4_DI         0x35
#define I2C_MST_STATUS      0x36
#define INT_PIN_CFG         0x37
#define INT_ENABLE          0x38
#define DMP_INT_STATUS      0x39
#define INT_STATUS          0x3A
#define ACCEL_XOUT_H        0x3B
#define ACCEL_XOUT_L        0x3C
#define ACCEL_YOUT_H        0x3D
#define ACCEL_YOUT_L        0x3E
#define ACCEL_ZOUT_H        0x3F
#define ACCEL_ZOUT_L        0x40
#define TEMP_OUT_H          0x41
#define TEMP_OUT_L          0x42
#define GYRO_XOUT_H         0x43
#define GYRO_XOUT_L         0x44
#define GYRO_YOUT_H         0x45
#define GYRO_YOUT_L         0x46
#define GYRO_ZOUT_H         0x47
#define GYRO_ZOUT_L         0x48
#define EXT_SENS_DATA_00    0x49
#define EXT_SENS_DATA_01    0x4A
#define EXT_SENS_DATA_02    0x4B
#define EXT_SENS_DATA_03    0x4C
#define EXT_SENS_DATA_04    0x4D
#define EXT_SENS_DATA_05    0x4E
#define EXT_SENS_DATA_06    0x4F
#define EXT_SENS_DATA_07    0x50
#define EXT_SENS_DATA_08    0x51
#define EXT_SENS_DATA_09    0x52
#define EXT_SENS_DATA_10    0x53
#define EXT_SENS_DATA_11    0x54
#define EXT_SENS_DATA_12    0x55
#define EXT_SENS_DATA_13    0x56
#define EXT_SENS_DATA_14    0x57
#define EXT_SENS_DATA_15    0x58
#define EXT_SENS_DATA_16    0x59
#define EXT_SENS_DATA_17    0x5A
#define EXT_SENS_DATA_18    0x5B
#define EXT_SENS_DATA_19    0x5C
#define EXT_SENS_DATA_20    0x5D
#define EXT_SENS_DATA_21    0x5E
#define EXT_SENS_DATA_22    0x5F
#define EXT_SENS_DATA_23    0x60
#define MOT_DETECT_STATUS   0x61
#define I2C_SLV0_DO         0x63
#define I2C_SLV1_DO         0x64
#define I2C_SLV2_DO         0x65
#define I2C_SLV3_DO         0x66
#define I2C_MST_DELAY_CTRL  0x67
#define SIGNAL_PATH_RESET   0x68
#define MOT_DETECT_CTRL     0x69
#define USER_CTRL           0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define PWR_MGMT_1          0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2          0x6C
#define DMP_BANK            0x6D  // Activates a specific bank in the DMP
#define DMP_RW_PNT          0x6E  // Set read/write pointer to a specific start address in specified DMP bank
#define DMP_REG             0x6F  // Register in DMP from which to read or to which to write
#define DMP_REG_1           0x70
#define DMP_REG_2           0x71 
#define FIFO_COUNTH         0x72
#define FIFO_COUNTL         0x73
#define FIFO_R_W            0x74
#define WHO_AM_I_MPU9250    0x75 // Should return 0x71
#define XA_OFFSET_H         0x77
#define XA_OFFSET_L         0x78
#define YA_OFFSET_H         0x7A
#define YA_OFFSET_L         0x7B
#define ZA_OFFSET_H         0x7D
#define ZA_OFFSET_L         0x7E

#define WhoAmIReturn        (0x71)
#define WhoAmIReturn2       (0x73)

void __INIT__MPU9250(TM_MPU9250_t* MPU9250)
{
    MPU9250->Ax=0; MPU9250->Ay=0; MPU9250->Az=0;
    MPU9250->Gx=0; MPU9250->Gy=0; MPU9250->Gz=0;
    MPU9250->Mx=0; MPU9250->My=0; MPU9250->Mz=0;

    MPU9250->Ax_Raw=0; MPU9250->Ay_Raw=0; MPU9250->Az_Raw=0;
    MPU9250->Gx_Raw=0; MPU9250->Gy_Raw=0; MPU9250->Gz_Raw=0;
    MPU9250->Mx_Raw=0; MPU9250->My_Raw=0; MPU9250->Mz_Raw=0;

    MPU9250->AMult=0; MPU9250->GMult=0; MPU9250->MMult=0;
    MPU9250->ASAX=0; MPU9250->ASAY=0; MPU9250->ASAZ=0;

    MPU9250->Accbiasx=0; MPU9250->Accbiasy=0; MPU9250->Accbiasz=0;
    MPU9250->Gybiasx=0; MPU9250->Gybiasy=0; MPU9250->Gybiasz=0;
    MPU9250->Magbiasx=0; MPU9250->Magbiasy=0; MPU9250->Magbiasz=0;
    MPU9250->Magscalex=0; MPU9250->Magscaley=0; MPU9250->Magscalez=0;

    MPU9250->I2C_Addr=0;
    MPU9250->I2C_Addr_Mag=0;
}


// Accelerometer and gyroscope self test; check calibration wrt factory settings
TM_MPU9250_Result_t MPU9250SelfTest(TM_MPU9250_t* MPU9250, float * Self_Test,  TM_MPU9250_Device_t dev) // Should return percent deviation from factory trim values, +/- 14 or less deviation is a pass
{
    uint8_t data;

     /* Set values */
    MPU9250->I2C_Addr = MPU9250_I2C_ADDR | (uint8_t)dev;
    MPU9250->I2C_Addr_Mag = MPU9250_I2C_ADDR_MAG;
    
    /* Init delay */
    //TM_DELAY_Init();
    
    /* Init I2C */
    TM_I2C_Init(MPU9250_I2C, MPU9250_I2C_PP, MPU9250_I2C_CLOCK);
    
    /* Check if device connected */
    if (TM_I2C_IsDeviceConnected(MPU9250_I2C, MPU9250->I2C_Addr) != TM_I2C_Result_Ok) {
        return TM_MPU9250_Result_DeviceNotConnected;
    }
    
    /* Check who I am */
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, WHO_AM_I_MPU9250, &data);
    if ((data != WhoAmIReturn) && (data != WhoAmIReturn2)) {
        return TM_MPU9250_Result_DeviceNotConnected;
    }

    uint8_t rawData[6] = {0, 0, 0, 0, 0, 0};
    uint8_t selfTest[6];
    int32_t gAvg[3] = {0}, aAvg[3] = {0}, aSTAvg[3] = {0}, gSTAvg[3] = {0};
    float factoryTrim[6];
    uint8_t FS = 0;

    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, SMPLRT_DIV, 0x00);    // Set gyro sample rate to 1 kHz
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, CONFIG, 0x02);        // Set gyro sample rate to 1 kHz and DLPF to 92 Hz
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, GYRO_CONFIG, FS<<3);  // Set full scale range for the gyro to 250 dps
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_CONFIG2, 0x02); // Set accelerometer rate to 1 kHz and bandwidth to 92 Hz
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_CONFIG, FS<<3); // Set full scale range for the accelerometer to 2 g

    for( int ii = 0; ii < 200; ii++) {  // get average current values of gyro and acclerometer    
    
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_XOUT_H, rawData, 6);  // Read the six raw data registers into data array
    
    aAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    aAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
    aAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
        
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, GYRO_XOUT_H, &rawData[0], 6);   // Read the six raw data registers sequentially into data array

    gAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    gAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
    gAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
    }
    
    for (int ii =0; ii < 3; ii++) {  // Get average of 200 values and store as average current readings
    aAvg[ii] /= 200;
    gAvg[ii] /= 200;
    }
    
    // Configure the accelerometer for self-test
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_CONFIG, 0xE0); // Enable self test on all three axes and set accelerometer range to +/- 2 g
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, GYRO_CONFIG, 0xE0); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
    Delayms(25);  // Delay a while to let the device stabilize

    for( int ii = 0; ii < 200; ii++) {  // get average self-test values of gyro and acclerometer
    
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_XOUT_H, &rawData[0], 6);  // Read the six raw data registers into data array
    aSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    aSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
    aSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
    
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, GYRO_XOUT_H, &rawData[0], 6);  // Read the six raw data registers sequentially into data array
    gSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    gSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
    gSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
    }
    
    for (int ii =0; ii < 3; ii++) {  // Get average of 200 values and store as average self-test readings
    aSTAvg[ii] /= 200;
    gSTAvg[ii] /= 200;
    }   
    
    // Configure the gyro and accelerometer for normal operation
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_CONFIG, 0x00);  
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, GYRO_CONFIG, 0x00);
    Delayms(25);  // Delay a while to let the device stabilize
    
    // Retrieve accelerometer and gyro factory Self-Test Code from USR_Reg
     
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, SELF_TEST_X_ACCEL, &selfTest[0]); // X-axis accel self-test results
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, SELF_TEST_Y_ACCEL, &selfTest[1]); // Y-axis accel self-test results
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, SELF_TEST_Z_ACCEL, &selfTest[2]); // Z-axis accel self-test results
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, SELF_TEST_X_GYRO, &selfTest[3]);  // X-axis gyro self-test results
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, SELF_TEST_Y_GYRO, &selfTest[4]);  // Y-axis gyro self-test results
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, SELF_TEST_Z_GYRO, &selfTest[5]);  // Z-axis gyro self-test results

    // Retrieve factory self-test value from self-test code reads
    factoryTrim[0] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[0] - 1.0) )); // FT[Xa] factory trim calculation
    factoryTrim[1] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[1] - 1.0) )); // FT[Ya] factory trim calculation
    factoryTrim[2] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[2] - 1.0) )); // FT[Za] factory trim calculation
    factoryTrim[3] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[3] - 1.0) )); // FT[Xg] factory trim calculation
    factoryTrim[4] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[4] - 1.0) )); // FT[Yg] factory trim calculation
    factoryTrim[5] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[5] - 1.0) )); // FT[Zg] factory trim calculation
    
    // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
    // To get percent, must multiply by 100
    for (int i = 0; i < 3; i++) {
        Self_Test[i]   = 100.0*((float)(aSTAvg[i] - aAvg[i]))/factoryTrim[i] - 100.;   // Report percent differences
        Self_Test[i+3] = 100.0*((float)(gSTAvg[i] - gAvg[i]))/factoryTrim[i+3] - 100.; // Report percent differences
    }   
    return TM_MPU9250_Result_Ok;
}

// Function which accumulates gyro and accelerometer data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers.
void calibrateMPU9250(TM_MPU9250_t* MPU9250)
{  
    uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
    uint16_t ii, packet_count, fifo_count;
    int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
    uint32_t tempbias[6] = {0,0,0,0,0,0};

    // reset device
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
    Delayms(100);

    // get stable time source; Auto select clock source to be PLL gyroscope reference if ready 
    // else use the internal oscillator, bits 2:0 = 001
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, PWR_MGMT_1, 0x01);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, PWR_MGMT_2, 0x00);
    Delayms(200);                                    

    // Configure device for bias calculation
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, INT_ENABLE, 0x00);   // Disable all interrupts
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, FIFO_EN, 0x00);      // Disable FIFO
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, PWR_MGMT_1, 0x00);   // Turn on internal clock source
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, I2C_MST_CTRL, 0x00); // Disable I2C master
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, USER_CTRL, 0x0C);    // Reset FIFO and DMP
    Delayms(15);

    // Configure MPU6050 gyro and accelerometer for bias calculation
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, CONFIG, 0x01);       // Set low-pass filter to 188 Hz
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, SMPLRT_DIV, 0x00);   // Set sample rate to 1 kHz
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity

    uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
    uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

    // Configure FIFO to capture accelerometer and gyro data for bias calculation
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, USER_CTRL, 0x40);   // Enable FIFO  
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in MPU-9150)
    Delayms(40); // accumulate 40 samples in 40 milliseconds = 480 bytes

    // At end of sample accumulation, turn off FIFO sensor read
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, FIFO_COUNTH, &data[0], 2); // read FIFO sample count
    fifo_count = ((uint16_t)data[0] << 8) | data[1];
    packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging

    for (ii = 0; ii < packet_count; ii++) {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, FIFO_R_W, &data[0], 12); // read data for averaging
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;    
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;

    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];
            
    }
    accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0]  /= (int32_t) packet_count;
    gyro_bias[1]  /= (int32_t) packet_count;
    gyro_bias[2]  /= (int32_t) packet_count;

    if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) accelsensitivity;}  // Remove gravity from the z-axis accelerometer bias calculation
    else {accel_bias[2] += (int32_t) accelsensitivity;}

    // Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
    data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
    data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
    data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
    data[3] = (-gyro_bias[1]/4)       & 0xFF;
    data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
    data[5] = (-gyro_bias[2]/4)       & 0xFF;

    // Push gyro biases to hardware registers
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, XG_OFFSET_H, data[0]);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, XG_OFFSET_L, data[1]);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, YG_OFFSET_H, data[2]);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, YG_OFFSET_L, data[3]);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ZG_OFFSET_H, data[4]);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ZG_OFFSET_L, data[5]);    

    // Output scaled gyro biases for display in the main program
    MPU9250->Gybiasx = (float) gyro_bias[0]/(float) gyrosensitivity;
    MPU9250->Gybiasy = (float) gyro_bias[1]/(float) gyrosensitivity;
    MPU9250->Gybiasz = (float) gyro_bias[2]/(float) gyrosensitivity;

    // Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
    // factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
    // non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
    // compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
    // the accelerometer biases calculated above must be divided by 8.

    int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, XA_OFFSET_H, &data[0], 2); // Read factory accelerometer trim values
    accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, YA_OFFSET_H, &data[0], 2);
    accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, ZA_OFFSET_H, &data[0], 2);
    accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

    uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
    uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis

    for(ii = 0; ii < 3; ii++) {
    if((accel_bias_reg[ii] & mask)) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
    }

    // Construct total accelerometer bias, including calculated average accelerometer bias from above
    accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
    accel_bias_reg[1] -= (accel_bias[1]/8);
    accel_bias_reg[2] -= (accel_bias[2]/8);

    data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
    data[1] = (accel_bias_reg[0])      & 0xFF;
    data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
    data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
    data[3] = (accel_bias_reg[1])      & 0xFF;
    data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
    data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
    data[5] = (accel_bias_reg[2])      & 0xFF;
    data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

    // Apparently this is not working for the acceleration biases in the MPU-9250
    // Are we handling the temperature correction bit properly?
    // Push accelerometer biases to hardware registers
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, XA_OFFSET_H, data[0]);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, XA_OFFSET_L, data[1]);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, YA_OFFSET_H, data[2]);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, YA_OFFSET_L, data[3]);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ZA_OFFSET_H, data[4]);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ZA_OFFSET_L, data[5]);

    // Output scaled accelerometer biases for display in the main program
    MPU9250->Accbiasx = (float)accel_bias[0]/(float)accelsensitivity; 
    MPU9250->Accbiasy = (float)accel_bias[1]/(float)accelsensitivity;
    MPU9250->Accbiasz = (float)accel_bias[2]/(float)accelsensitivity;
    
    
    HAL_FLASH_Unlock();
  
    FLASH_Erase_Sector(FLASH_SECTOR_5, FLASH_VOLTAGE_RANGE_3);      
    
    tempbias[0] = (int)(MPU9250->Accbiasx * 1000);
    tempbias[1] = (int)(MPU9250->Accbiasy * 1000);
    tempbias[2] = (int)(MPU9250->Accbiasz * 1000);
    tempbias[3] = (int)(MPU9250->Gybiasx * 1000);
    tempbias[4] = (int)(MPU9250->Gybiasy * 1000);
    tempbias[5] = (int)(MPU9250->Gybiasz * 1000);
    
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08020000, tempbias[0]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08020004, tempbias[1]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08020008, tempbias[2]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x0802000C, tempbias[3]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08020010, tempbias[4]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08020014, tempbias[5]);

    HAL_Delay(30);  
    HAL_FLASH_Lock();   
}

void AK8963SelfTest(TM_MPU9250_t* MPU9250, float* Self_Test_Mag)
{
    uint8_t data[7];
    uint8_t check;
    int32_t mAvg[3] = {0};

    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 0x00); // Power down magnetometer
    Delayms(10); // Wait for all registers to reset 
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_ASTC, 1<<6); // Write "1" to the SELF bit of the ASTC register.
    Delayms(10); // Wait for all registers to reset     
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 1<<4); // Enter self test Mode.
    Delayms(10); // Wait for all registers to reset

int numb = 0;
for( int ii = 0; ii < 200; ii++) {  // get average current values of gyro and acclerometer    
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_ST1, &check);    
    if (check & 0x01) {
        TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_XOUT_L, data, 7);
        if (!(data[6] & 0x08)) {
            mAvg[0] += (int16_t)(((int16_t)data[1] << 8) | data[0]);
            mAvg[1] += (int16_t)(((int16_t)data[3] << 8) | data[2]);
            mAvg[2] += (int16_t)(((int16_t)data[5] << 8) | data[4]); 
            numb++;            
        }
    }
    Delayms(12); 
    }
    
    for (int ii =0; ii < 3; ii++) {  // Get average of numb values and store as average current readings
    mAvg[ii] /= numb;
    }

    Self_Test_Mag[0] = (float)mAvg[0] * MPU9250->MMult * MPU9250->ASAX;
    Self_Test_Mag[1] = (float)mAvg[1] * MPU9250->MMult * MPU9250->ASAY;
    Self_Test_Mag[2] = (float)mAvg[2] * MPU9250->MMult * MPU9250->ASAZ;    

    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_ASTC, 0x00); // Write "0" to the SELF bit of the ASTC register.
    Delayms(10); // Wait for all registers to reset
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 0x00); // Power down magnetometer
    Delayms(10); // Wait for all registers to reset        
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 0x01<<4 | 0x06);
    Delayms(10); // Wait for all registers to reset 
}


TM_MPU9250_Result_t TM_MPU9250_Init(TM_MPU9250_t* MPU9250, TM_MPU9250_Device_t dev) {
    uint8_t data;
    
    /* Set values */
    MPU9250->I2C_Addr = MPU9250_I2C_ADDR | (uint8_t)dev;
    MPU9250->I2C_Addr_Mag = MPU9250_I2C_ADDR_MAG;
    
    /* Init delay */
    TM_DELAY_Init();
    
    /* Init I2C */
    TM_I2C_Init(MPU9250_I2C, MPU9250_I2C_PP, MPU9250_I2C_CLOCK);
    
    /* Check if device connected */
    if (TM_I2C_IsDeviceConnected(MPU9250_I2C, MPU9250->I2C_Addr) != TM_I2C_Result_Ok) {
        return TM_MPU9250_Result_DeviceNotConnected;
    }
    
    /* Check who I am */
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, WHO_AM_I_MPU9250, &data);
    if ((data != WhoAmIReturn) && (data != WhoAmIReturn2)) {
        return TM_MPU9250_Result_DeviceNotConnected;
    }
    
    // wake up device
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors 
    Delayms(100); // Wait for all registers to reset 

    // get stable time source
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, PWR_MGMT_1, 0x01);  // Auto select clock source to be PLL gyroscope reference if ready else
    Delayms(200); 

    // Configure Gyro and Thermometer
    // Disable FSYNC and set thermometer and gyro bandwidth to 41 and 42 Hz, respectively; 
    // minimum delay time for this setting is 5.9 ms, which means sensor fusion update rates cannot
    // be higher than 1 / 0.0059 = 170 Hz
    // DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
    // With the MPU9250, it is possible to get gyro sample rates of 32 kHz (!), 8 kHz, or 1 kHz
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, CONFIG, 0x03);  

    // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, SMPLRT_DIV, 0x04);  // Use a 200 Hz rate; a rate consistent with the filter update rate 
                            // determined inset in CONFIG above

    // Set gyroscope full scale range
    // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, GYRO_CONFIG, &data); // get current GYRO_CONFIG register value
    // c = c & ~0xE0; // Clear self-test bits [7:5] 
    data &= ~0x02; // Clear Fchoice bits [1:0] 
    data &= ~0x18; // Clear AFS bits [4:3]
    data |= 0x01 << 3; // Set full scale range for the gyro
    // c =| 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, GYRO_CONFIG, data); // Write new GYRO_CONFIG value to register

    // Set accelerometer full-scale range configuration
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_CONFIG, &data); // get current ACCEL_CONFIG register value
    // c = c & ~0xE0; // Clear self-test bits [7:5] 
    data &= ~0x18;  // Clear AFS bits [4:3]
    data |= 0x01 << 3; // Set full scale range for the accelerometer 
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_CONFIG, data); // Write new ACCEL_CONFIG register value

    // Set accelerometer sample rate configuration
    // It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
    // accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_CONFIG2, &data); // get current ACCEL_CONFIG2 register value
    data &= ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])  
    data |= 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_CONFIG2, data); // Write new ACCEL_CONFIG2 register value
    // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates, 
    // but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

    // Configure Interrupts and Bypass Enable
    // Set interrupt pin active high, push-pull, hold interrupt pin level HIGH until interrupt cleared,
    // clear on read of INT_STATUS, and enable I2C_BYPASS_EN so additional chips 
    // can join the I2C bus and all can be controlled by the Arduino as master
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, INT_PIN_CFG, 0x22);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr, INT_ENABLE, 0x01);
    
    /* Check if device connected */
    if (TM_I2C_IsDeviceConnected(MPU9250_I2C, MPU9250->I2C_Addr_Mag) != TM_I2C_Result_Ok) {
        return TM_MPU9250_Result_DeviceNotConnected;
    }
    
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 0x00); // Power down magnetometer  
    Delayms(10);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
    Delayms(10);
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 0x00); // Power down magnetometer  
    Delayms(10);
    // Configure the magnetometer for continuous read and highest resolution
    // set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
    // and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 1 << 4 | 2); // Set magnetometer data resolution and sample ODR
    Delayms(10);
    
    /* Calculate multiplicators */
    MPU9250->AMult = 4.0f / 32768.0f;
    MPU9250->GMult = 500.0f / 32768.0f;
    MPU9250->MMult = 10.0f * 4912.0f / 32768.0f;
    
    return TM_MPU9250_Result_Ok;
}



void TM_MPU9250_ReadAcce(TM_MPU9250_t* MPU9250) {
    uint8_t data[6];
    
    /* Read accelerometer data */
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, ACCEL_XOUT_H, data, 6);
    
    MPU9250->Ax_Raw = ((int16_t)data[0] << 8) | data[1];
    MPU9250->Ay_Raw = ((int16_t)data[2] << 8) | data[3];  
    MPU9250->Az_Raw = ((int16_t)data[4] << 8) | data[5];
    
    MPU9250->Ax = (float)MPU9250->Ax_Raw * MPU9250->AMult;
    MPU9250->Ay = (float)MPU9250->Ay_Raw * MPU9250->AMult;
    MPU9250->Az = (float)MPU9250->Az_Raw * MPU9250->AMult;
}

void TM_MPU9250_ReadGyro(TM_MPU9250_t* MPU9250) {
    uint8_t data[6];
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr, GYRO_XOUT_H, data, 6);
    
    MPU9250->Gx_Raw = ((int16_t)data[0] << 8) | data[1];
    MPU9250->Gy_Raw = ((int16_t)data[2] << 8) | data[3];  
    MPU9250->Gz_Raw = ((int16_t)data[4] << 8) | data[5];
    
    MPU9250->Gx = (float)MPU9250->Gx_Raw * MPU9250->GMult;
    MPU9250->Gy = (float)MPU9250->Gy_Raw * MPU9250->GMult;
    MPU9250->Gz = (float)MPU9250->Gz_Raw * MPU9250->GMult;
}

void TM_MPU9250_ReadMag(TM_MPU9250_t* MPU9250) 
{
    uint8_t data[7];
    uint8_t check;
    
    /* Check status */
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_ST1, &check);
    
    if (check & 0x01) {
        TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_XOUT_L, data, 7);
        if (!(data[6] & 0x08)) {
            MPU9250->Mx_Raw = ((int16_t)data[1] << 8) | data[0];
            MPU9250->My_Raw = ((int16_t)data[3] << 8) | data[2];
            MPU9250->Mz_Raw = ((int16_t)data[5] << 8) | data[4]; 
            
            MPU9250->Mx = (float)MPU9250->Mx_Raw * MPU9250->MMult * MPU9250->ASAX;
            MPU9250->My = (float)MPU9250->My_Raw * MPU9250->MMult * MPU9250->ASAY;
            MPU9250->Mz = (float)MPU9250->Mz_Raw * MPU9250->MMult * MPU9250->ASAZ;
        }
    }
}

void TM_MPU9250_ReadMag_Bias(TM_MPU9250_t* MPU9250) 
{
    uint8_t data[7];
    uint8_t check;
    
    /* Check status */
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_ST1, &check);
    
    if (check & 0x01) {
        TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_XOUT_L, data, 7);
        if (!(data[6] & 0x08)) {
            MPU9250->Mx_Raw = ((int16_t)data[1] << 8) | data[0];
            MPU9250->My_Raw = ((int16_t)data[3] << 8) | data[2];
            MPU9250->Mz_Raw = ((int16_t)data[5] << 8) | data[4];             
        }
    }
}

void TM_MPU9250_ReadMagASA(TM_MPU9250_t* MPU9250) {
    uint8_t data[3];
    
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 0x00); // Power down magnetometer
    Delayms(10); // Wait for all registers to reset 
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
    Delayms(10); // Wait for all registers to reset     
               
    TM_I2C_ReadMulti(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_ASAX, data, 3);
    MPU9250->ASAX = ((int8_t)data[0]);
    MPU9250->ASAY = ((int8_t)data[1]);
    MPU9250->ASAZ = ((int8_t)data[2]);   
    
    MPU9250->ASAX = (((float)(MPU9250->ASAX)) - 128)/256. + 1.;
    MPU9250->ASAY = (((float)(MPU9250->ASAY)) - 128)/256. + 1.;
    MPU9250->ASAZ = (((float)(MPU9250->ASAZ)) - 128)/256. + 1.;  
        
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 0x00); // Power down magnetometer
    Delayms(10); // Wait for all registers to reset 
    TM_I2C_Write(MPU9250_I2C, MPU9250->I2C_Addr_Mag, AK8963_CNTL, 0x01<<4 | 0x06);
    Delayms(10); // Wait for all registers to reset 
}

void MagCalibration(TM_MPU9250_t* MPU9250)
{
  uint16_t ii = 0, sample_count = 0;
  int32_t mag_bias[3] = {0, 0, 0};
  int32_t mag_scale[3] = {0, 0, 0};
  int16_t mag_max[3] = {-32767, -32767, -32767}, mag_min[3] = {32767, 32767, 32767};
  TM_MPU9250_t mag_temp;
  uint32_t tempbias[6] = {0,0,0,0,0,0};
  
  mag_temp.I2C_Addr_Mag = MPU9250_I2C_ADDR_MAG;
  
  // shoot for ~twenty seconds of mag data
   sample_count = 2000;//2000
   for(ii = 0; ii < sample_count; ii++) {
    TM_MPU9250_ReadMag_Bias(&mag_temp);
      if(mag_temp.Mx_Raw > mag_max[0]) mag_max[0] = mag_temp.Mx_Raw;
      if(mag_temp.My_Raw > mag_max[1]) mag_max[1] = mag_temp.My_Raw;
      if(mag_temp.Mz_Raw > mag_max[2]) mag_max[2] = mag_temp.Mz_Raw;

      if(mag_temp.Mx_Raw < mag_min[0]) mag_min[0] = mag_temp.Mx_Raw;
      if(mag_temp.My_Raw < mag_min[1]) mag_min[1] = mag_temp.My_Raw;
      if(mag_temp.Mz_Raw < mag_min[2]) mag_min[2] = mag_temp.Mz_Raw;    
    //Delayms(135);  // at 8 Hz ODR, new mag data is available every 125 ms
    Delayms(12);  // at 100 Hz ODR, new mag data is available every 10 ms

   }    
// Get hard iron correction
    mag_bias[0]  = (mag_max[0] + mag_min[0])/2;  // get average x mag bias in counts
    mag_bias[1]  = (mag_max[1] + mag_min[1])/2;  // get average y mag bias in counts
    mag_bias[2]  = (mag_max[2] + mag_min[2])/2;  // get average z mag bias in counts
        
    MPU9250->Magbiasx = ((float)mag_bias[0]) * MPU9250->MMult * MPU9250->ASAX;
    MPU9250->Magbiasy = ((float)mag_bias[1]) * MPU9250->MMult * MPU9250->ASAX;
    MPU9250->Magbiasz = ((float)mag_bias[2]) * MPU9250->MMult * MPU9250->ASAX;  
    
// Get soft iron correction estimate
    mag_scale[0]  = (mag_max[0] - mag_min[0])/2;  // get average x axis max chord length in counts
    mag_scale[1]  = (mag_max[1] - mag_min[1])/2;  // get average y axis max chord length in counts
    mag_scale[2]  = (mag_max[2] - mag_min[2])/2;  // get average z axis max chord length in counts

    float avg_rad = mag_scale[0] + mag_scale[1] + mag_scale[2];
    avg_rad /= 3.0;

    MPU9250->Magscalex = avg_rad/((float)mag_scale[0]);
    MPU9250->Magscaley = avg_rad/((float)mag_scale[1]);
    MPU9250->Magscalez = avg_rad/((float)mag_scale[2]);

//=====================Save Bias data in flash memory=================
    HAL_FLASH_Unlock();
  
    FLASH_Erase_Sector(FLASH_SECTOR_6, FLASH_VOLTAGE_RANGE_3);      
    
    tempbias[0] = (int)(MPU9250->Magbiasx * 10);
    tempbias[1] = (int)(MPU9250->Magbiasy * 10);
    tempbias[2] = (int)(MPU9250->Magbiasz * 10);
    tempbias[3] = (int)(MPU9250->Magscalex * 1000);
    tempbias[4] = (int)(MPU9250->Magscaley * 1000);
    tempbias[5] = (int)(MPU9250->Magscalez * 1000);
    
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08040000, tempbias[0]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08040004, tempbias[1]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08040008, tempbias[2]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x0804000C, tempbias[3]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08040010, tempbias[4]);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08040014, tempbias[5]);

    HAL_Delay(30);  
    HAL_FLASH_Lock();   
}


TM_MPU9250_Result_t TM_MPU9250_DataReady(TM_MPU9250_t* MPU9250) {
    uint8_t data;
    TM_I2C_Read(MPU9250_I2C, MPU9250->I2C_Addr, INT_STATUS, &data);
    
    if (data & 0x01) {
        return TM_MPU9250_Result_Ok;
    }
    return TM_MPU9250_Result_Error;
}
