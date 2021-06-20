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
/*
 * The application generates an interrupt every one minute using timer1 as base.
 Timer1 by itself can generate interrupts of 4 seconds only. This value has
 been multiplied to get interrupts corresponding to multiples of 4s.
 */

#include "mcc_generated_files/mcc.h"
#include "stdbool.h"

#define ONE_MINUTE 15
#define FIVE_MINUTES 4*ONE_MINUTE
#define ONE_QUARTER 15*ONE_MINUTE
#define HALF_HOUR 30*ONE_MINUTE
#define ONE_HOUR 60*ONE_MINUTE

/*Variables*/
static uint32_t waiting_time=ONE_MINUTE;
static volatile uint32_t counter=0;
static volatile bool time_elapsed=false;


void update_counter(void)
{
    LATAbits.LATA1^=1;
    counter=(counter+1)%waiting_time;
    if(counter==0)
        time_elapsed=true;
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
    TMR1_SetInterruptHandler(update_counter);
    //TMR4_SetInterruptHandler(toggle_LED);
    //TMR0_SetInterruptHandler(toggle_LED);
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    while (1)
    {
        // Add your application code
        //toggle_LED();
        if(time_elapsed)
        {
            time_elapsed=false;
        }
    }
       
}
/**
 End of File
*/