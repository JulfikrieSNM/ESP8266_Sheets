#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

// Wi-Fi Credentials
const char* ssid = "SpektraAdmin-2.4Ghz@unifi";
const char* password = "SPEKTRA4419";

// Vibration Sensor Pin (D0 output from module connected to D2)
#define VIBRATION_PIN D2

// Google Form URL (Replace with your own form's "formResponse" URL + entry ID)
const String form_url = "[Link URL Sheets]";

// Secure WiFi Client
WiFiClientSecure secureClient;

void setup() {
    Serial.begin(115200);
    pinMode(VIBRATION_PIN, INPUT);

    // Connect WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\n✅ Connected to WiFi!");
    secureClient.setInsecure(); // Skip SSL certificate check
}

void loop() {
    int vibrationValue = digitalRead(VIBRATION_PIN);

    Serial.print("Vibration Sensor (D2): ");
    Serial.println(vibrationValue);

    // === Send to Google Form ===
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        // Append value to Google Form entry
        String finalURL = form_url + String(vibrationValue) + "&submit=Submit";

        http.begin(secureClient, finalURL);
        int httpCode = http.GET();

        if (httpCode > 0) {
            Serial.println("✅ Data sent to Google Form!");
        } else {
            Serial.print("❌ Failed. HTTP code: ");
            Serial.println(httpCode);
        }
        http.end();
    } else {
        Serial.println("⚠️ WiFi not connected!");
    }

    delay(5000);  // Run every 5 seconds
}
