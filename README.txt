README.txt (2016-09-14)

C64/C128 Switchless 28-pin Multi Kernal Adapter
- 27C256/27C512 ROM adapter for C64 (short board) and C128
Designed by discoHR based on Bwack's 24-pin adapter design (https://github.com/bwack/C64-Switchless-Multi-Kernal-27C256-adapter)

It allows you to run either:
a) four kernals using a 27C512
b) two kernals using a 27C256

DISCLAIMER: THIS IS UNTESTED AND COULD FRY YOUR COMPUTER. USE AT OWN RISK!

I will test it on my C64s and C128s and remove the notice above when I find it safe to use.


Changelog:

2016-09-17
- added board images and PDF with schematic
- minor aesthetic changes on the silk layer

2016-09-15
- removed R4
- rearranged signal-pin pairs to allow using weak pull-up for the solder jumper
- minor design changes
- improved routing
- fixed incorrectly connected switch
- moved everything from _tsilk to tNames layer

2016-09-14
- added NC next to unconnected pin
- initial commit
