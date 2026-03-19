#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
// #include "esp_wpa2.h"
// #include "esp_wifi.h"

// #define WIFI_IDENTITY WIFI_IDENTITY
// #define WIFI_PASSWORD WIFI_PASSWORD

// const char* ssid = "eduroam";
const char* ssid = "make";
const char* password = "makeallthestuff";

const char* apiUrl = "https://www.omegav.ntnu.no/api/dooropen.php";

// const char* ca_cert =
// "-----BEGIN CERTIFICATE-----\n"
// "MIIFCTCCAvGgAwIBAgIQHyO1bU6VErdFdXc6SitiaTANBgkqhkiG9w0BAQsFADAX\n"
// "MRUwEwYDVQQDEwxOVE5VIFJvb3QgQ0EwHhcNMTcwMjE2MDc1ODQwWhcNMzcwMjE2\n"
// "MDgwODMzWjAXMRUwEwYDVQQDEwxOVE5VIFJvb3QgQ0EwggIiMA0GCSqGSIb3DQEB\n"
// "AQUAA4ICDwAwggIKAoICAQC/lpPy6tnWjHU4NoH+DB9aWN0FREdCFHM3KC8Zp5qD\n"
// "ItNh+3+M8m4KRwSKZC7fEDxArAz0+JJq/xTd6/ChYUF0ekOXcjdPIO4QJ/v1LcCw\n"
// "ox7DGewEmFVXC7hLBoGst2y1ryugqTYgqQ80bmeX7emWU0Jdn6fC1ejwkPt2E9Sr\n"
// "sMGRAFmnsB/7o8acbzOHmW9PJdAtywDIDaPeEeC+D5QO4YMeqZMCwfhpAFlqRL/4\n"
// "mPBWA6pLXdLM81n3wc5vbIovkqQ7dV5ld4MYZQYjNHfwMQVEX2ksTfbpM0fur/gZ\n"
// "ZYLtdcYKugBu8ujUIJFuJtDrTphhYhdGAhgqZNNgWzXZDzxJzyQE2sMT2nGtSP3q\n"
// "kt9jTKao/ir+sgwnKwpxeWE9xiZ5spf870HPjaceXvxr69ZlhPguUje7rRo6maeD\n"
// "stobztujmwrM7J52YDE+h/RAVfuvZvQn5iwuXrpFlBS5eMNzb+2qM1bqWJMs/QPg\n"
// "+7BqF1D3jjuE3+b3m1E4B14D2KBLWNgNPUxvX/V3+QLdGFBbjFuaMunMVyN3eQHw\n"
// "DIjV+URtSurPP1cjkxEFdiNCrkuGzwlnXns02eZLZOPSWSTGJiSFtgg/COX1ATXI\n"
// "6rI9kUMtrqKW4zQl73E0agwC0d7frZLyT1vKI5UQwlJpHKqeZKl0AIKqMD4TlJAz\n"
// "wQIDAQABo1EwTzALBgNVHQ8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n"
// "FgQUO1o7e1D0r80yyoohDcsZScAMpkMwEAYJKwYBBAGCNxUBBAMCAQAwDQYJKoZI\n"
// "hvcNAQELBQADggIBADAGiBbf0/5YCCG6EwLNef8+Y+FHlvBamDmMcuSf4nkSwzQm\n"
// "pgR7hn6ozzneE2P2nQU6+qJGKeS1tpQQsk53k4suPcDuwgMf79C03iGJV9tozieD\n"
// "0wLezLiyifIpbC6prKqFLW8IWYB/ILi7Xtj5W3ooXvvPT262sjDG1BSXnC0OHVuc\n"
// "hj6kQOw3cIzCP9SvhJeyiYRa3iqT4iXF6EmTDCB7B+401bkzlxCwL6LalCfx4rcm\n"
// "rDy1tnA1k8LkcilbWj1ZmI/OIWRs62W5JxQov4Y+5ozqikEK5/XArsKZ0NzAnVyj\n"
// "iJZ4SNaq4S/HLyakHB8KEohVa9ImRAE3GlPrOxiy9HCOyIlWhW61A7iL11bM0d+z\n"
// "Lw7PCXedh8lANFJzQb8ACNfsR1IOwSKC/fYoYU9m2ZIdHRGYgNMaDf6/h1ZnX4A3\n"
// "4DBag4FeZITBlDkywPvHw0tbYejIABbHoEzLrKiNBPqnOFrHOhJmhtaSICHK9zdJ\n"
// "cp2CD5O4r2pwgmuKqSqT9u4sWHldT+gm/glG+jynCZlIvKaXGgjv36B5/sqMdhFr\n"
// "beS3rjOLE8Jn92lIZHgtS4ILgVCubYA/54Hdtr0kn6sUC0DiHLwFgjUR5ob/Mz47\n"
// "W0SOrJdlXcoYjm+kQLlGlN3vuzfKn6FIzKoZPSm0J8Glhr1otGIf33ap637n\n"
// "-----END CERTIFICATE-----\n";

void WiFiEvent(WiFiEvent_t event) {
    switch(event) {
        case SYSTEM_EVENT_STA_START:
            Serial.println("STA Started");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("WiFi connected to AP, starting auth...");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("WiFi disconnected or auth failed");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.println("WiFi got IP address!");
            Serial.print("IP: "); Serial.println(WiFi.localIP());
            break;
        default:
            Serial.print("Other WiFi Event: "); Serial.println(event);
            break;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    // esp_log_level_set("wifi", ESP_LOG_DEBUG);
    // esp_log_level_set("wpa", ESP_LOG_DEBUG);
    // esp_log_level_set("wifi_sta", ESP_LOG_INFO);

    WiFi.onEvent(WiFiEvent);

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid);
    Serial.println("Connecting to eduroam...");


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
      Serial.print("Wi-Fi status: ");
      switch(WiFi.status()) {
          case WL_NO_SHIELD:    Serial.println("No shield"); break;
          case WL_IDLE_STATUS:  Serial.println("Idle"); break;
          case WL_NO_SSID_AVAIL:Serial.println("SSID not available"); break;
          case WL_SCAN_COMPLETED:Serial.println("Scan done"); break;
          case WL_CONNECTED:    Serial.println("Connected"); break;
          case WL_CONNECT_FAILED:Serial.println("Connect failed"); break;
          case WL_CONNECTION_LOST:Serial.println("Connection lost"); break;
          case WL_DISCONNECTED: Serial.println("Disconnected"); break;
          default:              Serial.println("Unknown"); break;
      }
      delay(10000);
      return;
  }

  else {
    HTTPClient http;

    http.begin(apiUrl); 
    int httpResponseCode = http.GET(); 
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response from API: " + response);

      const size_t capacity = JSON_OBJECT_SIZE(2);
      DynamicJsonDocument doc(capacity);
      DeserializationError error = deserializeJson(doc, response);

      if (!error) {
        int doorStatus = doc["door"];
        Serial.println("Door status: " + String(doorStatus));

      } else {
        Serial.println("Failed to parse JSON");
      }

  } else {
      Serial.println("Error on HTTP request: " + String(httpResponseCode));
  }

  http.end(); // Free resources

} 
delay(5000); // Wait 5 seconds before next request
}
