/**
 * @file sms_parser.h
 * @brief تحلیل فرمان‌های SMS
 */
#pragma once
#include <Arduino.h>
#include "core/registry.h"

class SmsParser {
 public:
    explicit SmsParser(Registry* r): reg(r) {}
    void handleSms(const String& raw, class StateMachine& fsm);
 private:
    Registry* reg;
    void parseCmd(const String& body, StateMachine& fsm);
};
