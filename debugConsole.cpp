#include "HardwareSerial.h"
#include "debugConsole.hpp"
#include "SPI_Master.hpp"

DebugConsole debugConsole; 

DebugConsole::DebugConsole(/* args */)
{
}

DebugConsole::~DebugConsole()
{
}

void DebugConsole::init(uint8_t mode, long serialBaud)
{
    /**
     * @brief Initialize the debug console
     * @param mode: DEBUG_CONSOLE_MODE_SPI, DEBUG_CONSOLE_MODE_UART, DEBUG_CONSOLE_MODE_UART_AND_SPI
     * @param serialBaud: Baud rate for UART
     * @return None
    */
    debugConsoleMode = mode;
    if (mode == DEBUG_CONSOLE_MODE_SPI)
    {
        SPIMasterInit();
    }
    else if (mode == DEBUG_CONSOLE_MODE_UART)
    {
        Serial.begin(serialBaud);
    }
    else if (mode == DEBUG_CONSOLE_MODE_UART_AND_SPI)
    {
        Serial.begin(serialBaud);
        SPIMasterInit();
    }
}

void DebugConsole::print(char *message)
{
    /**
     * @brief Print a message to the debug console
     * @param message: Message to be printed
     * @return None
    */
    if (debugConsoleMode == DEBUG_CONSOLE_MODE_SPI)
    {
        SPITransferMessage(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART)
    {
        Serial.print(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART_AND_SPI)
    {
        SPITransferMessage(message);
        Serial.print(message);
    }
}

void DebugConsole::println(char *message)
{
    /**
     * @brief Print a message to the debug console and add a new line
     * @param message: Message to be printed
     * @return None
    */
    if (debugConsoleMode == DEBUG_CONSOLE_MODE_SPI)
    {
        SPITransferMessageln(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART)
    {
        Serial.println(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART_AND_SPI)
    {
        Serial.println(message);
        Serial.flush();
        SPITransferMessageln(message);
    }
}

void DebugConsole::print(const char *message){
    /**
     * @brief Print a message to the debug console
     * @param message: Message to be printed
     * @return None
    */
    if (debugConsoleMode == DEBUG_CONSOLE_MODE_SPI)
    {
        SPITransferMessage(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART)
    {
        Serial.print(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART_AND_SPI)
    {
        SPITransferMessage(message);
        Serial.print(message);
    }
}

void DebugConsole::println(const char *message)
{
    /**
     * @brief Print a message to the debug console and add a new line
     * @param message: Message to be printed
     * @return None
    */
    if (debugConsoleMode == DEBUG_CONSOLE_MODE_SPI)
    {
        SPITransferMessageln(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART)
    {
        Serial.println(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART_AND_SPI)
    {
        Serial.println(message);
        Serial.flush();
        SPITransferMessageln(message);
    }
}

void DebugConsole::print(double message)
{
    /**
     * @brief Print a double to the debug console
     * @param message: Double to be printed
     * @return None
    */
    if (debugConsoleMode == DEBUG_CONSOLE_MODE_SPI)
    {
        SPITransferMessage(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART)
    {
        Serial.print(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART_AND_SPI)
    {
        SPITransferMessage(message);
        Serial.print(message);
    }
}

void DebugConsole::println(double message)
{
    /**
     * @brief Print a double to the debug console and add a new line
     * @param message: Double to be printed
     * @return None
    */
    if (debugConsoleMode == DEBUG_CONSOLE_MODE_SPI)
    {
        SPITransferMessageln(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART)
    {
        Serial.println(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART_AND_SPI)
    {
        Serial.println(message);
        Serial.flush();
        SPITransferMessageln(message);
    }
}

void DebugConsole::print(const __FlashStringHelper *message)
{
    /**
     * @brief Print a message to the debug console
     * @param message: Message to be printed
     * @return None
    */
    if (debugConsoleMode == DEBUG_CONSOLE_MODE_SPI)
    {
        SPITransferMessage(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART)
    {
        Serial.print(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART_AND_SPI)
    {
        Serial.print(message);
        Serial.flush();
        SPITransferMessage(message);
    }
}

void DebugConsole::println(const __FlashStringHelper *message)
{
    /**
     * @brief Print a message to the debug console and add a new line
     * @param message: Message to be printed
     * @return None
    */
    if (debugConsoleMode == DEBUG_CONSOLE_MODE_SPI)
    {
        SPITransferMessageln(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART)
    {
        Serial.println(message);
    }
    else if (debugConsoleMode == DEBUG_CONSOLE_MODE_UART_AND_SPI)
    {
        SPITransferMessageln(message);
        Serial.println(message);
    }
}

uint8_t DebugConsole::getConsoleMode(){
  return debugConsoleMode;
}