#include "AVRNotecardUtils.hpp"
#include "AVRNotecardLog.hpp"

static const unsigned char pr2six[256] PROGMEM = {
    /* ASCII table */
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int AVRUnBase64(char *bufplain) {
/**
 * @brief Decode a base64-encoded string (NUL terminated)
 * @param bufplain Both the input and output buffer for the coded/decoded string
 * @return The length of the decoded string
*/
  int nbytesdecoded;
  register unsigned char *bufin;
  register unsigned char *bufout;
  register int nprbytes;

  bufin = (unsigned char *) bufplain;
  while (pgm_read_byte(&pr2six[*(bufin++)]) <= 63);
  nprbytes = (bufin - (unsigned char *) bufplain) - 1;
  nbytesdecoded = ((nprbytes + 3) / 4) * 3;

  bufout = (unsigned char *) bufplain;
  bufin = (unsigned char *) bufplain;

  while (nprbytes > 4) {
      *(bufout++) =
          (unsigned char) (pgm_read_byte(&pr2six[*bufin]) << 2 | pgm_read_byte(&pr2six[bufin[1]]) >> 4);
      *(bufout++) =
          (unsigned char) (pgm_read_byte(&pr2six[bufin[1]]) << 4 | pgm_read_byte(&pr2six[bufin[2]]) >> 2);
      *(bufout++) =
          (unsigned char) (pgm_read_byte(&pr2six[bufin[2]]) << 6 | pgm_read_byte(&pr2six[bufin[3]]));
      bufin += 4;
      nprbytes -= 4;
  }

  /* Note: (nprbytes == 1) would be an error, so just ignore that case */
  if (nprbytes > 1) {
      *(bufout++) =
          (unsigned char) (pgm_read_byte(&pr2six[*bufin]) << 2 | pgm_read_byte(&pr2six[bufin[1]]) >> 4);
  }
  if (nprbytes > 2) {
      *(bufout++) =
          (unsigned char) (pgm_read_byte(&pr2six[bufin[1]]) << 4 | pgm_read_byte(&pr2six[bufin[2]]) >> 2);
  }
  if (nprbytes > 3) {
      *(bufout++) =
          (unsigned char) (pgm_read_byte(&pr2six[bufin[2]]) << 6 | pgm_read_byte(&pr2six[bufin[3]]));
  }

  *(bufout++) = '\0';
  nbytesdecoded -= (4 - nprbytes) & 3;
  return nbytesdecoded; 
}

void displayLoadingBar(int progress, uint8_t logMode) {
  avrNotecardLog.print("[", logMode); 
  int barLength = 20;
  int completedBars = (progress * barLength) / 100;
  
  // Print the completed part of the loading bar
  for (int i = 0; i < completedBars; i++) {
    avrNotecardLog.print("#", logMode);
  }
  
  // Print the remaining part of the loading bar
  for (int i = completedBars; i < barLength; i++) {
    avrNotecardLog.print("-", logMode);
  }

  // Print the percentage
  avrNotecardLog.print("] ", logMode);
  avrNotecardLog.print(progress, logMode);
  avrNotecardLog.println("%", logMode);
}