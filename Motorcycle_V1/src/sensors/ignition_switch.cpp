#include "sensors/ignition_switch.h"
#include "driver/gpio.h"

static IgnitionSwitch* selfPtr = nullptr;

/* ---------- public ---------- */
void IgnitionSwitch::begin() {
    selfPtr = this;

    /* تنظیم پایه ACC به ورودی + Pull-up */
    gpio_config_t cfg{};
    cfg.pin_bit_mask = 1ULL << ACC;
    cfg.mode         = GPIO_MODE_INPUT;
    cfg.pull_up_en   = GPIO_PULLUP_ENABLE;
    gpio_config(&cfg);

    last_level = gpio_get_level((gpio_num_t)ACC);

    /* فعال‌سازی سرویس وقفهٔ GPIO (یک‌بار کافی است) */
    gpio_install_isr_service(0);
    gpio_set_intr_type((gpio_num_t)ACC, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add((gpio_num_t)ACC, gpioIsr, nullptr);
}

bool IgnitionSwitch::isOn() const { return last_level; }

bool IgnitionSwitch::isOnChanged() {
    bool c = changed;
    changed = false;
    return c;
}

/* ---------- ISR ---------- */
void IRAM_ATTR IgnitionSwitch::gpioIsr(void* /*arg*/) {
    if (!selfPtr) return;
    uint8_t cur = gpio_get_level((gpio_num_t)ACC);
    if (cur != selfPtr->last_level) {
        selfPtr->last_level = cur;
        selfPtr->changed    = true;
    }
}
