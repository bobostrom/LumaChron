#include "functions.hpp"
#include "enums.hpp"
#include "../lib/I2Cbus.hpp"

extern I2Cbus i2cslave;

uint64_t getSerialNumber()
{
    //note: pico_get_unique_board isn't valid until after main
    pico_unique_board_id_t sn;
    uint64_t serNum=0;
    // string outStr=" ";
    pico_get_unique_board_id(&sn);
    for(int i=0; i< FLASH_UNIQUE_ID_SIZE_BYTES;i++){
        serNum += (uint64_t)sn.id[i]<<((FLASH_UNIQUE_ID_SIZE_BYTES-1-i)*8);
        // outStr+=to_string(sn.id[i]);
        // outStr+=" << ";
        // outStr+=to_string(((FLASH_UNIQUE_ID_SIZE_BYTES-1-i)*8));
        // outStr+=" = ";
        // outStr+=to_string(serNum);
    }
    // uart_puts(uart0,outStr.c_str());

    return serNum;
}



uint8_t calcFletcherCksum(uint8_t inRegnum,uint8_t inSize,int* inData){
    uint8_t c[2]={0,0};
    c[0]=(uint8_t)0;c[1]=(uint8_t)0;
    c[0] = (c[0]+(uint8_t)inRegnum)%256;
    // c[1] = (c[1]+c[0])%256;
    // c[0] = (c[0]+inSize)%256;
    c[1] = (c[1]+c[0])%256;
    for(int i=0;i<inSize;i++){
        c[0] = (c[0]+inData[i])%256;
        c[1] = (c[1]+c[0])%256;
    }
    
    // printf("checksum: 0x%04x\n",c[0]+(c[1]<<8)); 
    //return(c[0]+(c[1]<<8));

    return((c[0]+c[1])%256);
}


int getDataFrom(uint8_t source){
    // printf("getDataFrom %d\n",source);
    // sleep_ms(1000);
    switch(source){
        case SER:
            // uart_putc(uart0,'T');
            if(!uart_is_readable_within_us(uart0,1*1000)) {
                // uart_putc(uart0,'e');
                return PICO_ERROR_TIMEOUT;
                }
            else {
                // uart_putc(uart0,'G');
                return(uart_getc(uart0));
                }
            break;
        case I2C:
            return i2cslave.getchar_timeout_us(1000);
            
    }
    // printf("finishing up getDataFrom\n");
    return PICO_OK;
}