#pragma once
#include <Arduino.h>
class Log {
 public:
    void begin(){}   // Placeholder
    template<typename... A> void printf(const char* f, A... a) { ::printf(f, a...); }
};
