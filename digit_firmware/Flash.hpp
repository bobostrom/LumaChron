#ifndef FLASH_HPP
#define FLASH_HPP
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "pico/flash.h"
#include "hardware/sync.h"
#include <stdio.h>

#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)
//using namespace std;


class Flash{
    public:
    const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
    uint8_t pageBuf[FLASH_PAGE_SIZE];
    public:
    //uint8_t myAddr;
    Flash();
    int saveAddr(void);
    void erase(void);
    void flashprint(void);
    

};








#endif