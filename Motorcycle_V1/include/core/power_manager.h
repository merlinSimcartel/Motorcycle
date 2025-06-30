/**
 * @file power_manager.h
 * @brief مدیریت توان (بسیار ساده)
 * @license MIT
 */
#pragma once
#include <Arduino.h>

class PowerManager {
 public:
    void begin();                    // آماده‌سازی
    void deepSleep(uint64_t us);     // خواب عمیق
};
