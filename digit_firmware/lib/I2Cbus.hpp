#ifndef I2CBUS_HPP
#define I2CBUS_HPP
#include "pico/stdlib.h"
#include "hardware/i2c.h"


class I2Cbus {
    public:
    uint8_t myAddr;
    uint8_t sdapin = 2;
    uint8_t sclpin = 3;
    i2c_inst_t* bus = i2c1;
    I2Cbus(void);
    I2Cbus(i2c_inst_t* b, uint8_t sdapin, uint8_t sclpin);
    void setSlaveMode(uint8_t adr);
    int8_t writeI2c(uint8_t address, uint8_t *outarr, uint8_t arrsize);
    int8_t readI2c(uint8_t address, uint8_t *inarr, uint8_t arrsize);
    void readRawBlocking(uint8_t* destination,uint8_t size);
    uint8_t readAvailable(void);
    int getchar_timeout_us(uint32_t timeout_us);
    int i2cdetectNumDigits(uint8_t);
};

#endif