/*----------------------- notecard AVR header file -------------------------|
 | libabry version 0.3.1                                                    |
 |                  																												|
 | 2023 by Andrey Nosov, https://github.com/nosovand                        |
 |                                                                          |
 | C++ library for easy and memory friendly communication                   |
 | with Notecard on AVR mega processors.                                    |
 |-------------------------------------------------------------------------*/

#include <Notecard.h>
#include "AVRNotecardLog.hpp"

#define RETURN_SUCCESS 1
#define RETURN_ERROR 0
#define DEBUG_MODE true
#define RELEASE_MODE false
#define ALARM_A F("Alarm A")
#define ALARM_B F("Alarm B")


extern Notecard notecard; //test

int AVRNotecardInit();
int AVRInitNotecardGPS();
int AVRStartNotecardSync();
int AVRIsNotecardConnected();
long AVRCheckNotecardDFUMode(long maxUpdateSize, char* imageMD5);
int AVRSetNotecardToDFU();
int AVRReturnNotecardFromDFU(bool success);

char* AVRRetrieveNotecardPayloadChunk(int& numOfErrors, long offset, int& chunkSize);
void AVRNotecardCheckForUpdate();

uint8_t AVRNotecardSendStringMessage(const __FlashStringHelper* fileName, const char* const stringName, const __FlashStringHelper* string);

J* AVRNoteNewRequest(const __FlashStringHelper* request);
N_CJSON_PUBLIC(J*) AVRJAddStringToObject(J* const object, const char* const name, const __FlashStringHelper* string);

void moveStringToRAM(const char* source, char** destination);
void freeRAMString(char** str);

uint8_t memoryError();