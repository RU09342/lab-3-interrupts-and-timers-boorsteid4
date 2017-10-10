//***************************************************************************************
// LAB 3: INTERRUPTS AND TIMERS
//
// MSP430F5529 Button Interrupt
//
// Description: Toggle P1.0 and P4.7 by xor'ing them inside of a software loop.
//              Port 2 interrupt starts/stops blink on P1.0, Port 1 on P4.7
// ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430F5529
//           -----------
//        /|\|      XIN|-
//         | |         |
//         --|RST  XOUT|-
// Button1-->|P2.1 P1.0|-->LED1
// Button2-->|P1.1 P4.7|-->LED2
//
// Damon Boorstein
// Rowan University
// Date Created: September 25, 2017
// Date Updated: October 10, 2017
//
// Original Program by
// Aldo Briano
// Texas Instruments, Inc
// June 2010
//
// Built with Code Composer Studio v7.2.0
//***************************************************************************************
#include <msp430f5529.h>


#define LED1        BIT0
#define LED2        BIT7
#define LED1_OUT    P1OUT
#define LED2_OUT    P4OUT
#define LED1_DIR    P1DIR
#define LED2_DIR    P4DIR
#define BUTTON      BIT1

unsigned int blink2 = 0;
unsigned int blink1 = 0;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    LED1_DIR |= LED1; // Set P1.0
    LED2_DIR |= LED2; // and P4.7 to output direction
    LED1_OUT &= ~LED1; // Clear the LEDs
    LED2_OUT &= ~LED2;
    P2REN |= BUTTON; // Enable a puller-resistor on the button-pin
    P1REN |= BUTTON;
    P2OUT |= BUTTON; // Tell the resistor to pull up
    P1OUT |= BUTTON;
    P2IFG &= ~BUTTON; // Clear P2
    P1IFG &= ~BUTTON; // and P1 interrupt flags

    //      P2IES |= BUTTON;    // Triggers when you PRESS the button :: or...
    /**/    P2IES &= ~BUTTON;   // Triggers when you RELEASE the button :: ...or pick the previous

    /**/    P1IES |= BUTTON;
    //      P1IES &= ~BUTTON;

    P2IE |= BUTTON; // Mask interrupts for
    P1IE |= BUTTON; // each button-pin only


    __enable_interrupt(); // Interrupts get enabled *here* - they were disabled thus far.

    for (;;) // Infinite loop, polling for blink2 and blink1
    {

        if(blink2 > 0)
        {
        LED1_OUT ^= LED1; // Toggle P4.7 using xor
        __delay_cycles(100000); // SW Delay of 10000 cycles at 1Mhz
        }

        if(blink1 > 0)
        {
        LED2_OUT ^= LED2; // Toggle P1.0 using xor
        __delay_cycles(100000);
        }


    }

}

// Port 2 interrupt service routine
#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
blink2 ^= 0x01; // Toggle blink
P2IFG &= ~BUTTON; // P2.1 IFG cleared
LED1_OUT &= ~LED1; // Clear LED1
}

// Port 1 interrupt service routine
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
blink1 ^= 0x01;
P1IFG &= ~BUTTON; // P2.1 IFG cleared
LED2_OUT &= ~LED2; // Clear LED1
}
