# Kolibri

Kolibri - a custom drone project.

## Introduction

Kolibri is the name of my first custom made drone project. The outline of the
development is based on the tutorial of [Carbon Aeronautics](https://github.com/CarbonAeronautics).

to be continued...

## Safety Measures

Serveral safety concerns have been taken into account and implemented when flying the quadcopter.
Some of them require proper configuration of the transmitter/receiver combination in order to
work properly. The following table shows the addressed topics:


| Security Concern | Mitigation |
| ---------------- | ---------- |
| Receiver malfunction, no further channel data received  | Firmware detects loss of PPM communication and enters recoverable failsafe mode, see below. |
| Transmitter signal lost | Channel 8 is expected to switch to a value above 50 % / 1500 µs PPM - this has to be configured on the receiver, check manual of your device. One occured, the firmware enters a non-recoverable failsafe mode, see below. |
| Throttle user request too high | The throttle request of the user is capped at 80 % / 1800 µs PPM in order to have 20% room for controller corrections. |
| Throttle calculation request too high | Throttle values resulting from any calculation are capped at 100 % / 2000 µs to avoid motor overload. |
| Throttle user / calculation request too low during flight | Throttle requests below an idle of 18 % / 1180 µs PPM during flight are clamped to idle. A request lower than 5 % / 1050 µs PPM are interpreted as switch off request, also during flight! |
| PID controller builds up I / D components over time | I and D components of the PID controller are clamped within a range of [-400, 400]. |

The failsafe mode currently does the following:

- activate idle throttle (see above)
- set roll / pitch / yaw rates to 0 °/s

In case of an emergency, this will lead to an uncontrolled crash of the quadcopter,
but try to mitigate it by stopping any (intended) angular movements and give a
minimum of throttle to hopefully make the inevitable crash a little softer.
