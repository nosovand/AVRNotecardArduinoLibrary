#include "Arduino.h"
#include "AVRNotecardParameters.hpp"

AVRNotecardParameters notecardParameters{
  NOTECARD_LOG_RELEASE_MESSSAGES,
  NOTECARD_DEBUG_STREAM_OFF,
  NOTE_SERIAL,
  DEBUG_SERIAL,
  GPS_ON,
  GPS_CONNECTION_PERIOD_60_SEC,
};
