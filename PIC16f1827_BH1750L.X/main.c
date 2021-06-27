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
#include "BH1750.h"

#define ONE_MINUTE 15
#define FIVE_MINUTES 5*ONE_MINUTE
#define ONE_QUARTER 15*ONE_MINUTE
#define HALF_HOUR 30*ONE_MINUTE
#define ONE_HOUR 60*ONE_MINUTE

/*Interrupt flags*/
static volatile bool time_elapsed=false;
static volatile bool flag_get_data_i2c1=false;
static volatile bool flag_read_light_sensor=false;

/*Variables*/
static uint32_t measure_interval=ONE_MINUTE;
static volatile uint32_t T1_interrupt_counter=0;

/*IMU data variables*/
static uint16_t light_intensity=0;

/*ISR*/
void update_flag_get_data_i2c1(void)
{
    T1_interrupt_counter=(T1_interrupt_counter+1)%measure_interval;
    if(T1_interrupt_counter==0)
        flag_read_light_sensor=true;
}

uint16_t read_Light_Sensor(void)
{
    uint16_t intensity=0;
    intensity=i2c_read2ByteRegister(BH1750_I2CADDR, BH1750_CONTINUOUS_HIGH_RES_MODE);
    return intensity;
}
/*
Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TMR1_StartTimer();
    //TMR4_StartTimer();
    TMR1_SetInterruptHandler(update_flag_get_data_i2c1);
    

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    while (1)
    {
        // Add your application code
        if(flag_read_light_sensor==true)
        {
            flag_read_light_sensor=false;
            light_intensity=read_Light_Sensor();                           
        }
            
    }
       
}
/**
 End of File
*/