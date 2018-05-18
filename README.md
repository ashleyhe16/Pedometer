# Pedometer
Simple pedometer project for ECE 3140 Final Project using the FRDM-K64 microcontroller.
Reads z-axis values from the FXO8700CQ onboard accelerometer and if a set threshold value is surpassed within a given time period, the number of steps will be updated accordingly. Displays the number of steps onto an AdaFruit 16x2 monochrome LCD and is refreshed at a rate of 110000 Hz.

* additional mbed library required for proper execution (use mbed online compiler and drag .bin file).
