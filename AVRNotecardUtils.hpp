#ifndef AVR_NOTECARD_UTILS
#define AVR_NOTECARD_UTILS

#include <Arduino.h>

int AVRUnBase64(char *bufplain);
void displayLoadingBar(int progress, uint8_t logMode);


#endif