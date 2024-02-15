#ifndef _LED_H
#define _LED_H

typedef enum {
    LED_POWER_OFF = 0,
    LED_POWER_ON = 1,
} led_power_t;

void led_init(void);
void led_power_set(led_power_t led_power);
void led_toggle();

#endif // _LED_H
