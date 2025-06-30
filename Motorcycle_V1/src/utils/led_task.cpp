#include "utils/led_task.h"

void LedTask::begin() {
    pinMode(MCU_LED, OUTPUT);
    digitalWrite(MCU_LED, LOW);
    autoOff = millis() + LED_ACTIVE_MS;
}

void LedTask::set(LedPattern p) {
    pattern = p;
    autoOff = millis() + LED_ACTIVE_MS;
}

void LedTask::loop() {
    if (pattern == LedPattern::kOff) { digitalWrite(MCU_LED, LOW); return; }
    if (millis() > autoOff)          { pattern = LedPattern::kOff; return; }

    unsigned long now = millis();
    switch (pattern) {
        case LedPattern::kBoot:
            digitalWrite(MCU_LED, HIGH);
            break;
        case LedPattern::kIdleArmed:
            if (now - lastBlink >= 500) {
                lastBlink = now; blink = !blink; digitalWrite(MCU_LED, blink);
            }
            break;
        case LedPattern::kAlarm:
            if (now - lastBlink >= 125) {
                lastBlink = now; blink = !blink; digitalWrite(MCU_LED, blink);
            }
            break;
        case LedPattern::kLearn:
            if (now - lastBlink >= 100) {
                lastBlink = now; blink = !blink; digitalWrite(MCU_LED, blink);
            }
            break;
        default: break;
    }
}
