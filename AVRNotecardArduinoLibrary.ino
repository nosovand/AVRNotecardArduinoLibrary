#include "notecard_AVR.h"
#include "memoryTest.h"
#include "debugConsole.hpp"

void setup() {
  debugConsole.init(DEBUG_CONSOLE_MODE_UART_AND_SPI);
  initFreeMemory();
  //Serial.begin(115200);
  debugConsole.println(F("Starting program"));
  SHOW_SIZES
  // initialize notecard library
  AVRNotecardInit(true);

}

void loop() {
  debugConsole.println(F("############## SOFTWARE VERSION 0.1.0 #############"));
  SHOW_SIZES
  // check for update
  AVRNotecardCheckForUpdate();
  delay(15000);
}
