#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "wifiPass.h"

#define LED 2

// const char* ssid = "make";
// const char* password = "makeallthestuff";

const char* apiUrl = "https://www.omegav.ntnu.no/api/dooropen.php";

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(LED, OUTPUT);       // for testing internal LED

    WiFi.mode(WIFI_STA); 
    WiFi.begin(ssid, password);
    Serial.println("Connecting to wifi...");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

void loop() {

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
