#include "main.h"

//WiFi params
const char *ssid = "Brixel";
const char *password = "CLUBMATE2015";

void setup()
{
   pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
   Serial.begin(115200);
   setup_wifi();
   InitMQTTHandler();
   init_matrix_display();

   UpdateMessage(String("ur name here? Publish something to MQTT topic /brixel/room1/ledmatrix/message"));
}

void loop()
{
   MQTTHandlerLoop();
   MatrixDisplayLoop();
}

void setup_wifi()
{

   delay(10);
   // We start by connecting to a WiFi network
   Serial.println();
   ESP_LOGI(TAG, "Connecting to %s", ssid);

   WiFi.begin(ssid, password);

   while (WiFi.status() != WL_CONNECTED)
   {
      delay(500);
      Serial.print(".");
   }

   randomSeed(micros());

   Serial.println("");
   ESP_LOGI(TAG, "WiFi connected, IP address: %i", WiFi.localIP().toString().c_str());
}
