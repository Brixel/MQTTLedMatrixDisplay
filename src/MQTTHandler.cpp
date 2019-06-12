#include "MQTTHandler.h"
#include <ArduinoJson.h>

WiFiClient nsClient;
PubSubClient psClient(nsClient);
const char *mqtt_server = "192.168.20.100";
long lastMsg = 0;
char msg[50];
int value = 0;
#define BASE_TOPIC "/brixel/room1/ledmatrix"
#define STATUS_TOPIC "/brixel/room1/ledmatrix/status"
#define MSG_TOPIC "/brixel/room1/ledmatrix/message"
#define SETTINGS_TOPIC "/brixel/room1/ledmatrix/settings"

void InitMQTTHandler()
{
   psClient.setServer(mqtt_server, 1883);
   psClient.setCallback(callback);
}

void MQTTHandlerLoop()
{
   if (!psClient.connected())
   {
      reconnect();
   }
   psClient.loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
   ESP_LOGI(TAG, "Message arrived [%s]: %s", topic, payload);

   if (strcasecmp(topic, MSG_TOPIC) == 0)
   {
      String strPayload;
      for (uint i = 0; i < length; i++)
      {
         strPayload += (char)payload[i];
      }
      UpdateMessage(strPayload);
   }
   else if (strcasecmp(topic, SETTINGS_TOPIC) == 0)
   {
      StaticJsonDocument<200> settings;
      DeserializationError error = deserializeJson(settings, payload);
      // Test if parsing succeeds.
      if (error)
      {
         ESP_LOGE(TAG, "deserializeJson() failed: %s", error.c_str());
         return;
      }

      if (settings.containsKey("ScrollInterval"))
      {
         SetScrollInterval((unsigned long)settings["ScrollInterval"]);
      }
      ESP_LOGI(TAG, "Successfully set scroll step interval to %lu", (unsigned long)settings["ScrollInterval"]);
   }
}

void reconnect()
{
   // Loop until we're reconnected
   while (!psClient.connected())
   {
      ESP_LOGI(TAG, "Attempting MQTT connection...");
      // Create a random psClient ID
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (psClient.connect(clientId.c_str(), STATUS_TOPIC, 1, false, "0"))
      {
         ESP_LOGI(TAG, "MQTT connected");
         // Once connected, publish an announcement...
         psClient.publish(STATUS_TOPIC, "1");
         // ... and resubscribe
         psClient.subscribe(MSG_TOPIC);
         psClient.subscribe(SETTINGS_TOPIC);
      }
      else
      {
         ESP_LOGE(TAG, "failed, rc=%i,  try again in 5 seconds", psClient.state());
         delay(5000);
      }
   }
}
