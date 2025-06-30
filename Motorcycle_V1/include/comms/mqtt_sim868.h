/**
 * @file mqtt_sim868.h
 * @brief Thin wrapper برای MQTT داخلی SIM868
 */
#pragma once
#include "sim868.h"
#include "config.h"

class MqttSim868 {
 public:
    explicit MqttSim868(Sim868* m): modem(m) {}

    bool connect() {
        /* ایجاد رشتهٔ URL با String تا نیاز به ماکروی STRINGIFY نباشد */
        modem->at(String("AT+SMCONF=\"URL\",\"") + MQTT_HOST + "\"," + String(MQTT_PORT));
        modem->at(String("AT+SMCONF=\"USERNAME\",\"") + MQTT_USER + "\"");
        modem->at(String("AT+SMCONF=\"PASSWORD\",\"") + MQTT_PASS + "\"");
        return modem->at("AT+SMCONN", "OK", 8000);
    }

    void publishEvent(const String& evt) {
        String topic = "/motor/" + evt;
        modem->at(String("AT+SMPUB=\"") + topic + "\"," + evt.length() + ",0,0");
        modem->at(evt);   // payload
    }
 private:
    Sim868* modem;
};
