#ifndef COLORCHRON
#define COLORCHRON

#include "RGB.hpp"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <signal.h>
#include <algorithm>
#include <limits>
#include <map>
// #include "colorConstants.hpp"
#include "json.hpp"
using json = nlohmann::json;


class colorCron{




    public:
    inline static const std::map<std::string,int8_t> timeModeMap  {
        {"NOCHG", -1},
        {"DDHHMM", 0},
        {"HHMMSS", 6},
        {"MMSSTT", 12},
        {"AUTO", 16}
    };
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
    uint8_t brightness = 0xFF;
    RGB symbolColor;
    RGB colonColor;
    RGB dpColor;
    // RGB secondsColor;
    int8_t timeMode = timeModeMap.at("AUTO");
    // enum timeMode_t : int8_t {
    //     //these are also used as the starting digit in the display string
    //     NOCHG = -1,
    //     DDHHMM = 0,
    //     HHMMSS = 6,
    //     MMSSTT = 12,
    //     AUTO   = 16
    // } ;
    

    // static const std::map<std::string,int8_t> timeModeMap;
    int load(std::string);
    int load(json);
    colorCron(uint64_t ds, uint8_t sec,uint8_t min,uint8_t hr,uint8_t dy,RGB sC,RGB cC,RGB dC,int8_t m,uint8_t sb,uint8_t cb,uint8_t db, uint8_t br){
        decSec_ms = ds;
        seconds=sec;
        minutes=min;
        hours=hr;
        days=dy;
        symbolColor=sC;
        colonColor=cC;
        dpColor=dC;
        timeMode=m;
        symbolBlink=sb;
        colonBlink=cb;
        dpBlink=db;
        brightness = br;
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
        timeMode=timeModeMap.at("AUTO");
        symbolBlink=0;
        colonBlink=0;
        dpBlink=0;
        brightness = 0xFF;
    };
};
// const std::map<std::string,int8_t> colorCron::timeModeMap  {
//     {"NOCHG", -1},
//     {"DDHHMM", 0},
//     {"HHMMSS", 6},
//     {"MMSSTT", 12},
//     {"AUTO", 16}
// };
#endif