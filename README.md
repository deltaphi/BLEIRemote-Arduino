# BLEIRemote - Bluetooth Low Energy Infrared Remote

The BLEIRemote project is about creating an IR remote control for Bluetooth Low Energy-enabled android smartphones and tablets.
It consists of an Arduino Sketch (http://github.com/deltaphi/BLEIRemote-Arduino) and an Android App (http://github.com/deltaphi/BLEIRemote-Android).
See the repective repositories for more information about the accoring projects.

## BLEIRemote Arduino Sketch

The BLEIRemote Arduino Sketch is designed to make an ATMega8/168/328 interact with an Olimex nRF8001 Bluetooth Low Energy Transmitter (https://www.olimex.com/Products/Modules/RF/MOD-nRF8001/).
The transmitter is set to constantly broadcast a custom UUID which can be identified by the Android App.
The ATMega is configured for minimum power consumption, i.e., it enters SLEEP_MODE_POWER_DOWN whenever there is no action to perform.

## Hardware

If you want to replicate the project, you can find a GIMP drawing of my hardware setup, indicating the connections between the Arduino and the nRF8001 board.

## Software

This Arduino Sketch depends on the IRMP library modified for Arduino and adjusted to the hardware configuration.
You can find the patched source at https://github.com/deltaphi/irmp/tree/BLEIRemote.
