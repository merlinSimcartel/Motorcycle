/**
 * @file siren.h
 * @brief درایور آژیر
 */
#pragma once
#include <Arduino.h>
#include "config.h"
class Siren {
 public:
    void begin()     { pinMode(SIREN, OUTPUT); digitalWrite(SIREN, LOW); }
    void set(bool o) { digitalWrite(SIREN, o ? HIGH : LOW); }
};
