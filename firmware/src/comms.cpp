#include "comms.h"
#include "config.h"
#include <esp_now.h>
#include <WiFi.h>

struct BearMessage {
    uint8_t bear_id;
    bool    is_hugged;
};

static void (*s_on_receive)(bool) = nullptr;

static void on_data_recv(const uint8_t *mac_addr, const uint8_t *data, int len) {
    (void)mac_addr;
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
