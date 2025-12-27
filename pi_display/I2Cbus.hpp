#ifndef I2CBUS_HPP
#define I2CBUS_HPP
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>


class I2Cbus {
    public:
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    std::string busName;
    // uint8_t myAddr=0x30;
    int bus = 0;
    I2Cbus(std::string name){
        this->busName = name;
        if((this->bus=::open(busName.c_str(), O_RDWR)) < 0){
            perror("I2C: failed to open the bus\n");
            // return -1;
        }
    };
    int8_t init(std::string);
    
    // void setSlaveMode(uint8_t adr);
    int8_t writeI2c(uint8_t digitNum, uint8_t *outarr, uint8_t arrsize);
    int8_t readI2c(uint8_t digitNum, uint8_t *inarr, uint8_t arrsize);
};

#endif