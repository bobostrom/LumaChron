#ifndef PIXELS_HPP
#define PIXELS_HPP
//#include "pico/stdlib.h"
#include "hardware/pio.h"
#include <string>
#include <map>
#include "colorConstants.hpp"
#include <algorithm>
#include "RGB.hpp"

#include "Registers.hpp" /////////////////
// extern Registers regs;
extern Registers regs;
class Pixels {
    const uint8_t IS_RGBW=false;
    uint16_t numpix;
    public:
    RGB* leds;
    uint8_t sm;
    PIO pio = pio0;
    void init(uint8_t ledOut, uint16_t numpix, uint8_t statemachine);
    void show();
    void clear(void);
    // uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
    void put_pixel(int sm, uint32_t pixel_grb);
    //colorMap cmap;
};


#endif
