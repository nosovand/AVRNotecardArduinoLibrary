#include <Notecard.h>

#define RETURN_SUCCESS 1
#define RETURN_ERROR 0
#define NOTE_PRODUCT_UID F("com.gmail.work.kp.an:firsttest")
#define usbSerial Serial
#define txRxPinsSerial Serial2
#define DEBUG_MODE true
#define RELEASE_MODE false
#define ALARM_A F("Alarm A")
#define ALARM_B F("Alarm B")





int AVRNotecardInit(bool debugMode);
int AVRStartNotecardSync();
int AVRIsNotecardConnected();
int AVRCheckNotecatdDFUMode(long maxUpdateSize, char* imageMD5);
int AVRSetNotecardToDFU(int maxWaitTime_sec);
int AVRReturnNotecardFromDFU();

unsigned char* AVRRetrieveNotecardPayloadChunk(int& numOfErrors, long offset, int& chunkSize);
void AVRNotecardCheckForUpdate();

J* AVRNoteNewRequest(const __FlashStringHelper* request);
N_CJSON_PUBLIC(J*) AVRJAddStringToObject(J* const object, const char* const name, const __FlashStringHelper* string);

void moveStringToRAM(const char* source, char** destination);
void freeRAMString(char** str);
