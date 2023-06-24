//#include "note-c/note.h"
//#include "note-c/n_cjson.h"
#include "Arduino.h"
#include "notecard_AVR.h"
#include "myBase64.h"
#include "InternalStorageAVR.h"
#include "memoryTest.h"

Notecard notecard;

bool noteCardIsSyncing = false;

void moveStringToRAM(const char* source, char** destination) {
  size_t len = strlen_P(source) + 1; // +1 for the null terminator
  *destination = (char*) malloc(len);
  if (*destination) {
    strcpy_P(*destination, source);
  }
}

void freeRAMString(char** str) {
  if (*str) {
    free(*str);
    *str = NULL;
  }
}

J* AVRNoteNewRequest(const __FlashStringHelper* request) {
  // Get the length of the flash string
  size_t len = strlen_P((PGM_P)request);

  // Allocate memory for the copied string
  char* copiedString = (char*)malloc(len + 1); // +1 for the null terminator

  // Copy the flash string to the allocated memory
  strncpy_P(copiedString, (PGM_P)request, len);
  copiedString[len] = '\0'; // Add the null terminator

  // Call the original NoteNewRequest with the copied string
  J* result = NoteNewRequest(copiedString);

  // Free the allocated string
  free(copiedString);

  // Return the value from NoteNewRequest call
  return result;
}

N_CJSON_PUBLIC(J*) AVRJAddStringToObject(J* const object, const char* const name, const __FlashStringHelper* string) {
  // Get the length of the flash string
  size_t len = strlen_P((PGM_P)string);

  // Allocate memory for the copied string
  char* copiedString = (char*)malloc(len + 1); // +1 for the null terminator

  // Copy the flash string to the allocated memory
  strncpy_P(copiedString, (PGM_P)string, len);
  copiedString[len] = '\0'; // Add the null terminator

  // Call the original JAddStringToObject with the copied string
  J* result = JAddStringToObject(object, name, copiedString);

  // Free the allocated string
  free(copiedString);

  // Return the value from JAddStringToObject call
  return result;
}

int AVRNotecardInit(bool debugMode){
    while (!usbSerial) {
      ; // wait for serial port to connect. Needed for native USB
    }
    //start notecard communication
    notecard.begin(txRxPinsSerial, 9600);
    if(debugMode){
      // Initialize the Notecard debug port
      notecard.setDebugOutputStream(usbSerial);
    }
    J *req = notecard.newRequest("hub.set");
    if (req != NULL) {
      AVRJAddStringToObject(req, "product", NOTE_PRODUCT_UID);
      AVRJAddStringToObject(req, "mode", F("continuous"));
      notecard.sendRequest(req);
    }
    else{
      return RETURN_ERROR;
    }
    // req = AVRNoteNewRequest(F("card.dfu"));
    // if (req != NULL){
    //   AVRJAddStringToObject(req, "name", F("-"));
    //   JAddBoolToObject(req, "off", true);
    //   notecard.sendRequest(req);
    // }
    // else{
    //   return RETURN_ERROR;
    // }
    if(!AVRStartNotecardSync()){
      return RETURN_ERROR;
    }
    return RETURN_SUCCESS;
}

int AVRIsNotecardConnected(){
    char status[30];
    long time = 0;
    J* req = AVRNoteNewRequest(F("hub.sync.status"));
    J* syncRsp = NULL;
    if(req != NULL){
      syncRsp = notecard.requestAndResponse(req);
    }
    char* tmpStatus = NULL;
    if(syncRsp != NULL){
      tmpStatus = JGetString(syncRsp, "status");
      strlcpy(status, tmpStatus, sizeof(status));
      time = JGetInt(syncRsp, "time");
      notecard.deleteResponse(syncRsp);
    }
    if(strstr(status, "{connected}") != NULL || strstr(status, "{sync-end}") != NULL || time != 0){
      noteCardIsSyncing = false;
      return 1;
    }
    else{
      return 0;
    }
}

int AVRStartNotecardSync(){
  if(!noteCardIsSyncing){
      J* req = AVRNoteNewRequest(F("hub.sync"));
      if (req != NULL) {
      JAddBoolToObject(req, "sync", true);
      notecard.sendRequest(req);
      noteCardIsSyncing = true;
    }
    else {
      return RETURN_ERROR;
    }
  }
  return RETURN_SUCCESS;
}

int AVRCheckNotecatdDFUMode(int maxUpdateSize) {
  // variables to store the response from dfu.status
  char mode[20];
  char status[40];
  long updateSize = 0;
  // set dfu.status to on to allow the notecard to download new firmware
  J* req = AVRNoteNewRequest(F("dfu.status"));
  if (req != NULL) {
    JAddBoolToObject(req, "on", true);
    J* rsp = notecard.requestAndResponse(req);
    if (rsp != NULL) {
      // get the mode and status from the response
      char* tempMode = JGetString(rsp, "mode");
      strlcpy(mode, tempMode, sizeof(mode));
      char* tempStatus = JGetString(rsp, "status");
      strlcpy(status, tempStatus, sizeof(status));
      //if mode is ready, retrieve update size
      if (strcmp(mode, "ready") == 0) {
        J* body = JGetObject(rsp, "body");
        updateSize = JGetInt(body, "length");
        if (updateSize == 0) {
          updateSize = maxUpdateSize;
        }
      }
      notecard.deleteResponse(rsp);
    }
    else {
      return RETURN_ERROR;
    }
  }
  else{
    return RETURN_ERROR;
  }
  return updateSize;
}

int AVRSetNotecardToDFU(int maxWaitTime_sec){
  //set notecard to dfu mode
  J* req = AVRNoteNewRequest(F("hub.set"));
  if(req != NULL){
    AVRJAddStringToObject(req, "mode", F("dfu"));
    notecard.sendRequest(req);
  }
  else{
    return RETURN_ERROR;
  }
  // Wait until we have successfully entered the mode.  The fact that this loop isn't
  // just an infinitely loop is simply defensive programming.  If for some odd reason
  // we don't enter DFU mode, we'll eventually come back here on the next update check
  bool inDFUMode = false;
  long DFUModeCheck = 0;
  long DFUdelay = 2500;
  long maxWaitTime = maxWaitTime_sec;
  while (!inDFUMode && DFUModeCheck < (maxWaitTime * 1000)) {
    //verify the notecard is in dfu mode using dfu.get
    req = AVRNoteNewRequest(F("dfu.get"));
    JAddNumberToObject(req, "length", 0);
    if (J* rsp = notecard.requestAndResponse(req)) {
        if (!notecard.responseError(rsp)){
            inDFUMode = true;
        }
        else{
            //Serial.println(F("DFU error:"));
            //Serial.println(JGetString(rsp, "err"));
        }
        notecard.deleteResponse(rsp);
    }
    if (!inDFUMode){
        delay(DFUdelay);
        DFUModeCheck += DFUdelay;
    }
  }
  // If we failed, leave DFU mode immediately
  if (!inDFUMode) {
      if (J *req = AVRNoteNewRequest(F("hub.set"))) {
          AVRJAddStringToObject(req, "mode", F("dfu-completed"));
          notecard.sendRequest(req);
      }
      usbSerial.println(F("Failed to enter DFU mode"));
      return RETURN_ERROR;
  }
  return RETURN_SUCCESS;
}

unsigned char* AVRRetrieveNotecardPayloadChunk(int& numOfErrors, long offset, int& chunkSize) {
    int8_t max_retries = 5;
    //static unsigned char payload[64]; // assuming a fixed size of 512 for payload
    //dynamic allocation of payload
    //check if enough memory is available
    unsigned char* payload = new unsigned char[chunkSize];

    bool payloadEmpty = true;

    for (int retry = 0; retry < max_retries; retry++) {
        //if its not first try, half the size of the allocated payload
        if (retry > 0) {
            chunkSize = chunkSize / 2;
            delete[] payload;
            payload = new unsigned char[chunkSize];
        }
        //if this is the last try, set the chunk size to 1
        //or if there was not enough memory to allocate the payload
        //this is to ensure that we get the last chunk of data
        if ((retry == max_retries - 1 || payload == NULL) && chunkSize > 1) {
            chunkSize = 1;
            delete[] payload;
            payload = new unsigned char[chunkSize];
        }

        usbSerial.print(F("dfy: reading chunk (offset: "));
        usbSerial.print(offset);
        usbSerial.print(F(" length: "));
        usbSerial.print(chunkSize);
        usbSerial.print(F(" try: "));
        usbSerial.println(retry + 1);
        
        // Request the next chunk from the notecard
        J* req = AVRNoteNewRequest(F("dfu.get"));
        if (req == NULL) {
            usbSerial.println(F("dfu: insufficient memory\n"));
            delete[] payload;
            return NULL;
        }
        JAddNumberToObject(req, "length", chunkSize);
        JAddNumberToObject(req, "offset", offset);

        // Requesting current chunk of data
        J* rsp = notecard.requestAndResponse(req);
        
        if (rsp == NULL) {
            usbSerial.println(F("dfu: insufficient memory\n"));
            notecard.deleteResponse(rsp);
            delete[] payload;
            return NULL;
        } else if (notecard.responseError(rsp)) {
            usbSerial.print(F("dfu: error on read: "));
            usbSerial.println(JGetString(rsp, "err"));
            notecard.deleteResponse(rsp);
            numOfErrors++;
            continue;
        } else {
            char* payloadB64 = JGetString(rsp, "payload");
            
            if (payloadB64[0] == '\0') {
                usbSerial.println(F("dfu: no payload"));
                notecard.deleteResponse(rsp);
                payloadEmpty = true;
                break;
            }

            int num_bytes = UnBase64((unsigned char*)payload, (const unsigned char*)payloadB64, strlen(payloadB64));

            if (num_bytes < 0) {
                usbSerial.println(F("dfu: can't decode payload\n"));
                notecard.deleteResponse(rsp);
                payloadEmpty = true;
                continue;
            } else if (num_bytes != chunkSize) {
                usbSerial.println(F("dfu: payload size mismatch\n"));
                notecard.deleteResponse(rsp);
                payloadEmpty = true;
                continue;
            } else {
                usbSerial.println(F("dfu: payload decoded"));
                payloadEmpty = false;
                notecard.deleteResponse(rsp);
                break;
            }
        }
    }

    if (payloadEmpty) {
        delete[] payload;
        return NULL;
    } else {
        return payload;
    }
}

int AVRReturnNotecardFromDFU(){
  J* req = AVRNoteNewRequest(F("dfu.status"));
  if(req != NULL){
    JAddBoolToObject(req, "stop", true);
    AVRJAddStringToObject(req, "status", F("firmware update successful"));
    notecard.sendRequest(req);
  }
  else{
    return RETURN_ERROR;
  }
  //return to continuous operation
  req = NoteNewRequest("hub.set");
  if(req != NULL){
    AVRJAddStringToObject(req, "mode", F("continuous"));
    notecard.sendRequest(req);
  }
  else{
    return RETURN_ERROR;
  }
  RETURN_SUCCESS;
}

void AVRNotecardCheckForUpdate(){
  /*
  This function checks for an update and if there is one, it puts the notecard in DFU mode and peforms the update
  */

  //start notecard sync and wait if it connects or not for one minute
  //if it does not connect, return
  usbSerial.println(F("Starting notecard sync"));
  Serial.flush();
  int maxWaitTime_sec = 120;
  AVRStartNotecardSync();
  
  for(int i = 0; i < maxWaitTime_sec; i+=1){
    delay(1000);
    usbSerial.println("Time waited:");
    usbSerial.println(i);
    if(AVRIsNotecardConnected()){
      break;
    }
    usbSerial.flush();
    txRxPinsSerial.flush();
  }
  if(!AVRIsNotecardConnected()){
    usbSerial.println("Waited too long for connection, return");
    return;
  }

  usbSerial.println(F("Checking notecard dfu status"));
  Serial.flush();
  // check if dfu mode is ready and if so, retrieve the update size
  long updateSize = AVRCheckNotecatdDFUMode(100000);
  
  // if updateSize is zero, the update is not ready
  if (!updateSize) {
    usbSerial.println(F("No update available"));
    return;
  }

  //put notecard in dfu mode with max wait time 120 seconds
  usbSerial.println(F("putting notecard in dfu mode"));
  if(!AVRSetNotecardToDFU(120)){
    usbSerial.println(F("Could not enter dfu"));
    return;
  }

  // check if there is enough space to store the update
  if (!InternalStorage.open(updateSize)) {
    AVRReturnNotecardFromDFU();
    Serial.println(F("There is not enough flash space to store the update. Can't continue with update."));
    return;
  } 

  bool payloadEmpty = false;
  int chunkSize = 1024;
  long offset = 0;
  unsigned char* payload;
  int maxNumOfErrors = 10;
  int numOfErrors = 0;

  //receive update chunk by chunk and save it to flash memory
  for (offset; offset < updateSize; offset+=chunkSize)
  {  
    if(maxNumOfErrors < numOfErrors)
        break;

    //retrieve decoded payload chunk with given offset
    payload = AVRRetrieveNotecardPayloadChunk(numOfErrors, offset, chunkSize);
    if (payload == NULL) {
        AVRReturnNotecardFromDFU();
        return;
    }

    //after receiving and decoding payload
    //write received data to flash
    for (int m = 0; m < chunkSize; m++) {
      InternalStorage.write(payload[m]);
    }
    //free memory
    delete[] payload;
    // Move to next chunk
    usbSerial.print(F("dfu: chunk successfully saved to flash, offset: "));
    usbSerial.println(offset);
    notecard.logDebugf("dfu: successfully transferred offset:%d, offset");
  }
    

  //after saving new update, stop dfu mode
  AVRReturnNotecardFromDFU();

  //close flash
  InternalStorage.close();
  Serial.println(F("Sketch update apply and reset."));
  Serial.flush();
  //apply update (swap flash)
  InternalStorage.apply(); // this doesn't return
}



