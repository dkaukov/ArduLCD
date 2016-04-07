// vim ts=4 noexpandtab ai
#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

// based on info got from here:
// http://lcdproc.cvs.sourceforge.net/viewvc/lcdproc/lcdproc/server/drivers/hd44780-serial.h?content-type=text%2Fplain

// Your LCD dimensions, in number of chars WxH
const int LCDW = 20;
const int LCDH = 4;

// include the LCD library code:
#include <LiquidCrystal.h>

// And this is ths Hardware serial port (which is also bound to the UART->USB chip)
#include <HardwareSerial.cpp>

// initialize the library with the numbers of the interface pins
/*  Note, while all the ardu documentation and schematics show 4-bit operation. 
	It seems the library actually supports both 8-bit and 4-bit mode. It attempts
	8-bit, then falls back to 4-bit */
LiquidCrystal lcd(12, 11, 9,8,7,6, 5, 4, 3, 2);
//LiquidCrystal lcd(12, 11,  5, 4, 3, 2); //4bit mode

void setup() {
	// set up the LCD's number of columns and rows:
	lcd.begin(LCDW, LCDH);
	// set up serial
	Serial.begin(57600); 
	lcd.display();
	lcd.noCursor();
	lcd.clear();

}

void loop() {
	// Todo: Investigate how to to interrupt driven serial I/O, like on PICs.
	// sitting in a tight loop like this is quite wasteful. 
	char cmd; //will hold our sent command
	while (1) {
		
		cmd = Serial.read();
		if (cmd == -1) { continue; }
		switch(cmd) {
			case 0x11:
				// Write LCD instruction -- used by lcdproc driver
                while (1) {
					cmd = Serial.read();
					if (cmd != -1) { break; }
                }   
                lcd.command(cmd);
				break;
			case 0x12:
				// Write LCD Data -- used by lcdproc driver
                while (1) {
					cmd = Serial.read();
					if (cmd != -1) { break; }
                }   
                lcd.write(cmd);
				break;
			case 0x00:
                while (1) {
					cmd = Serial.read();
					if (cmd != -1) { break; }
                }   
                lcd.write(cmd);
				break;

		
			default:
				lcd.write(cmd);
				break;

		}}}

