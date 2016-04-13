/*
 *  PadType class for reading data from NES / SNES controllers
 *
 *  SNESPad.h
 *
 *  Arduino Class for reading the state of a SNES controller.
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
#ifndef SNESPad_h
#define SNESPad_h

#include <stdint.h>

namespace NESBox {
	enum PadType { Unknown=0, NES=8, SNES=16 };
}

class SNESPad {

private:
    static uint8_t count;
	NESBox::PadType controller;
	struct Pins {
		uint8_t latch;
		uint8_t clock;
		uint8_t data;
	};
	Pins pins;
	uint8_t data;
	uint8_t id;
	char *string;

public:
    SNESPad();
	static uint8_t getPadTypeCount();
    void attach(NESBox::PadType, int, int, int);
    void poll();
    int getData();
    uint8_t getID();
	char *toString();
};

#endif
