#include <SNESPad.h>

#define LED   13
#define P1_LATCH 6
#define P1_CLOCK 7
#define P1_DATA  8
#define P2_LATCH 9
#define P2_CLOCK 10
#define P2_DATA  11

snespad::SNESPad Pad1;
snespad::SNESPad Pad2;

void setup(){
  pinMode(LED, OUTPUT);
  
  Pad1.attach(P1_LATCH, P1_CLOCK, P1_DATA);
  Pad2.attach(P2_LATCH, P2_CLOCK, P2_DATA);
  
  // 60Hz (16.66ms) Timer1
  noInterrupts();
  TCNT1 = 0;                // reset the timer counter
  TCCR1A = 0x00;            // Normal mode, whatever that means
  TCCR1B = 0x02;            // 16MHz with /8 prescaler, .5us increments
  OCR1A = 33333;            // 16.66 ms / 0.5us == 3333
  TCCR1B |= (1 << WGM12);   // CTC mode
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

// 60Hz Timer ISR
ISR(TIMER1_COMPA_vect) {
  Pad1.poll();
  Pad2.poll();
}

// Blink your LED if you're alive
void loop(){
  digitalWrite(LED, HIGH);
  delayMicroseconds(30);
  digitalWrite(LED, LOW);
  delay(1000);
}

