#include "HardwareSerial.h"
#include "Arduino.h"
#ifndef SPI_MASTER_HPP
#define SPI_MASTER_HPP

#define MAX_MASTER_MESSAGE_SIZE 16
#define MAX_SLAVE_MESSAGE_SIZE 16

#define MESSAGE_RECEIVED 1
#define NO_MESSAGE_RECEIVED 0

#include <SPI.h>
#include "SPI_Master.hpp"

char txMsg[MAX_MASTER_MESSAGE_SIZE] = "";
char rxMsg[MAX_SLAVE_MESSAGE_SIZE] = "";

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

size_t getFStringLength(const __FlashStringHelper *ifsh)
{
  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  size_t n = 0;
  for (n = 0; n < 128; n++) {
    unsigned char c = pgm_read_byte(p++);
    if (c == 0) break;
    if (c = '\0') break;
  }
  return n;
}

uint8_t SPIMaxReceiveMessageSize(){
  return MAX_SLAVE_MESSAGE_SIZE;
}


void SPIEraseRxMessageString(){
  memset(rxMsg, '\0', MAX_SLAVE_MESSAGE_SIZE);
}

uint8_t checkReceivedSPIValue(uint8_t receivedValue, size_t& index, uint8_t& transmittedValue){
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  if(receivedValue && receivedValue != transmittedValue && receivedValue != '>'){
    if(index == 0){
      SPIEraseRxMessageString();
    }
    if(index < MAX_SLAVE_MESSAGE_SIZE-1){
      rxMsg[index] = receivedValue;
      index++;
    }
    returnValue = MESSAGE_RECEIVED;
  }
  return returnValue;
}

uint8_t checkReceivedSPIValue(uint8_t receivedValue, size_t& index){
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  if(receivedValue){
    if(index == 0){
      SPIEraseRxMessageString();
    }
    if(index < MAX_SLAVE_MESSAGE_SIZE-1){
      rxMsg[index] = receivedValue;
      index++;
    }
    returnValue = MESSAGE_RECEIVED;
  }
  return returnValue;
}

uint8_t terminateRxString(uint8_t index){
  if(index < MAX_SLAVE_MESSAGE_SIZE-1){
    rxMsg[index] = '\0';
  }
}

uint8_t remainingRxMessageLength(uint8_t length){
  if((length+2)>MAX_SLAVE_MESSAGE_SIZE){
    return 0;
  }
  else{
    return MAX_SLAVE_MESSAGE_SIZE - length - 2;
  }
}

//SPI init
void SPIMasterInit()
{
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128); // 16 MHz/128
  pinMode(MISO, INPUT);
}

// Specialization for const __FlashStringHelper*
uint8_t SPITransferMessage(const __FlashStringHelper* message)
{
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  size_t index = 0;
  uint8_t x;
  uint8_t transmittedValue;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = getFStringLength(message);

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('<'); // start mark
  transmittedValue = '&';
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '<';

  for (size_t i = 0; i < length; i++)
  {
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(pgm_read_byte_near(strPtr + i)); // SPI transfer is byte-by-byte
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
    transmittedValue = pgm_read_byte_near(strPtr + i);
  }

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('>');  // end mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '>';

  for(int r = 0; r < remainingRxMessageLength(length); r++){
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(0);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
  }
  if(index > 0) terminateRxString(index);
  return returnValue;
}

// Specialization for const __FlashStringHelper*
uint8_t SPITransferMessageln(const __FlashStringHelper* message)
{
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  size_t index = 0;
  uint8_t x;
  uint8_t transmittedValue;
  const char* strPtr = reinterpret_cast<const char*>(message);
  //size_t length = strlen(strPtr);
  size_t length = getFStringLength(message);

  //Serial3.flush(); //for some reason helps to improve stability
  transmittedValue = '&';
  x = SPI.transfer('<'); // start mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '<';

  for (size_t i = 0; i < length; i++)
  {
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(pgm_read_byte_near(strPtr + i)); // SPI transfer is byte-by-byte
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
    transmittedValue = pgm_read_byte_near(strPtr + i);
  }

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('&');  // end mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '&';

  for(int r = 0; r < remainingRxMessageLength(length); r++){
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(0);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
  }
  if(index > 0) terminateRxString(index);
  return returnValue;
}


uint8_t SPITransferMessage(char* message)
{
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  size_t index = 0;
  uint8_t x;
  uint8_t transmittedValue;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = strlen(strPtr);

  //Serial3.flush(); //for some reason helps to improve stability
  transmittedValue = '&';
  x = SPI.transfer('<'); // start mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '<';

  for (size_t i = 0; i < length; i++)
  {
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(strPtr[i]);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
    transmittedValue = strPtr[i];
  }

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('>');  // end mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '>';
  
  for(int r = 0; r < remainingRxMessageLength(length); r++){
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(0);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
  }
  if(index > 0) terminateRxString(index);
  return returnValue;
}

uint8_t SPITransferMessageln(char* message)
{
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  size_t index = 0;
  uint8_t x;
  uint8_t transmittedValue;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = strlen(strPtr);

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('<'); // start mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '<';

  for (size_t i = 0; i < length; i++)
  {
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(strPtr[i]);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
    transmittedValue = strPtr[i];
  }

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('&');  // end mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '&';

  for(int r = 0; r < remainingRxMessageLength(length); r++){
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(0);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
  }
  if(index > 0) terminateRxString(index);
  return returnValue;
}

uint8_t SPITransferMessage(String& message){
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  size_t index = 0;
  uint8_t x;
  uint8_t length = message.length();
  uint8_t transmittedValue;

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('<'); // start mark
  transmittedValue = '&';
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '<';

  for (size_t i = 0; i < length; i++)
  {
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(message[i]);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
    transmittedValue = message[i];
  }

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('>');  // end mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '>';

  for(int r = 0; r < remainingRxMessageLength(length); r++){
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(0);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
  }
  if(index > 0) terminateRxString(index);
  return returnValue;
}

uint8_t SPITransferMessageln(String& message){
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  size_t index = 0;
  uint8_t x;
  uint8_t length = message.length();
  uint8_t transmittedValue;

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('<'); // start mark
  transmittedValue = '&';
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '<';

  for (size_t i = 0; i < length; i++)
  {
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(message[i]);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
    transmittedValue = message[i];
  }

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('&');  // end mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '&';

  for(int r = 0; r < remainingRxMessageLength(length); r++){
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(0);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
  }
  if(index > 0) terminateRxString(index);
  return returnValue;
}

uint8_t SPITransferMessage(const char* message)
{
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  size_t index = 0;
  uint8_t x;
  uint8_t transmittedValue;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = strlen(strPtr);

  //Serial3.flush(); //for some reason helps to improve stability
  transmittedValue = '&';
  x = SPI.transfer('<'); // start mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '<';

  for (size_t i = 0; i < length; i++)
  {
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(strPtr[i]);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
    transmittedValue = strPtr[i];
  }

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('>');  // end mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '>';

  for(int r = 0; r < remainingRxMessageLength(length); r++){
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(0);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
  }
  if(index > 0) terminateRxString(index);
  return returnValue;
}

uint8_t SPITransferMessageln(const char* message)
{
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  size_t index = 0;
  uint8_t x;
  uint8_t transmittedValue;
  const char* strPtr = reinterpret_cast<const char*>(message);
  size_t length = strlen(strPtr);

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('<'); // start mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '<';

  for (size_t i = 0; i < length; i++)
  {
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(strPtr[i]);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
    transmittedValue = strPtr[i];
  }

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('&');  // end mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '&';

  for(int r = 0; r < remainingRxMessageLength(length); r++){
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(0);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
  }

  if(index > 0) terminateRxString(index);
  return returnValue;
}

uint8_t SPITransferMessage(double message)
{
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  size_t index = 0;
  uint8_t x;
  uint8_t transmittedValue;
  int integerDigits = 0, decimalDigits = 0;

  countDoubleDigits(message, &integerDigits, &decimalDigits);
  if(abs(integerDigits) > MAX_MASTER_MESSAGE_SIZE - 2){
    //check for buffer overflow
    returnValue = SPITransferMessageln(F("ovf"));
    return returnValue;
  }
  if((abs(integerDigits) + abs(decimalDigits)) > (MAX_MASTER_MESSAGE_SIZE - 2)){
    //adjust demial digits according to number size
    decimalDigits = MAX_MASTER_MESSAGE_SIZE - integerDigits - 2;
  }
  //convert double to char array
  dtostrf(message, integerDigits+decimalDigits, decimalDigits, txMsg);
  txMsg[MAX_MASTER_MESSAGE_SIZE-1] = '\0';
  size_t length = strlen(txMsg);

  //Serial3.flush(); //for some reason helps to improve stability
  transmittedValue = '&';
  x = SPI.transfer('<'); // start mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '<';
  

  for (size_t i = 0; i < length; i++)
  {
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(txMsg[i]);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
    transmittedValue = txMsg[i];
  }

  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('>');  // end mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '>';

  for(int r = 0; r < remainingRxMessageLength(length); r++){
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(0);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
  }
  if(index > 0) terminateRxString(index);
  return returnValue;
}


uint8_t SPITransferMessageln(double message)
{
  uint8_t returnValue = NO_MESSAGE_RECEIVED;
  size_t index = 0;
  uint8_t x;
  uint8_t transmittedValue;
  int integerDigits = 0, decimalDigits = 0;

  countDoubleDigits(message, &integerDigits, &decimalDigits);
  if(abs(integerDigits) > MAX_MASTER_MESSAGE_SIZE - 2){
    //check for buffer overflow
    returnValue = SPITransferMessageln(F("ovf"));
    return returnValue;
  }
  if((abs(integerDigits) + abs(decimalDigits)) > (MAX_MASTER_MESSAGE_SIZE - 2)){
    //adjust demial digits according to number size
    decimalDigits = MAX_MASTER_MESSAGE_SIZE - integerDigits - 2;
  }
  //convert double to char array
  dtostrf(message, integerDigits+decimalDigits, decimalDigits, txMsg);
  txMsg[MAX_MASTER_MESSAGE_SIZE-1] = '\0';
  size_t length = strlen(txMsg);

  //Serial3.flush(); //for some reason helps to improve stability
  transmittedValue = '&';
  x = SPI.transfer('<'); // start mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '<';

  for (size_t i = 0; i < length; i++)
  {
    x = SPI.transfer(txMsg[i]);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
    transmittedValue = txMsg[i];
  }
  //Serial3.flush(); //for some reason helps to improve stability
  x = SPI.transfer('&');  // end mark
  if(checkReceivedSPIValue(x, index, transmittedValue)){
    returnValue = MESSAGE_RECEIVED;
  }
  transmittedValue = '&';

  for(int r = 0; r < remainingRxMessageLength(length); r++){
    //Serial3.flush(); //for some reason helps to improve stability
    x = SPI.transfer(0);
    if(checkReceivedSPIValue(x, index, transmittedValue)){
      returnValue = MESSAGE_RECEIVED;
    }
  }
  if(index > 0) terminateRxString(index);
  return returnValue;
}

char* SPIReceiveLastMessage(){
  SPITransferMessage("");
  return rxMsg;
}

uint8_t SPIMessageAvailable(){
  SPIReceiveLastMessage();
  return strlen(rxMsg) > 0;
}


#endif