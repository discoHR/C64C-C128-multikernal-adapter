C64C/C128 Switchless Multi-Kernal Adapter
27C512/27C256 ROM adapter for C64C and C128
https://github.com/discoHR/C64C-C128-multikernal-adapter
Designed by discoHR, based on Bwack's 24-pin adapter design.
(https://github.com/bwack/C64-Switchless-Multi-Kernal-27C256-adapter)
CERN OPEN HARDWARE LICENSE v1.2
http://www.ohwr.org/

It allows you to run either:
a) four kernals using a 27C512 (SJ1 open)
b) two kernals using a 27C256 (SJ1 closed)

Hold the restore key to enter the select mode. You will notice a fast flash
on the LED. Release the restore key when the flashing stops and press it
shortly as many times as you want to switch to the other kernals.
The LED colour will indicate which kernal is selected.
The C64 will reset to selected kernal shortly after you stop pressing the
restore key.

The Reset button (if present) behaves similar to the restore key but
unlike the restore key, the reset button is always in select mode.

There are two LED colour themes but due to hardware limitations, they don't
differ much. You can hold the restore key during power-on to toggle between
the two themes.

Theme #1:
2 kernals: yellow, blue
4 kernals: red, blue, green, cyan

Theme #2:
2 kernals: orange, magenta
4 kernals: red, magenta, lime, white

For R1, R2 and R3 you can use any value up to 12k (perhaps even more), depends
how bright you want the RGB LED.

===============================================================================
DISCLAIMER: THIS IS UNTESTED AND COULD FRY YOUR COMPUTER. USE AT OWN RISK!

I'm testing it on my C64C and C128 and will remove the notice above when I find
it safe to use.

C64C 2-kernals: TESTED OK
C128 2-kernals: TESTED OK
C64C 4-kernals: TESTED OK
C128 4-kernals: ?
===============================================================================


Changelog:

PCB rev1.1 (final)

2016-10-02
- expanded the pcb a bit
- increased the distance between EPROM and connectors
- increased font sizes
- moved some texts to the bottom
- added CERN OHL and GitHub URL to the silkscreen
- finalized rev1.1

2016-09-30
- first version of firmware

2016-09-28
- swapped BLUE and GREEN signals to match the RGB LED pinout
- using 6.7k value for R1, R2 and R3

2016-09-19
- bumped resistor values significantly because the LED is still too bright at 6.7k
- increased the board size a bit and rerouted most of the traces

2016-09-18
- added license
- moved pin headers to the bottom where they belong
- using larger pads


PCB rev1.0 (final)

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
