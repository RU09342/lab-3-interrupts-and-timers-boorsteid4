# Button Interrupt

Two LEDs are toggled using xor inside a software loop. A port interrupt vector is used to toggle LED blinking on a button press.

# Implementation

Setting up the Select, Direction, Output, and Resistor-ENable registers was the same here as in Lab 2. Since we are using interrupts, there are a few more registers that need to be accessed. Vectors must also be addressed.

Interrupts are not stops in a program, but routines with a certain priority. The MSP430 has a priority list of programs it must perform, with the watchdog timer being the highest priority. When a user sets an interrupt, they are actually masking a certain vector in a preexisting interrupt with its own priority. Whenever this masked value changes, an interrupt will fire. The current process will finish, and the compiler will try to meet the conditions set by the interrupt.

To set an interrupt routine, I used the following statement:

```c
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
}
```

and changed the PORTx_VECTOR according to which port the button pin was on.

Once the interrupt routines are set up, simply <code>__enable_interrupt();</code> and code whatever you want your microprocessor to do. Here, I made a simple polling loop to control when the LEDs should blink. The blink variable, more importantly, is controlled by interrupts.

### Interrupt Edge Select (PxIES)

This allows you to select on which edge of the trigger should an interrupt flag appear. Clearing with the button bit will allow you to fire an interrupt when the button is released. Setting with the same bit will fire when the button is pressed. In my code, I have included both, but commented one out.

### Interrupt Edge (PxIE)

Masking this with the button bit will tell the processor to look for interrupts on that pin. For instance, if you wanted to look for interrupts on P1.3, the code would look like <code>P1IE |= BIT3</code>.

### Interrupt Flag

This is set whenever an interrupt is triggered to alert the processor. It can be cleared in the program setup and in the interrupt routine.

## EXTRA WORK: EnergyTrace

Using EnergyTrace on the FR series boards, I found that the button blink code that used interrupts had slightly better energy performance than the button blink code. A green delta means that the energy profile on the left is better than the one on the right.

### FR2311
![FR2311](https://raw.githubusercontent.com/RU09342/lab-3-interrupts-and-timers-boorsteid4/master/Button%20Interrupt/FR2311/energytrace/energytrace_compare.png)

### FR5994
![FR5994](https://raw.githubusercontent.com/RU09342/lab-3-interrupts-and-timers-boorsteid4/master/Button%20Interrupt/FR5994/energytrace/energytrace_compare%20(2).png)

### FR6989
![FR6989](https://raw.githubusercontent.com/RU09342/lab-3-interrupts-and-timers-boorsteid4/master/Button%20Interrupt/FR6989/energytrace/energytrace_compare%20(3).png)

## EXTRA WORK: 8-bit Shift Register

I found, through Texas Instruments, an 8-bit shift register program for the MSP430x20x2 series processors. By changing the include, I was able to get it working on the G2553. In a roundabout way, the code creates a ping-pong effect using a software loop inside of the main function. Several other functions are included in the main to set up registers, shift bits, latch values, count, and delay.

P1DIR is set up to use bits, which are defined in the program as `DATA`, `CLOCK`, `LATCH`, and `ENABLE`. 
* `DATA` is connected to LED1. 
* `CLOCK` is pulsed in a void function. 
* `LATCH` is used to retain the value from the previous shift and toggled to write to the next. 
* `ENABLE` is used as a shortcut to turn on or turn off the LED array.

If I had a 74HC595 Shift Register, I could fully implement this using external components. I would use the circuit in the following figure.

![Schematic](http://processors.wiki.ti.com/images/4/46/74HC595.png)

The inputs to this circuit are P1.0, 1.4, 1.5, and 1.6. The current being driven through each LED is controlled by a 270 Ohm resistor. The IC is being powered by VCC and is referenced at GND. Optionally, I could add a 0.1 microF cap between the latch pin and GND to stabilize the output.