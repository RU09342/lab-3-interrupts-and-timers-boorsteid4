# TIMER A Blink
TIMER A/B is used to toggle a pair of LEDs.

## Implementation
Using the built-in timers is an energy-saving way of controlling LEDs. The processor is not wasting energy by constantly polling for a change in a software loop. Rather, it is using an interrupt routine to execute a set of instructions. Setting up a timer interrupt is similar to setting up a port interrupt. The following code shows the general format for setting up a timer interrupt:

```c
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void){
	//interrupt routine
}
```

There are some important registers to be aware of when setting up timers.

### Timer A Register (TAxR)
This is literally the register for Timer A. The max value this can attain is 0xFFFF.

### Timer A Capture/Compare Register (TAxCCRn)
The timer can be in either capture mode or compare mode. Capture mode is used to record time values, whereas Compare mode is used to interrupt on time values. TAxCCRn can be set to a value and fire an interrupt when the value of TAxR is equal to the value of TAxCCRn. In this exercise, the timer is only ever in compare mode, since we only need to compare the count of TAxR to a value in TAxCCRn.

#### TAxCCR0
This specific register is used to set the period of Timer A, whose control is TAxCTL. 

### Timer A Control Register (TAxCTL)
This register controls the timer counting mode. In Up mode, the timer counts up to TAxCCR0, and starts back at 0 on the next clock cycle. In Continuous mode, the timer counts up to 0xFFFF before returning to 0 the next clock cycle. In Up/Down mode, the timer counts up to TAxCCR0, then counts down to 0. For this exercise, I used Up mode, but any mode will work relatively the same.

### Timer A Capture Compare Control Register (TAxCCTLn)
You guessed it: there's also a control register for TAxCCRn. Because we are only utilizing compare mode, we can set this register up with 
`c TAxCCTLn |= CCIE;` where `CCIE` stands for Capture/Control Interrupt Enable.

## convertHz

This file initializes a Timer (A or B) for each board. The code is generally the same, and follows the format in this block of code:

```c
void convertHz(int f){ // Example: f = 10
    TA0CCTL0 = CCIE; // Enable interrupt in compare mode
    TA0CTL = TASSEL_2 + MC_1 + ID_2; // SMCLK/4, Up
    TA0CCR0 = 250000 / f; // 250000 / 10 = 25000, (10^6 [Hz] / 4) / (25000) = 10Hz
}
```

### FR Series

For the FR series microprocessors, I found that Timer A was unaccessible. I could not call on it in the code. Therefore, I resorted to using Timer B. Timer B is the same as Timer A except in name. All the information about registers above holds true, but the A is replaced with a B. 

