# RoboRemote
**RoboRemote** is an Arduino project for a Bluetooth based remote controller.
It uses a 4-axis joystick, several push buttons, a potentiometer and an 1602 LCD
to control self build robots.
For communication, 2 HC-05 Bluetooth modules are required.
The Sketch is running on an Arduino Pro Mini with an ATmega328(5V, 16MHz).

![RoboRemote](./docs/RoboRemote.png)

## Tutorial
A Tutorial for building **RoboRemote** is shown on YouTube:

  part 1 - preparing the hardware: https://www.youtube.com/watch?v=xHsCnpfgwEI

  part 2 - schematic and wiring  : https://www.youtube.com/watch?v=I3VOlUwofhE

## Schematic
Here is the schematic of **RoboRemote**. It is explained in the videos
mentioned in the tutorial.

![Schematic](./docs/RoboRemote-Schematic_v1.0.png)

## Part List
  * 1x Arduino Pro Mini
  * 2x HC-05 Bluetooth modules
  * 1x 1602 LCD with I2C interface
  * 1x 4-axis Joystick with 10K Ohm potentiometers
  * 7x 12mm push button switches
  * 4x LEDs (diffent colors)
  * 4x 270 Ohm resistors
  * 3x 1K Ohm resistors
  * 3x 1,8K Ohm resistors
  * 2x 4,7K Ohm resistors
  * 1x 10K Ohm resistor
  * 1x 10K Ohm potentiometer
  * 1x on-off switch
  * 1x 9V battery clip
  * 1x 6x4cm universal prototype PCB
  * 1x 3x2cm prototype PCB with strips
  * 4x M3 screws and nuts
  * 2x M3x12mm hexagonal standoff pillars
  * 1x 40pin 2.54mm right angle single row male header
  * 1x 40pin 2.54mm single row male header
  * 1x 40pin 2.54mm single row female header
  * 1x 2mm heat shrink tube, 20cm
  * 2x 2pin Dupont connector
  * 4x 3pin Dupont connector
  * 1x 5pin Dupont connector
  * 20x female Dupont jumper wire terminal connector pins
  * 0.75mm wires, diverse colors
  
## Copyright
**RoboRemote** is written by Andreas Trappmann from
[Trappmann-Robotics.de](https://www.trappmann-robotics.de/). It
is published under the MIT license, check LICENSE for more information.
All text above must be included in any redistribution.

## Release Notes

Version 1.1 - 19.04.2020

  * Added Sketch "RoboRemote_HC05_Terminal" for HC05 Bluetooth module setup.
  * Added Sketch "RoboRemote-Controller" for RoboRemote Bluetooth Controller.
  * Added Sketch "RoboRemote-Receiver" as a demo receiver for the RoboRemote Bluetooth Controller.

Version 1.0 - 19.04.2020

  * Published Sketch "RoboRemote_HW_Test" for basic Hardware testing.
