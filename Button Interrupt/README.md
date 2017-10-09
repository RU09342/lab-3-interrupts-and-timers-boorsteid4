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