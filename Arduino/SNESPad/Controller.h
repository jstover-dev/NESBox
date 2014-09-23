/*
  Controller class for reading data from NES / SNES controllers
  Constructor accepts the pin numbers of the latch, clock and data lines
*/
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Arduino.h"

// Number of clock cycles to send to the controller, 8 for NES, 16 for SNES
#define CLOCK_CYCLES 8

class Controller {
  private:
    byte data;
    int latch_pin;
    int clock_pin;
    int data_pin;
  public:
    Controller();
    void poll();
    void attach(int latch, int clock, int data);
};

#endif
