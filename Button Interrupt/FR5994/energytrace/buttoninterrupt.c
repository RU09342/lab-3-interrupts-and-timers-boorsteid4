//***************************************************************************************
// LAB 3: INTERRUPTS AND TIMERS
//
// MSP430FR5994 Button Interrupt
//
// Description: Toggle P1.0 and P1.1 by xor'ing them inside of a software loop.
//              Port 5 interrupt starts/stops blink
// ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430FR5994
//           -----------
//        /|\|      XIN|-
//         | |         |
//         --|RST  XOUT|-
// Button -->|P5.6 P1.0|-->LED1
// Button -->|P5.5 P1.1|-->LED2
//
// Damon Boorstein
// Rowan University
// Date Created: September 25, 2017
// Date Updated: October 1, 2017
//
// Original Program by
// Aldo Briano
// Texas Instruments, Inc
// June 2010
//
// Built with Code Composer Studio v7.2.0
//***************************************************************************************
#include <msp430fr5994.h>


#define LED1        BIT0
#define LED2        BIT1
#define LED_OUT     P1OUT
#define LED_DIR     P1DIR
#define BUTTON1     BIT6
#define BUTTON2     BIT5

unsigned int blink = 0;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO default

    P1SEL0 &= ~(LED2 + LED1); // Select I/O direction for P1.0 and P1.1
    P1SEL1 &= ~(LED2 + LED1);
    P5SEL0 &= ~(BUTTON2 + BUTTON1); // Select I/O direction for P5.6
    P5SEL1 &= ~(BUTTON2 + BUTTON1);
    LED_DIR |= (LED2 + LED1); // Set P1.0 to output direction
    LED_OUT &= ~(LED2 + LED1); // Turn LED1 and LED2 off
    P5DIR &= ~(BUTTON2 + BUTTON1); // Set P5.6 to input direction
    P5REN |= BUTTON2 + BUTTON1; // Enable a puller-resistor on the button-pin
    P5OUT |= BUTTON2 + BUTTON1; // Tell the resistor to pull up
    P5IFG &= ~(BUTTON2 + BUTTON1); // Clear P1 interrupt flag

    //      P5IES |= (BUTTON2 + BUTTON1);    // Triggers when you PRESS the button :: or...
    /**/    P5IES &= ~(BUTTON2 + BUTTON1);   // Triggers when you RELEASE the button :: ...or pick the previous

    P5IE |= (BUTTON2 + BUTTON1); // Mask interrupts for each button-pin only


    __enable_interrupt(); // Interrupts get enabled *here* - they were disabled thus far.

    for (;;) // Infinite loop, polling for blink
    {

        if(blink > 0)
        {
        LED_OUT ^= (LED2 + LED1); // Toggle LED1 and LED2 using xor

        __delay_cycles(100000); // SW Delay of 10000 cycles at 1Mhz
        }

    }

}

// Port 1 interrupt service routine
#pragma vector = PORT5_VECTOR
__interrupt void Port_5(void)
{
blink ^= 0x01; // Toggle blink
P5IFG &= ~(BUTTON2 + BUTTON1); // P5.6 and P5.5 IFG cleared
LED_OUT &= ~(LED2 + LED1);
}
