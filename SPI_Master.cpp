#include "HardwareSerial.h"
#include "Arduino.h"
#ifndef SPI_MASTER_HPP
#define SPI_MASTER_HPP

#define MAX_MASTER_MESSAGE_SIZE 16

#include <SPI.h>
#include "SPI_Master.hpp"

char txMsg[MAX_MASTER_MESSAGE_SIZE] = "";

void countDoubleDigits(double number, int *integerDigits, int *decimalDigits) {
    // Handling negative numbers by taking the absolute value
    number = fabs(number);
    // Counting main digits
    *integerDigits = number == 0 ? 1 : (int)log10(number) + 1;
    // Counting decimal digits
    *decimalDigits = 0;

    double decimalPart = number - floor(number);
    double precision = 1e-4; // Adjust the precision as needed

    while (decimalPart > precision) {
        decimalPart *= 10;
        decimalPart = fmod(decimalPart, 1.0);
        (*decimalDigits)++;
        if (decimalPart > (0.999))
          break;
    }
}

//SPI init
void SPIMasterInit()
{
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128); // 16 MHz/128
}

// Specialization for const __FlashStringHelper*
void SPITransferMessage(const __FlashStringHelper* message)
{
  int x;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = strlen_P(strPtr);

  x = SPI.transfer('<'); // start mark

  for (size_t i = 0; i < length; i++)
  {
   x = SPI.transfer(pgm_read_byte_near(strPtr + i)); // SPI transfer is byte-by-byte
  }

  x = SPI.transfer('>');  // end mark
}

// Specialization for const __FlashStringHelper*
void SPITransferMessageln(const __FlashStringHelper* message)
{
  int x;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = strlen_P(strPtr);

  x = SPI.transfer('<'); // start mark

  for (size_t i = 0; i < length; i++)
  {
    x = SPI.transfer(pgm_read_byte_near(strPtr + i)); // SPI transfer is byte-by-byte
  }

  x = SPI.transfer('&');  // end mark
}


void SPITransferMessage(char* message)
{
  int x;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = strlen(strPtr);

  x = SPI.transfer('<'); // start mark

  for (size_t i = 0; i < length; i++)
  {
    x = SPI.transfer(strPtr[i]);
  }

  x = SPI.transfer('>');  // end mark
}

void SPITransferMessageln(char* message)
{
  int x;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = strlen(strPtr);

  x = SPI.transfer('<'); // start mark

  for (size_t i = 0; i < length; i++)
  {
    x = SPI.transfer(strPtr[i]);
  }

  x = SPI.transfer('&');  // end mark
}

void SPITransferMessage(const char* message){
  int x;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = strlen(strPtr);

  x = SPI.transfer('<'); // start mark

  for (size_t i = 0; i < length; i++)
  {
    x = SPI.transfer(strPtr[i]);
  }

  x = SPI.transfer('>');  // end mark
}

void SPITransferMessageln(const char* message)
{
  int x;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = strlen(strPtr);

  x = SPI.transfer('<'); // start mark

  for (size_t i = 0; i < length; i++)
  {
    x = SPI.transfer(strPtr[i]);
  }

  x = SPI.transfer('&');  // end mark
}

void SPITransferMessage(double message)
{
  int x;
  int integerDigits = 0, decimalDigits = 0;

  countDoubleDigits(message, &integerDigits, &decimalDigits);
  if(abs(integerDigits) > MAX_MASTER_MESSAGE_SIZE - 2){
    //check for buffer overflow
    SPITransferMessageln(F("ovf"));
    return;
  }
  if((abs(integerDigits) + abs(decimalDigits)) > (MAX_MASTER_MESSAGE_SIZE - 2)){
    //adjust demial digits according to number size
    decimalDigits = MAX_MASTER_MESSAGE_SIZE - integerDigits - 2;
  }
  //convert double to char array
  dtostrf(message, integerDigits+decimalDigits, decimalDigits, txMsg);
  size_t length = strlen(txMsg);

  x = SPI.transfer('<'); // start mark

  for (size_t i = 0; i < length; i++)
  {
    x = SPI.transfer(txMsg[i]);
  }

  x = SPI.transfer('>');  // end mark
}


void SPITransferMessageln(double message)
{
  int x;
  int integerDigits = 0, decimalDigits = 0;

  countDoubleDigits(message, &integerDigits, &decimalDigits);
  if(abs(integerDigits) > MAX_MASTER_MESSAGE_SIZE - 2){
    //check for buffer overflow
    SPITransferMessageln(F("ovf"));
    return;
  }
  if((abs(integerDigits) + abs(decimalDigits)) > (MAX_MASTER_MESSAGE_SIZE - 2)){
    //adjust demial digits according to number size
    decimalDigits = MAX_MASTER_MESSAGE_SIZE - integerDigits - 2;
  }
  //convert double to char array
  dtostrf(message, integerDigits+decimalDigits, decimalDigits, txMsg);
  size_t length = strlen(txMsg);

  x = SPI.transfer('<'); // start mark

  for (size_t i = 0; i < length; i++)
  {
    x = SPI.transfer(txMsg[i]);
  }

  x = SPI.transfer('&');  // end mark
}


#endif