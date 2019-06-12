// clang-format off

#ifndef _LOLIN32_H
#define _LOLIN32_H

#include <stdint.h>

// Hardware related definitions for lolin32lite (without LoRa shield)

#define HAS_LED LED_BUILTIN // on board LED on GPIO5
#define LED_ACTIVE_LOW 1    // Onboard LED is active when pin is LOW

#define MATRIX_DISPLAY_SCAN_US                                                 \
  500 // Matrix display scan rate in microseconds (1ms is about 'acceptable')
#define LED_MATRIX_LATCHPIN 13 // Connects to LAT pin on display
#define LED_MATRIX_CLOCKPIN 32 // Connects to CLK pin on display
#define LED_MATRIX_EN_74138 12 // Connects to OE pin on display
#define LED_MATRIX_LA_74138 14 // Connects to LA pin on display
#define LED_MATRIX_LB_74138 27 // Connects to LB pin on display
#define LED_MATRIX_LC_74138 25 // Connects to LC pin on display
#define LED_MATRIX_LD_74138 26 // Connects to LD pin on display
#define LED_MATRIX_DATA_R1 33  // Connects to R1 pin on display

#endif
