#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include <string>
#include <map>
// #include <forward_list>
#include <list>
// #include <vector>
#include "Digit.hpp"
#include "I2Cbus.hpp"
#include "colorCron.hpp"
#include "RGB.hpp"
// #include "colorConstants.hpp"
#include "stopWatch.hpp"
#include "Buttons.hpp"
#include <thread>
#include <atomic>
#include "GPIOaccess.hpp"
#include <iostream>
#include <string>

class Display {
    GPIOaccess *dispgpios;
    
    // forward_list<colorCron> colorList;
    // colorMap_c cM;

    //TODO:  some of these constants should be in the digit class, not here
    const uint8_t NUMPIXELSPERSEGMENT=4;
    const uint8_t NUMSEGMENTSPERDIGIT=7;
    const uint8_t NUMDIGITSPERDISPLAY=6;
    const uint8_t NUMCOLONSPERDIGIT=1;
    const uint8_t NUMPIXELSPERCOLON=2;
    const uint8_t NUMPIXELSPERDECIMAL=1;
    const uint8_t NUMDECIMALSPERDIGIT=1;
    const uint8_t NUMPIXELSPERDIGIT=(NUMPIXELSPERSEGMENT*NUMSEGMENTSPERDIGIT)+(NUMPIXELSPERCOLON*NUMCOLONSPERDIGIT)+(NUMPIXELSPERDECIMAL*NUMDECIMALSPERDIGIT);
    const uint8_t NUMPIXELS= NUMDIGITSPERDISPLAY * NUMPIXELSPERDIGIT;

    uint8_t baseAddress;

    I2Cbus* ibus;
    uint8_t numDigits=0;
    public:
    // void updateAllDigits();
    int dispBrightness = 0;
    std::list<colorCron> colorList;
    enum displayMode_t:uint8_t {PAUSE, STOPWATCH, CLOCK, COUNTDOWN, STRING, BLANK, RAW} displayMode=CLOCK;

    colorCron currColorCron;

    Digit* digit;

    Display(I2Cbus* ib, uint8_t baddr, GPIOaccess*);
    ~Display() {
        clockRunning = false;
        stopWatchRunning = false;
        clockThread.join();
        stopWatchThread.join();
    }
    
    std::atomic<bool> clockRunning;
    std::thread clockThread;
    void clockThreadFunc();

    stopWatch stopwatch;
    std::atomic<bool> stopWatchRunning;
    std::thread stopWatchThread;
    void stopWatchThreadFunc();

    int8_t detectNumDigits();
    void update(std::string, uint8_t, uint8_t, uint8_t,uint8_t);
    void update(uint64_t);



};
#endif