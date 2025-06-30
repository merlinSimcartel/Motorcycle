/**
 * @file ignition_switch.h
 * @brief مانیتور سوئیچ ACC – نسخهٔ ایمن برای ISR
 * @license MIT
 */
#pragma once
#include <Arduino.h>
#include "config.h"

class IgnitionSwitch {
 public:
    void begin();                   // فراخوانی در setup()
    bool isOn()        const;       // وضعیت فعلی
    bool isOnChanged();             // true اگر لبۀ جدید ثبت شد

 private:
    volatile bool     changed    {false};
    volatile uint8_t  last_level {0};

    static void IRAM_ATTR gpioIsr(void* arg);  // ISR
};
