/**
 * @file rf_receiver.h
 * @brief Wrapper کتابخانه rc-switch
 */
#pragma once
#include <RCSwitch.h>
#include "config.h"

class RfReceiver {
 public:
    void begin()        { pinMode(REM_CS, OUTPUT); digitalWrite(REM_CS, HIGH); rc.enableReceive(REM_DATA); }
    bool available()    { return rc.available();   }
    uint32_t readCode() { return rc.getReceivedValue(); }
    void reset()        { rc.resetAvailable();     }
 private:
    RCSwitch rc;
};
