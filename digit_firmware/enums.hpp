#ifndef ENUMS
#define ENUMS
#include "pico/stdlib.h"




//data sources
enum dataSources:uint8_t{SER=0,I2C=1};

// // enumeration for register names
// enum regName_t:uint8_t {
//     SYMBOL = 0x81,//the symbol displayed
//     COLON, //turn on/off the colon (82 3A 74)on (82 20 40)off
//     DECIMALPT, //turn on/off the decimal point
//     SYMBOLBLINK, //enable the symbol blinking
//     COLONBLINK, //enable the colon blinking
//     DPBLINK, //enable the decimal point blinking
//     SYMBOL_COLOR, //red, green, blue bytes of character color
//     COLON_COLOR, //red, green, blue bytes of colon color
//     DP_COLOR, //red, green, blue bytes of decimal point color
//     LADDR, //set the Location of the display 
//     DEVTYPE, // what type of device is this? 
//     NUM_PIXELS, // the number of LEDS in the string
//     BOOTSEL, //reboot into programming mode
//     REBOOT, //reboot
//     ERASEFLASH, //erase the page of flash we're using for address storage
//     PRINTFLASH, //"register" to print the contents of the flash to the serial port
//     MODE, //set the mode of the digit
//     SERIALNUM //the serial number of the digit (from the unique id of the flash memory)

//   };

enum digitMode:uint8_t{
  NORMAL=0x00,
  RAWPIXEL,
  DEMO
};

enum boot:uint8_t{
  SOFTBOOT=0x00,  //processor resets itself
  COLDBOOT, //processsor was powered up
  BOOTSEL, //reboot into USB drive mode
  UNDEF=0xFF
};

enum devType:uint8_t {
  MONOCOLORDIGIT=0x00,
  NEOPIXELDIGIT=0x03,
  DIAL=0x80,
  BARGRAPH=81,
  NONE = 0xFF
};

typedef uint64_t serialNum_t;



#endif