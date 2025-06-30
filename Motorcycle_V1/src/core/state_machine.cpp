/**
 * @file state_machine.cpp
 * @brief پیاده‌سازی FSM
 */
#include "core/state_machine.h"
#include "config.h"
#include "actuators/siren.h"
#include "actuators/relay.h"
#include "comms/sim868.h"
#include "comms/mqtt_sim868.h"

/* اشیای محلی */
static Siren      siren;
static Relay      relay;
static MqttSim868 mqtt(&Sim868::instance()); // متد instance() بعداً اضافه می‌شود

/* ---------- API ---------- */
void StateMachine::begin() {
    siren.begin();
    relay.begin();
    transition(AlarmState::kIdleDisarmed);
}

void StateMachine::postEvent(AlarmEvent ev) {
    switch (ev) {
        case AlarmEvent::kShock:
            if (currentState == AlarmState::kIdleArmed)
                transition(AlarmState::kPreAlert);
            break;
        case AlarmEvent::kRemoteDisarm:
        case AlarmEvent::kSmsDisarm:
            transition(AlarmState::kIdleDisarmed);
            break;
        case AlarmEvent::kRemoteArm:
        case AlarmEvent::kSmsArm:
            transition(AlarmState::kIdleArmed);
            break;
        default:
            break;
    }
}

void StateMachine::loop() {
    handleState();
    if (timerDue && millis() >= timerDue) {
        AlarmEvent ev = timerEvent;
        timerDue   = 0;
        postEvent(ev);
    }
}

/* ---------- منطق داخلی ---------- */
void StateMachine::handleState() {
    if (currentState == AlarmState::kPreAlert &&
        millis() - stateTs > 5000) {          // ۵ ثانیه فرصت
        transition(AlarmState::kAlarm);
    }
}

void StateMachine::transition(AlarmState next) {
    /* خروج از حالت قبلی */
    if (currentState == AlarmState::kAlarm) {
        startSiren(false);
        relay.toggle(false);
    }

    currentState = next;
    stateTs      = millis();

    /* ورود به حالت جدید */
    switch (next) {
        case AlarmState::kIdleDisarmed:
        case AlarmState::kIdleArmed:
            cancelTimers();
            break;

        case AlarmState::kPreAlert:
            /* همچنان ۵ ثانیه مهلت */
            break;

        case AlarmState::kAlarm:
            startSiren(true);
            sendAlarmSms();
            relay.toggle(true);               // قطع برق ۱۰ ثانیه
            timerDue   = millis() + 10000;
            timerEvent = AlarmEvent::kAlarmTimeout;
            break;

        case AlarmState::kTrack1H:
            mqtt.publishEvent("track1h");
            scheduleNextReport();
            break;

        default:
            break;
    }
}

void StateMachine::scheduleNextReport() {
    if (currentState == AlarmState::kTrack1H) {
        timerDue   = millis() + 3600000UL;     // 1 h
        timerEvent = AlarmEvent::kTimer1H;
    } else if (currentState == AlarmState::kTrack5H) {
        timerDue   = millis() + 18000000UL;    // 5 h
        timerEvent = AlarmEvent::kTimer5H;
    } else if (currentState == AlarmState::kTrack24H) {
        timerDue   = millis() + 86400000UL;    // 24 h
        timerEvent = AlarmEvent::kTimer24H;
    }
}

void StateMachine::cancelTimers() { timerDue = 0; }

void StateMachine::sendAlarmSms() {
    Sim868::instance().sendSmsAdmin("ALARM! 🚨 Motorcycle triggered.");
    mqtt.publishEvent("alarm");
}

void StateMachine::startSiren(bool on) { siren.set(on); }
