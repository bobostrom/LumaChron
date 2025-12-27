#ifndef DIGIT_HPP
#define DIGIT_HPP

#include <string>
#include <map>

#include "Pixels.hpp"
#include "Registers.hpp"
#include "RGB.hpp"



#include <iostream>
#include <stdio.h>
#include "constants.hpp"
#include "charactermap.hpp"

#include <bitset>


 



class Digit {
    public:

    // 7 segment pin definitions
    const uint8_t SEGMENT_A   = 26;
    const uint8_t SEGMENT_B   = 27;
    const uint8_t SEGMENT_C   = 28;
    const uint8_t SEGMENT_D   = 29;
    const uint8_t SEGMENT_E   = 6;
    const uint8_t SEGMENT_F   = 7;
    const uint8_t SEGMENT_G   = 0;
    static const uint8_t SEGMENT_COLON       = 4;
    static const uint8_t SEGMENT_DECPT       = 1;
    const uint8_t segmentPinArray[7] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G};
    const uint8_t DGPIXELPIN = 4;//1;//28;
    //const uint8_t TYPEINPUTPIN= 4
    const uint8_t TYPEINPUTPINS[2] = {26,27};//{SEGMENT_DECPT,SEGMENT_COLON};//


    //Registers regs;
//    RGB currentColor;
    std::string descr;
    //static int objectCount;
    Pixels pixels;
    //static std::map<char,int> colorMap;
    //static std::map<std::string, RGB> colorMap;
    void initMONOCOLORDIGIT(void);
    void initNEOPIXELDIGIT(void);
    Digit(void);
    ~Digit();
    // void demo(void);
    // void fullColorDemo(void);
    // void allColorDemo(void);
    void updateDigit();
};

#endif