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
  debugConsole.println(F("############## SOFTWARE VERSION 0.3.1 #############"));
}


void loop() {
  char c;
  int num;
  //SHOW_SIZES
  // check for update
  if(debugConsole.available()){
    SHOW_SIZES
    debugConsole.print("received command: ");
    debugConsole.println(debugConsole.readString());
    debugConsole.readParsedString(c, num);
    if(c == 'C' && num == 1){
      AVRNotecardCheckForUpdate();
    }
    else if(c == 'M' && num == 1){
      AVRNotecardSendStringMessage(F("testFile.qo"), "myMessage", F("hello from notecard"));
    }
    else if(c == 'M' && num == 2){
      AVRNotecardSendStringMessage(F("testFile"), "myMessage", F("hello from notecard"));
    }
    else {
      debugConsole.println("For sending test message print '++M1'");
      debugConsole.println("For sending unvalid test message print '++M2'");
      debugConsole.println("For checking for a new update print '++C1'");
    }
  }
  //delay(15000);
}
