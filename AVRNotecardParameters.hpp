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

struct AVRNotecardParameters{
  uint8_t libraryMode;
  bool notecardDebugStream;
};

extern AVRNotecardParameters notecardParameters;

