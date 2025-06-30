/**
 * @file relay.h
 * @brief خروجی رله (Normally-Closed)
 */
#pragma once
#include <Arduino.h>
#include "config.h"
class Relay {
 public:
    void begin()          { pinMode(OFF_SWITCH, OUTPUT); digitalWrite(OFF_SWITCH, LOW); }
    void toggle(bool cut) { digitalWrite(OFF_SWITCH, cut ? HIGH : LOW); }
};
