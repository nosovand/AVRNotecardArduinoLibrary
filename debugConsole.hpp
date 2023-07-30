/*----------------------- debugConsole header file -------------------------|
 | libabry version 0.1.0                                                    |
 |                  																												|
 | 2023 by Andrey Nosov, https://github.com/nosovand                        |
 | Released to public domain                                                |
 |                                                                          |
 | This library allows you to use SPI and/or Serial to print messages       |
 | from your AVR device using a single debug console.                       |
 |                                                                          |
 | For SPI communication an external Mega 2560 board is required running    |
 | SPITranslator software.                                                  |
 |-------------------------------------------------------------------------*/

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
    void print(String& message);
    void println(String& message);
    void print(const __FlashStringHelper *message);
    void println(const __FlashStringHelper *message);

    uint8_t available();
    const char* readString();
    void readParsedString(char& character, int& number);
    uint8_t receivedString(char* string);
    uint8_t getConsoleMode();
    void println();
};

extern DebugConsole debugConsole;

#endif