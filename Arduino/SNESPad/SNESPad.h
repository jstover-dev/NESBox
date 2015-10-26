/*
 *  Controller class for reading data from NES / SNES controllers
 *
 *  SNESPad.h
 *
 *  C++ Class for reading the state of a SNES controller.
 *  This class is nothing but a wrapper around the snespad C functions.
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

#include "sp_controller.h"

namespace snespad {

    class SNESPad : private sp_controller {

    public:
		// create a new instance attached to the given Arduino pins
        SNESPad(int latch_pin, int clock_pin, int data_pin);
        
		// read the current button state from the controller
		void poll();
		
		// get the current button state
        int getData();

        // copy assignment from sp_controller
        SNESPad& operator=(const sp_controller& c);
		
		~SNESPad();
    };
}

 #endif
