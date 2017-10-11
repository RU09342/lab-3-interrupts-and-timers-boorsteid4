/*
 * convertHz.c: Convert Hz to proper values for TIMER module operation
 *
 *  Created on: Oct 10, 2017
 *  Updated on: Oct 10, 2017
 *      Author: Damon Boorstein
 *              Rowan University
 *
 */

#include <msp430.h>

void convertHz(int f){ // Example: f = 10
    TA0CCTL0 = CCIE; // Enable interrupt in compare mode
    TA0CTL = TASSEL_2 + MC_1 + ID_2; // SMCLK/4, Up
    TA0CCR0 = 250000 / f; // 250000 / 10 = 25000, (10^6 [Hz] / 4) / (25000) = 10Hz
}


