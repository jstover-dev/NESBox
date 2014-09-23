#include "Controller.h"

Controller::Controller(){}

void Controller::attach(int latch, int clock, int data) {
   latch_pin = latch;
   clock_pin = clock;
   data_pin = data;
   pinMode(latch_pin, OUTPUT);
   pinMode(clock_pin, OUTPUT);
   pinMode(data_pin, INPUT);
}

void Controller::poll(){
  // latch the controller
  digitalWrite(latch_pin, HIGH);
  delayMicroseconds(8);
  digitalWrite(latch_pin, LOW);
  delayMicroseconds(2);
  
  // CLK line goes high and DATA line is read
  for(int clockCycle = 0; clockCycle<CLOCK_CYCLES; clockCycle++) {
    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(2);
    data = data << 1;
    data |= digitalRead(data_pin);
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(2);
  }
}
