#ifndef DS3231RTC_HPP
#define DS3231RTC_HPP
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "I2Cbus.hpp"



class DS3231rtc {
    I2Cbus* ibus;
    
    static const uint8_t address = 0x68;
    uint64_t timeout=150000;  //also used to test how long the timeout should be
    public:

    DS3231rtc(I2Cbus* ib);
    bool updateRTCfromDS3231(void);
    void updateDS3231fromRtc(void);
    datetime_t getRtcTime(void);
    datetime_t getDs3231Time(void);

};

#endif