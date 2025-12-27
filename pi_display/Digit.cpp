
#include "Digit.hpp"
#include "I2Cbus.hpp"
//#include "hardware/i2c.h"
#include <stdio.h>
#include <iostream>
#include <limits>


// #define REGCOLON 0x01
// #define REGDECIMAL 0x02
// #define REGCHARCOLOR 0x03
// #define REGCOLONCOLOR 0x04
// #define REGDPCOLOR 0x05
// #define REGCHAR 0x06
// enum:uint8_t {BEGINREG=0, REGCOLON, REGDECIMAL, REGCHAR, REGCOLONCOLOR, REGDPCOLOR, REGCHARCOLOR, ENDREG};
//I don't think this function is used anywhere
Digit::Digit(I2Cbus* ib, uint8_t addr){
    character = 0;
    ibus = ib;
    address = addr;
    //std::cout << "initializing Digit at address: " << std::hex << +address << std::dec << std::endl << std::flush;

}
 Digit::Digit(){
     character = 0;
     //std::cout << "initializing Digit with no bus or address" << std::endl << std::flush;

}
void Digit::init(I2Cbus* ib, uint8_t addr){
    character = 0;
    colon = 0;
    decimalpt= 0;
    ibus = ib;
    address = addr;
    symbolBlink = 0;
    colonBlink = 0;
    dpBlink = 0;
    brightness = 0;
    charColor = RGB(0xFF,0xFF,0xFF);
    colonColor = RGB(0xFF,0xFF,0xFF);
    dpColor = RGB(0xFF,0xFF,0xFF);
    //std::cout << "init Digit at address: " << std::hex << +address << std::dec << std::endl << std::flush;
    updateDigit(std::string("0.:"),charColor, colonColor, dpColor, brightness, symbolBlink,colonBlink,dpBlink);
}


void Digit::calcCksum(uint8_t *arr,uint8_t size){
    arr[size]=0;
    arr[size+1]=0;
    for(uint8_t i =0; i<size ; i++){
        arr[size] = (arr[size]+arr[i])%256;
        arr[size+1] = (arr[size+1]+arr[size])%256;
    }
}



/// @brief Update a digit
/// @param newDigit the symbol to update including punctuation "0:."
/// @param newCharColor the color for the digit
/// @param newColonColor the color for the colon
/// @param newdpColor the color for the decimal point
/// @param newBrightness the brightness for the digit
/// @param newSymbolBlink the blink rate for the digit
/// @param newColonBlink the blink rate for the colon
/// @param newdpBlink the blink rate for the decimal point
void Digit::updateDigit(std::string newDigit, RGB newCharColor, RGB newColonColor, RGB newdpColor, uint8_t newBrightness,
                        uint8_t newSymbolBlink, uint8_t newColonBlink, uint8_t newdpBlink){
    uint8_t outarr[20];
    uint8_t checksum;
    int8_t rv;
    bool updated=false;
    
    //sleep_ms(1000); 
    //std::cout << "incoming substring|" << newDigit << "|" << std::endl;
    char newChar = newDigit.at(0);
    char newDecPt = newDigit.at(1);
    char newColon = newDigit.at(2);
    if(verbose) printf("%s:0x%02x ",this->ibus->busName.c_str(),address);
    // fullUpdate = true;
    if(newdpBlink != dpBlink || fullUpdate)
    {
        if(verbose) printf("Updating dpBlink");
        updated=true;
        outarr[0] = DPBLINK;
        // outarr[1] = 1; //size of data field
        outarr[1] = newdpBlink;
        outarr[2] = 0xff;
        // calcCksum(outarr,3);
        //std::cout << "*";
        rv = ibus->writeI2c(address, outarr,3); 
        if(rv!=3){
            //retry
            rv = ibus->writeI2c(address, outarr,3); 
        }
        if(rv >=0 ) dpBlink = newdpBlink;
    }
    if(newColonBlink != colonBlink || fullUpdate)
    {
        if(verbose) printf("Updating colonBlink");
        updated=true;
        outarr[0] = COLONBLINK;
        // outarr[1] = 1; //size of data field
        outarr[1] = newColonBlink;
        outarr[2] = 0xff;
        // calcCksum(outarr,3);
        //std::cout << "*";
        rv = ibus->writeI2c(address, outarr,3); 
        if(rv!=3){
            //retry
            rv = ibus->writeI2c(address, outarr,3); 
        }
        if(rv >=0 ) colonBlink = newColonBlink;
    }
    if(newSymbolBlink != symbolBlink || fullUpdate)
    {
        if(verbose) printf("Updating symbolBlink");
        updated=true;
        outarr[0] = SYMBOLBLINK;
        // outarr[1] = 1; //size of data field
        outarr[1] = newSymbolBlink;
        outarr[2] = 0xff;
        // calcCksum(outarr,3);
        //std::cout << "*";
        rv = ibus->writeI2c(address, outarr,3); 
        if(rv!=3){
            //retry
            rv = ibus->writeI2c(address, outarr,3); 
        }
        if(rv >=0 ) symbolBlink = newSymbolBlink;
    }
    if(newBrightness != brightness || fullUpdate)
    {
        if(verbose) printf("Updating brightness");
        updated=true;
        outarr[0] = BRIGHTNESS;
        // outarr[1] = 1; //size of data field
        outarr[1] = newBrightness;
        outarr[2] = 0xff;
        // calcCksum(outarr,3);
        //std::cout << "*";
        rv = ibus->writeI2c(address, outarr,3); 
        if(rv!=3){
            //retry
            //TODO: busy_wait_ms(100);
            rv = ibus->writeI2c(address, outarr,3); 
        }
        if(rv >=0 ) brightness = newBrightness;
    }
    if(newChar != character || fullUpdate)
    {
        if(verbose) printf("Updating character");
        updated=true;
        outarr[0] = SYMBOL;
        // outarr[1] = 1; //size of data field
        outarr[1] = newChar;
        outarr[2] = 0xff;
        // calcCksum(outarr,3);
        //std::cout << "*";
        rv = ibus->writeI2c(address, outarr,3); 
        if(rv!=3){
            //retry
            rv = ibus->writeI2c(address, outarr,3); 
        }
        if(rv >=0 ) character = newChar;
    }  
    
    if(newDecPt != decimalpt || fullUpdate)
    {
        if(verbose) printf("Updating decimal point");
        updated=true;
        outarr[0] = DECIMALPT;
        // outarr[1] = 1; //size of data field
        outarr[1] = newDecPt;
        outarr[2]=0xff;
        // calcCksum(outarr,3);
        //std::cout << "*";
        rv = ibus->writeI2c(address, outarr,3); 
        if(rv!=3){
            //retry
            rv = ibus->writeI2c(address, outarr,3); 
        }
        if(rv >=0 ) decimalpt = newDecPt;

    }
    if(newColon != colon || fullUpdate)
    {
        if(verbose) printf("Updating colon");
        updated=true;
        outarr[0] = COLON;
        // outarr[1] = 1; //size of data field
        outarr[1] = newColon;
        outarr[2] = 0xff;
        // calcCksum(outarr,3);
        //std::cout << "*";
        rv = ibus->writeI2c(address, outarr,3); 
        if(rv != 3) {
            //retry
            rv = ibus->writeI2c(address, outarr,3); 
        }
        if(rv >=0 ) colon = newColon;

    }
    if(charColor != newCharColor || fullUpdate)
    {
        if(verbose) printf("Updating charColor");
        updated=true;
        outarr[0] = SYMBOL_COLOR;
        // outarr[1] = 3; //size of data field
        outarr[1] = newCharColor.r;
        outarr[2] = newCharColor.g;
        outarr[3] = newCharColor.b;
        // calcCksum(outarr,5);
        outarr[4]=0xff;
        //std::cout << "*" << std::flush;
        rv = ibus->writeI2c(address, outarr, 5);
        if(rv != 5) {
            //retry
            rv = ibus->writeI2c(address, outarr, 5);
        }
        //std::cout << "." << std::flush;
        if(rv >= 0) charColor=newCharColor;
    }
    if(dpColor != newdpColor || fullUpdate)
    {
        if(verbose) printf("Updating dpColor");
        updated=true;
        outarr[0] = DP_COLOR;
        // outarr[1] = 3; //size of data field
        outarr[1] = newdpColor.r;
        outarr[2] = newdpColor.g;
        outarr[3] = newdpColor.b;
        // calcCksum(outarr,5);
        outarr[4]=0xff;
        //std::cout << "*";
        rv = ibus->writeI2c(address, outarr, 5);
        if(rv != 5) {
            //retry
            rv = ibus->writeI2c(address, outarr, 5);
        }
        if(rv >= 0) dpColor=newdpColor;
    }
    if(colonColor != newColonColor || fullUpdate)
    {  
        if(verbose) printf("Updating colonColor");
        updated=true;
        outarr[0] = COLON_COLOR;
        // outarr[1] = 3; //size of data field
        outarr[1] = newColonColor.r;
        outarr[2] = newColonColor.g;
        outarr[3] = newColonColor.b;
        // calcCksum(outarr,5);
        outarr[4]=0xff;
        //std::cout << "*";
        rv = ibus->writeI2c(address, outarr, 5);
        if(rv != 5) {
            //retry
            rv = ibus->writeI2c(address, outarr, 5);
        }
        if(rv >= 0) colonColor=newColonColor;
    }
    fullUpdate = false;
    if(verbose) if(updated == false){printf("no change");}
    if(verbose) printf("\n");
}