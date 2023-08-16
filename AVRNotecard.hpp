/*----------------------- notecard AVR header file -------------------------|
 | libabry version 0.3.1                                                    |
 |                  																												|
 | 2023 by Andrey Nosov, https://github.com/nosovand                        |
 |                                                                          |
 | C++ library for easy and memory friendly communication                   |
 | with Notecard on AVR mega processors.                                    |
 |-------------------------------------------------------------------------*/

#include <Notecard.h>
#ifndef AVR_NOTECARD_HPP
#define AVR_NOTECARD_HPP

#include "notecard_AVR.h"

class AVRNotecard{
  /**
   * @brief  class that stores functions for high level interaction with notecard
   */
  public:
    int init();
    void receiveUpdate();
    int sendStringMessage(const __FlashStringHelper* fileName, const char* const stringName, const __FlashStringHelper* string);

};

extern AVRNotecard notecard;

#endif