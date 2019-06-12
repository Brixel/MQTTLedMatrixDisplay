#ifndef _MAIN_H
#define _MAIN_H

#include <Arduino.h>
#include "main.h"
#include <WiFi.h>
#include "LedMatrixHandler.h"
#include "MQTTHandler.h"

#include <esp_spi_flash.h>   // needed for reading ESP32 chip attributes
#include <esp_event_loop.h>  // needed for Wifi event handler
#include <esp32-hal-timer.h> // needed for timers
#include <esp_coexist.h>     // needed for showing coex sw version

void callback(char *topic, byte *payload, unsigned int length);
void reconnect();
void setup_wifi();
#endif
