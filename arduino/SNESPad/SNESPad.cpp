/*
 *  Controller class for reading data from NES / SNES controllers
 *
 *  SNESPad.cpp
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
#define DEBUG

#ifdef DEBUG
    #include <Arduino.h>
    #include <SerialDebug.h>
#endif

#include "SNESPad.h"
#include "sp_controller.h"


SNESPad::SNESPad(){
    SNESPad::count++;
    controller_id = SNESPad::count;
}

SNESPad::~SNESPad(){
    sp_controller_destroy(this);
}

void SNESPad::attach(int latch_pin, int clock_pin, int data_pin) {
    sp_controller_attach(this, latch_pin, clock_pin, data_pin);
        _Debug
        _Debug_print("SNESPad: ") _Debug_printlnf(this->getID(), HEX)
}

void SNESPad::poll() {
    sp_controller_poll(this);
}

int SNESPad::getData() { return sp_controller_data(this); }

uint8_t SNESPad::getID() { return this->controller_id; }

uint8_t SNESPad::count = 0;
