#ifndef COLORCHRON
#define COLORCHRON
#include "pico/stdlib.h"
#include "../lib/RGB.hpp"



class colorCron{
    public:
    int32_t decSec_ms= -1;
    int8_t seconds=-1;
    int8_t minutes=-1;
    int8_t hours=-1;
    int8_t days=-1;
    // these three RGB inherit thier initial values from
    //their class default constructors of 0xFF,0xFF,0xFF
    uint8_t symbolBlink = 0;
    uint8_t colonBlink = 0;
    uint8_t dpBlink = 0;
    RGB symbolColor;
    RGB colonColor;
    RGB dpColor;
    RGB secondsColor;
    enum displayMode : int8_t {
        //these are also used as the starting digit in the display string
        NOCHG = -1,
        DDHHMM = 0,
        HHMMSS = 6,
        MMSSTT = 12,
        AUTO   = 16
    }  mode = AUTO;

    colorCron(uint64_t ds, uint8_t sec,uint8_t min,uint8_t hr,uint8_t dy,RGB sC,RGB cC,RGB dC,displayMode m,uint8_t sb,uint8_t cb,uint8_t db){
        decSec_ms = ds;
        seconds=sec;
        minutes=min;
        hours=hr;
        days=dy;
        symbolColor=sC;
        colonColor=cC;
        dpColor=dC;
        mode=m;
        symbolBlink=sb;
        colonBlink=cb;
        dpBlink=db;
    };
    colorCron(){
        decSec_ms = 0;
        seconds=0;
        minutes=0;
        hours=0;
        days=0;
        symbolColor=RGB();
        colonColor=RGB();
        dpColor=RGB();
        mode=DDHHMM;
        symbolBlink=0;
        colonBlink=0;
        dpBlink=0;};
};

#endif