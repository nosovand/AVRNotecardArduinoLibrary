#include "AVRNotecard.hpp"

AVRNotecard notecard;

int AVRNotecard::init(){
  return AVRNotecardInit();
}

void AVRNotecard::receiveUpdate(){
  return AVRNotecardCheckForUpdate();
}

int AVRNotecard::sendStringMessage(const __FlashStringHelper *fileName, const char *const stringName, const __FlashStringHelper *string){
  return AVRNotecardSendStringMessage(fileName, stringName, string);
}
