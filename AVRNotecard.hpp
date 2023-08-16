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