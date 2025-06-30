#include "core/power_manager.h"

void PowerManager::begin() {
    // در نسخهٔ فعلی کاری نمی‌کند؛ GPIOها را اینجا تنظیم کنید.
}

void PowerManager::deepSleep(uint64_t us) {
    esp_sleep_enable_timer_wakeup(us);
    esp_deep_sleep_start();
}
