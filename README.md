# ArduLCD
Arduino based interface for HD44780 LCD to lcdproc/lcd4linux and equivalent.

# Beginnings

As time goes on, it is harder and harder to connected HD44780 LCDs to computer systems. It used to be that every computer had a parallel port, and you just wired it up there.  However fewer and fewer machines have parallel ports, and in some cases the software won't work (e.g. lcdproc on FreeBSD locks up if you try to use the parallel port). 

So having bought an arduino, and realising how cheap and powerful it was, I thought it would be a good fit for a LCD to USB interface. It is also a relatively simple first project. 

# Design

-- 02/04/2016

Well, I found a user manual for the original PIC-an-LCD: http://www.phanderson.com/PIC-n-LCD/user_manual.html

Turns out the protocol is far more advanced than I expected, allowing for quite a bit of control, higher level LCD functions, and the ability to either output a tone from a speaker, or to fire off 4 GPIO pins.

In addition, after looking at the lcdproc source code, none of the advanced features are 
used. lcdproc basically just uses the "raw passthrough" escape characters to talk to the l
cd directly.

-- 29/03/2016

I decided to emulate the old "pic-an-lcd" interface. This was a (proprietery) serial to HD44780 interface that was used in the late 90's. I knew of it back then, but never bothered due to the abundance of parallel ports.  I decided to emulate this because:

1. It is old enough that it is supposed by most LCD software. In particular the software I tend to use (LCDproc and lcd4linux)
2. It is serial based, so I can make use of the existing USB->Serial interface provided by the arduino. 

Because the Arduino is stupidly overpowered for such a simple task, I worked on making it fast. The HD44780 makes use of 8 bit transfers, and serial speed is at 115200.

This code is nowhere near good, it is more like a hack than a proper development, but posted here in case someone else is interested in such a thing, and saves them the time to reverse engineer the protocol. The original "pic-an-lcd" page seems to have dropped off the internet (http://www.cyberramp.net/~dale) , so had to work out what it is doing from the LCDproc docs and other sources.


# Current status / known-bugs

Well... it works. However on my testbench the LCD gets corrupted after a while. Not sure if that is due to the software timings being off, or due the length of wiring + noise (or the LCD itself, which was pulled from a junked telephone). So will have to investigate further. Specifically transferring custom characters (as LCDproc does) to the LCD ends up with corruption. 

# Building

I don't know about you, but I use "ino" on the command line. cd to the root of the repor dir, and run "ino build", when completed successfully, run "ino upload" to push to arduino. 

## To Change the display size (default: 24x2)

At the moment you have to edit the code directly, there are two constants you have to change LCDW and LCDH, which govern the LCD dimensions in character width and height respectively

# Schematic

Todo: Add schematic here for how to wire up the arduino to a HD44780 LCD
