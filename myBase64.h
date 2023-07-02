#include <Arduino.h>

#ifndef MY_BASE64_H
#define MY_BASE64_H


// static const char  table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
// static const int   BASE64_INPUT_SIZE = 57;

// bool isbase64(char c)
// {
//     return c && strchr(table, c) != NULL;
// }

// inline char value(char c)
// {
//     const char *p = strchr(table, c);
//     if(p) {
//       return p-table;
//     } else {
//       return 0;
//     }
// }

// int UnBase64(unsigned char *dest, const unsigned char *src, int srclen)
// {
//     *dest = 0;
//     if(*src == 0) 
//     {
//       return 0;
//     }
//     unsigned char *p = dest;
//     do
//     {

//       char a = value(src[0]);
//       char b = value(src[1]);
//       char c = value(src[2]);
//       char d = value(src[3]);
//       *p++ = (a << 2) | (b >> 4);
//       *p++ = (b << 4) | (c >> 2);
//       *p++ = (c << 6) | d;
//       if(!isbase64(src[1])) 
//       {
//           p -= 2;
//           break;
//       } 
//       else if(!isbase64(src[2])) 
//       {
//           p -= 2;
//           break;
//       } 
//       else if(!isbase64(src[3])) 
//       {
//           p--;
//           break;
//       }
//       src += 4;
//       while(*src && (*src == 13 || *src == 10)) src++;
//     }
//     while(srclen-= 4);
//     *p = 0;
//     return p-dest;
// }

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

// int AVRUnBase64(char *bufplain, const char *bufcoded){
//   int nbytesdecoded;
//   register const unsigned char *bufin;
//   register unsigned char *bufout;
//   register int nprbytes;

//   bufin = (const unsigned char *) bufcoded;
//   while (pgm_read_byte(&pr2six[*(bufin++)]) <= 63);
//   nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
//   nbytesdecoded = ((nprbytes + 3) / 4) * 3;

//   bufout = (unsigned char *) bufplain;
//   bufin = (const unsigned char *) bufcoded;

//   while (nprbytes > 4) {
//       *(bufout++) =
//           (unsigned char) (pgm_read_byte(&pr2six[*bufin]) << 2 | pgm_read_byte(&pr2six[bufin[1]]) >> 4);
//       *(bufout++) =
//           (unsigned char) (pgm_read_byte(&pr2six[bufin[1]]) << 4 | pgm_read_byte(&pr2six[bufin[2]]) >> 2);
//       *(bufout++) =
//           (unsigned char) (pgm_read_byte(&pr2six[bufin[2]]) << 6 | pgm_read_byte(&pr2six[bufin[3]]));
//       bufin += 4;
//       nprbytes -= 4;
//   }

//   /* Note: (nprbytes == 1) would be an error, so just ingore that case */
//   if (nprbytes > 1) {
//       *(bufout++) =
//           (unsigned char) (pgm_read_byte(&pr2six[*bufin]) << 2 | pgm_read_byte(&pr2six[bufin[1]]) >> 4);
//   }
//   if (nprbytes > 2) {
//       *(bufout++) =
//           (unsigned char) (pgm_read_byte(&pr2six[bufin[1]]) << 4 | pgm_read_byte(&pr2six[bufin[2]]) >> 2);
//   }
//   if (nprbytes > 3) {
//       *(bufout++) =
//           (unsigned char) (pgm_read_byte(&pr2six[bufin[2]]) << 6 | pgm_read_byte(&pr2six[bufin[3]]));
//   }

//   *(bufout++) = '\0';
//   nbytesdecoded -= (4 - nprbytes) & 3;
//   return nbytesdecoded; 
// }

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

#endif