# turn_indicator_code

WIP!

# usage
[LEFT] [OFF] [RIGHT]
(short press direction for 10s timer -> indicator blinks)
long press for continuous -> indicator on

short press OFF to turn off
(long press OFF to turn on stand lights (always on red dot) -> indicator on)

idempotent inputs instead of toggles, resists bouncing and is easier to understand



# Considerations

power off? 
- disconnect battery (possible?) 
- disconnect after board: easy, check standby usage
- deep sleep chip and LED off
- deep sleep chip and LED disconnect via MOSFET

APA idle: 1mA per LED
LEDs: 3mA per (3mm) LED on
Nano board: 30mA when on and working
- 12 mA when sleeping
-> test pro micro with desoldered power LED! should be 0.1mA

18650 boost board: 
0.0mA with switch open
0.8mA without load, probably power led

Conclusion: Disconnect at board, use whatever code and HW you want. 

# Design choices

"A Class with one instance is like a group with one person. Use a namespace instead"
classes won't allow pointers and make anonymous function usage difficult
