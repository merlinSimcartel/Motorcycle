/**
 * @file main.cpp
 * @brief نقطهٔ ورود برنامه
 */
#include <Arduino.h>
#include <Wire.h>
#include "config.h"

/* ===== هدرهای داخلی ===== */
#include "core/state_machine.h"
#include "core/power_manager.h"
#include "core/registry.h"
#include "utils/led_task.h"
#include "comms/sim868.h"
#include "rf/rf_receiver.h"
#include "sensors/adxl345.h"
#include "sensors/ignition_switch.h"
#include "sensors/battery.h"
#include "comms/sms_parser.h"
#include "sensors/ignition_switch.h"

/* ===== اشیای سراسری ===== */
StateMachine fsm;
PowerManager pwr;
Registry reg;
LedTask leds;
Sim868 &modem = Sim868::instance(); // ← تغییر این خط
RfReceiver rf;
IgnitionSwitch ignition;
BatteryMonitor bat;
SmsParser sms(&reg);
Adxl345Sensor accel; // از آدرس پیش‌فرض 0x53

void setup()
{
  Serial.begin(115200);

  leds.begin();
  pwr.begin();
  reg.begin();
  accel.begin(20, 20, 0x20); // 20s سکون، 0.6g ضربه، 0.3g سکون
  modem.begin(&reg);         // singleton را مقداردهی می‌کنیم
  rf.begin();
  ignition.begin();
  bat.begin();

  fsm.begin();
  ignition.begin();
  Serial.println(F("Motor-Alarm ready"));
}

void loop()
{
  /* -------- ورودی‌های سنسور و ریموت -------- */

  if (accel.shockDetected())
  {
    Serial.println("🔔 Activity!");
    fsm.postEvent(AlarmEvent::kShock);
  }
  if (accel.inactivityDetected())
  {
    Serial.println("⏱️ Inactivity!");
  }

  if (ignition.isOnChanged())
  {
    fsm.postEvent(ignition.isOn() ? AlarmEvent::kIgnitionOn : AlarmEvent::kIgnitionOff);
  }

  if (rf.available())
  {
    uint32_t code = rf.readCode();
    if (reg.isKnownRemote(code))
      fsm.postEvent(AlarmEvent::kRemoteDisarm);
    rf.reset();
  }

  /* -------- پردازش URC مودم -------- */
  modem.processUrc(sms, fsm);

  /* -------- حلقه‌های اصلی -------- */
  fsm.loop();
  leds.loop();

  delay(10);
}
