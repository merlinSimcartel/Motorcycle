#include "sensors/adxl345.h"

/* ---------- تعریف پرچم‌های static ---------- */
volatile bool Adxl345Sensor::shockFlag = false;
volatile bool Adxl345Sensor::inactFlag = false;

/* ---------- سازنده ---------- */
Adxl345Sensor::Adxl345Sensor(uint8_t addr) : i2cAddr(addr) {}

/* ---------- توابع کمکی رجیستر ---------- */
void Adxl345Sensor::writeReg(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(i2cAddr);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

uint8_t Adxl345Sensor::readReg(uint8_t reg) {
    Wire.beginTransmission(i2cAddr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(i2cAddr, (uint8_t)1);
    return Wire.read();
}

/* ---------- راه‌اندازی ---------- */
void Adxl345Sensor::begin(uint8_t inactivitySecs,
                          uint8_t activityThresh,
                          uint8_t inactThresh) {

    /* روشن‌کردن ماژول */
    pinMode(ADXL_EN_PIN, OUTPUT);
    digitalWrite(ADXL_EN_PIN, HIGH);
    delay(10);

    /* راه‌اندازی I²C (در صورت نیاز) */
    Wire.begin(ADXL_SDA_PIN, ADXL_SCL_PIN,100000);

    /* تأیید Device ID */
    uint8_t id = readReg(0x00);
    if (id != 0xE5) {
        Serial.printf("ADXL345 not found (ID=0x%02X)\n", id);
        return;
    }

    /* Standby */
    writeReg(REG_POWER_CTL, 0x00);

    /* آستانه‌ها */
    writeReg(REG_THRESH_ACT,   activityThresh);
    writeReg(REG_THRESH_INACT, inactThresh);
    writeReg(REG_TIME_INACT,   inactivitySecs);

    /* AC-coupled و XYZ فعال برای هر دو مد */
    writeReg(REG_ACT_INACT_CTL, 0xFF);   // 0xFF = XYZ AC برای Act & Inact

    /* نگاشت: Activity → INT2 (BIT4=1) ، Inactivity → INT1 (BIT3=0) */
    writeReg(REG_INT_MAP, 0x10);

    /* فعال‌سازی هر دو وقفه */
    writeReg(REG_INT_ENABLE, 0x10 | 0x08);

    /* ورود به حالت Measure */
    writeReg(REG_POWER_CTL, 0x08);

    /* پیکربندی پایه‌های وقفه روی MCU */
    uint8_t mode = ADXL_USE_PULLUP ? INPUT_PULLUP : INPUT;
    pinMode(ADXL_INT1_PIN, mode);
    pinMode(ADXL_INT2_PIN, mode);

    attachInterrupt(digitalPinToInterrupt(ADXL_INT2_PIN),
                    Adxl345Sensor::isrActivity,
                    RISING);
    attachInterrupt(digitalPinToInterrupt(ADXL_INT1_PIN),
                    Adxl345Sensor::isrInactivity,
                    RISING);

    /* دیباگ رجیسترها */
    Serial.printf("ADXL INIT OK  INT_MAP=0x%02X  INT_EN=0x%02X\n",
                  readReg(REG_INT_MAP), readReg(REG_INT_ENABLE));

Serial.printf("TIME_INACT=%d  THRESH_INACT=%d  INT_SRC=0x%02X\n",
              readReg(REG_TIME_INACT),
              readReg(REG_THRESH_INACT),
              readReg(REG_INT_SOURCE));


    readReg(REG_INT_SOURCE);   // ← همین خط، وقفه‌های در صف را پاک می‌کند
}

/* ---------- ISR ها ---------- */
void IRAM_ATTR Adxl345Sensor::isrActivity()   { Adxl345Sensor::shockFlag = true;  }
void IRAM_ATTR Adxl345Sensor::isrInactivity() { Adxl345Sensor::inactFlag = true; }

/* ---------- متدهای بررسی پرچم ---------- */
bool Adxl345Sensor::shockDetected() {
    if (!shockFlag) return false;
    shockFlag = false;
    readReg(REG_INT_SOURCE);              // پاک‌کردن بیت وقفه
    return true;
}

bool Adxl345Sensor::inactivityDetected() {
    if (!inactFlag) return false;
    inactFlag = false;
    readReg(REG_INT_SOURCE);
    return true;
}
