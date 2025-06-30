/**
 * @file config.h
 * @brief Pin-map & ثوابت کلی
 * @license MIT
 */
#pragma once
#include <Arduino.h>

/* ---------- پین‌های سخت‌افزار ---------- */
#define SIREN             13
#define OFF_SWITCH        14
#define MCU_LED           26
#define NETLIGHT_LED      21
#define GSMSTATUS_LED     47
#define ACC               12
#define ADXL_SCL_PIN      35
#define ADXL_SDA_PIN      36
#define ADXL_EN_PIN       39
#define ADXL_INT1_PIN     38
#define ADXL_INT2_PIN     37
#define GNSS_EN           33
#define GSM_TX1           41
#define GSM_RX1           42
#define GSM_PWRK          46
#define GSM_DTR           40
#define REM_CS            10
#define REM_DATA           9
#define QC_PIN             0     // حالت سرویس (نصب)
#define ADXL_USE_PULLUP   1   // 0 = بدون، 1 = با Pull-up داخلی

 /* ---------- تنظیمات عمومی ---------- */
#define LED_ACTIVE_MS     30000  // خاموشی خودکار LED پس از رویداد (ms)
#define PASSWORD_LEN_MIN  4
#define PASSWORD_LEN_MAX  6
constexpr char DEFAULT_PASSWORD[PASSWORD_LEN_MAX+1] = "123456";

/* ---------- MQTT ---------- */
#define MQTT_USER   "guest"
#define MQTT_PASS   "123445"
#define MQTT_HOST   "mqtt.example.com"
#define MQTT_PORT   1883

/* ---------- ماکرو کمکی ---------- */
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
