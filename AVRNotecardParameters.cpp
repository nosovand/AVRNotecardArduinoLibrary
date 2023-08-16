#include "Arduino.h"
#include "AVRNotecardParameters.hpp"

AVRNotecardParameters notecardParameters{
  NOTECARD_LOG_RELEASE_MESSSAGES,
  NOTECARD_DEBUG_STREAM_OFF,
  NOTE_SERIAL_0,
  DEBUG_SERIAL_3,
  GPS_ON,
  GPS_CONNECTION_PERIOD_60_SEC,
  DFU_WAIT_2_MIN,
  CHUNK_SIZE_512_BYTES,
  INBOUND_MESSAGES_PERIOD_2_MIN,
};
