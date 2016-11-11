# haybale_sensor

This program times pulses on two input pins (D4 & D5) and converts to a frequency. It then publishes the frequenciesto the Particle Cloud.

__THSense__
This measures a variable frequency square wave. The pulseIn function is used to measure the low and high pulses, and the two are added for total period. 

__CapSense__
This measures a wave that is nearly square, but not enough for pulseIn to function properly. Instead an interrupt is used to count how many times the line goes high in 1 second, which itself is the frequency. 

__End of program sleep__
The photon goes into deep sleep mode for 15 minutes after every program execution. In this mode, when it restarts it does so from the very beginning. Only data in EEPROM (not used in this program) is saved.
