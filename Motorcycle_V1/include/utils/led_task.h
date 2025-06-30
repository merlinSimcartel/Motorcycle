/**
 * @file led_task.h
 * @brief تسک LED وضعیت
 */
#pragma once
#include <Arduino.h>
#include "config.h"

enum class LedPattern : uint8_t { kOff, kBoot, kIdleArmed, kAlarm, kLearn };

class LedTask {
 public:
    void begin();
    void loop();
    void set(LedPattern p);

 private:
    LedPattern    pattern   {LedPattern::kBoot};
    unsigned long lastBlink {0};
    uint8_t       blink     {0};
    unsigned long autoOff   {0};
};
