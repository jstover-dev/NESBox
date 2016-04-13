/*
 *  PadType class for reading data from NES / SNES controllers
 *
 *  SNESPad.h
 *
 *  C++ Class for reading the state of a SNES controller.
 *
 *  Copyright (c) 2015 Josh Stover
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

#include <Arduino.h>
#include <SNESPad.h>

uint8_t SNESPad::count = 0;

SNESPad::SNESPad(){
	SNESPad::count++;
	id = SNESPad::count;
}

void SNESPad::attach(NESBox::PadType type, int latch_pin, int clock_pin, int data_pin) {
	controller = type;
	string[static_cast<uint8_t>(controller)+1];
	pins.latch = latch_pin;
	pins.clock = clock_pin;
	pins.data = data_pin;
    pinMode(pins.latch, OUTPUT);
    pinMode(pins.clock, OUTPUT);
    pinMode(pins.data, INPUT);
	digitalWrite(pins.data, HIGH); // pullup
	digitalWrite(pins.latch, LOW);
	digitalWrite(pins.clock, LOW);
}

void SNESPad::poll() {
	// LATCH
    digitalWrite(pins.latch, HIGH);
    delayMicroseconds(6);
    digitalWrite(pins.latch, LOW);
	delayMicroseconds(1);

    // CLK line goes high and DATA line is read
    uint8_t clk_cycle;
	uint8_t buttons = static_cast<uint8_t>(controller);
    for(clk_cycle=0; clk_cycle<buttons; clk_cycle++) {
        data = data << 1;
        data |= (digitalRead(pins.data)^1);
        digitalWrite(pins.clock, HIGH);
        delayMicroseconds(7);
        digitalWrite(pins.clock, LOW);
        delayMicroseconds(2);
    }
}

int SNESPad::getData() {
	return data;
}

uint8_t SNESPad::getID(){
	return id;
}

uint8_t SNESPad::getPadTypeCount(){
	return SNESPad::count;
}

char* SNESPad::toString(){
	char *p = string;
	for(uint8_t btn=0; btn<16; btn++){
		*p = 65+btn;
		p++;
	}
	*p = NULL;
	return string;
}

