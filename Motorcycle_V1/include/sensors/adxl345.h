#ifndef ADXL345_SENSOR_H
#define ADXL345_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include "config.h"

/* ---------- Registers & Address ---------- */
constexpr uint8_t ACCEL_ADDR        = 0x53;   // 0x1D اگر CS به VCC است
constexpr uint8_t REG_THRESH_ACT    = 0x24;
constexpr uint8_t REG_TIME_INACT    = 0x25;
constexpr uint8_t REG_THRESH_INACT  = 0x26;
constexpr uint8_t REG_ACT_INACT_CTL = 0x27;
constexpr uint8_t REG_INT_ENABLE    = 0x2E;
constexpr uint8_t REG_INT_MAP       = 0x2F;
constexpr uint8_t REG_POWER_CTL     = 0x2D;
constexpr uint8_t REG_INT_SOURCE    = 0x30;

#ifndef ADXL_USE_PULLUP
#define ADXL_USE_PULLUP 1           // 1=INPUT_PULLUP ، 0=INPUT
#endif

class Adxl345Sensor {
 public:
  explicit Adxl345Sensor(uint8_t addr = ACCEL_ADDR);

  /** راه‌اندازی
   * @param inactivitySecs  ثانیهٔ سکون برای تولید وقفه Inactivity
   * @param activityThresh  آستانهٔ ضربه (0x10 ≈ 0.6g)
   * @param inactThresh     آستانهٔ سکون  (0x08 ≈ 0.3g)
   */
  void begin(uint8_t inactivitySecs = 20,
             uint8_t activityThresh = 0x10,
             uint8_t inactThresh    = 0x08);

  bool shockDetected();          // لرزش/ضربه
  bool inactivityDetected();     // بی‌حرکتی طولانی

  /* پرچم‌های مشترک بین ISR و برنامهٔ اصلی */
  static volatile bool shockFlag;
  static volatile bool inactFlag;

 private:
  uint8_t i2cAddr;
  void    writeReg(uint8_t reg, uint8_t val);
  uint8_t readReg (uint8_t reg);

  /* ISR ها */
  static void IRAM_ATTR isrActivity();
  static void IRAM_ATTR isrInactivity();
};

#endif  // ADXL345_SENSOR_H
