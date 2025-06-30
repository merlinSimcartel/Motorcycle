#include "sensors/adxl345.h"

Adxl345Sensor accel;  // از آدرس پیش‌فرض 0x53

void setup() {
  Serial.begin(115200);
  accel.begin(40, 20, 0x20);   // 20s سکون، 0.6g ضربه، 0.3g سکون
}

void loop() {
  if (accel.shockDetected()) {
    Serial.println("🔔 Activity!");
  }
  if (accel.inactivityDetected()) {
    Serial.println("⏱️ Inactivity!");
  }
  delay(10);
}
