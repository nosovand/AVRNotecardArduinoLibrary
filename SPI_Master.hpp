#ifndef SPI_MASTER_HPP
#define SPI_MASTER_HPP

//SPI init
void SPIMasterInit();

// Specialization for const __FlashStringHelper*
void SPITransferMessage(const __FlashStringHelper* message);
void SPITransferMessageln(const __FlashStringHelper* message);
void SPITransferMessage(char* message);
void SPITransferMessageln(char* message);
void SPITransferMessage(const char* message);
void SPITransferMessageln(const char* message);
void SPITransferMessage(double message);
void SPITransferMessageln(double message);

void countDoubleDigits(double number, int *mainDigits, int *decimalDigits);

#endif