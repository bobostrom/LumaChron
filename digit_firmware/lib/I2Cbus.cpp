#include "hardware/i2c.h"
#include "I2Cbus.hpp"
#include <stdio.h>
#include <iostream>
const uint32_t I2CBUSBAUD = 1000*1000;
I2Cbus::I2Cbus(void){
    // std::cout << "initializing i2c on bus: " << bus << " on pins " << +sdapin << " " << +sclpin << std::endl;
    gpio_set_function(sdapin, GPIO_FUNC_I2C);
    gpio_set_function(sclpin, GPIO_FUNC_I2C);
    gpio_pull_up(sdapin);
    gpio_pull_up(sclpin);
    gpio_set_pulls(sdapin,true,false);
    gpio_set_pulls(sclpin,true,false);
    // gpio_set_input_hysteresis_enabled(sdapin,true);
    // gpio_set_input_hysteresis_enabled(sclpin,true);
    // gpio_set_slew_rate(sdapin,GPIO_SLEW_RATE_SLOW);
    // gpio_set_slew_rate(sclpin,GPIO_SLEW_RATE_SLOW);
    i2c_init(bus, I2CBUSBAUD);// * 1000);
    //sleep_ms(1000);

};

I2Cbus::I2Cbus(i2c_inst_t* b, uint8_t sdapinIN, uint8_t sclpinIN){
    bus = b;
    sdapin = sdapinIN;
    sclpin = sclpinIN;
    // std::cout << "initializing i2c on bus: " << bus << " on pins " << +sdapin << " " << +sclpin << std::endl;
    gpio_set_function(sdapin, GPIO_FUNC_I2C);
    gpio_set_function(sclpin, GPIO_FUNC_I2C);
    gpio_set_pulls(sdapin,true,false);
    gpio_set_pulls(sclpin,true,false);
    gpio_set_input_hysteresis_enabled(sdapin,true);
    gpio_set_input_hysteresis_enabled(sclpin,true);
    gpio_set_slew_rate(sdapin,GPIO_SLEW_RATE_FAST);
    gpio_set_slew_rate(sclpin,GPIO_SLEW_RATE_FAST);
    i2c_init(bus,I2CBUSBAUD);//*1000
    //sleep_ms(1000);
};

int I2Cbus::i2cdetectNumDigits(uint8_t testData){
    int rv;
    int i=0;
    do{
        rv =i2c_write_timeout_us(bus,0x30,&testData,1,false,1000000);

    } while(rv != 1);

    for(i=0; i<16 ;i++){
        for(int k=0;k<2;k++){
            rv =i2c_write_timeout_us(bus,0x30+i,&testData,1,false,1000000);
            if(rv == 1) break;
            sleep_ms(1000);
        }
        if(rv != 1) break;
    }
    return i;
};


int I2Cbus::getchar_timeout_us(uint32_t timeout_us){
    uint8_t buf;
    absolute_time_t timeout;
    timeout = make_timeout_time_us(timeout_us);


    while(readAvailable()==0){
        if(absolute_time_diff_us(get_absolute_time(), timeout)< 0){
            return PICO_ERROR_TIMEOUT;
            }
        assert(PICO_ERROR_TIMEOUT);

    }
    readRawBlocking(&buf,1);
    return buf;

}
uint8_t I2Cbus::readAvailable(void){
    return(i2c_get_read_available(bus));
};

void I2Cbus::readRawBlocking(uint8_t* destination,uint8_t size){
    i2c_read_raw_blocking(bus, destination, size);
}

void I2Cbus::setSlaveMode(uint8_t adr){
    myAddr = adr;
    i2c_set_slave_mode(bus,true,myAddr);
};

int8_t I2Cbus::writeI2c(uint8_t address, uint8_t *outarr, uint8_t arrsize){
    int8_t rv;
    rv = i2c_write_blocking(bus, address, outarr, arrsize/*+2*/, false);
    
    // if(rv >= 0 && rv != arrsize/*+2*/) {uart_putc(uart0,'q');std::cout << "failed to write all the bytes, only wrote " << +rv << std::endl;}
    // if(rv ==  PICO_ERROR_TIMEOUT){std::cout << +address << " write timeout error " << +rv <<  std::endl;}
    // if(rv ==  PICO_ERROR_GENERIC){std::cout << +address << " write generic error " << +rv << std::endl;}
    return rv;
}


int8_t I2Cbus::readI2c(uint8_t address, uint8_t *inarr, uint8_t arrsize){
    int8_t rv;
    
    //not sure why these were there
    //inarr[0]=0;
    //rv = i2c_write_blocking(bus, address, inarr, 1, true);
    rv = i2c_read_blocking(bus, address, inarr, arrsize, false);

    // if(rv >= 0 && rv != arrsize) {std::cout << "failed to write all the bytes, only wrote " << +rv << std::endl;}
    // if(rv ==  PICO_ERROR_TIMEOUT){std::cout << +address << " color write timeout error " << +rv <<  std::endl;}
    // if(rv ==  PICO_ERROR_GENERIC){std::cout << +address << " color write generic error " << +rv << std::endl;}

    return rv;
}