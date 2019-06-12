// Fonts.h

#ifndef _FONTS_h
#define _FONTS_h

#include "Arduino.h"

struct FONT_CHAR_INFO
{
   uint8_t width; // width in bits
   uint8_t height;
   uint16_t offset; // offset of char into char array
};

struct FONT_INFO
{
   uint8_t CharHeight;
   char StartChar;
   char EndChar;
   uint8_t SpaceWidth;
   const FONT_CHAR_INFO *Descriptors;
   const uint8_t *Bitmap;
};

// Font data for Comic Sans MS 10pt
extern const uint8_t comicSansMS_10ptBitmaps[];
extern const FONT_INFO comicSansMS_10ptFontInfo;
extern const FONT_CHAR_INFO comicSansMS_10ptDescriptors[];

#endif