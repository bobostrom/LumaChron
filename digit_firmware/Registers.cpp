#include "Registers.hpp"

static void save(void *param ){
    flashmem.saveAddr();
    return;
}
static void erase(void *param ){
    flashmem.erase();
    return;
}




Registers::Registers(){
    // symbol.blinker=false;
    getLADDR();
//this doesn't get run because we instantiate Registers outside of main

};

uint8_t Registers::getLADDR(){
    int i;
    for (i=0;i<FLASH_PAGE_SIZE;i++){
        if(flashmem.flash_target_contents[i] == 0xFF){
            break;
        }
    }
    
    laddr = flashmem.flash_target_contents[i-1];
    // char temp[20];
    // sprintf(temp,"Got location address: 0x%02x from flash memory",laddr.data);
    // ser.puts(temp);
    return laddr;
}

// uint64_t getSerialNumber()
// {
//     //note: pico_get_unique_board isn't valid until after main
//     pico_unique_board_id_t sn;
//     uint64_t serNum=0;
//     // string outStr=" ";
//     pico_get_unique_board_id(&sn);
//     for(int i=0; i< FLASH_UNIQUE_ID_SIZE_BYTES;i++){
//         serNum += (uint64_t)sn.id[i]<<((FLASH_UNIQUE_ID_SIZE_BYTES-1-i)*8);
//         // outStr+=to_string(sn.id[i]);
//         // outStr+=" << ";
//         // outStr+=to_string(((FLASH_UNIQUE_ID_SIZE_BYTES-1-i)*8));
//         // outStr+=" = ";
//         // outStr+=to_string(serNum);
//     }
//     // ser.puts(outStr.c_str());

//     return serNum;
// }


// int getDataFrom(uint8_t source){
//     // printf("getDataFrom %d\n",source);
//     // sleep_ms(1000);
//     switch(source){
//         case SER:
//             // ser.puts('T');
//             if(!uart_is_readable_within_us(uart0,1*1000)) {
//                 // ser.puts('K');
//                 return PICO_ERROR_IO;
//                 }
//             else {
//                 // ser.puts('E');
//                 return(uart_getc(uart0));
//                 }
//             break;
//         case I2C:
//             return PICO_ERROR_NOT_PERMITTED;
//     }
//     // printf("finishing up getDataFrom\n");
//     return PICO_OK;
// }

// uint8_t calcFletcherCksum(uint8_t inRegnum,uint8_t inSize,int* inData){
//     uint8_t c[2]={0,0};
//     c[0]=(uint8_t)0;c[1]=(uint8_t)0;
//     c[0] = (c[0]+(uint8_t)inRegnum)%256;
//     // c[1] = (c[1]+c[0])%256;
//     // c[0] = (c[0]+inSize)%256;
//     c[1] = (c[1]+c[0])%256;
//     for(int i=0;i<inSize;i++){
//         c[0] = (c[0]+inData[i])%256;
//         c[1] = (c[1]+c[0])%256;
//     }
    
//     // printf("checksum: 0x%04x\n",c[0]+(c[1]<<8)); 
//     //return(c[0]+(c[1]<<8));

//     return((c[0]+c[1])%256);
// }

/*Construction functions where the value given is a different type than the type of the register*/
//create a device type register
// template<>
// Register<devType>::Register(regNames regNumIn){
//     size=1;
//     regnum=regNumIn;
//     data=NONE;
  
// };
// //construct a uint8_t register
// template<>
// Register<uint8_t>::Register(regNames regNumIn){
//   size=1;
//   regnum=regNumIn;
//   data=0x00;
// };
// //construct a char register
// template<>
// Register<char>::Register(regNames regNumIn){
//   size=1;
//   regnum=regNumIn;
//   data=' ';
// };
// //construct a RGB register
// template<>
// Register<RGB>::Register(regNames regNumIn){
//   size=3;
//   regnum=regNumIn;
//   data.r=0xFF;
//   data.g=0xFF;
//   data.b=0xFF;
// };
// //construct a digit mode register (uint8_t)
// template<>
// Register<digitMode>::Register(regNames regNumIn){
//   size=1;
//   regnum=regNumIn;
//   data=NORMAL;
// };
// //construct a serial number type register (uint64_t)
// //dependent on a typedef for serialNum_t
// template<>
// Register<serialNum_t>::Register(regNames regNumIn){
//   size=8;
//   regnum=regNumIn;
//   data =getSerialNumber();
// };













int Registers::getReg(){
    uint8_t dataFrom;
    int inRegnum;
    
    //int inData[NUMLEDS];

    dataFrom = SER;
    
    // sleep_ms(1000);
    // ser.puts('S');
    inRegnum = getDataFrom(dataFrom);//(uint8_t)getchar_timeout_us(1000);
    if(inRegnum < 0) {
        dataFrom = I2C;
        inRegnum = getDataFrom(dataFrom);
        if(inRegnum <0) return inRegnum; 
    }
    
    
    
    
    
    // sleep_ms(1000);
    switch((uint8_t)inRegnum){
        case SYMBOL:{
            // ser.puts('8');
            if(symbol.getmyData(dataFrom)<0){return(PICO_ERROR_GENERIC);}
            mode=NORMAL;
            break;
        }
        case COLON: {
            if(colon.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            mode=NORMAL;
            break;
        }
        case DECIMALPT: {
            // if(.size != decimalpt.size)return PICO_ERROR_GENERIC;    
            if(decimalpt.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            mode=NORMAL;
            break;

        }
        case SYMBOLBLINK:{
            if(symbolBlink.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            mode=NORMAL;
            break;  
        }
        case COLONBLINK:{
            if(colonBlink.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            mode=NORMAL;
            break;
        }
        case DPBLINK:{  
            if(decimalptBlink.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);  
            mode=NORMAL;
            break;
        }
        case SYMBOL_COLOR:{
            if(symbolColor.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            ser.puts("got a symbol color");
            mode=NORMAL;
            break;
        }
        case COLON_COLOR:{
            if(colonColor.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            mode=NORMAL;
            break;
        }
        case DP_COLOR:{
            if(decimalptColor.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            mode=NORMAL;
            break;
        }
        case LADDR:{
            uint8_t currAddr;
            currAddr = laddr;
            if(laddr.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            if(currAddr == laddr){
                sprintf(outStr,"\nAddress change not needed: 0x%02X\n",laddr);
                ser.puts(outStr);
            } else {
                sprintf(outStr,"\nChanging address from: 0x%02X to 0x%02X\n",currAddr,laddr);
                ser.puts(outStr);
                flash_safe_execute(save,NULL,10000);
                ser.puts("...done\n");
            }
            break;
        }
        case FLASHCOMM:{
            if(flashComm.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            if(flashComm == 0x00){
                ser.puts("Printing flash contents:\n");
                flashmem.flashprint();
            } else if(flashComm == 0x01){
                flash_safe_execute(erase,NULL,10000);
            }
            break;
        }
        case DEVTYPE:{
            devType temp;
            temp=deviceType;
            if(deviceType.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            deviceType = temp;

            sprintf(outStr,"device type is : %d \n",deviceType);ser.puts(outStr);
            break;
        }
        case NUMPIX:{
            uint8_t temp;
            temp=numPixels;
            if(numPixels.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            numPixels = temp;
            sprintf(outStr,"there are %d pixels\n",numPixels);ser.puts(outStr);
            break;
        }
        case REBOOT:{  
            if(bootMethod.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            symbol=' ';
            colon=' ';
            decimalpt=' ';
            if(bootMethod.data == BOOTSEL){
                // ser.puts("usb boot");
                reset_usb_boot(0,0);
            } else watchdog_reboot(0,0,0);
        }
        case BRIGHTNESS:{
            ser.puts("Setting Brightness");
            if(brightness.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            mode=NORMAL;
            break;
        }
        case MODE:
            if(mode.getmyData(dataFrom)<0) return(PICO_ERROR_GENERIC);
            // if(mode.data == DEMO){mode.data=DEMO;ser.puts('d');}
            // if(mode.data == RAWPIXEL){
            //     int in;
            //     queueItem qMessage;
                
            //     for(int i=0;i<NUMPIXELS*3;i++){
            //         in =getDataFrom(dataFrom);
            //         if(in<0)return PICO_ERROR_GENERIC;
            //         qMessage.data[i]=in;
            //     }
            //     ser.puts("\nReceived:\n");
            //     for(int i=0;i<NUMPIXELS*3;i++){
            //         sprintf(outStr," 0x%02X",qMessage.data[i]);ser.puts(outStr);
  
            //     }
            //     ser.puts('\n');
            //     //now send a message to the other core with pixel data
            //     qMessage.command = mode.regnum;
            //     qMessage.size = 3*NUMPIXELS;
                
            //     queue_add_blocking(&coreQueue,&qMessage);
                
            // }
            break;
        

        default:   {

            sprintf(outStr,"Bad Input: 0x%02x\n",inRegnum);ser.puts(outStr);
            return PICO_ERROR_GENERIC;
            }
    }
return SUCCESS;  //processed one command packet
}
