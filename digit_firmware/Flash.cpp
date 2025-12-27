#include <stdio.h>

#include "Flash.hpp"
#include "Registers.hpp"
extern Registers regs;
Flash::Flash(){
    // uint32_t ints = save_and_disable_interrupts();
    // flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    // restore_interrupts (ints);
    // printf ("initializing flashmem\n");
    // int i;
    // for (i=0;i<FLASH_PAGE_SIZE;i++){
    //     if(flash_target_contents[i] == 0xFF){
    //         break;
    //     }

    // }
    // // printf("Using address 0x%02x found at 0x%02x\n",flash_target_contents[i-1]);
    // regs.laddr = flash_target_contents[i-1];
    

    

}
void Flash::erase(void){
    // printf("Current Flash:\n");
    // for(int i=0;i<FLASH_PAGE_SIZE;i++){
        // if(i!=0 && i%16==0) printf("\n");
        // printf("0x%02x ",flash_target_contents[i]); 
        // sleep_ms(10);
    // }
    // stdio_flush();
    // sleep_ms(1000);
    // uint32_t ints = save_and_disable_interrupts();
    
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    // restore_interrupts(ints);
    // printf("Erased Flash:\n");
    // for(int i=0;i<FLASH_PAGE_SIZE;i++){
        // if(i!=0 && i%16==0) printf("\n");
        // printf("0x%02x ",flash_target_contents[i]); 
    // }
}
void Flash::flashprint(void){
    absolute_time_t startTime = get_absolute_time();
    absolute_time_t timeout = make_timeout_time_us(30000000);
    char temp[10];
    for(int i=0;i<FLASH_PAGE_SIZE;i++){
        pageBuf[i]=flash_target_contents[i];
    }
    for(int i=0;i<FLASH_PAGE_SIZE;i++){
        // uart_putc(uart0,'*');
        if(i!=0 && i%16==0) uart_putc(uart0,'\n');
        // uart_putc(uart0,'.');        
        while(!uart_is_writable(uart0)){
            if(absolute_time_diff_us(startTime,get_absolute_time()) > 1*1000*1000) return;
        }
        sprintf(temp,"0x%02x ",pageBuf[i]); 
        uart_puts(uart0,temp);
        // sleep_ms(1);
    }
    uart_putc(uart0,'\n');
    
    return;

}
int Flash::saveAddr(void){
    int newaddr = regs.laddr;
    int i = 0;
    uint32_t ints;
    //first, read the page into a buffer
    for(i=0;i<FLASH_PAGE_SIZE;i++){
        pageBuf[i]= flash_target_contents[i];
        
    }
    // printf(".");
    // stdio_flush();
    //now find the last entry
    for(i=0;i<FLASH_PAGE_SIZE;i++){
        if(pageBuf[i] == 0xFF){ //go until read FF and stop
            if(i!= 0 && pageBuf[i-1] == newaddr) return PICO_OK; //addr didn't actually change or at the first location
            pageBuf[i] = newaddr;
            break;
        }
    }
    // printf("0x%02x",i);
    // stdio_flush();
    if(i == FLASH_PAGE_SIZE){ //got to the end clear them all and prepare write into 0
        //first set up the new pagebuf with new addr at 0 and all else FF
        for(i=0;i<FLASH_PAGE_SIZE;i++){
            pageBuf[i] = 0xFF;
        }
        // printf("1");
        pageBuf[0]=newaddr;
        //now erase the actual flash back to FF
        // ints = save_and_disable_interrupts();
        
        flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
        
        // restore_interrupts(ints);
        
        // printf("^");
        // stdio_flush();
        //the write happens next like always

    }
    // printf("|");stdio_flush();
    // printf("A");stdio_flush();
    // ints = save_and_disable_interrupts();
    flash_range_program(FLASH_TARGET_OFFSET, pageBuf, FLASH_PAGE_SIZE);
    // restore_interrupts(ints);
    
    // printf("-");stdio_flush();
    // stdio_flush();
   // sleep_ms(100);
    return PICO_OK;
}


// #define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)
// using namespace std;
// const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
// uint8_t dataBuf[FLASH_PAGE_SIZE];

//     sleep_ms(1000);

//     for(int i =0;i<FLASH_PAGE_SIZE;i++){
//         dataBuf[i] = 0xFF;//(uint8_t) rand();
//     }   
//     dataBuf[0] = 0x06;
//     for (int i = 0; i < FLASH_PAGE_SIZE; ++i) {
//         printf("%02x ",flash_target_contents[i]);
//         if((i+1)%16 == 0)  printf("\n");
//     }
// printf("\n\n");
//     // Note that a whole number of sectors must be erased at a time.
//     printf("\nErasing target region...\n");


//  uint32_t ints = save_and_disable_interrupts();
//     flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);


//     printf("\nProgramming target region...\n");
//     flash_range_program(FLASH_TARGET_OFFSET, dataBuf, FLASH_PAGE_SIZE);

// restore_interrupts (ints);

//     for (int i = 0; i < FLASH_PAGE_SIZE; ++i) {
//         printf("%02x ",flash_target_contents[i]);
//         if((i+1)%16 == 0) printf("\n");
//     }
//     printf("\n\n");
    
    




