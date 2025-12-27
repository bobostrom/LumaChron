#ifndef FUNCTIONS
#define FUNCTIONS
#include "pico/stdlib.h"
#include "pico/unique_id.h"



#include "pico/stdlib.h"
#include "hardware/flash.h"
// #include "pico/flash.h"
// #include "hardware/watchdog.h"
// #include "pico/bootrom.h"
// #include "constants.hpp"
// #include "Flash.hpp"
// #include "pico/util/queue.h"
// #include "Queue.hpp"

// #include "RGB.hpp"
// #include "Digit.hpp"
// #include "Register.hpp"
// #include "enums.hpp"


uint64_t getSerialNumber();
uint8_t calcFletcherCksum(uint8_t,uint8_t,int*);
int getDataFrom(uint8_t);








#endif