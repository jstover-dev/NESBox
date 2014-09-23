#include  "msp430g2553.h"
#include "main.h"

volatile unsigned int pause = 0;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;	// Stop WDT

	// DCO
	DCOCTL = CALDCO_8MHZ;
	BCSCTL1 = CALBC1_8MHZ;

	// Timer_A
	TA1CCTL0 = CCIE;					// Enable counter interrupts
	TA1CTL = TASSEL_2 + MC_1 + ID_2;	// SMCLK/2, UP Mode
	TA1CCR0 = 0x81C2;					// 60Hz

	// Output pins
	P1DIR |= PIN_TIMER + PIN_LATCH + PIN_PULSE;
	P1OUT &= ~(PIN_TIMER + PIN_PULSE + PIN_LATCH);

	// Button
	P1DIR |= BIT0; //debug
	P1REN |= BUTTON;
	P1IE |= BUTTON;
	P1IFG &= ~BUTTON;

	// Low Power Mode
	__bis_SR_register(LPM0_bits + GIE);
}

/* start/stop when a button pressed */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
	P1OUT &= ~(PIN_TIMER + PIN_PULSE + PIN_LATCH);
	TA1CTL ^= MC_1;
	P1IFG &= ~BUTTON;
}

/* 60Hz Timer ISR */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0(void) {
	P1OUT ^= PIN_TIMER;
	latch_signal();
	wait6();
	pulse8();
}

inline void latch_signal(){
	P1OUT ^= PIN_LATCH;
	wait12();
	P1OUT ^= PIN_LATCH;
}

inline void square_12us(){
	P1OUT ^= PIN_PULSE;
	if (P1IN & PIN_DATA) {
		// pin is high
	} else {
		// pin is low
	}
	__delay_cycles(44);
	P1OUT ^= PIN_PULSE;
	__delay_cycles(43);
}

inline void pulse8(){
	square_12us();
	square_12us();
	square_12us();
	square_12us();
	square_12us();
	square_12us();
	square_12us();
	square_12us();
}
