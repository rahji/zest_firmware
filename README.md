# Zest Firmware

This is software for an microcontroller used to convert an Epilog Zing laser cutter/engraver to a machine for exposing cyanotype prints with an off-the-shelf UV laser. The goal of the project is to share all the electronic and mechanical designs, software, and documentation needed to reproduce this transformation from a proprietary defunct media device to an open source system for experimental mark making.

## What Does it Do?

### Filtering the Data from the Zing

The microcontroller takes, as input, a PWM-like data signal that appears on the "data cable" that normally connects to the Epilog Zing's CO<sub>2</sub> laser tube. It filters out any incoming pulses of less than 53 microseconds (see <https://github.com/rahji/50us_filter>) before mirroring the incoming data to an output pin. This is done to ignore the short pulses that the laser cutter sends to the CO<sub>2</sub> laser tube when the machine is idle. This may have some use for the original laser tube (keeping it warmed up?), but the replacement laser driver sees it as a PWM signal and causes the laser to turn on (dimly) when the machine is idle. That's potentially a problem when we are loading the photosensitive paper.

## Which Microcontroller?

This was originally designed for an Arduino Shield (which uses the Atmega328p chip). It has since been updated to work with a custom PCB that uses an Attiny85 instead. The Atmega328p version was never tested with the Epilog machine - only with instruments on the bench.

## Links

* [Five Principles of Zombie Media](https://www.researchgate.net/publication/273062231_Five_Principles_of_Zombie_Media)
* [2023 R-CADE Symposium: Repair](https://rcade.camden.rutgers.edu/2023symposium.html)
* [REBOOT Laboratory](https://cfa.fsu.edu/special-projects/reboot/)
