#include "Core1.hpp"

extern Registers regs;
bool core1_digitUpdate_callback(struct repeating_timer *t) {

    Digit *digit = (Digit *)t->user_data;
    //digit->updateDigit();
    // if(regs.mode==NORMAL) 
        digit->updateDigit();//    digit.updateDigit();
    return true;
}
bool core1_demoTimer_callback(struct repeating_timer *t){
    Digit *digit = (Digit *)t->user_data;
    // if(regs.mode == DEMO){//demo mode
    //     // regs.brightness++;
    //     // uart_putc(uart0,'*');
    //     RGB temp = digit->pixels.leds[0];
    //     for(int i=0;i<regs.numPixels-1;i++){
    //         digit->pixels.leds[i]=digit->pixels.leds[i+1];
    //         // char outStr[100];
    //         // sprintf(outStr,"setting led %d to 0x%02x 0x%02x 0x%02x ",i+1,digit->pixels.leds[i].r,digit->pixels.leds[i].g,digit->pixels.leds[i].b);
    //         // uart_puts(uart0,outStr);
    //         // uart_putc(uart0,'.');
    //     }
    //     digit->pixels.leds[regs.numPixels-1]=temp;
    //     digit->pixels.show();
    //     // uart_putc(uart0,'^');
    // }    
    return true;
}
bool core1_heartbeat_callback(struct repeating_timer *t) {
   
    gpio_put(CORE1_HB,!gpio_get(CORE1_HB));
    return true;
}

void core1_entry() {

   flash_safe_execute_core_init();
    // sleep_ms(20000);
    // flash_safe_execute_core_deinit();
    Digit digit;
    queueItem queueMessage;
    gpio_init(CORE1_HB);
    gpio_set_dir(CORE1_HB, GPIO_OUT);

    gpio_set_drive_strength(CORE1_HB,GPIO_DRIVE_STRENGTH_2MA);
    gpio_put(CORE1_HB, 0);

    struct repeating_timer HBtimer;
    struct repeating_timer updateTimer;
    struct repeating_timer demoTimer;
    add_repeating_timer_ms(-1000, core1_heartbeat_callback, &digit, &HBtimer);
    add_repeating_timer_ms(-5, core1_digitUpdate_callback, &digit, &updateTimer);
    add_repeating_timer_ms(-500, core1_demoTimer_callback, &digit, &demoTimer);
    printf("%s\n",digit.descr.c_str(),regs.deviceType);
    while(true){


        if(!queue_is_empty(&coreQueue)){
            queue_remove_blocking(&coreQueue,&queueMessage);
            switch(queueMessage.command){
                case RAWPIXEL:{ //load up the raw pixels coming in
                    uint32_t color;
                    for(int i = 0;i<queueMessage.size;i=i+3){
                        color=0;

                        for(int k=0;k<3;k++){
                            color+=(queueMessage.data[i+k])<<((2-k)*8);
                        }
                        //if(i%10==0 && i!=0)printf("\n");                  
                        digit.pixels.leds[i/3]=color; //set the color into the pixel array
                    }
                    // uart_putc(uart0,'p');
                    digit.pixels.show();
                    break;
                }
                case DEMO:{ //do a demo mode(usually coming from main timeout)
                    if(regs.mode != DEMO){                
                        // uart_puts(uart0,"starting a demo\n");
                        for(int i=0;i<regs.numPixels;i++){
                            if(i%3==0){
                                digit.pixels.leds[i].r=0xFF;
                                digit.pixels.leds[i].g=0x00;
                                digit.pixels.leds[i].b=0x00;
                            }else if(i%3==1){
                                digit.pixels.leds[i].r=0x00;
                                digit.pixels.leds[i].g=0xFF;
                                digit.pixels.leds[i].b=0x00;
                            }else if (i%3==2){
                                digit.pixels.leds[i].r=0x00;
                                digit.pixels.leds[i].g=0x00;
                                digit.pixels.leds[i].b=0xFF;
                            }
                        
                        }
                    }
                    regs.mode = DEMO;
                    break;
                }
                default: {
                    // uart_putc(uart0,'%');
                    // printf("queue recieved: %d %d",queueMessage.command,queueMessage.size);
                    // for(int i=0;i<queueMessage.size;i++){printf(" 0x%02X",queueMessage.data[i]);}
                    // printf("\n");
                    break;
                }
            }
            
        }
    }


}
