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
  AVRNotecardInit();
  debugConsole.println(F("############## SOFTWARE VERSION 0.3.0 #############"));
}

void loop() {

  //SHOW_SIZES
  // check for update
  if(debugConsole.available()){
    debugConsole.print("received command: ");
    debugConsole.println(debugConsole.readString());
    if(debugConsole.receivedString("++CU")){
      AVRNotecardCheckForUpdate();
    }
    else {
      debugConsole.println("For checking for a new update print '++CU'");
    }
  }
  //delay(15000);
}
