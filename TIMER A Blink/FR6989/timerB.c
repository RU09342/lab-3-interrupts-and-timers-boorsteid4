//***************************************************************************************
// LAB 3: INTERRUPTS AND TIMERS
//
// MSP430FR6989 TIMER B
//
// Description: Toggle P1.0 and P9.7 using TIMER B and pre-dividers.
//
// ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430FR6989
//           -----------
//        /|\|      XIN|-
//         | |         |
//         --|RST  XOUT|-
//           |     P1.0|-->LED1
//           |     P9.7|-->LED2
//
// Damon Boorstein
// Rowan University
// Date Created: October 4, 2017
// Date Updated: October 4, 2017
//
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

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO

    P1SEL0 &= ~LED1; // Select I/O direction for P1.0
    P1SEL1 &= ~LED1;
    P1SEL0 &= ~LED1; // Select I/O direction for P9.7
    P1SEL1 &= ~LED1;
    LED1_DIR |= LED1; // Set P1.0
    LED2_DIR |= LED2; // ... and P9.7 to output direction
    LED1_OUT &= ~LED1; // Clear the LEDs
    LED2_OUT &= ~LED2;

    TB0CCTL0 |= CCIE;
    TB0CTL |= TBSSEL_2 + MC_2 + ID_3; // Set the timer B to SMCLCK/8, Continuous
    // Clear the timer and enable timer interrupt
    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled

}


// Timer B0 interrupt service routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0 (void)
{
    LED1_OUT ^= LED1; // Toggle the LEDs
    LED2_OUT ^= LED2;
}


