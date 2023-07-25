#ifndef AVR_DEBUG_CONSOLE_HPP
#define AVR_DEBUG_CONSOLE_HPP

#define DEBUG_CONSOLE_MODE_SPI 0
#define DEBUG_CONSOLE_MODE_UART 1
#define DEBUG_CONSOLE_MODE_UART_AND_SPI 2

#include "Arduino.h"

class DebugConsole
{
private:
    uint8_t debugConsoleMode;
public:
    DebugConsole(/* args */);
    ~DebugConsole();
    void init(uint8_t mode, long serialBaud = 115200);
    void print(char *message);
    void println(char *message);
    void print(const char *message);
    void println(const char *message);
    void print(double message);
    void println(double message);
    void print(const __FlashStringHelper *message);
    void println(const __FlashStringHelper *message);
    uint8_t getConsoleMode();
};

extern DebugConsole debugConsole;

#endif