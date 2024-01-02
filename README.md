# Weihnachten

All adjustable settings are in 'Weihnachten.ino'.

Available settings are:

- pins for button and rgbleds

- rgbleds (not tested with more or less than 3 rgbleds)

     - *RGBLEDs* array with *RGBLed*: ( pin_r, pin_g, pin_b, val_r, val_g, val_b )

     - values are semi-optional as they are automatically by the patterns on start

- default pattern on startup
    - *currentPattern* is index of *patterns* array

###### analog pins require software pulse width modulation which might flicker for small values

---

Can connect to serial monitor with baud 115200 to debug and send commands.

Available commands are:

- p\<num>   // change to pattern number <num> (e.g. p0 changes to first pattern)

###### \n and \r will be ignored

---

If on, the button will cycle through the patterns when pressed, and turn the lighting off if held.

If off, the button will turn on the lighting if pressed or held.
