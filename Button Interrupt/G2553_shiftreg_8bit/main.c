// Name: Damon Boorstein
//       Rowan University
// Date: 10/10/17
//
//  LAB 3: INTERRUPTS AND TIMERS
//      EXTRA WORK: MSP430G2553 8-bit Shift Register
//
// ORIGINAL CODE BY ANDREW MORTON AT TEXAS INSTRUMENTS:
//***************************************************************************************
// MSP430 Driver for 74HC595 Shift Register
//
// Description; Drives 8 LED's with 3 digital pins of the MSP430, via a shift register
//
// MSP430G2553
//
//***************************************************************************************
#include <msp430g2553.h>

//Define our pins
#define DATA BIT0 // DS -> 1.0
#define CLOCK BIT4 // SH_CP -> 1.4
#define LATCH BIT5 // ST_CP -> 1.5
#define ENABLE BIT6 // OE -> 1.6
// The OE pin can be tied directly to ground, but controlling
// it from the MCU lets you turn off the entire array without
// zeroing the register

// Declare functions
void delay ( unsigned int );
void pulseClock ( void );
void shiftOut ( unsigned char );
void enable ( void );
void disable ( void );
void init ( void );
void pinWrite ( unsigned int, unsigned char );


int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR |= (DATA + CLOCK + LATCH + ENABLE);  // Setup pins as outputs
  enable(); // Enable output (pull OE low)

  int i;
  //Do a "ping-pong" effect back and forth
  for(;;){
    for ( i = 0 ; i < 8 ; i++ ){ // Beginning of the effect
      shiftOut(1 << i); // Shift a '1' left i bits
      delay(50);
    }
    for ( i = 7 ; i >= 0 ; i-- ){ // Returning in the opposite direction
      shiftOut(1 << i); //
      delay(50);
    }
  }
}

// Delays by the specified milliseconds
// thanks to:
// http://www.threadabort.com/archive/2010/09/05/msp430-delay-function-like-the-arduino.aspx
void delay(unsigned int ms)
{
    while (ms--)
        __delay_cycles(1000); // set for 16Mhz change it to 1000 for 1 Mhz
}

// Writes a value to the specified bitmask/pin. Use built in defines
// when calling this, as the shiftOut() function does.
// All nonzero values are treated as "high" and zero is "low"
void pinWrite( unsigned int bit, unsigned char val )
{
    if (val)
        P1OUT |= bit;
    else
        P1OUT &= ~bit;
}

// Pulse the clock pin
void pulseClock( void )
{
  P1OUT |= CLOCK;
  P1OUT ^= CLOCK;

}

// Take the given 8-bit value and shift it out, LSB to MSB
void shiftOut(unsigned char val)
{
  //Set latch to low (should be already)
  P1OUT &= ~LATCH;

  char i; // NOTE: char i != int i above

  // Iterate over each bit, set data pin, and pulse the clock to send it
  // to the shift register
  for (i = 0; i < 8; i++)  {
      pinWrite(DATA, (val & (1 << i)));
      pulseClock();
  }

  // Pulse the latch pin to write the values into the storage register
  P1OUT |= LATCH;
  P1OUT &= ~LATCH;
}

// These functions are just a shortcut to turn on and off the array of
// LED's when you have the enable pin tied to the MCU. Entirely optional.
void enable( void )
{
  P1OUT &= ~ENABLE;
}

void disable( void )
{
  P1OUT |= ENABLE;
}
