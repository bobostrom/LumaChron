#include "Pixels.hpp"
#include "ws2812.pio.h"

// #include "pico/stdlib.h"
// #include "pico/unique_id.h"
// #include "pico/binary_info.h"
 #include "pico/sync.h"
// #include "pico/util/queue.h"
#include <stdio.h>



//#include <string>
//#include <iostream>
//#include "../../lib/RGB.hpp"
//#include "../../lib/constants.hpp"
//#include "pico/stdlib.h"
//#include "../../lib/seeed_xiao_rp2040.h"

void Pixels::init(uint8_t ledOut, uint16_t np, uint8_t statemachine)
{
    gpio_init(ledOut);
    gpio_set_dir(ledOut, GPIO_OUT);
    gpio_set_pulls(ledOut,false,false);
    sm = statemachine;
    numpix = np;
    //cout << "initializing " << numpix << " pixels on " << (int)sm << endl;
    leds = new RGB[numpix];
    //sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, statemachine, offset, ledOut, 800000, IS_RGBW);
    //sleep_ms(5000);
    clear();
};

void Pixels::show(void){
    //cout << "updating " << numpix << " pixels" << "on" << (int)sm << endl;
    
    // uart_putc(uart0,'P');
    // uart_tx_wait_blocking(uart0);
    int32_t ints = save_and_disable_interrupts();
    int i;
    for(i=0;i<numpix;i++){
        //if(sm==1) cout << "." << (int)leds[i].r << ":" << (int)leds[i].g << ":" << (int)leds[i].b << endl;
        //put_pixel(sm,leds[i].urgb_u32());
        //uint32_t pixel_grb = leds[i].urgb_u32();
        //pio_sm_put_blocking(pio0, sm, pixel_grb << 8u);
        //pio_sm_put_blocking(pio0, sm, leds[i] << 8u);
        // pixels.leds[lednum].r=((((float)regs.symbolColor.data.r)/(255-regs.brightness)))*segmentonoff;
        float scale;
        scale = (float)regs.brightness/(float)255;
        // RGB temp;
        // temp = temp*scale;
        // temp.r = (uint16_t)((float)leds[i].r*scale);
        // temp.g = (uint16_t)((float)leds[i].g*scale);
        // temp.b = (uint16_t)((float)leds[i].b*scale);
        //need to reassemble into the correct rgb order in the upper 24 bits of the data going to the state machine
        //this needs to be grb order for the WS2812B leds
        uint32_t out = (uint32_t)(leds[i].g*scale)<<24  | (uint32_t)(leds[i].r*scale) <<16 | (uint32_t)(leds[i].b*scale)<<8;
        //printf("%08X_%08X\n",leds[i],out);
        pio_sm_put_blocking(pio0, sm, out);
    }
    restore_interrupts (ints);
    // char temp[10];
    // sprintf(temp,"%d ",i);
    // uart_puts(uart0,temp);
};



void Pixels::clear(void){
    for(int i=0;i<numpix;i++){
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 0;
    }
    show(); 
};

// uint32_t NeoPixels::urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
//     return
//     ((uint32_t) (r) << 8) |
//     ((uint32_t) (g) << 16) |
//     (uint32_t) (b);
// };

void Pixels::put_pixel(int sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, sm, pixel_grb << 8u);
};
