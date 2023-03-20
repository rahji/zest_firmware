# 50ms (or so) Pulse Filter

This is software for an Arduino Uno that will mirror incoming pulses (at pin 2) to an output pin (pin 3). Only pulses that are at least 53 microseconds long will be mirrored. A side effect of this filtering is that all output pulses begin 53us after the corresponding input pulse (i.e.: they are 53us shorter).

This is part of a project to convert an obsolete Epilog Zing laser cutter into an open source system for creating cyanotypes. More information will be available on the project website.
