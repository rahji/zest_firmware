# Zest Shield Firmware

This is software for an Arduino Uno (with the [Zest Shield](https://github.com/rahji/zestshield)) used to convert an Epilog Zing laser cutter/engraver to a machine for exposing cyanotype prints with an off-the-shelf UV laser. The goal of the project is to share all the electronic and mechanical designs, software, and documentation needed to reproduce this transformation from a proprietary mostly-obsolete machine ("dead media" device, cf [Bruce Sterling](http://www.deadmedia.org/)) to an open source device for creation ("zombie media" device, cf [Garnet Hertz and Jussi Parikka](https://www.researchgate.net/publication/273062231_Five_Principles_of_Zombie_Media)).

## What Does it Do?

### Filtering the Data from the Zing

The Arduino takes, as input, a PWM-like data signal that appears on the "data cable" that normally connects to the Epilog Zing's CO<sub>2</sub> laser tube. It filters out any incoming pulses of less than 53 microseconds (see <https://github.com/rahji/50us_filter>) before mirroring the incoming data to an output pin. This is done to ignore the short pulses that the laser cutter sends to the CO<sub>2</sub> laser tube when the machine is idle. This may have some use for the original laser tube (keeping it warmed up?), but the replacement laser driver sees it as a PWM signal and causes the laser to turn on (dimly) when the machine is idle. That's potentially a problem when we are loading the photosensitive paper.

### Hijacking the Red Dot Laser

The Zing has a red laser diode that helps with positioning the material in the machine. It is turned on using a button on the machine's control pad. Since this won't be useful in our redesign, the connector that normally goes to the laser diode is instead connected as an input to the Arduino (through an optocoupler). This can be used to turn on the UV laser for experimental purposes, or when aligning the laser.

The power of the UV laser during this "on" time is set using an 8-position DIP switch on the Zest Shield. The 8-bit value set on the DIP switch is the value that will be sent to the laser driver, where 0 is no power and 255 is full power. **The value of the DIP switch is only checked when the Arduino is being powered on or when it is reset.**

*Note that this system does not work with power values that are close to zero, so some experimentation may be necessary to get extremely dim laser output.*
