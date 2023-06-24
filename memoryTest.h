// MemoryFree library based on code posted here:
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1213583720/15
// Extended by Matthew Murdoch to include walking of the free list.
// Further extended by Ray Livingston for additinal output options

#ifndef MEMORY_FREE_H
#define MEMORY_FREE_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif


#define SHOW_SIZES  showSizes( F( __FILE__ ),__LINE__ );
#define SET_MARK    setMark( F( __FILE__ ), __LINE__);
#define SHOW_MARK   showMark( F( __FILE__ ), __LINE__);

void initFreeMemory();
int freeMemory();
void showMemory(const char *s);
void showMem();
void showMemory_P(const __FlashStringHelper *s);
void showSizes(const __FlashStringHelper *s, int line);
void setMark(const __FlashStringHelper *s, int line);
void showMark(const __FlashStringHelper *s, int line);
int freeListSize();

#ifdef  __cplusplus
}
#endif

#endif
