# Button Based Delay
The frequency of an LED blink is controlled by a button delay. The delay period is equivalent to the time the button has been pressed down. Uses Timer A/B and Port_x interrupt vectors. At program startup, the LED is blinked at a default frequency.

## Implementation
A software loop is used to count the number of timer cycles that have passed from the moment the button was pressed to the point of release. This loop utilizes Timer A or B, and Port interrupt vectors. First, initalize all inputs, and outputs. Remember to stop the watchdog timer and disable GPIO high-impedance mode for the FR series boards.

A function is used to initialize the timer, called `initTIMER(int)`. It initalizes the clock as in the main function, but it allows the user to set the fequency by using an integer argument. After enabling interrupts and sending the microprossesor into LPM0 with GIE, you can start to set up the interrupt vectors.

The Port interrupt vector sets the value of some important status variables and resets `i`. The status variables used here are `en` and `buttonPressed`. `en` enables timer counting, `i` is used for comparison, and `buttonPressed` is to acknowledge the button has been pressed. This routine also clears the interrupt on the button-pin.

The Timer A (or B) routine uses a variable `count` to count the cycles between Port interrupt edges. It first checks that count is enabled (`en != 0`). While count is enabled, it checks if the button has been released (or is up). If so, it resets everything, including variable `count`. As long as the button is held down, the LED will remain on and `i` will increment.

On the other hand, if count is not enabled, or `en = 0`, `count` will increment until the count equals the delay set by the button. Once count is equivalent to i (`count == i`), the LED will toggle and count will reset. As you can imagine, this process will continue, giving you a delayed blinking rate.