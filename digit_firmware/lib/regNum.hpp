#ifndef REGNUM
#define REGNUM
#include "pico/stdlib.h"


// enumeration for register names
enum  regNum_t:uint8_t {
    SYMBOL = 0x81,//the symbol displayed
    COLON, //turn on/off the colon (82 3A 74)on (82 20 40)off
    DECIMALPT, //turn on/off the decimal point
    SYMBOLBLINK, //enable the symbol blinking
    COLONBLINK, //enable the colon blinking
    DPBLINK, //enable the decimal point blinking
    SYMBOL_COLOR, //red, green, blue bytes of character color
    COLON_COLOR, //red, green, blue bytes of colon color
    DP_COLOR, //red, green, blue bytes of decimal point color
    LADDR, //set the Location of the display 
    DEVTYPE, // what type of device is this? 
    // NUM_PIXELS, // the number of LEDS in the string
    REBOOT, //reboot
    FLASHCOMM, //erase or print the page of flash we're using for address storage
    // PRINTFLASH, //"register" to print the contents of the flash to the serial port
    MODE, //set the mode of the digit including set the string of led values
    BRIGHTNESS, //set the brightness for the digit
    NUMPIX,
    SERIALNUM //the serial number of the digit (from the unique id of the flash memory)

  };

  #endif