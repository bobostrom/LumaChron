#include "ds3231rtc.hpp"
#include "I2Cbus.hpp"
//#include "hardware/i2c.h"
#include <stdio.h>
#include <iostream>
#include <limits>
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"


DS3231rtc::DS3231rtc(I2Cbus* ib){
    ibus = ib;
}

datetime_t DS3231rtc::getRtcTime(void){
    datetime_t t;
    rtc_get_datetime(&t);
    return t;
}

datetime_t DS3231rtc::getDs3231Time(void){
    datetime_t t;
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    uint8_t inarr[20];
    uint8_t rv;

    inarr[0]=0x00;
    rv=ibus->writeI2c(address,inarr,1); //this write is needed to set the register pointer
    rv = ibus->readI2c(address,inarr, 0x13); //read all nineteen registers
    t = {
        .year   = (int16_t)((inarr[6]&0x0F) + 10*((inarr[6]>>4)&0x0F)+2000),
        .month  = (int8_t)((inarr[5]&0x0F) + 10*((inarr[5]>>4)&0x01)),
        .day    = (int8_t)((inarr[4]&0x0F) + 10*((inarr[4]>>4)&0x03)),
        .dotw   = (int8_t)((inarr[3]&0x07)-1),
        .hour   = (int8_t)((inarr[2]&0x0F) + 10*((inarr[2]>>4)&0x01) + 20*((inarr[2]>>5)&0x01)),
        .min    = (int8_t)((inarr[1]&0x0F) + 10*((inarr[1]>>4)&0x07)),
        .sec    = (int8_t)((inarr[0]&0x0F) + 10*((inarr[0]>>4)&0x07))
    };
    return t;
}

void DS3231rtc::updateDS3231fromRtc(void){
    uint8_t inarr[20];
    uint8_t rv;
    datetime_t t;
    rtc_get_datetime(&t);
    inarr[0]=0x00; //init the register pointer to 0x00
    inarr[1]=(((t.sec/10)<<4)+((t.sec%10)&0x0F)); // seconds
    inarr[2]=(((t.min/10)<<4)+((t.min%10)&0x0F)); //minutes
    inarr[3]=(((t.hour/10)<<4)+((t.hour%10)&0x0F)); //hours
    inarr[4]=t.dotw+1; //day of week
    inarr[5]=(((t.day/10)<<4)+((t.day%10)&0x0F)); //date
    inarr[6]=(((t.month/10)<<4)+((t.month%10)&0x0F)); //month
    if(t.year>2000 && t.year<2100){
        inarr[6]=inarr[6]|0x80; //set century bit
    } else {
        inarr[6]=inarr[6]&0x7F; //clear century bit
    }   
    inarr[7]=(((t.year%100)/10)<<4)+(((t.year%100)%10)&0x0F); //year
    rv=ibus->writeI2c(address,inarr,8);
}
bool DS3231rtc::updateRTCfromDS3231(void){
    //  std::cout<<"updatingRTC"<< std::endl;
    // datetime_t t;
    char datetime_buf[256];
    datetime_t t;
    char *datetime_str = &datetime_buf[0];
    uint8_t inarr[20];
    volatile uint8_t rv;

    inarr[0]=0x00;
    rv=ibus->writeI2c(address,inarr,1); //this write is needed to set the register pointer
    rv = ibus->readI2c(address,inarr, 0x13); //read all nineteen registers
    t = {
        .year   = (int16_t)((inarr[6]&0x0F) + 10*((inarr[6]>>4)&0x0F)+2000),
        .month  = (int8_t)((inarr[5]&0x0F) + 10*((inarr[5]>>4)&0x01)),
        .day    = (int8_t)((inarr[4]&0x0F) + 10*((inarr[4]>>4)&0x03)),
        .dotw   = (int8_t)((inarr[3]&0x07)-1),
        .hour   = (int8_t)((inarr[2]&0x0F) + 10*((inarr[2]>>4)&0x01) + 20*((inarr[2]>>5)&0x01)),
        .min    = (int8_t)((inarr[1]&0x0F) + 10*((inarr[1]>>4)&0x07)),
        .sec    = (int8_t)((inarr[0]&0x0F) + 10*((inarr[0]>>4)&0x07))
    };
    //adjust for AK time zone
    // for(int i=0;i<9;i++){
    //     if(t.hour==0){
    //         t.hour=23;
    //         t.day--;
    //     }else{
    //         t.hour--;
    //     }
    // }
    rv = rtc_set_datetime(&t);
    busy_wait_us(1000);//wait for the rtc to update
    return(rv);

}