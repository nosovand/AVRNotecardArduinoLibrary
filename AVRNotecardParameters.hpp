#define NOTECARD_DEBUG_STREAM_ON true
#define NOTECARD_DEBUG_STREAM_OFF false

struct AVRNotecardParameters{
  uint8_t libraryMode;
  bool notecardDebugStream;
};

extern AVRNotecardParameters notecardParameters;

