/**
 * @file registry.h
 * @brief ذخیره‌سازی NVS برای Admin / Password / Remote Codes
 */
#pragma once
#include <Arduino.h>
#include <nvs_flash.h>

class Registry {
 public:
    void begin();

    /* --- Admin --- */
    bool   adminSet() const      { return adminNum.length(); }
    String admin()     const     { return adminNum; }
    bool   saveAdmin(const String& num);

    /* --- Password --- */
    String password()  const     { return pwd; }
    bool   checkPassword(const String& p) const;

    /* --- Rf Remotes --- */
    bool   saveRemote(uint32_t code);
    bool   isKnownRemote(uint32_t code) const;

 private:
    nvs_handle_t handle{};
    String   adminNum;
    String   pwd;
    uint32_t rem[2]{0xFFFFFFFF,0xFFFFFFFF};
    void     load();
};
