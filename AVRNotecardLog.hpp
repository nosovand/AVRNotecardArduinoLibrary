#ifndef AVRNOTECARDLOG_H
#define AVRNOTECARDLOG_H

#include "Arduino.h"
#include "AVRNotecardParameters.hpp"

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