#include <Arduino.h>
#include <WiFi.h>

// ── Test: Board alive + MAC address ──────────────────────────────────────────
// Nothing needs to be connected — just the ESP32-S3 via USB.
// Open Serial Monitor at 115200 baud.
// You should see the MAC address print every 3 seconds.
// Copy this MAC — you'll need it when setting up the bear pair.

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    Serial.println("─────────────────────────────");
    Serial.println("CutiePI board test — OK");
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());
    Serial.println("─────────────────────────────");
}

void loop() {
    Serial.print("Still alive — MAC: ");
    Serial.println(WiFi.macAddress());
    delay(3000);
}
