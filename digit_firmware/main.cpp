#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "seeed_XIAO_details.h"
#include "pico/time.h"
#include "Registers.hpp"
#include "Queue.hpp"
#include "../lib/I2Cbus.hpp"
#include "Core1.hpp"
#include "constants.hpp"
#include "../lib/Serial.hpp"



//global variables
I2Cbus i2cslave(i2c1,2,3);
Flash flashmem;
queue_t coreQueue;
absolute_time_t startTime;
Registers regs;
// Serial ser(uart0,1,0,250000);
Serial ser(nullptr,1,0,250000);



//defines for the status leds
#define DATA_LED XIAO_BLUE_LED_PIN
#define CORE0_HB XIAO_GREEN_LED_PIN
#define ADDRCHGPIN 26 //button where input to change I2C address (LADDR:Location Address)

bool core0_heartbeat_callback(struct repeating_timer *t) {
    gpio_put(CORE0_HB,!gpio_get(CORE0_HB));
    return true;
}
bool core0_blinker_callback(struct repeating_timer *t){
    if(regs.symbolBlink==0){
        regs.symbol.blinker=1;
        regs.symbol.blinkerCounter=0;
    }
    else if(regs.symbol.blinkerCounter>=regs.symbolBlink){
            regs.symbol.blinker = !regs.symbol.blinker;
            regs.symbol.blinkerCounter=0;
    }
    regs.symbol.blinkerCounter++;
    if(regs.colonBlink==0){
        regs.colon.blinker=1;
        regs.colon.blinkerCounter=0;
    }
    else if(regs.colon.blinkerCounter>=regs.colonBlink){
            regs.colon.blinker = !regs.colon.blinker;
            regs.colon.blinkerCounter=0;
    }
    regs.colon.blinkerCounter++;
    if(regs.decimalptBlink==0){
        regs.decimalpt.blinker=1;
        regs.decimalpt.blinkerCounter=0;
    }
    else if(regs.decimalpt.blinkerCounter>=regs.decimalptBlink){
            regs.decimalpt.blinker = !regs.decimalpt.blinker;
            regs.decimalpt.blinkerCounter=0;
    }
    regs.decimalpt.blinkerCounter++;
    return true;
}

static void save(void *param ){
    flashmem.saveAddr();
    return;
}
void addrCheckAtStartup(void*){
    absolute_time_t myTime;
    gpio_init(ADDRCHGPIN);
    gpio_set_pulls(ADDRCHGPIN,true,false);
    gpio_set_dir(ADDRCHGPIN,GPIO_IN);
    gpio_init(28);
    gpio_set_dir(28,GPIO_OUT);
    gpio_put(28,0);
    // if(gpio_get(ADDRCHGPIN)) return; //the button must be held down at boot to start this
    if(regs.laddr == 0xFF){regs.laddr = 0x00;} //this might happen if the flash were just erased
    sprintf(&regs.symbol.data,"%d",(char)regs.laddr);
    // sprintf(&regs.symbol.data," ",(char)regs.laddr);
    // regs.symbolBlink= 0;
    // regs.symbolColor = 0x0000FF;
    regs.symbolBlink = 0;
    regs.symbolColor = 0x0000FF;
    myTime= get_absolute_time();

    while(absolute_time_diff_us(myTime,get_absolute_time())<(1000000*5)){
        watchdog_update();
        // uart_putc(uart0,(char)(gpio_get(ADDRCHGPIN)+0x30));
        // regs.laddr.data=0x00;  force it to set to 0 for testing...
        if(!gpio_get(ADDRCHGPIN)){
            regs.symbolBlink = (char)3;
            regs.laddr.data ++;
            if(regs.laddr.data >9) regs.laddr.data=0;
            sprintf(&regs.symbol.data,"%d",regs.laddr.data);
            // char temp[10];
            // sprintf(temp,"laddr0x%02x",regs.laddr.data);ser.puts(temp);
            
            while(!gpio_get(ADDRCHGPIN)){
                // uart_putc(uart0,'w');
                // uart_putc(uart0,(char)(gpio_get(ADDRCHGPIN)+0x30));
                // uart_putc(uart0,':');
                watchdog_update();
                for(myTime= get_absolute_time();absolute_time_diff_us(myTime,get_absolute_time())<(1000*100); ){watchdog_update();continue;}
                // sleep_ms(500);
                }
            // sleep_ms(500);
            myTime = get_absolute_time();
            
        }
    }
    // char temp[25];
    // sprintf(temp,"Saving to flash 0x%02x\n", (char)regs.laddr);
    // ser.puts(temp);
    
    flash_safe_execute(save,NULL,10000);
    gpio_set_dir(ADDRCHGPIN,GPIO_OUT);
    regs.symbolBlink = 0;
    sprintf(&regs.symbol.data," ",regs.laddr);
}




int main() {
    int8_t inData;
    queueItem qMessage;
    string outStr;
    
    startTime = get_absolute_time();
    absolute_time_t myTime;
    
    
    // gpio_set_function(0,GPIO_FUNC_UART);
    // gpio_set_function(1, GPIO_FUNC_UART);
    // uart_init(uart0, 250000);//115200);//921600);
  
    // set up the queue used to communicate with the other core
    queue_init(&coreQueue, sizeof(queueItem), 128); //initialize the queue

    
    // Initialize LED pins
    gpio_init(XIAO_BLUE_LED_PIN);
    gpio_set_dir(XIAO_BLUE_LED_PIN, GPIO_OUT);
    gpio_init(XIAO_RED_LED_PIN);
    gpio_set_dir(XIAO_RED_LED_PIN, GPIO_OUT);
    gpio_init(XIAO_GREEN_LED_PIN);
    gpio_set_dir(XIAO_GREEN_LED_PIN, GPIO_OUT);
    gpio_init(CORE0_HB);
    gpio_set_dir(CORE0_HB, GPIO_OUT);
    gpio_init(XIAO_DEFAULT_WS2812_PWR_PIN);
    gpio_set_dir(XIAO_DEFAULT_WS2812_PWR_PIN,GPIO_OUT);
    gpio_init(DATA_LED);
    gpio_set_dir(DATA_LED, GPIO_OUT);

    
    gpio_set_dir(DATA_LED, GPIO_OUT);



  // boot the other core
    multicore_launch_core1(core1_entry);
  
    struct repeating_timer HBtimer;
    struct repeating_timer blinkerTimer;
    // add_repeating_timer_ms(-500, core0_heartbeat_callback, &digit, &timer);
    add_repeating_timer_ms(-500, core0_heartbeat_callback, NULL, &HBtimer);
    add_repeating_timer_ms(-100, core0_blinker_callback, NULL, &blinkerTimer);
    queue_init(&coreQueue,sizeof(queueItem),128);

    if(watchdog_caused_reboot() || watchdog_enable_caused_reboot()){
        ser.puts("\nwatchdog caused reboot\n");
    } else {ser.puts("\nsomething other than watchdog caused reboot\n");}
    watchdog_enable(0x7fffff,true); //about 7 or eight minutes


    addrCheckAtStartup(NULL);

    // can now start up I2C slave
    i2cslave.setSlaveMode(regs.laddr+0x30);
    
    //print the serial number
    outStr = "\nSerial Number:  ";
    outStr += to_string(regs.serialNumber);
    outStr += "\n";
    ser.puts( outStr.c_str());
    regs.symbol = '8';
    regs.colon = ':';
    regs.decimalpt = '.';
    sleep_ms(5000);
    ser.puts( "\nBOOTED!!!\n");
    regs.symbol     =' ';
    regs.symbolBlink=0;
    regs.symbolColor=0xFFFFFF;
    regs.colon     =' ';
    regs.colonBlink=0;
    regs.colonColor=0xFFFFFF;
    regs.decimalpt     =' ';
    regs.decimalptBlink=0;
    regs.decimalptColor=0xFFFFFF;

    myTime = get_absolute_time();
    uint8_t savedBrightness;
    while(true){  
        watchdog_update();
        // check for incoming messages
        if(regs.getReg() == SUCCESS){
            myTime = get_absolute_time();
            ser.puts("SUCCESS\n");            
        }
        // // if there's no comm activity for more than 5 minutes, go into demo mode
        // if(absolute_time_diff_us(myTime,get_absolute_time())>((int64_t)1000000*60*5)){
        //     if(regs.mode==DEMO) continue;
        //     // if(regs.mode==RAWPIXEL) regs.mode=DEMO;
        //     if(regs.mode==NORMAL){
        //         myTime = get_absolute_time();
        //         ser.puts("\nTimed out, going into demo mode\n");
        //         if(!queue_is_full(&coreQueue)){
        //             // ser.puts("adding to queue\n");
        //             qMessage.command = (uint8_t)DEMO;
        //             qMessage.size = 0;
        //             queue_add_blocking(&coreQueue,&qMessage);
        //         }
        //     }            
            
        // }
    }
}

