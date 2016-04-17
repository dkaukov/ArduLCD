/*
 * vim: ts=4 ai
 *
 * Copyright 2016 Ziva-Vatra ( zv@ziva-vatra.com - Belgrade )
 * Licensed under the GNU GPL v2 or later.
 *
 * ArduLCD Matrix Orbital emulator for Arduino - 17.04.2016 
 *
 * LCD 8 bit mode
 *
 * Inspired by https://code.google.com/archive/p/arduino-matrixorbital
 * but in the end wrote my own version (couldn't get the above to work).
 *
 * Made use of the Matrix Orbital manual (in reference folder).
 * Also made use of the LCDproc driver:
 * http://lcdproc.cvs.sourceforge.net/viewvc/lcdproc/lcdproc/server/drivers/MtxOrb.c?view=markup
 *
 */
 
#include <LiquidCrystal.h>
#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

// And this is ths Hardware serial port (which is also bound to the UART->USB chip)
#include <HardwareSerial.cpp>


// You can define your own pins here.
#define RS 12
#define E  11
#define D0 9
#define D1 8
#define D2 7
#define D3 6
#define D4 5
#define D5 4
#define D6 3
#define D7 2
#define BR 6

#define VERSION 3

#define GPO1 10 // Not yet implemented. (pg. 18 of the manual)

// Define the size of your LCD here
#define LCDW 24
#define LCDH 2


//LiquidCrystal lcd(RS, E, D7, D6, D5, D4, D3, D2, D1, D0);
LiquidCrystal lcd(RS, E, D0, D1, D2, D3, D4, D5, D6, D7);
//D4, D5, D6, D7);

void setup() {
	Serial.begin(19200); // Default baudrate.
	lcd.begin(LCDW, LCDH); // Change this for other screen sizes.

	analogWrite(BR, 0); // Set maximum brightness.
/*
	lcd.print("ArduLCD Version ");
	lcd.print(VERSION);
	lcd.setCursor(0, 1);
	lcd.print("Matrix Orbital!");

	//Defaults
    lcd.autoscroll();
	*/
}

//Fast wait until byte is received, then returns it. 
char cmd;
char sget() {
	while(1) {
		cmd = Serial.read();
		if (cmd == -1) { continue; }
		return cmd;
	}
}


char in; //our received bytea
char x; //bitbucket
void loop() {
	while(1) {
		in = sget();
		
		if (in == 0xFE) {
			switch (sget()) {
				// From lcdproc
				case 0x37:
					// Return byte indicating LCD type
					Serial.write((uint8_t)9);
					Serial.flush();
					break;
				case 0x36:
					// Returns firmware revision (1 byte)
					Serial.write((uint8_t)VERSION);
					Serial.flush();
					break;
				case 0x35:
					// Return Serial number (2 bytes)
					Serial.write(0xF0);
					Serial.write(0x0F);
					Serial.flush();
					break;


				// From the manual
				case 0x33:
					// Change I2C Slave address (not implemented)
					x = sget(); //next byte is addr
					break;
				case 0x39:
					// Change the baud rate
					x = sget();
					switch(sget()) {
						case 83:
							x = 1200;
							break;
						case 41:
							x = 2400;
							break;
						case 207:
							x = 4800;
							break;
						case 103:
							x = 9600;
							break;
						case 51:
							x = 19200;
							break;
						case 34:
							x = 28800;
							break;
						case 25:
							x = 38400;
							break;
						case 15:
							x = 57600;
							break;
						case 8:
							x = 115200;
							break;
						default:
							break;
					}
					Serial.end();
					Serial.begin(x);
					break;
				case 0xA4:
					// Set custom baud rate (and pray the Oscillator is 16MHz)
					Serial.end();
					Serial.begin(sget());
					break;
				case 0xA0:
					// Set protocol (UART or I2C) for the LCD->TARGET direction
					// (so can differ from in direction). Not implemented
					x = sget();
					break;
				case 0x51:
					// The entire contents of screen are shifted up one line when
					// the end of the screen is reached. Default is on.
					lcd.autoscroll();
					break;
				case 0x52:
					// New text is written over the top line when the end of the 
					// screen is reached. Default is off (no autoscroll)
					lcd.noAutoscroll();
					break;
				case 0x58:
					// Clears the screen
					lcd.clear();
					break;
				case 0x40:
					// Changes start up message (not implemented)
					for (int x = 0; x != 80; x++)
						x = sget();
					break;
				case 0x43:
					// Enable auto line wrap (not implemented)
					break;
				case 0x44:
					// Disable auto line wrap (not implemented)
					break;
				case 0x47:
					// Set cursor position (2 parameters, column, row)
				    lcd.setCursor(sget() - 1, sget() - 1);
					break;
				case 0x48:
					// Set cursor to home position (0,0)
					lcd.setCursor(0,0);
					break;
				case 0x4C:
                	// Move cursor to the left
				    lcd.command(16);
					break;
				case 0x4D:
					// Move cursor to the right
					lcd.command(20);
					break;
				case 0x4A:
					// Enable cursor
					lcd.cursor();
					break;
				case 0x4B:
					// Disable cursor
					lcd.noCursor();
					break;
				case 0x53:
					// Enable flashing block
					lcd.blink();
					break;
				case 0x54:
					// Disable flashing block
					lcd.noBlink();
					break;
				case 0x4E:
					// Load up custom character
					lcd.command(64 + (sget() * 8));
					for ( x = 7; x != 0; x--) {
						lcd.print(sget());
					}
				case 0xC1: 
					// Save custom character (3 parameters, bank, id, data)
					// Should be saved to Ardino memory
					// Not implemented. 10 bytes
					for ( x = 0; x != 10; x++)
						sget();
					break;
				case 0xC0:
					// Load custom character bank from memory. Not implemented
					// 1 byte 
					sget();
					break;
				case 0xC2:
					// save custom char to memory (not implemented). 
					// 9 bytes
					for ( x = 0; x != 9; x++)
						sget();
					break;
				case 0x6D:
					// Initialise medium number (not implemented)
					sget();
				case 0x6F:
					// Place medium number (3 bytes
					for ( x = 0; x != 3; x++)
						sget();
					break;
				case 0x6E:
					// Loads large number (not implemented)
					break;
				case 0x23:
					// Place large number (not implemented) (2 bytes)
					sget(); sget(); 
				case 0x68:
					// Loads horizontal bar (not implemented)
					break;
				case 0x7C:
					// Place horizontal bar (not implemented) (4 bytes)
					for ( x = 0; x != 4; x++) { sget(); }
					break;
				case 0x73:
					// Init vertical bar (not implemented)
					break;
				case 0x76:
					// Load vertical bar (not implemented)
					break;
				case 0x3D:
					// Place vertical bar (not implemented) (2 byte)
					sget(); sget();
				case 0x56:
					// GPO off (1 byte)
					// gpo 1 is pin10 for us
					x = sget();
					switch (x) {
						case 0:
							digitalWrite(GPO1, LOW);
						default:
							break;
						}
				case 0x57:
					// GPO on (1 byte)
					// gpo 1 is pin10 for us
					x = sget();
					switch (x) {
						case 0:
							digitalWrite(GPO1, HIGH);
						default:
							break;
						}
				case 0xC3:
					 // Set startup GPO state (2 bytes, number, state)
					 // not implemented
					 sget(); sget();
					 break;
				default:
					// Ignore other commands, and bin parameters
					x = sget();
			}
		} else {
			lcd.write(in);
		}
  			
	}
}
