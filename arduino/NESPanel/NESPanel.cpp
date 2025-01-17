/*
 *  NES Power/Reset/LED board interface
 *
 *  NESPanel.cpp
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
#include "NESPanel.h"

NESPanel::NESPanel(){}

void NESPanel::attach(int led, int power, int reset){
	pins.led = led;
	pins.power = power;
	pins.reset = reset;
	pinMode(led, OUTPUT);
	pinMode(power, INPUT);
	pinMode(reset, INPUT);
	digitalWrite(led, LOW);
	led_on = false;
}

void NESPanel::setLEDState(bool state){
	digitalWrite(pins.led, state);
	led_on = state;
}

bool NESPanel::powerButtonPushed(){
	return digitalRead(pins.power);
}

bool NESPanel::resetButtonPushed(bool force){
	if (!force){
		return digitalRead(pins.reset);

	} else if (led_on) {
		return digitalRead(pins.reset);
	}
	digitalWrite(pins.led, HIGH);
	bool r = digitalRead(pins.reset);
	digitalWrite(pins.led, LOW);
	return r;
}
