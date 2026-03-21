#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include "esp_wpa2.h"

#include "wifiPass.h"

#define LED 2         // internal led
#define LED_PIN 5     // GPIO pin
#define NUM_LEDS 2    // number of *groups*, not individual LEDs

const char* ssid = "eduroam";

// const char* ssid = "make";
// const char* password = "makeallthestuff";

const char* apiUrl = "https://www.omegav.ntnu.no/api/dooropen.php";

CRGB leds[NUM_LEDS];

void setup() {

    Serial.begin(115200);
    delay(1000);

    pinMode(LED, OUTPUT);       // for testing internal LED

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);

    // for connecting to wifi (not eduroam)
    // WiFi.begin(ssid, password);    

    // connecting to eduroam
    WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD); 

    WiFi.begin(ssid); 
    Serial.println("Connecting to wifi...");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {

  // led strip blinking
  // fill_solid(leds, NUM_LEDS, CRGB::Red);
  // FastLED.show();
  // Serial.println("Red");
  // delay(2000);
  
  // fill_solid(leds, NUM_LEDS, CRGB::Black);
  // FastLED.show();
  // Serial.println("Black");
  // delay(2000);

  if(WiFi.status() != WL_CONNECTED) {
      Serial.print("Wi-Fi not connected");
      return;
  }

  else {
    HTTPClient http;

    http.begin(apiUrl); 
    int httpResponseCode = http.GET();        // response is 200 if OK
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response from API: " + response);

      // const size_t capacity = JSON_OBJECT_SIZE(2) + 20;
      // DynamicJsonDocument doc(capacity);
      DynamicJsonDocument doc(256);
      DeserializationError error = deserializeJson(doc, response);

      if (!error) {
        int doorStatus = doc["open"];
        Serial.println("Door status: " + String(doorStatus));
        if (doorStatus) {
          digitalWrite(LED,HIGH);
        } else {
          digitalWrite(LED,LOW);
        }
      } else {
        Serial.println("Failed to parse JSON: ");
        Serial.println(error.c_str());
      }
      

  } else {
      Serial.println("Error on HTTP request: " + String(httpResponseCode));
  }

  http.end(); 

  } 
  delay(5000); // Wait 5 seconds before next request

  // testing led blinking
//   delay(500);
//   digitalWrite(LED,HIGH);
//   delay(2000);
//   digitalWrite(LED,LOW);
}
