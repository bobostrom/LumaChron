
#include "I2Cbus.hpp"
#include <stdio.h>
#include <iostream>




int8_t I2Cbus::init(std::string name){
    this->busName = name;
    if((this->bus=::open(busName.c_str(), O_RDWR)) < 0){
        perror("I2C: failed to open the bus\n");
        return -1;
    }
    // if(ioctl(this->file, I2C_SLAVE, this->device) < 0){
    //     perror("I2C: Failed to connect to the device\n");
    // }
    return 0;
};

int8_t I2Cbus::writeI2c(uint8_t addr, uint8_t *outarr, uint8_t arrsize){
    int8_t rv = 0;
    this->messages[0].addr = addr;
    this->messages[0].flags = 0;
    this->messages[0].len = arrsize;
    this->messages[0].buf = outarr;
    this->packets.msgs = messages;
    this->packets.nmsgs = 1;
    if(ioctl(this->bus, I2C_RDWR, &this->packets) < 0){
        // perror("I2C: Failed to write to the device");
        rv = -1;
    }
    else{
        // printf("I2C: Write successful\n");
    }

    return rv;
}


int8_t I2Cbus::readI2c(uint8_t address, uint8_t *inarr, uint8_t arrsize){
    int8_t rv = 0;
    

    return rv;
}
