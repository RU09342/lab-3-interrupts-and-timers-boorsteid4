//***************************************************************************************
// LAB 3: INTERRUPTS AND TIMERS
//
// MSP430F5529 TIMER A
//
// Description: Toggle P1.0 and P4.7 using TIMER B and pre-dividers.
//
// ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430F5529
//           -----------
//        /|\|      XIN|-
//         | |         |
//         --|RST  XOUT|-
//           |     P1.0|-->LED1
//           |     P4.7|-->LED2
//
// Damon Boorstein
// Rowan University
// Date Created: October 2, 2017
// Date Updated: October 10, 2017
//
//
// Built with Code Composer Studio v7.2.0
//***************************************************************************************
#include <msp430f5529.h>
#include "convertHz.h"

#define LED1        BIT0
#define LED2        BIT7
#define LED1_OUT    P1OUT
#define LED2_OUT    P4OUT
#define LED1_DIR    P1DIR
#define LED2_DIR    P4DIR

void convertHz(int);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    P1SEL &= ~LED1; // Select I/O direction for P1.0
    P4SEL &= ~LED2; // ... and P4.7
    LED1_DIR |= LED1; // Set P1.0
    LED2_DIR |= LED2; // ...and P4.7 to output direction
    LED1_OUT &= ~LED1; // Clear the LEDs
    LED2_OUT &= ~LED2;

    convertHz(20); // 20Hz

    // Clear the timer and enable timer interrupt
    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled

    for(;;){}

}


// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    LED1_OUT ^= LED1; // Toggle the LEDs
    LED2_OUT ^= LED2;
}


