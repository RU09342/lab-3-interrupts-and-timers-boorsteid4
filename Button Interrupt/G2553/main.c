//***************************************************************************************
// LAB 3: INTERRUPTS AND TIMERS
//
// MSP430G2553 Blink LED / Start Stop Blinking with Button Demo - Software Toggle P1.0 & P1.6
//
// Description; Toggle P1.0 and P1.6 by xor'ing them inside of a software loop. To
// start/stop blink, an interrupt in Port 1 will toggle the condition statement.
// ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430G2553
//           -----------
//        /|\|      XIN|-
//         | |         |
//         --|RST  XOUT|-
//           |     P1.6|-->LED
// Button -->|P1.3 P1.0|-->LED
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
#include <msp430g2553.h>


#define LED1    BIT0
#define LED2    BIT6
#define LED_OUT P1OUT
#define LED_DIR P1DIR
#define BUTTON BIT3

unsigned int blink = 0;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    P1SEL &= ~(LED2 + BUTTON + LED1); // Select I/O for P1.6, 1.3. and 1.0
    LED_DIR |= (LED2 + LED1); // Set P1.0 and P1.6 to output direction
    LED_OUT &= ~(LED2 + LED1); // Clear the LEDs
    P1REN |= BUTTON; // Enable a puller-resistor on the button-pin
    P1OUT |= BUTTON; // Tell the resistor to pull up

    /**/  P1IES |= BUTTON;    // Triggers when you PRESS the button :: or...
    //    P1IES &= ~BUTTON;   // Triggers when you RELEASE the button :: ...or pick the previous


    P1IE |= BUTTON; // Mask interrupts for the button-pin only

    __enable_interrupt(); // Interrupts get enabled *here* - they were disabled thus far.

    for (;;) // Infinite loop
    {

    if(blink > 0)
    {
    P1OUT ^= (LED2 + LED1); // Toggle P1.0 and P1.6 using xor

    __delay_cycles(100000); // SW Delay of 10000 cycles at 1Mhz

    }
    }

}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
blink ^= 0x01;
P1IFG &= ~BUTTON; // P1.3 IFG cleared
LED_OUT &= ~(LED2 + LED1); // Clear the LEDs so they start in OFF state

}
