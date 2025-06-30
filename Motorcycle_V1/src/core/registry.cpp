#include "core/registry.h"
#include "config.h"

void Registry::begin() {
    nvs_flash_init();
    nvs_open("alarm_cfg", NVS_READWRITE, &handle);
    load();
}

void Registry::load() {
    char buf[24] = {0};  size_t len;

    /* Admin */
    len = sizeof(buf);
    if (nvs_get_str(handle, "admin", buf, &len) == ESP_OK)
        adminNum = buf;

    /* Password */
    len = sizeof(buf);
    if (nvs_get_str(handle, "pwd", buf, &len) == ESP_OK &&
        len >= PASSWORD_LEN_MIN)
        pwd = buf;
    else
        pwd = DEFAULT_PASSWORD;

    /* Remotes */
    for (int i = 0; i < 2; ++i)
        nvs_get_u32(handle, ("rem" + String(i)).c_str(), &rem[i]);
}

bool Registry::saveAdmin(const String& num) {
    if (nvs_set_str(handle, "admin", num.c_str()) != ESP_OK) return false;
    nvs_commit(handle);  adminNum = num;  return true;
}

bool Registry::checkPassword(const String& p) const {
    return p == pwd &&
           p.length() >= PASSWORD_LEN_MIN &&
           p.length() <= PASSWORD_LEN_MAX;
}

bool Registry::saveRemote(uint32_t code) {
    for (int i = 0; i < 2; ++i)
        if (rem[i] == 0xFFFFFFFF) {
            rem[i] = code;
            nvs_set_u32(handle, ("rem" + String(i)).c_str(), code);
            nvs_commit(handle);
            return true;
        }
    return false;
}

bool Registry::isKnownRemote(uint32_t code) const {
    for (uint32_t r : rem) if (r == code) return true;
    return false;
}
