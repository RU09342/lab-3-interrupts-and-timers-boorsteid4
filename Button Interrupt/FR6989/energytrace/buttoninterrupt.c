//***************************************************************************************
// LAB 3: INTERRUPTS AND TIMERS
//
// MSP430FR6989 Button Interrupt
//
// Description: Toggle P1.0 and P9.7 by xor'ing them inside of a software loop.
//              Port 1 interrupt starts/stops blink
// ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430FR6989
//           -----------
//        /|\|      XIN|-
//         | |         |
//         --|RST  XOUT|-
// Button -->|P1.1 P1.0|-->LED1
// Button -->|P1.2 P9.7|-->LED2
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
#include <msp430fr6989.h>


#define LED1        BIT0
#define LED2        BIT7
#define LED1_OUT    P1OUT
#define LED2_OUT    P9OUT
#define LED1_DIR    P1DIR
#define LED2_DIR    P9DIR
#define BUTTON1     BIT1
#define BUTTON2     BIT2

unsigned int blink = 0;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO default

    P1SEL0 &= ~(BUTTON2 + BUTTON1 + LED1); // Select I/O for P1.0-2
    P1SEL1 &= ~(BUTTON2 + BUTTON1 + LED1);
    P9SEL0 &= ~LED2; // Select I/O for P9.7
    P9SEL1 &= ~LED2;
    LED1_DIR |= LED1; // Set P1.0
    LED2_DIR |= LED2; // and P9.7 to output direction
    LED1_OUT &= ~LED1; // Turn LED1
    LED2_OUT &= ~LED1; // and LED2 off
    P1DIR &= ~(BUTTON2 + BUTTON1); // Set P1.1 and P1.2 to input direction
    P1REN |= (BUTTON2 + BUTTON1); // Enable a puller-resistor on the button-pins
    P1OUT |= (BUTTON2 + BUTTON1); // Tell the resistors to pull up
    P1IFG &= ~(BUTTON2 + BUTTON1); // Clear P1 interrupt flag

    //      P1IES |= (BUTTON2 + BUTTON1);    // Triggers when you PRESS the button :: or...
    /**/    P1IES &= ~(BUTTON2 + BUTTON1);   // Triggers when you RELEASE the button :: ...or pick the previous

    P1IE |= (BUTTON2 + BUTTON1); // Mask interrupts for each button-pin only


    __enable_interrupt(); // Interrupts get enabled *here* - they were disabled thus far.

    for (;;) // Infinite loop, polling for blink
    {

        if(blink > 0)
        {
        LED1_OUT ^= LED1; // Toggle P1.0
        LED2_OUT ^= LED2; // and P9.7 using xor

        __delay_cycles(100000); // SW Delay of 10000 cycles at 1Mhz
        }

    }

}

// Port 1 interrupt service routine
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
blink ^= 0x01; // Toggle blink
P1IFG &= ~(BUTTON2 + BUTTON1); // P2.1 IFG cleared
LED1_OUT &= ~LED1; // Clear LED1
LED2_OUT &= ~LED2; // and LED2
}
