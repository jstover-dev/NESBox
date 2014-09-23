/*
  NES / SNES Controller Handler for ATMega328P
  Author: Josh Stover
*/
#include "Controller.h"

#define LED   13
#define P1_LATCH 6
#define P1_CLOCK 7
#define P1_DATA  8
#define P2_LATCH 9
#define P2_CLOCK 10
#define P2_DATA  11

Controller Pad1;
Controller Pad2;

void setup(){
  pinMode(LED, OUTPUT);
  pinMode(P1_LATCH, OUTPUT);
  pinMode(P1_CLOCK, OUTPUT);
  pinMode(P1_DATA, INPUT);
  
  Pad1.attach(P1_LATCH, P1_CLOCK, P1_DATA);
  Pad2.attach(P2_LATCH, P2_CLOCK, P2_DATA);
  
  noInterrupts();
  TCNT1 = 0;                // reset the timer counter
  TCCR1A = 0x00;            // Normal mode, whatever that means
  TCCR1B = 0x02;            // 16MHz with prescaler, .5us increments
  OCR1A = 33333;            // 16.66 ms
  TCCR1B |= (1 << WGM12);   // CTC mode
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

// 60Hz Timer ISR
ISR(TIMER1_COMPA_vect) {
  Pad1.poll();
}

void loop(){
  digitalWrite(LED, HIGH);
  delayMicroseconds(30);
  digitalWrite(LED, LOW);
  delay(1000);
}

