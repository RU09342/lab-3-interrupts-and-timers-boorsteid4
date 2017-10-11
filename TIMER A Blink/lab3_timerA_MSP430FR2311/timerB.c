//***************************************************************************************
// LAB 3: INTERRUPTS AND TIMERS
//
// MSP430FR2311 TIMER B
//
// Description: Toggle P1.0 and P2.0 using TIMER B and pre-dividers.
//
// ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430FR2311
//           -----------
//        /|\|      XIN|-
//         | |         |
//         --|RST  XOUT|-
//           |     P1.0|-->LED1
//           |     P2.0|-->LED2
//
// Damon Boorstein
// Rowan University
// Date Created: October 4, 2017
// Date Updated: October 10, 2017
//
//
// Built with Code Composer Studio v7.2.0
//***************************************************************************************
#include <msp430fr2311.h>
#include "convertHz.h"

#define LED         BIT0
#define LED1_OUT    P1OUT
#define LED2_OUT    P2OUT
#define LED1_DIR    P1DIR
#define LED2_DIR    P2DIR

void convertHz(int);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO

    P1SEL0 &= ~LED; // Select I/O direction for P1.0
    P1SEL1 &= ~LED;
    P2SEL0 &= ~LED; // ... and P2.0
    P2SEL1 &= ~LED;
    LED1_DIR |= LED; // Set P1.0
    LED2_DIR |= LED; // ...and P2.0 to output direction
    LED1_OUT &= ~LED; // Clear the LEDs
    LED2_OUT &= ~LED;

    convertHz(2); // 2Hz

    // Clear the timer and enable timer interrupt
    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled

}


// Timer B0 interrupt service routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0 (void)
{
    LED1_OUT ^= LED; // Toggle the LEDs
    LED2_OUT ^= LED;
}


