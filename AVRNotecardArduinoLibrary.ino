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
  Serial.println(F("############# feature/alarmNotification ############"));
  SHOW_SIZES
  // check for update
  //AVRNotecardCheckForUpdate();
  // check serial input
  // if serial input is a/b, send alarm notification
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == 'a') {
      AVRNotecardSendAlarmNotification(ALARM_A);
    } else if (input == 'b') {
      AVRNotecardSendAlarmNotification(ALARM_B);
    }
  }
  delay(15000);
}
