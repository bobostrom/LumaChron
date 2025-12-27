#ifndef DIGIT_HPP
#define DIGIT_HPP

//#include "hardware/i2c.h"
#include "I2Cbus.hpp"
#include "regNum.hpp"

#include <string>

#include "RGB.hpp"


class Digit{
    // static const uint8_t CHARACTER = 0x00;
    // static const uint8_t COLON = 0x01;
    // static const uint8_t DECIMALPT = 0x02;
    
    uint8_t symbolBlink = 0;
    uint8_t colonBlink = 0;
    uint8_t dpBlink = 0;
    bool verbose = false;
    
    unsigned char character = 0;
    unsigned char decimalpt = 0;
    unsigned char colon = 0;
    uint8_t brightness=0;
    RGB charColor=(0,0,0); //character color
    RGB dpColor=(0,0,0); //decimal point color
    RGB colonColor=(0,0,0); //colon color
    I2Cbus* ibus;
    
    uint64_t timeout=150000;  //also used to test how long the timeout should be
    public:
    bool fullUpdate = true;
    uint8_t address;
    Digit(I2Cbus* ib, uint8_t addr);
    Digit();
    void init(I2Cbus* ib, uint8_t addr);
    void updateDigit(std::string, RGB, RGB, RGB, uint8_t, uint8_t, uint8_t, uint8_t);
    void updatePunc(uint8_t, RGB);
    void calcCksum(uint8_t *arr,uint8_t size);
    //int8_t writeI2c(uint8_t*,uint8_t);


    //static int i2c_write(i2c_inst_t * i2c, uint8_t 	addr,const uint8_t * src, size_t len, bool nostop,uint timeout_us );
    

};
#endif