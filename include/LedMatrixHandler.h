#ifndef _MATRIX_DISPLAY_H
#define _MATRIX_DISPLAY_H

#include "LEDMatrix.h"
#include "ledmatrixfonts.h"

#define LED_MATRIX_WIDTH 64  // Width in pixels (LEDs) of your display
#define LED_MATRIX_HEIGHT 16 // Height in pixels (LEDs ) of your display

extern LEDMatrix _matrix;

void init_matrix_display();
void MatrixDisplayLoop(void);
void HandleScrollingText();
void DrawASCII(String strInput);
void UpdateMessage(String strMsg);
uint8_t GetCharFromFont(char cChar);
uint8_t GetCharWidth(char cChar);
String TruncateStringForDisplay(String strInput);
void SetScrollInterval(unsigned long interval);

#endif