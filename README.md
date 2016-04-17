# ArduLCD
Arduino based interface for HD44780 LCD to lcdproc/lcd4linux and equivalent.

# Beginnings

As time goes on, it is harder and harder to connected HD44780 LCDs to computer systems. It used to be that every computer had a parallel port, and you just wired it up there.  However fewer and fewer machines have parallel ports, and in some cases the software won't work (e.g. lcdproc on FreeBSD locks up if you try to use the parallel port). 

So having bought an arduino, and realising how cheap and powerful it was, I thought it would be a good fit for a LCD to USB interface. It is also a relatively simple first project. 

# Design

-- 17/04/2016

After chatting to the lcdproc dev's, they actually recommended against PIC-an-LCD, due to its age. While the code I wrote worked, they pointed out that so few LCD-an-PIC
devices exist, that the code is somewhat abandoned and may or may not work in future. 

Instead they recommended I emulate the matrixOrbital interface, because:

a) It is still used, so lots of LCD software still does active support
b) There is a company behind it, making sure that the interface remains in use
c) It supports more features

So decided to try switching over to the MatrixOrbital structure. This is made easier by the fact there is already a piece of software that does this. There is an abandoned project on google code ( https://code.google.com/archive/p/arduino-matrixorbital/ ), that implemented this. So rather than reinvent the wheel, I decided to base my work on that. 

This branch deals with this version, with the intent to eventually replace the pic-an-lcd one.

# Current status / known-bugs


# Building

I use "ino" on the command line. cd to the root of the repor dir, and run "ino build", when completed successfully, run "ino upload" to push to arduino. 

## To Change the display size (default: 24x2)

At the moment you have to edit the code directly, there are two constants you have to change LCDW and LCDH, which govern the LCD dimensions in character width and height respectively

# Schematic

Todo: Add schematic here for how to wire up the arduino to a HD44780 LCD
