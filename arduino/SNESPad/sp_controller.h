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
#ifndef _sp_controller_h
#define _sp_controller_h

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/** Number of clock cycles to send to the controller, 8 for NES, 16 for SNES */
#define BUTTON_COUNT 8


/** SNES controller data structure */
typedef struct sp_controller {
    uint8_t pin_latch;
    uint8_t pin_clock;
    uint8_t pin_data;
    uint8_t data;
} sp_controller;

/** SNES controller buttons */
typedef enum {
    UP     = 0x0001,    //< D-Pad Up
    DOWN   = 0x0002,    //< D-Pad Down
    LEFT   = 0x0004,    //< D-Pad Left
    RIGHT  = 0x0008,    //< D-Pad Right
    A      = 0x0010,    //< A Button
    B      = 0x0020,    //< B Button
    START  = 0x0040,    //< Start
    SELECT = 0x0080,    //< Select
    ULEFT  = 0x0100,    //< D-Pad Up-Left
    URIGHT = 0x0200,    //< D-Pad Up-Right
    DLEFT  = 0x0400,    //< D-Pad Down-Left
    DRIGHT = 0x0800,    //< D-Pad Down-Right
    X      = 0x1000,    //< X Button
    Y      = 0x2000,    //< Y Button
    LBUMP  = 0x4000,    //< Left Bumper
    RBUMP  = 0x8000     //< Right Bumper
} sp_button ;


/**
 * \brief Return a pointer to a new controller
 */
sp_controller *sp_controller_create();

/**
 * \brief Destroy an existing controller
 */
void sp_controller_destroy(sp_controller *p);

/**
 * \brief Configure the controller connection to the MCU
 */
void sp_controller_attach(sp_controller *p, uint8_t latch, uint8_t clock, uint8_t data);

/**
 * \brief Read the current state of the controller into the controller object
 */
void sp_controller_poll(sp_controller *p);

/**
 * \brief Get the current state of a button
 */
uint8_t sp_controller_readbutton(sp_controller *p, sp_button button_type);

/*
 * \brief Returns the raw data value as a 8-bit number
 */
uint8_t sp_controller_data(sp_controller *p);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // snespad_controller_h
