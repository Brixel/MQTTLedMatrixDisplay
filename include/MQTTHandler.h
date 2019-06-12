#ifndef _MQTTHANDLER_H
#define _MQTTHANDLER_H

#include "Arduino.h"
#include "WiFi.h"
#include <ArduinoJson.h>
#include "PubSubClient.h"
#include "LedMatrixHandler.h"

void InitMQTTHandler();
void MQTTHandlerLoop();
void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

#endif