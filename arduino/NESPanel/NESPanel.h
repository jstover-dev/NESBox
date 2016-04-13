/*
 *  NES Power/Reset/LED board interface
 *
 *  NESPanel.h
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
#ifndef _NESPanel_h
#define _NESPanel_h

#include <stdint.h>


class NESPanel {

private:
	struct Pins {
		uint8_t led;
		uint8_t power;
		uint8_t reset;
	};
	Pins pins;
	
	bool led_on;

public:
	NESPanel();
	void attach(int, int, int);
	void setLEDState(bool);
	bool powerButtonPushed();
	bool resetButtonPushed(bool = false);
};


#endif // _NESPanel_h
