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

/* Author: Aniket Mazumder
 * (c) 2021 Aniket Mazumder
 * MIT License

    Copyright (c) 2021 Aniket Mazumder

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 * 
 * Description:
 * This application reads data from the BH1750 Light Sensor and switches on the 
 * pump according to set time and runs a pump everyday for a fixed period set
 * by ALLOWED_PUMP_RUNNING_DURATION
 * Change the value in day_counter variable in timer1 ISR to set a desired 
 * timer reset for the clock.
 * Verify the cut of values for the light sensor for your application.
 */

#include "mcc_generated_files/mcc.h"
#include "stdbool.h"
//#include "BH1750.h"

#define FOUR_SEC 1
#define EIGHT_SEC 2*FOUR_SEC 
#define TWELVE_SEC 3*FOUR_SEC 
#define ONE_MINUTE 15*FOUR_SEC
#define FIVE_MINUTES 5*ONE_MINUTE
#define ONE_QUARTER 15*ONE_MINUTE
#define HALF_HOUR 30*ONE_MINUTE
#define ONE_HOUR 60*ONE_MINUTE
#define ONE_DAY 24*ONE_HOUR

#define ALLOWED_PUMP_RUNNING_DURATION TWELVE_SEC
#define CUT_OFF_LIGHT_INTENSITY 100

/*Interrupt flags*/
static volatile bool time_elapsed=false;
static volatile bool flag_get_data_i2c1=false;
static volatile bool flag_read_light_sensor=false;
static volatile bool flag_pump_on=false;
static volatile bool flag_new_day=true;

/*Counter variables*/
static uint32_t light_sensor_measure_interval=FOUR_SEC;
static volatile uint32_t T1_interrupt_counter=0;
static volatile uint32_t pump_running_time_counter=0;
static volatile uint32_t day_counter=0;

/*Sensor data variables*/
static uint16_t light_intensity=0;

/*Timer1 ISR Runs every 4s*/
void update_flags_get_data_i2c1(void)
{
    /*Resets every chosen sensor measurement interval*/
    T1_interrupt_counter=(T1_interrupt_counter+1)%light_sensor_measure_interval;
    if(T1_interrupt_counter==0)
        flag_read_light_sensor=true;
    
    /*Select the reset duration*/
    day_counter=(day_counter+1)%ONE_MINUTE;
    if(day_counter==0)
        flag_new_day=true;        
    
    if(flag_pump_on==true)
        pump_running_time_counter+=1;
}

uint16_t read_Light_Sensor(void)
{
    uint16_t intensity=0;
    intensity=i2c_read2ByteRegister(BH1750_I2CADDR, BH1750_CONTINUOUS_HIGH_RES_MODE);
    return intensity;
}

void pump_ON()
{
    PUMP_CONTROL_PIN_SetHigh();
}

void pump_OFF()
{
    PUMP_CONTROL_PIN_SetLow();
}



/*
Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TMR1_StartTimer();
    TMR1_SetInterruptHandler(update_flags_get_data_i2c1);
    
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    while (1)
    {
        if(flag_read_light_sensor==true)
        {
            flag_read_light_sensor=false;
            light_intensity=read_Light_Sensor();     
            //PUMP_CONTROL_PIN_Toggle();

	    if(light_intensity<CUT_OFF_LIGHT_INTENSITY)
	    {
                if(flag_new_day==true)
                {
                    flag_new_day=false;
                    flag_pump_on=true;
                    pump_ON();                                  
                }
		
	    }
        }

//	/*Switch off pump after running for the ALLOWED_PUMP_RUNNING_DURATION*/
	if(flag_pump_on==true && pump_running_time_counter>=ALLOWED_PUMP_RUNNING_DURATION)
	{
	    flag_pump_on=false;
	    pump_running_time_counter=0;
	    pump_OFF();
	}
    }
       
}
/**
 End of File
*/
