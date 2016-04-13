#include "Wire.h"
#include <SNESPad.h>
#include <NESPanel.h>

/* ==================== Hardware Setup ==================== */
// Controller 1
#define P1_PULSE 4      // Red
#define P1_LATCH 5      // Orange
#define P1_DATA  6      // Yellow

// Controller 2
#define P2_PULSE 7      // Red
#define P2_LATCH 8      // Orange
#define P2_DATA  9      // Yellow

// Front Panel
#define PANEL_LED   10  // ?
#define PANEL_POWER 11  // ?
#define PANEL_RESET 12  // ?


/* ==================== OpCodes ==================== */
#define CMD_PAD_STATE    0x80
#define CMD_PAD_1_STATE  0x81
#define CMD_PAD_2_STATE  0x82
#define CMD_RESET_STATE  0x83
#define CMD_POWER_STATE  0x84
#define CMD_LED_ON       0x85
#define CMD_LED_OFF      0x86


/* ==================== Variables ==================== */
SNESPad Pad1;
SNESPad Pad2;
NESPanel Panel;
uint8_t incomingOpCode = 0;
uint16_t padData = 0;

/* ==================== Setup Functions ==================== */

void setupTimer(){
    noInterrupts();
    TCNT1 = 0;                // reset the timer counter
    TCCR1A = 0x00;            // Normal mode
    TCCR1B = 0x02;            // 16MHz with /8 prescaler, .5us increments
    OCR1A = 33333;            // 16.66 ms / 0.5us == 3333 (60Hz) 
    TCCR1B |= (1 << WGM12);   // CTC mode
    TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
    interrupts();             // enable all interrupts
}

void setupFrontPanel(){
  Panel.attach(PANEL_LED, PANEL_POWER, PANEL_RESET);
  Panel.setLEDState(true);
}

void setupControllers(){
    Pad1.attach(Controller::NES, P1_LATCH, P1_PULSE, P1_DATA);
    Pad2.attach(Controller::NES, P2_LATCH, P2_PULSE, P2_DATA);
}

/* ==================== Main ==================== */

void setup() {
    Serial.begin(9600);
    setupFrontPanel();
    setupControllers();
    setupTimer();
}


void loop(){

  if (Serial.available()){
    incomingOpCode = Serial.read();
    
    switch(incomingOpCode){
      
      case CMD_PAD_1_STATE:
        Serial.write(Pad1.getData()<<8);
        break;
 
      case CMD_PAD_2_STATE:
        Serial.write(Pad2.getData());
        break;

      case CMD_RESET_STATE:
        Serial.write(Panel.resetButtonPushed());
        break;
        
      case CMD_POWER_STATE:
        Serial.write(Panel.powerButtonPushed());
        break;
        
      case CMD_LED_ON:
        Panel.setLEDState(true);
        break;
        
      case CMD_LED_OFF:
        Panel.setLEDState(false);
        break;
    }

  }
}


/* ==================== Interrupts ==================== */

// Poll the Controllers at 60Hz
ISR(TIMER1_COMPA_vect) {
    noInterrupts();
    Pad1.poll();
    Pad2.poll();
    interrupts();
}

/* ==================== Utility ==================== */

void printStatus(){
  Serial.print("| PWR:");  Serial.print(Panel.powerButtonPushed());
  Serial.print(" | RST:"); Serial.print(Panel.resetButtonPushed());
  Serial.print(" | Pad1:"); print_binary(Pad1.getData(), 8);
  Serial.print(" | Pad2:"); print_binary(Pad2.getData(), 8);
  Serial.println(" |");
}

void print_binary(int v, int width) {
  int mask=0, n;
  for (n=1; n<=width; n++) {mask=(mask<<1)|0x0001;}
  v = v & mask;
  while(width){Serial.print((v&(0x0001<<width-1))?"1":"0");--width;}
}
