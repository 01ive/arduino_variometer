# Arduino variometer

Mini Paragliding variometer based on Arduino technology.

Aim of this project is to have the smallest and cheapest paragliding variometer.
Mechanical fixation will be realized using action cam standard allowing tones of fixing possibilityes.

![circuit](./3D/render.jpg)

-----------------------------------------------------------------------

## Component and prices

* Arduino Nano Atmega328 *[4.33€]*
* Piezo Buzzer passive *[2.49€]*
* BMP280 sensor *[2.66€]*
* Batterie Shield *[2.99€]*
* Battery *[12.23€]*
* Switch On/Off *[0.28€]*
* I2c DC - DC adapter *[0.80€]*

Total Price **25.78€**

-----------------------------------------------------------------------

## Software environnment

* arduino-1.8.13 IDE
* Visual code 1.53
    * vsciot-vscode.vscode-arduino
    * ms-vscode.cpptools
* blender 2.80
* fritzing.0.8.7b.pc
* fritzing library https://github.com/adafruit/Fritzing-Library

-----------------------------------------------------------------------

## Schematics

### Representative prototype

![normal](./schematic/normal.jpg)

### Schematic

![schem](./schematic/schematic_normal.jpg)

-----------------------------------------------------------------------

## Mechanic

![meca](./3D/meca.jpg)

-----------------------------------------------------------------------

## Callibration mode

Software allows debug and callibration modes

### Serial mode

Using USB serial console to print information. Buzzers are disabled quiet mode.

Uncomment ```#define DEBUG 1``` to activate.

### Sound setup

In order to test and found best suitable sound you can use these mode and serial debug link to test buzzer sound.

Uncomment ```#define SOUND_TEST 1``` to activate.

-----------------------------------------------------------------------

## docs

BMP280
https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf

MPU6056
https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf

Nano pinout 
https://content.arduino.cc/assets/Pinout-NANO_latest.pdf
