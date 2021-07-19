/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F1827
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/*This application reads data from the MPU9250 IMU and reports the acceleration
 in m/s. Full scale range was kept at default value of +-2G.
 */

#include "mcc_generated_files/mcc.h"
#include "stdbool.h"
#include "mpu9250.h"
#define IMU_SENSITIVITY_SCALE_FACTOR 16384.0/*For 2G limit only*/

#define ONE_MINUTE 15
#define FIVE_MINUTES 4*ONE_MINUTE
#define ONE_QUARTER 15*ONE_MINUTE
#define HALF_HOUR 30*ONE_MINUTE
#define ONE_HOUR 60*ONE_MINUTE

/*Interrupt flags*/
static volatile bool time_elapsed=false;
static volatile bool flag_get_data_i2c1=false;

/*Variables*/
static uint32_t waiting_time=ONE_MINUTE;
static volatile uint32_t counter=0;

/*IMU data variables*/
static int16_t IMU_ax_Z=0;
static IMU IMU1={0,0,0};
float acc_Z=0.0;

/*ISR*/
void update_flag_get_data_i2c1(void)
{
    flag_get_data_i2c1=true;    
}

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TMR1_StartTimer();
    TMR4_StartTimer();
    TMR1_SetInterruptHandler(update_flag_get_data_i2c1);
    //TMR4_SetInterruptHandler(update_flag_get_data_i2c1);
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    while (1)
    {
        // Add your application code
        if(flag_get_data_i2c1)
        {
            flag_get_data_i2c1=false;
            IMU1.accelZ=i2c_read2ByteRegister(IMU_ADDRESS_LOW, ACCEL_ZOUT_H);
            acc_Z=(IMU1.accelZ/IMU_SENSITIVITY_SCALE_FACTOR)*9.8;
        }
    }
       
}
/**
 End of File
*/