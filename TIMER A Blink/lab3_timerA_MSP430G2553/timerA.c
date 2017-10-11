//***************************************************************************************
// LAB 3: INTERRUPTS AND TIMERS
//
// MSP430G2553 TIMER A
//
// Description: Toggle P1.0 and P1.6 using TIMER A and pre-dividers.
//
// ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430G2553
//           -----------
//        /|\|      XIN|-
//         | |         |
//         --|RST  XOUT|-
//           |     P1.0|-->LED1
//           |     P1.6|-->LED2
//
// Damon Boorstein
// Rowan University
// Date Created: October 4, 2017
// Date Updated: October 4, 2017
//
//
// Built with Code Composer Studio v7.2.0
//***************************************************************************************
#include <msp430g2553.h>
#include "convertHz.h"

#define LED1        BIT0
#define LED2        BIT6
#define LED_OUT     P1OUT
#define LED_DIR     P1DIR

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    P1SEL &= ~(LED1 + LED2); // Select I/O direction for P1.0 and P1.6
    LED_DIR |= LED1 + LED2; // Set P1.0 and P1.6 to output direction
    LED_OUT &= ~(LED1 + LED2); // Turn the LEDs off

    convertHz(5); //5Hz

    // Clear the timer and enable timer interrupt
    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled

}


// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    LED_OUT ^= LED1 + LED2;
}


