#include "comms.h"
#include "config.h"
#include <WiFi.h>

static void (*s_on_receive)(bool) = nullptr;

// ─────────────────────────────────────────────────────────────────────────────
#ifdef USE_MQTT
// MQTT section is identical between Arduino ESP32 2.x and 3.x — PubSubClient
// is a third-party library with no core-version dependency.
// This block is an exact copy of src/comms.cpp's MQTT section.
// ─────────────────────────────────────────────────────────────────────────────
#include <PubSubClient.h>

static WiFiClient   s_wifi_client;
static PubSubClient s_mqtt(s_wifi_client);
static char         s_pub_topic[32];
static char         s_sub_topic[32];
static uint32_t     s_last_reconnect_ms = 0;

static void mqtt_callback(char* topic, byte* payload, unsigned int len) {
    (void)topic;
    if (len < 1) return;
    if (s_on_receive) s_on_receive(payload[0] == '1');
}

static void reconnect_mqtt() {
    if (s_mqtt.connected()) return;
    if (WiFi.status() != WL_CONNECTED) return;
    if (millis() - s_last_reconnect_ms < 5000) return;
    s_last_reconnect_ms = millis();

    char client_id[32];
    snprintf(client_id, sizeof(client_id), "cutiepie-%d-%lu", BEAR_ID, millis());

    Serial.print("MQTT connecting...");
    bool ok = (strlen(MQTT_USER) > 0)
        ? s_mqtt.connect(client_id, MQTT_USER, MQTT_PASS)
        : s_mqtt.connect(client_id);

    if (ok) {
        Serial.println(" connected");
        s_mqtt.subscribe(s_sub_topic);
    } else {
        Serial.printf(" failed (rc=%d) — retrying in 5 s\n", s_mqtt.state());
    }
}

void comms_init(void (*on_receive)(bool remote_is_hugged)) {
    s_on_receive = on_receive;

    snprintf(s_pub_topic, sizeof(s_pub_topic), "cutiepie/%d/hug", BEAR_ID);
    snprintf(s_sub_topic, sizeof(s_sub_topic), "cutiepie/%d/hug", 1 - BEAR_ID);

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
        delay(250);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf(" connected (%s)\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println(" FAILED — check WIFI_SSID / WIFI_PASSWORD in config.h");
    }

    s_mqtt.setServer(MQTT_BROKER, MQTT_PORT);
    s_mqtt.setCallback(mqtt_callback);
    reconnect_mqtt();
}

void comms_send(bool is_hugged) {
    if (!s_mqtt.connected()) {
        Serial.println("comms_send skipped — MQTT not connected");
        return;
    }
    // Retained message: a bear that comes online later will immediately get
    // the current hug state without waiting for the next press/release.
    bool ok = s_mqtt.publish(s_pub_topic, is_hugged ? "1" : "0", /*retain=*/true);
    if (!ok) Serial.println("comms_send failed");
}

void comms_update() {
    reconnect_mqtt();
    s_mqtt.loop();
}

// ─────────────────────────────────────────────────────────────────────────────
#else
// Arduino ESP32 3.x ESP-NOW — only difference from 2.x is the callback signature.
// 2.x: (const uint8_t *mac_addr, const uint8_t *data, int len)
// 3.x: (const esp_now_recv_info_t *info, const uint8_t *data, int len)
// ─────────────────────────────────────────────────────────────────────────────
#include <esp_now.h>

struct BearMessage {
    uint8_t bear_id;
    bool    is_hugged;
};

static void on_data_recv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    (void)info;
    if (len != sizeof(BearMessage)) return;
    BearMessage msg;
    memcpy(&msg, data, sizeof(msg));
    if (s_on_receive) s_on_receive(msg.is_hugged);
}

void comms_init(void (*on_receive)(bool remote_is_hugged)) {
    s_on_receive = on_receive;

    WiFi.mode(WIFI_STA);
    Serial.print("This bear's MAC: ");
    Serial.println(WiFi.macAddress());

    if (esp_now_init() != ESP_OK) {
        Serial.println("ERROR: ESP-NOW init failed");
        return;
    }

    esp_now_register_recv_cb(on_data_recv);

    esp_now_peer_info_t peer = {};
    memcpy(peer.peer_addr, PEER_MAC, 6);
    peer.channel = 0;
    peer.encrypt = false;
    if (esp_now_add_peer(&peer) != ESP_OK) {
        Serial.println("ERROR: Failed to add peer — check PEER_MAC in config.h");
    }
}

void comms_send(bool is_hugged) {
    BearMessage msg = { .bear_id = BEAR_ID, .is_hugged = is_hugged };
    esp_err_t err = esp_now_send(PEER_MAC, (uint8_t *)&msg, sizeof(msg));
    if (err != ESP_OK) {
        Serial.printf("comms_send failed: %s\n", esp_err_to_name(err));
    }
}

void comms_update() {} // no-op — ESP-NOW is interrupt-driven, no polling needed

#endif
