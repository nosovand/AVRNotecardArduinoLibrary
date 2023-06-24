#include "notecard_AVR.h"
#include "memoryTest.h"

void setup() {
  initFreeMemory();
  Serial.begin(115200);
  Serial.println(F("Starting program"));
  SHOW_SIZES
  // initialize notecard library
  AVRNotecardInit(true);

}

void loop() {
  Serial.println(F("############## SOFTWARE VERSION 0.0.1 #############"));
  SHOW_SIZES
  // check for update
  AVRNotecardCheckForUpdate();
  delay(15000);
}
