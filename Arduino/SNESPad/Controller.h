/*
  Controller class for reading data from NES / SNES controllers
*/
#ifndef CONTROLLER_H
#define CONTROLLER_H

// Cheat a bit and use the Arduino header
#include "Arduino.h"

// Number of clock cycles to send to the controller, 8 for NES, 16 for SNES
#define CLOCK_CYCLES 8

class Controller {
  private:
  
	// button states are stored in a single byte, will need another to support SNES
    byte data;
    
    // I/O pins
    int latch_pin;
    int clock_pin;
    int data_pin;
    
  public:
  
    // Initialize the I/O pins
    void attach(int latch, int clock, int data);

    // Poll the controller and update the data byte
    void poll();
    
    Controller();
};

#endif
