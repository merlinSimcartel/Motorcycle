/**
 * @file sim868.h
 * @brief درایور سادهٔ AT برای SIM868
 */
#pragma once
#include <Arduino.h>
#include "core/registry.h"

class Sim868 {
 public:
    static Sim868& instance() { static Sim868 s; return s; }

    void  begin(Registry* r);
    void  wake();
    void  sleep();
    void  sendSmsAdmin(const String& txt);
    void  publishLocation();
    void  processUrc(class SmsParser& sp, class StateMachine& fsm);

    /* Low-level */
    bool   at(const String& cmd, const String& ok = "OK", uint16_t tout = 1200);
    String query(const String& cmd, uint16_t tout = 1200);

 private:
    Sim868() = default;                        // Singleton
    Registry*       reg{};
    HardwareSerial  gsm{1};
    void   send(const String& s) { gsm.print(s); gsm.print("\r"); }
    bool   waitFor(const String& key, uint16_t tout);
};
