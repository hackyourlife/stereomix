Stereo Mix
==========

This is the repository for a simple stereo mixer with 4 mono channels as input
and digital volume and panning controls.

Features
--------

- 4 fully analog AC coupled channels
- Stereo mix bus / output
- Channel mute LEDs
- Master level meter LEDs
- Fully MIDI controlled
- Module chaining (digital control and mix bus) to create a bigger mixer
- 4HP Eurorack module
- Manufacturing cost around 20 EUR per module

MIDI Implementation
-------------------

All parameters are controlled via MIDI CC on channel 16:

| CC | Description          |
|----|----------------------|
| 32 | Channel 1 Volume     |
| 33 | Channel 2 Volume     |
| 34 | Channel 3 Volume     |
| 35 | Channel 4 Volume     |
| 36 | Channel 1 Pan        |
| 37 | Channel 2 Pan        |
| 38 | Channel 3 Pan        |
| 39 | Channel 4 Pan        |
| 40 | Channel 1 Volume (L) |
| 41 | Channel 1 Volume (R) |
| 42 | Channel 2 Volume (L) |
| 43 | Channel 2 Volume (R) |
| 44 | Channel 3 Volume (L) |
| 45 | Channel 3 Volume (R) |
| 46 | Channel 4 Volume (L) |
| 47 | Channel 4 Volume (R) |

Repository Organization
-----------------------

- `firmware`: Source code for the microcontroller firmware
- `photos`: The photos shown on this page
- `schematics.png`: Schematics of the module

Photos
------

![Photo 1](https://raw.githubusercontent.com/hackyourlife/stereomix/master/photos/stereomix-1.jpg)

![Photo 2](https://raw.githubusercontent.com/hackyourlife/stereomix/master/photos/stereomix-2.jpg)
