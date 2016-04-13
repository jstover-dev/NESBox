#include <SNESPad.h>

#define P1_LATCH 6
#define P1_CLOCK 7
#define P1_DATA  8
#define P2_LATCH 9
#define P2_CLOCK 10
#define P2_DATA  11

snespad::SNESPad Pad1;
snespad::SNESPad Pad2;

void setup(){
  Pad1.attach(P1_LATCH, P1_CLOCK, P1_DATA);
  Pad2.attach(P2_LATCH, P2_CLOCK, P2_DATA);
}

void loop(){
  Pad2.poll();
  Pad1.poll();
  delay(1000);
}

