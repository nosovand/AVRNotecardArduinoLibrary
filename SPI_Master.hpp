#ifndef SPI_MASTER_HPP
#define SPI_MASTER_HPP

//SPI init
void SPIMasterInit();

extern char* rxMsg;

// Specialization for const __FlashStringHelper*
uint8_t SPITransferMessage(const __FlashStringHelper* message);
uint8_t SPITransferMessageln(const __FlashStringHelper* message);
uint8_t SPITransferMessage(char* message);
uint8_t SPITransferMessageln(char* message);
uint8_t SPITransferMessage(String& message);
uint8_t SPITransferMessageln(String& message);
uint8_t SPITransferMessage(const char* message);
uint8_t SPITransferMessageln(const char* message);
uint8_t SPITransferMessage(double message);
uint8_t SPITransferMessageln(double message);

char* SPIReceiveLastMessage();
uint8_t SPIMessageAvailable();

uint8_t SPIMaxReceiveMessageSize();

void countDoubleDigits(double number, int *mainDigits, int *decimalDigits);
void SPIEraseRxMessageString();
uint8_t checkReceivedSPIValue(uint8_t receivedValue, size_t& index, uint8_t& transmittedValue);
uint8_t checkReceivedSPIValue(uint8_t receivedValue, size_t& index);
uint8_t terminateRxString(uint8_t index);
uint8_t remainingRxMessageLength(uint8_t length);

#endif