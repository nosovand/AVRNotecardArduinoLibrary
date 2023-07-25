//#include "note-c/note.h"
//#include "note-c/n_cjson.h"
#include "Arduino.h"
#include "notecard_AVR.h"
#include "myBase64.h"
#include "InternalStorageAVR.h"
#include "memoryTest.h"
#include "debugConsole.hpp"

Notecard notecard;

bool noteCardIsSyncing = false;

int AVRInitNotecardGPS(){
  /**
   * @brief initialize the notecard GPS
   * @return int 1 if success, 0 if error
  */
  J *req = NoteNewRequest("card.location.mode");
  if (req==NULL){
    return RETURN_ERROR;
  }
  AVRJAddStringToObject(req, "mode", F("periodic"));
  JAddNumberToObject(req, "seconds", GPS_CONNECTION_PERIOD_SEC);
  notecard.sendRequest(req);
  return RETURN_SUCCESS;
}

void moveStringToRAM(const char* source, char** destination) {
  /**
   * @brief move a string from flash memory to RAM
   * @param source the string in flash memory
   * @param destination the string in RAM
   * @return void
  */
  size_t len = strlen_P(source) + 1; // +1 for the null terminator
  *destination = (char*) malloc(len);
  if (*destination) {
    strcpy_P(*destination, source);
  }
}

void freeRAMString(char** str) {
  /**
   * @brief free a string in RAM
   * @param str the string in RAM
   * @return void
  */
  if (*str) {
    free(*str);
    *str = NULL;
  }
}

J* AVRNoteNewRequest(const __FlashStringHelper* request) {
  /**
   * AVR wrapper for NoteNewRequest
   * @brief create a new notecard request
   * @param request the request name in flash memory
   * @return J* the request JSON object
  */
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
  /**
   * AVR wrapper for JAddStringToObject
   * @brief add a string to a JSON object
   * @param object the JSON object
   * @param name the name of the string in JSON object
   * @param string the added string in flash memory
   * @return J* the JSON object
  */
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

N_CJSON_PUBLIC(void) AVRJDeleteWithoutPayload(J *item)
{
  /**
   * @brief delete a JSON object without deleting the payload
   * @param item the JSON object
   * @return void
  */
    J *next = NULL;
    while (item != NULL) {
        next = item->next;
        if (!(item->type & JIsReference) && (item->child != NULL)) {
            AVRJDeleteWithoutPayload(item->child);
        }
        if (!(item->type & JIsReference) && (item->valuestring != NULL)) {
          if(strstr(item->string, "payload") == NULL){
            NoteFree(item->valuestring);
          }
        }
        if (!(item->type & JStringIsConst) && (item->string != NULL)) {
              NoteFree(item->string);
        }
        NoteFree(item);
        item = next;
    }
}

int AVRNotecardInit(bool debugMode){
    /**
     * @brief initialize the notecard
     * @param debugMode true if debug mode is on, false otherwise
     * @return int 1 if success, 0 if error
    */
    // while (!usbSerial) {
    //   ; // wait for serial port to connect. Needed for native USB
    // }
    //start notecard communication
    notecard.begin(txRxPinsSerial, 9600);
    if(debugMode && debugConsole.getConsoleMode() != DEBUG_CONSOLE_MODE_SPI && usbSerial != txRxPinsSerial){
      // Initialize the Notecard debug port
      debugConsole.println("Notecard debug stream was activated");
      notecard.setDebugOutputStream(usbSerial);
    }
    else{
      debugConsole.println("Notecard debug stream was not activated");
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

    //TODO: Notify notecard of out current software verion

    if(!AVRStartNotecardSync()){
      return RETURN_ERROR;
    }
    if(!AVRInitNotecardGPS()){
      debugConsole.println(F("Not enough memory for gps init"));
    }
    return RETURN_SUCCESS;
}

int AVRIsNotecardConnected(){
    /**
     * @brief check if the notecard is connected
     * @return int 1 if connected, 0 if not connected
    */
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
    /**
     * @brief start notecard sync
     * @return int 1 if success, 0 if error
    */
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

long AVRCheckNotecatdDFUMode(long maxUpdateSize, char* imageMD5) {
  /**
   * Checks the notecard DFU mode and returns the size of the update if one is available
   * @brief check the notecard DFU mode
   * @param maxUpdateSize the maximum size of the 
   * @param imageMD5 pointer to the array to store the MD5 hash
   * @return available update size if mode is ready, 0 if error or no update available
  */
  long updateSize = 0;
  // set dfu.status to on to allow the notecard to download new firmware
  J* req = AVRNoteNewRequest(F("dfu.status"));
  if (req != NULL) {
    JAddBoolToObject(req, "on", true);
    J* rsp = notecard.requestAndResponse(req);
    if (rsp != NULL) {
      //check if mode is ready
      if (strcmp(JGetString(rsp, "mode"), "ready") == 0) {
        //TODO: check if the update software version is the same as current software verion
        J* body = JGetObject(rsp, "body");
        updateSize = JGetInt(body, "length");
        if (updateSize == 0) {
          updateSize = maxUpdateSize;
        }
        strlcpy(imageMD5, JGetString(body, "md5"), NOTE_MD5_HASH_STRING_SIZE);
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
  /**
   * @brief set the notecard to DFU mode
   * @param maxWaitTime_sec the maximum time to wait for the notecard to enter DFU mode
   * @return int 1 if success, 0 if error
  */
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
      debugConsole.println(F("Failed to enter DFU mode"));
      return RETURN_ERROR;
  }
  return RETURN_SUCCESS;
}

char* AVRRetrieveNotecardPayloadChunk(int& numOfErrors, long offset, int& chunkSize) {
    /**
     * @brief retrieve a chunk of downloaded update from the notecard
     * @param numOfErrors the current number of errors that occured during the download
     * @param offset the offset of the chunk
     * @param chunkSize the size of the chunk
     * @return unsigned char* the chunk of data
    */
    int8_t max_retries = 5;
    bool payloadEmpty = true;
    char* payload = NULL;

    for (int retry = 0; retry < max_retries; retry++) {
        
        //if its not first try, half the size of the allocated payload
        if (retry > 0) {
            chunkSize = chunkSize / 2;
        }
        //if this is the last try, set the chunk size to 1
        //or if there was not enough memory to allocate the payload
        //or if for some reason chunksize is of incorrect value
        //this is to ensure that we get the last chunk of data
        if (((retry == max_retries - 1) && chunkSize > 1) || chunkSize < 1) {
            chunkSize = 1;
        }
        debugConsole.print(F("dfy: reading chunk (offset: "));
        debugConsole.print(offset);
        debugConsole.print(F(" length: "));
        debugConsole.print(chunkSize);
        debugConsole.print(F(" try: "));
        debugConsole.println(retry + 1);
        // Request the next chunk from the notecard
        J* req = AVRNoteNewRequest(F("dfu.get"));
        if (req == NULL) {
            debugConsole.println(F("dfu: insufficient memory\n"));
            return NULL;
        }
        JAddNumberToObject(req, "length", chunkSize);
        JAddNumberToObject(req, "offset", offset);
        
        // Requesting current chunk of data
        J* rsp = notecard.requestAndResponse(req);
        if (rsp == NULL) {
            debugConsole.println(F("dfu: insufficient memory\n"));
            notecard.deleteResponse(rsp);
            return NULL;
        } else if (notecard.responseError(rsp)) {
            //with heighest probability means that we are requesting data that are out of update size
            //the chunk size will be smaller for the next retry
            debugConsole.print(F("dfu: error on read: "));
            debugConsole.println(JGetString(rsp, "err"));
            notecard.deleteResponse(rsp);
            numOfErrors++;
            continue;
        } else {
            payload = JGetString(rsp, "payload");
            if (payload[0] == '\0') {
                debugConsole.println(F("dfu: no payload"));
                notecard.deleteResponse(rsp);
                payloadEmpty = true;
                break;
            }
            int num_bytes = AVRUnBase64((char*)payload);
            
            const char *expectedMD5 = JGetString(rsp, "status");
            char chunkMD5[NOTE_MD5_HASH_STRING_SIZE] = {0};
            NoteMD5HashString((uint8_t *)payload, num_bytes, chunkMD5, sizeof(chunkMD5));
            
            if (num_bytes < 0) {
                debugConsole.println(F("dfu: can't decode payload\n"));
                notecard.deleteResponse(rsp);
                payloadEmpty = true;
                continue;
            } else if (num_bytes != chunkSize) {
                debugConsole.println(F("dfu: payload size mismatch\n"));
                notecard.deleteResponse(rsp);
                payloadEmpty = true;
                continue;
            } else if (strcmp(chunkMD5, expectedMD5)!=0){
                debugConsole.println(F("dfu: MD5 mismatch\n"));
                notecard.deleteResponse(rsp);
                payloadEmpty = true;
                continue;
            } else {
                debugConsole.println(F("dfu: payload decoded"));
                payloadEmpty = false;
                //notecard.deleteResponse(rsp);
                AVRJDeleteWithoutPayload(rsp);
                break;
            }
        }
    }

    if (payloadEmpty) {
        return NULL;
    } else {
        return payload;
    }
}

int AVRReturnNotecardFromDFU(){
  /**
  * This function returns the notecard from DFU mode to continuous operation
  * @return int 0 if success, 1 if error
  */
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
  /**
  * Checks for an update and if there is one, it puts the notecard in DFU mode and peforms the update
  * @brief Checks for an update
  * @return void
  */

  //start notecard sync and wait if it connects or not for one minute
  //if it does not connect, return
  debugConsole.println(F("Starting notecard sync"));
  Serial.flush();
  int maxWaitTime_sec = 120;
  int waitPeriod_sec = 20;
  AVRStartNotecardSync();
  
  for(int i = 0; i < maxWaitTime_sec; i+=waitPeriod_sec){
    delay(waitPeriod_sec*1000);
    debugConsole.println("Time waited:");
    debugConsole.println(i);
    if(AVRIsNotecardConnected()){
      break;
    }
    usbSerial.flush();
    txRxPinsSerial.flush();
  }
  if(!AVRIsNotecardConnected()){
    debugConsole.println("Waited too long for connection, return");
    return;
  }

  debugConsole.println(F("Checking notecard dfu status"));
  Serial.flush();
  // check if dfu mode is ready and if so, retrieve the update size
  char imageMD5[NOTE_MD5_HASH_STRING_SIZE] = {0};
  long updateSize = AVRCheckNotecatdDFUMode(100000, imageMD5);
  
  // if updateSize is zero, the update is not ready
  if (!updateSize) {
    debugConsole.println(F("No update available"));
    return;
  }

  //put notecard in dfu mode with max wait time 120 seconds
  debugConsole.println(F("putting notecard in dfu mode"));
  if(!AVRSetNotecardToDFU(120)){
    debugConsole.println(F("Could not enter dfu"));
    return;
  }

  // check if there is enough space to store the update
  if (!InternalStorage.open(updateSize)) {
    AVRReturnNotecardFromDFU();
    Serial.println(F("There is not enough flash space to store the update. Can't continue with update."));
    return;
  } 

  bool payloadEmpty = false;
  int chunkSize = 256;
  long offset = 0;
  char* payload;
  int maxNumOfErrors = 10;
  int numOfErrors = 0;
  NoteMD5Context md5Context;
  NoteMD5Init(&md5Context);

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
    // MD5 the chunk
    NoteMD5Update(&md5Context, (uint8_t *)payload, chunkSize);

    //after receiving and decoding payload
    //write received data to flash
    for (int m = 0; m < chunkSize; m++) {
      InternalStorage.write(payload[m]);
    }
    //free memory
    delete[] payload;
    // Move to next chunk
    debugConsole.print(F("dfu: chunk successfully saved to flash, offset: "));
    debugConsole.println(offset);
    //notecard.logDebugf("dfu: successfully transferred offset:%d, offset");
  }

  //after saving new update, stop dfu mode
  AVRReturnNotecardFromDFU();

  // Validate the MD5
  uint8_t md5Hash[NOTE_MD5_HASH_SIZE];
  NoteMD5Final(md5Hash, &md5Context);
  char md5HashString[NOTE_MD5_HASH_STRING_SIZE];
  NoteMD5HashToString(md5Hash, md5HashString, sizeof(md5HashString));
  debugConsole.print(F("dfu:    MD5 of image:"));
  debugConsole.println(imageMD5);
  debugConsole.print(F("dfu: MD5 of download:"));
  debugConsole.println(md5HashString);
  if (strcmp(imageMD5, md5HashString) != 0) {
      notecard.logDebugf("Error: MD5 MISMATCH - ABANDONING DFU\n");
      return;
  }

  //close flash
  InternalStorage.close();
  Serial.println(F("Sketch update apply and reset."));
  Serial.flush();
  //apply update (swap flash)
  InternalStorage.apply(); // this doesn't return
}



