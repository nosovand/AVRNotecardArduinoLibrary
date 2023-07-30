#include "notecard_AVR.h"
#include "memoryTest.h"
#include "debugConsole.hpp"

void setup() {
  debugConsole.init(DEBUG_CONSOLE_MODE_SPI);
  initFreeMemory();
  //Serial.begin(115200);
  debugConsole.println(F("Starting program"));
  SHOW_SIZES
  // initialize notecard library
  AVRNotecardInit(true);
  debugConsole.println(F("############## SOFTWARE VERSION 0.1.0 #############"));
}

void loop() {

  //SHOW_SIZES
  // check for update
  if(debugConsole.receivedString("++CU")){
    AVRNotecardCheckForUpdate();
  }
  //delay(15000);
}
