# OpenGL_Performance_Precision
Test the graphics performance and integer and floating-point precision of OpenGL on your hardware.

Left-click the window that pops up to toggle between 3 different tests:
- Integer operations per second (results are printed to console)
- Floating-point operations per second (results are printed to console)
- Precision of ints and floats (results are displayed in window)

For the precision test, the squares represent the number of bits the GPU accounts for.

The number of green squares on the left is how many bits (maximum) can be in an int value (1 requires 1 bit, 2 requires 2 bits, 4 requires 3 bits.)

The number of green squares on the right is how many bits (maximum) can be in the mantissa of a float value (1.0f requires 0 bits, 1.5f requires 1 bit, 1.75f requires 2 bits.)

"precision highp int" and "precision highp float" are used by default, but these can be changed.
