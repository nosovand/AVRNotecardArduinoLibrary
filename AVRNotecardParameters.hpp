#include <Arduino.h>

#define NOTECARD_LOG_EVERYTHING 0
#define NOTECARD_LOG_DEBUG_MESSSAGES 1
#define NOTECARD_LOG_RELEASE_MESSSAGES 2
#define NOTECARD_LOG_ERRORS_ONLY 3
#define NOTECARD_LOG_NOTHING 4

#define DEBUG_LOG 0
#define RELEASE_LOG 1
#define ERROR_LOG 2

#define NOTECARD_DEBUG_STREAM_ON true
#define NOTECARD_DEBUG_STREAM_OFF false

#define NOTE_PRODUCT_UID F("com.gmail.work.kp.an:firsttest")

#define NOTE_SERIAL_0 Serial
#define NOTE_SERIAL_1 Serial1
#define NOTE_SERIAL_2 Serial2
#define NOTE_SERIAL_3 Serial3

#define DEBUG_SERIAL_0 Serial
#define DEBUG_SERIAL_1 Serial1
#define DEBUG_SERIAL_2 Serial2
#define DEBUG_SERIAL_3 Serial3

#define GPS_ON true
#define GPS_OFF false

#define GPS_CONNECTION_PERIOD_30_SEC 30
#define GPS_CONNECTION_PERIOD_60_SEC 60
#define GPS_CONNECTION_PERIOD_5_MIN 300
#define GPS_CONNECTION_PERIOD_10_MIN 600

#define DFU_WAIT_2_MIN 120

#define CHUNK_SIZE_256_BYTES 256
#define CHUNK_SIZE_512_BYTES 512
#define CHUNK_SIZE_1024_BYTES 1024

#define INBOUND_MESSAGES_PERIOD_1_MIN 1
#define INBOUND_MESSAGES_PERIOD_2_MIN 2
#define INBOUND_MESSAGES_PERIOD_5_MIN 5
#define INBOUND_MESSAGES_PERIOD_1_HOUR 60

struct AVRNotecardParameters{
  uint8_t libraryMode;
  bool notecardDebugStream;
  HardwareSerial& notecardSerial;
  HardwareSerial& debugSerial;
  bool gpsMode;
  int gpsPeriod; 
  int dfuWait;
  int chunckSize;
  int inboundPeriod;
};

extern AVRNotecardParameters notecardParameters;

