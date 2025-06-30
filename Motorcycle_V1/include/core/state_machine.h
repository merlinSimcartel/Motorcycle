/**
 * @file state_machine.h
 * @brief منطق FSM سطح بالا
 * @license MIT
 */
#pragma once
#include <Arduino.h>

/* --- رویدادهای سیستم --- */
enum class AlarmEvent : uint8_t {
    kBoot, kIgnitionOn, kIgnitionOff, kShock,
    kRemoteDisarm, kRemoteArm,
    kSmsDisarm, kSmsArm,
    kTimer1H, kTimer5H, kTimer24H,
    kTimeoutGrace, kAlarmTimeout
};

/* --- حالت‌ها --- */
enum class AlarmState : uint8_t {
    kBoot, kIdleDisarmed, kIdleArmed,
    kPreAlert, kAlarm,
    kTrack1H, kTrack5H, kTrack24H,
    kShutdown
};

class StateMachine {
 public:
    void begin();
    void loop();
    void postEvent(AlarmEvent ev);
    AlarmState state() const { return currentState; }

 private:
    AlarmState    currentState {AlarmState::kBoot};
    unsigned long stateTs      {0};     // زمان ورود به حالت
    unsigned long timerDue     {0};     // سررسید تایمر
    AlarmEvent    timerEvent   {AlarmEvent::kBoot};

    /* کمکی‌ها */
    void transition(AlarmState next);
    void handleState();
    void scheduleNextReport();
    void cancelTimers();
    void sendAlarmSms();
    void startSiren(bool on);
};
