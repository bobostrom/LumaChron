#ifndef REGISTERS
#define REGISTERS

#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include "pico/bootrom.h"
#include "constants.hpp"
#include "Flash.hpp"
#include "pico/util/queue.h"
#include "Queue.hpp"

#include "RGB.hpp"
// #include "Digit.hpp"
#include "Register.hpp"
#include "enums.hpp"
#include "regNum.hpp"
#include "../lib/Serial.hpp"

//external "global" variables
extern Flash flashmem;
extern queue_t coreQueue;
extern Serial ser;

class Registers{
  public:

  char outStr[200];//variable for building strings to output
  Registers(void);
  Register <char> symbol=           {.regnum=regNum_t::SYMBOL,      .size=1, .data=' '};//character to display
  Register <char> colon=            {.regnum=regNum_t::COLON,       .size=1, .data=' '};//colon character to display
  Register <char> decimalpt=        {.regnum=regNum_t::DECIMALPT,   .size=1, .data=' '};//decimal character to display
  Register <uint8_t> symbolBlink=   {.regnum=regNum_t::SYMBOLBLINK, .size=1, .data=0x00, .blinker=0};//0 for no blink, in deciseconds
  Register <uint8_t> colonBlink=    {.regnum=regNum_t::COLONBLINK,  .size=1, .data=0x00, .blinker=0};//0 for no blink, in deciseconds
  Register <uint8_t> decimalptBlink={.regnum=regNum_t::DPBLINK,     .size=1, .data=0x00, .blinker=0};//0 for no blink, in deciseconds
  Register <RGB> symbolColor=       {.regnum=regNum_t::SYMBOL_COLOR,.size=3, .data=RGB(0xFF,0XFF,0xFF)};//red; green; blue bytes of character color
  Register <RGB> colonColor=        {.regnum=regNum_t::COLON_COLOR, .size=3, .data=RGB(0xFF,0XFF,0xFF) };//red; green; blue bytes of colon color
  Register <RGB> decimalptColor=    {.regnum=regNum_t::DP_COLOR,    .size=3, .data=RGB(0xFF,0XFF,0xFF) };//red; green; blue bytes of decimal point color
  Register <uint8_t> laddr=         {.regnum=regNum_t::LADDR,       .size=1, .data=0xFF};//I2C Location/address
  Register<devType> deviceType=     {.regnum=regNum_t::DEVTYPE,     .size=1, .data=devType::NONE};//what type of device is this
  Register <boot> bootMethod=       {.regnum=regNum_t::REBOOT,      .size=1, .data=boot::UNDEF};//reboot method
  Register <uint8_t> numPixels=     {.regnum=regNum_t::NUMPIX,   .size=1, .data=NUMPIXELSPERDIGIT};
  Register <uint8_t> flashComm=     {.regnum=regNum_t::FLASHCOMM,  .size=1, .data=0xFF};
  Register <digitMode> mode=        {.regnum=regNum_t::MODE,        .size=1, .data=digitMode::NORMAL}; //show raw pixels, disables digit update
  Register <uint8_t> brightness=    {.regnum=regNum_t::BRIGHTNESS,  .size=1, .data=0xFF}; // set the brightness of the display
  Register <uint64_t> serialNumber= {.regnum=regNum_t::SERIALNUM,   .size=8,  .data=0xFFFFFFFFFFFFFFFF};// serial number (read from flash chip)
  int getReg();
 
  
  uint8_t getLADDR();
};
  

  
#endif



