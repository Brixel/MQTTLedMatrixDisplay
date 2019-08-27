#include "LedMatrixHandler.h"

#define NUMCHARS 5

// local Tag for logging
static const char TAG[] = __FILE__;

LEDMatrix matrix(LED_MATRIX_LA_74138, LED_MATRIX_LB_74138, LED_MATRIX_LC_74138,
                 LED_MATRIX_LD_74138, LED_MATRIX_EN_74138, LED_MATRIX_DATA_R1,
                 LED_MATRIX_LATCHPIN, LED_MATRIX_CLOCKPIN);

// Display Buffer 128 = 64 * 16 / 8
uint8_t displaybuf[LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT / NUMCHARS];

const FONT_INFO *ActiveFontInfo = &comicSansMS_10ptFontInfo;
const uint8_t *iaActiveFont = ActiveFontInfo->Bitmap;
const FONT_CHAR_INFO *ActiveFontCharInfo = ActiveFontInfo->Descriptors;

String strCurMsg = "";
bool bMsgScrolling = false;
unsigned long ulLastScrollStep = 0;
unsigned long ulScrollStepInterval = 800;
uint iScrollCurrentFirstChar = 0;
uint iScrollCharStart = 0;

void init_matrix_display()
{
   ESP_LOGI(TAG, "Initializing LED Matrix display");
   matrix.begin(displaybuf, LED_MATRIX_WIDTH, LED_MATRIX_HEIGHT);
   matrix.reverse();
   matrix.clear();
   DrawASCII(String("0"));
} // init_display

void MatrixDisplayLoop()
{
   matrix.scan();
   HandleScrollingText();
}

void HandleScrollingText()
{
   if (!bMsgScrolling || millis() - ulLastScrollStep < ulScrollStepInterval)
   {
      return;
   }
   ulLastScrollStep = millis();
   /*iScrollCharStart++;
   if (iScrollCharStart > GetCharWidth(strCurMsg.charAt(iScrollCurrentFirstChar)))
   {
      iScrollCurrentFirstChar++;
      iScrollCharStart = 0;
   }*/

   String strNextPart = strCurMsg.substring(iScrollCurrentFirstChar);
   String strTmpMsg = TruncateStringForDisplay(strNextPart);
   matrix.clear();
   DrawASCII(strTmpMsg);
   //ESP_LOGD(TAG, "Scrolling to next char at pos %i, remainder: %s, truncated: %s", iScrollCurrentFirstChar, strNextPart.c_str(), strTmpMsg.c_str());
   if (strTmpMsg.length() == strNextPart.length())
   {
      iScrollCurrentFirstChar = 0;
   }
   else
   {
      iScrollCurrentFirstChar++;
   }
}

// (x, y) top-left position, x should be multiple of 8
void DrawChar(uint16_t x, uint16_t y, char cChar)
{
   // Get address of char in font char descriptor from font descriptor
   int CharDescAddress = (cChar - ActiveFontInfo->StartChar);

   // Get offset of char into font bitmap
   uint16_t FontBitmapOffset = ActiveFontCharInfo[CharDescAddress].offset;
   //ESP_LOGV(TAG, "Got offset of char %c at index %i (%i - %i): %lu", cChar, CharDescAddress, (int)cChar, (int)ActiveFontInfo->StartChar, FontBitmapOffset);
   // Serial.printf("Address of %c is %i, bitmap offset is %u\r\n", cChar,
   //              CharDescAddress, FontBitmapOffset);

   // Check font height, if it's less than matrix height we need to
   // add some empty lines to font does not stick to the top
   if (ActiveFontInfo->CharHeight < (LED_MATRIX_HEIGHT - y))
   {
      uint8_t FillerLines = (LED_MATRIX_HEIGHT - y) - ActiveFontInfo->CharHeight;
      if (FillerLines % 2 == 0)
      {
         // Use floor (round down) to get heighest possible divider
         // for missing lines
         y = floor(FillerLines / 2);
      }
   }

   int iDst = (x / 8) + (y * 8);
   int Shift = x % 8;
   // Serial.printf("Got hex '%x'\r\n", pSrc);
   for (uint8_t i = 0; i < ActiveFontCharInfo[CharDescAddress].height; i++)
   {
      int iDigitA = iaActiveFont[FontBitmapOffset];

      int Left = iDigitA >> Shift;
      int Right = iDigitA << (8 - Shift);

      displaybuf[iDst] |= Left;
      displaybuf[iDst + 1] |= Right;

      if (ActiveFontCharInfo[CharDescAddress].width > 8)
      {
         int iDigitB = iaActiveFont[FontBitmapOffset + 1];
         Left = iDigitB >> Shift;
         Right = iDigitB << (8 - Shift);

         displaybuf[iDst + 1] |= Left;
         displaybuf[iDst + 2] |= Right;
         FontBitmapOffset++;
      }

      FontBitmapOffset++;
      iDst += 8;
   }
}

void DrawASCII(String strInput)
{
   uint8_t iLength = strInput.length();
   uint8_t iDigitPos = 0;
   for (int i = 0; i < iLength; i++)
   {
      DrawChar(iDigitPos, 0, strInput.charAt(i));
      iDigitPos = iDigitPos + GetCharWidth(strInput.charAt(i)) +
                  ActiveFontInfo->SpaceWidth;
      if (iDigitPos + GetCharWidth(strInput.charAt(i + 1)) > LED_MATRIX_WIDTH)
      {
         return;
      }
   }
}

uint8_t GetCharFromFont(char cChar)
{
   auto cStartChar = ActiveFontInfo->StartChar;
   auto iCharLocation = cChar - cStartChar;
   auto iCharBitmap = iaActiveFont[iCharLocation];

   return iCharBitmap;
}

uint8_t GetCharWidth(char cChar)
{
   // Get address of char in font char descriptor from font descriptor
   auto CharDescAddress = (cChar - ActiveFontInfo->StartChar);

   // Get offset of char into font bitmap
   auto CharDescriptor = ActiveFontCharInfo[CharDescAddress];
   // Serial.printf("Char %c is %i wide\r\n", cChar, CharDescriptor.width);
   return CharDescriptor.width;
}

void UpdateMessage(String strMsg)
{
   matrix.clear();
   ESP_LOGD(TAG, "Updating display: %s", strMsg.c_str());
   String strTempMsg = TruncateStringForDisplay(strMsg);
   strCurMsg = strMsg;
   DrawASCII(strTempMsg);
   if (strCurMsg.length() != strTempMsg.length())
   {
      bMsgScrolling = true;
      strCurMsg = PadMessageString(strCurMsg);
   }
   else
   {
      bMsgScrolling = false;
   }
}

String TruncateStringForDisplay(String strInput)
{
   uint8_t iLength = strInput.length();
   uint8_t iDigitPos = 0;
   String strOutput = "";
   uint8_t charWidths[iLength];
   for (int i = 0; i < iLength; i++)
   {
      char c = strInput.charAt(i);

      charWidths[i] = GetCharWidth(c);
      iDigitPos = iDigitPos + charWidths[i];
      if (i + 1 < strInput.length())
      {
         iDigitPos += ActiveFontInfo->SpaceWidth;
      }
      if (iDigitPos + 1 <= LED_MATRIX_WIDTH)
      {
         strOutput += c;
      }
   }
   /*
   ESP_LOGV(TAG, "Total length of input: %i:", iDigitPos);
   for (int i = 0; i < iLength; i++)
   {
      ESP_LOGV(TAG, " POS %i: %i + %i", i, charWidths[i], i + 1 < strInput.length() ? ActiveFontInfo->SpaceWidth : 0);
   }
*/
   return strOutput;
}

String PadMessageString(String input)
{
   String strOutput = input;
   for (int i = 0; i < LED_MATRIX_WIDTH - iDigitPos; i++)
   {
      strOutput += " ";
   }
   return strOutput;
}

void SetScrollInterval(unsigned long interval)
{
   ulScrollStepInterval = interval;
}
