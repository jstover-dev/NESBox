/*
 * main.h
 *
 *  Created on: 26/07/2014
 *      Author: josh
 */

#ifndef MAIN_H_
#define MAIN_H_

#define PIN_LATCH BIT0
#define PIN_PULSE BIT1
#define PIN_DATA BIT2
#define BUTTON BIT3
#define PIN_TIMER BIT6

#define wait12(){ __delay_cycles(91); }
#define wait6(){ __delay_cycles(44); }

inline void latch_signal();
inline void pulse8();
inline void square_12us();
#endif
