#ifndef AVRNOTECARDLOG_H
#define AVRNOTECARDLOG_H

#include "Arduino.h"

#define NOTECARD_LOG_EVERYTHING 0
#define NOTECARD_LOG_DEBUG_MESSSAGES 1
#define NOTECARD_LOG_RELEASE_MESSSAGES 2
#define NOTECARD_LOG_ERRORS_ONLY 3
#define NOTECARD_LOG_NOTHING 4

#define DEBUG_LOG 0
#define RELEASE_LOG 1
#define ERROR_LOG 2


class AVRNotecardLog
{
private:
    uint8_t notecardLogMode;
public:
    AVRNotecardLog(uint8_t logMode);
    ~AVRNotecardLog();

    void print(char *message, uint8_t loggingLevel = DEBUG_LOG);
    void println(char *message, uint8_t loggingLevel = DEBUG_LOG);
    void print(const char *message, uint8_t loggingLevel = DEBUG_LOG);
    void println(const char *message, uint8_t loggingLevel = DEBUG_LOG);
    void print(double message, uint8_t loggingLevel = DEBUG_LOG);
    void println(double message, uint8_t loggingLevel = DEBUG_LOG);
    void print(String& message, uint8_t loggingLevel = DEBUG_LOG);
    void println(String& message, uint8_t loggingLevel = DEBUG_LOG);
    void print(const __FlashStringHelper *message, uint8_t loggingLevel = DEBUG_LOG);
    void println(const __FlashStringHelper *message, uint8_t loggingLevel = DEBUG_LOG);
    void println(uint8_t loggingLevel = DEBUG_LOG);

    void setMode(uint8_t logMode);
};

extern AVRNotecardLog avrNotecardLog;

#endif