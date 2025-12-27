#ifndef SEED_XIAO_DETAILS_H
#define SEED_XIAO_DETAILS_H

// --- LED ---

#ifndef XIAO_BLUE_LED_PIN
#define XIAO_BLUE_LED_PIN 25
#endif
#ifndef XIAO_GREEN_LED_PIN
#define XIAO_GREEN_LED_PIN 16
#endif
#ifndef XIAO_RED_LED_PIN
#define XIAO_RED_LED_PIN 17
#endif


// --- WS2812 ---
#ifndef XIAO_DEFAULT_WS2812_PIN
#define XIAO_DEFAULT_WS2812_PIN 12
#endif
#ifndef XIAO_DEFAULT_WS2812_PWR_PIN
#define XIAO_DEFAULT_WS2812_PWR_PIN 11
#endif
// PICO_DEFAULT_WS2812_PIN



// Drive high to force power supply into PWM mode (lower ripple on 3V3 at light loads)
//#define PICO_SMPS_MODE_PIN 23



#endif
