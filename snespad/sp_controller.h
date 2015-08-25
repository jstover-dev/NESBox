/*
 *  Controller class for reading data from NES / SNES controllers
 *
 *  snespad_controller.h
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
#ifndef sp_controller_h
#define sp_controller_h

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>
#include <stdint.h>
#include <Arduino.h>

// Number of clock cycles to send to the controller, 8 for NES, 16 for SNES
#define BUTTON_COUNT 8

typedef struct sp_controller {
    uint8_t pin_latch;
    uint8_t pin_clock;
    uint8_t pin_data;
    uint8_t data;
} sp_controller;

typedef enum {
    UP     = 0x01,
    DOWN   = 0x02,
    LEFT   = 0x04,
    RIGHT  = 0x08,
    A      = 0x10,
    B      = 0x20,
    START  = 0x40,
    SELECT = 0x80
} sp_button ;


sp_controller *sp_controller_create() {
    sp_controller *c = (sp_controller*) malloc(sizeof(sp_controller));
    return c;
}

void sp_controller_destroy(sp_controller *p) {
    free(p);
}

void sp_controller_attach(sp_controller *p, uint8_t latch, uint8_t clock, uint8_t data) {
    p->pin_latch = latch;
    p->pin_clock = clock;
    p->pin_data = data;
    pinMode(latch, OUTPUT);
    pinMode(clock, OUTPUT);
    pinMode(data, INPUT);
}

void sp_controller_poll(sp_controller *p) {
    digitalWrite(p->pin_latch, HIGH);
    delayMicroseconds(8);
    digitalWrite(p->pin_latch, LOW);

    // CLK line goes high and DATA line is read
    uint8_t clk_cycle;
    for(clk_cycle=0; clk_cycle<BUTTON_COUNT; clk_cycle++) {
        digitalWrite(p->pin_clock, HIGH);
        delayMicroseconds(2);
        p->data = p->data << 1;
        p->data |= digitalRead(p->pin_data);
        digitalWrite(p->pin_clock, LOW);
        delayMicroseconds(2);
    }
}


uint8_t sp_controller_readbutton(sp_controller *p, sp_button button_type) {
    return (p->data & button_type)==button_type;
}

uint8_t sp_controller_data(sp_controller *p) {
    return p->data;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // snespad_controller_h
