//***************************************************************************************
// LAB 3: INTERRUPTS AND TIMERS
//
// MSP430F5529 Button-Based Delay
//
// Description: P1.0 is toggled at a constant rate. Duration the button is pressed
//              sets the new blink rate. Port 2 and Timer A interrupts.
// ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430F5529
//           -----------
//        /|\|      XIN|-
//         | |         |
//         --|RST  XOUT|-
//           |         |
// Button -->|P2.1 P1.0|-->LED1
//
// Damon Boorstein
// Rowan University
// Date Created: September 25, 2017
// Date Updated: October 8, 2017
//
// Built with Code Composer Studio v7.2.0
//***************************************************************************************


#include <msp430f5529.h>

#define LED     BIT0
#define BUTTON  BIT1

unsigned int buttonPressed;
unsigned int en = 0x00;
unsigned int count = 0;
unsigned int i = 0;

void initTIMER(int f);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR |= LED; // Set P1.0 as output
    P1OUT &= ~LED; // Turn LED off

    P2DIR &= ~BUTTON; // Set P2.1 as input
    P2REN |= BUTTON; // Enable puller resistor on button-pin
    P2OUT |= BUTTON; // Make resistor pull up

    P2IES |= BUTTON; // Interrupts when you press button
    P2IE |= BUTTON; // Enable interrupt on button pin
    P2IFG &= ~BUTTON; // Clear interrupt flag


    initTIMER(10);    // initialize timer to 10Hz

    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

void initTIMER(int f) // call function with desired frequency to initialize timer module
{
    int n;
    TA0CCTL0 = CCIE; // Enable interrupt in compare mode
    TA0CTL = TASSEL_2 + MC_1 + ID_2; // SMCLK/4, Up
    n = 250000 / f; //250000 / 10 = 25000
    TA0CCR0 = n; // ( 10^6 [Hz] / 4) / (25000) = 10Hz
}

// TIMER A interrupt routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    if (en != 0)
    {
        if (P2IN & BUTTON) // Reset count and enabled when button is up
        {
            buttonPressed = 0;
            en = 0;
            count = 0;
        }

        else    // Increment i while the button is pressed on each timer interrupt
        {
            P1OUT |= LED;
            i++;
        }


    }

    else // Increment until count == i
    {
        if (count == i) // Toggle every i interrupts (i * 0.1 = 5 s)
        {
            P1OUT ^= LED; //Toggle LED
            count = 0;  // reset count
        }

        else
            count++;    // increment count each interrupt until count = i
    }
}

// Port 1 interrupt routine
#pragma vector = PORT2_VECTOR
__interrupt void PORT_2(void)
{
    en = 1;
    i = 0; // Reset timing counter
    buttonPressed = 1;
    P2IFG &= ~BUTTON; // Clear interrupt flag
}
