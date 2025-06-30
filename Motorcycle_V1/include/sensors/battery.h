/**
 * @file battery.h
 * @brief اندازه‌گیری ولتاژ باتری
 */
#pragma once
#include <Arduino.h>
class BatteryMonitor {
 public:
    void  begin() { analogSetPinAttenuation(34, ADC_11db); }
    float read() {
        int sum = 0;
        for (int i = 0; i < 10; ++i) { sum += analogRead(34); delay(2); }
        float mv = (sum / 10.0f) * 3.3f / 4095.0f * 2;   // ضرب در نسبت تقسیم
        return mv;
    }
};
