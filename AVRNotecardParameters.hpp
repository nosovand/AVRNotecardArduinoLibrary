#define NOTECARD_LIBRARY_DEBUG_MODE 0
#define NOTECARD_LIBRARY_RELEASE_MODE 1
#define NOTECARD_LIBRARY_NO_PRINT_MODE 2

#define NOTECARD_DEBUG_STREAM_ON true
#define NOTECARD_DEBUG_STREAM_OFF false

struct AVRNotecardParameters{
  uint8_t libraryMode;
  bool notecardDebugStream;
};

extern AVRNotecardParameters notecardParameters;

