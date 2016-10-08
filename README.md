# 8x8x8_LedCube
I've made a custom version of an ARDUINO Led Cube.
It is based on the one you can find at :
[pyroelectro tutorial](http://www.pyroelectro.com/projects/8x8x8_led_cube/).
I've added a 74hc595 flipflop and a 3to8 decoder to reduce the number
of pines used.
The ISR, pin connections and the data writing routine had to be modified. 
Contains two sets of code : Generic and Customized.

## Generic
Code found on the internet developed by other people.
## Customized
Generic code modified to fit my circuit.
