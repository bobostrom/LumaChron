


#include "Display.hpp"
//#include "stopWatch.hpp"

//extern stopWatch stopwatch;
// std::string busName, 
// void Display::updateAllDigits(){
//     // printf("full update\n");
//     for(int i=0; i< this->numDigits; i++){
//         this->digit[i].fullUpdate = true;
//     }
// }

int8_t Display::detectNumDigits(){
    int rv;
    int i=0;
    i=6;
    return i;
};

Display::Display(I2Cbus* ib, uint8_t baddr, GPIOaccess* gpios_in) /*: clockRunning(true), clockThread(&Display::clockThreadFunc, this)*/ {
    // printf("Display constructor on %s at address 0x%0x\n",ib->busName.c_str(),baddr);
    this->dispgpios = gpios_in;
    this->dispgpios->eventVector.clear();
    clockRunning = true;
    clockThread = std::thread(&Display::clockThreadFunc, this);
    stopWatchRunning = true;
    stopWatchThread = std::thread(&Display::stopWatchThreadFunc, this);

    ibus=ib;
    baseAddress = baddr;
    numDigits = detectNumDigits();
    
    colorList.assign({
        //tenths, seconds, minutes, hours, days, symbolColor, colonColor, dpColor, mode, symbolBlink, colonBlink, dpBlink
        colorCron(-0001, 00, -01, -01, -01, RGB(0xFF,0xFF,0xFF), RGB(0xFF,0xFF,0xFF), RGB(0xFF,0xFF,0xFF),16,0x00,0x00,0x00,0x10)
    });
    digit = new Digit[numDigits]; // Don't forget to delete [] ; when you're done!
    for(int i = 0;i<numDigits; i++){
         digit[i].init(ibus,baseAddress+i);
         //TODO:busy_wait_ms(100);
    }
    //TODO:busy_wait_ms(500);
    update(0);
}
void Display::clockThreadFunc(){
    std::map<uint8_t,std::string> keymap = {{20,"BRIGHTEN"},{21,"DIM"},{22,"SET"},{23,"HOUR"},{24,"MIN"},{25,"SEC"}};
    while(1){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        // printf("Hello Bob from thread! %s\n",ibus->busName.c_str());
        if(displayMode == Display::displayMode_t::CLOCK){
            for(int i = 0; i < this->dispgpios->eventVector.size();i++){
                std::cout << "Clock button at " << this->dispgpios->eventVector[i].timestamp_ns()
                << " " << keymap[this->dispgpios->eventVector[i].line_offset()] << " button" << std::endl;
                if(this->dispgpios->eventVector[i].type() == gpiod::edge_event::event_type::FALLING_EDGE) {
                    if(keymap[this->dispgpios->eventVector[i].line_offset()] == "BRIGHTEN"){
                        this->dispBrightness += 10;
                        std::cout << "setting display brightness to " << this->dispBrightness << std::endl;
                    } else 
                    if(keymap[this->dispgpios->eventVector[i].line_offset()] == "DIM"){
                        this->dispBrightness -= 10;
                        std::cout << "setting display brightness to " << this->dispBrightness << std::endl;
                    } else 
                    if(keymap[this->dispgpios->eventVector[i].line_offset()] == "SET"){
                        
                    } else 
                    if(keymap[this->dispgpios->eventVector[i].line_offset()] == "HOUR"){

                    } else 
                    if(keymap[this->dispgpios->eventVector[i].line_offset()] == "MIN"){

                    } else 
                    if(keymap[this->dispgpios->eventVector[i].line_offset()] == "SEC"){

                    } else {
                        std::cout << "UNKOWN button pressed" << std::endl;
                    }
                }
            }
            this->dispgpios->eventVector.clear();            // auto currTime = std::chrono::steady_clock::now();
            std::time_t ct = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

            // std::cout << "Hello World from main!" << "\n";
            time_t timestamp = time(&timestamp);
            struct tm datetime = *localtime(&timestamp);
            update(((uint64_t)((datetime.tm_hour*60*60+datetime.tm_min*60+datetime.tm_sec)))*1000000);
        }
        
        
    }
    
}

void Display::stopWatchThreadFunc(){
    // std::cout << "stop watch thread gpio event count" << this->dispgpios->edgeEventBuffer.num_events() << std::endl;
    std::map<uint8_t,std::string> keymap = {{20,"BRIGHTEN"},{21,"DIM"},{22,"RESET"},{23,"LAP"},{24,"STOP"},{25,"START"}};

    stopwatch.startTime= std::chrono::high_resolution_clock::now();
    stopwatch.stopTime= std::chrono::high_resolution_clock::now();
    uint64_t dur;
    while(1){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if(displayMode == Display::displayMode_t::STOPWATCH){
            for(int i = 0; i < this->dispgpios->eventVector.size();i++){
                std::cout << "Stopwatch button at " << this->dispgpios->eventVector[i].timestamp_ns()
                << " " << keymap[this->dispgpios->eventVector[i].line_offset()] << " button" << std::endl;
                if(keymap[this->dispgpios->eventVector[i].line_offset()] == "BRIGHTEN"){
                    this->dispBrightness += 10;
                    std::cout << "setting display brightness to " << this->dispBrightness << std::endl;
                } else 
                if(keymap[this->dispgpios->eventVector[i].line_offset()] == "DIM"){
                    this->dispBrightness -= 10;
                    std::cout << "setting display brightness to " << this->dispBrightness << std::endl;
                } else 
                if(keymap[this->dispgpios->eventVector[i].line_offset()] == "RESET"){
                    stopwatch.startTime = stopwatch.stopTime = std::chrono::high_resolution_clock::now();
                    stopwatch.state = stopWatch::State::STOPPED;
                } else 
                if(keymap[this->dispgpios->eventVector[i].line_offset()] == "START"){
                    if(stopwatch.state == stopWatch::State::STOPPED){
                        stopwatch.startTime = std::chrono::high_resolution_clock::now();
                    }
                    stopwatch.state = stopWatch::State::RUNNING;
                } else 
                if(keymap[this->dispgpios->eventVector[i].line_offset()] == "STOP"){
                    stopwatch.state = stopWatch::State::STOPPED;
                } else 
                if(keymap[this->dispgpios->eventVector[i].line_offset()] == "LAP"){
                    stopwatch.state = stopWatch::State::PAUSED;
                } else {
                    std::cout << "UNKOWN button pressed" << std::endl;
                }
            }
            this->dispgpios->eventVector.clear();
            switch(stopwatch.state){
                case stopWatch::State::RUNNING:
                    update(std::chrono::duration_cast<std::chrono::microseconds>
                        (std::chrono::high_resolution_clock::now() - stopwatch.startTime).count());
                    break;
                case stopWatch::State::STOPPED:
                    dur = std::chrono::duration_cast<std::chrono::microseconds>(stopwatch.stopTime - stopwatch.startTime).count();
                    update(std::chrono::duration_cast<std::chrono::microseconds>
                        (stopwatch.stopTime - stopwatch.startTime).count());
                    break;
                case stopWatch::State::PAUSED:
                    // update(std::chrono::duration_cast<std::chrono::microseconds>
                    //     (stopwatch.lapTime - stopwatch.startTime).count());
                    // if(std::chrono::duration_cast<std::chrono::seconds>
                    //     (std::chrono::high_resolution_clock::now() - stopwatch.lapTime).count() > 10){
                    //     stopwatch.state = stopWatch::State::RUNNING;
                    // }
                    break;
                default:
                    break;
            }
                        
        }
        
        
    }
    
}





// };
/// @brief update a string to the display
/// @param dispStr
void Display::update(std::string instr, uint8_t r, uint8_t g, uint8_t b, uint8_t br){
    std::string dispStr;
    displayMode = Display::displayMode_t::STRING;
    for(int i=0; i<numDigits; i++){
        dispStr.clear();
        dispStr = instr.substr(i,1) + "  ";
        digit[numDigits-1-i].updateDigit(dispStr,RGB(r,g,b),RGB(0),RGB(0),br,0x00,0x00,0x00);
        // busy_wait_us(100);

    }
};
/// @brief update a time value
/// @param et_us 
void Display::update(uint64_t et_us){  
    //  could do this different, build up a string
    //  and iterate through it, and if it finds a
    //  : or . then set the pixels but don't increment
    //   the display digit iterator
    //std::cout << "updating the display with a new time" << et_us << std::endl;
    std::string segs;
    //std::string::iterator dsIt;
    int k=0;
    int segmentonoff;
    std::string displayStr;// = "0.:0.:0.:0.:0.:0.:0.:0.:0.:0.:";
    //string tempStr = to_string(et_us);

    uint64_t rem;
    uint64_t days;
    uint64_t hours;
    uint64_t minutes;
    uint64_t seconds;
    uint64_t ms;
    uint64_t ds;
    uint16_t highestChanged;
    std::size_t firstDigit=0;
    // uint8_t symbolBlink;
    // uint8_t colonBlink;
    // uint8_t dpBlink;
    // et_us = (99*24*60*60)*1E6;
    seconds = et_us/1E6;
    ds      = et_us%(uint64_t)1E6;
    days    = seconds/(24*60*60);
    rem     = seconds%(24*60*60);
    hours   = rem/(60*60);
    rem     = rem%(60*60);
    minutes = rem/60;
    seconds = rem%60;
    highestChanged = 0;

    if(colorList.empty()){
        // printf("%s: colorList is empty\n",ibus->busName.c_str());
        // return;
        // colorList.assign({
        //     //tenths, seconds, minutes, hours, days, symbolColor, colonColor, dpColor, mode, symbolBlink, colonBlink, dpBlink
        //     colorCron(-0001, 00, -01, -01, -01, RGB(0xFF,0xFF,0xFF), RGB(0xFF,0xFF,0xFF), RGB(0xFF,0xFF,0xFF),16,0x00,0x00,0x00,0xFF)
        // });
        currColorCron = colorCron();
    }

    for (colorCron& colorC : colorList){
        // if(seconds==8)
        //     junk=colorC.seconds;
        //     junk=colorC.symbolBlink;
        if( (ds/100000 >= colorC.decSec_ms || colorC.decSec_ms<0) &&
            (seconds >= colorC.seconds || colorC.seconds<0) &&
            (minutes >= colorC.minutes || colorC.minutes<0) &&
            (hours   >= colorC.hours   || colorC.hours  <0) &&
            (days    >= colorC.days    || colorC.days   <0)) 
            {
                currColorCron = colorC;                
        }
    }
    
    if(displayMode == Display::displayMode_t::CLOCK) currColorCron.timeMode = colorCron::timeModeMap.at("HHMMSS");
    // else mode = colorCron::AUTO;
    // if first digit is AUTO, then we need to build up a string
    // otherwise we just need to update the digits using the enum value
    // important to not change the enum values, they are used to index the string
    
        //build up a longer string containing all the days, hours, minutes, seconds, tenths
        //pick the starting digits for the display in the next step
    char tempStr[40];
    displayStr =  "0  0 :0  0 :0  0 :0  0. 0  0  ";
    sprintf(tempStr,"%02d",days);
    displayStr.replace(0,1,1,tempStr[0]);
    displayStr.replace(3,1,1,tempStr[1]);
    sprintf(tempStr,"%02d",hours);
    displayStr.replace(6,1,1,tempStr[0]);
    displayStr.replace(9,1,1,tempStr[1]);
    sprintf(tempStr,"%02d",minutes);
    displayStr.replace(12,1,1,tempStr[0]);
    displayStr.replace(15,1,1,tempStr[1]);
    sprintf(tempStr,"%02d",seconds);
    displayStr.replace(18,1,1,tempStr[0]);
    displayStr.replace(21,1,1,tempStr[1]);
    sprintf(tempStr,"%06d",ds);
    displayStr.replace(24,1,1,tempStr[0]);
    displayStr.replace(27,1,1,tempStr[1]);

    //find the first digit that is not a space, 0, :, or .
    firstDigit = displayStr.find_first_not_of(" 0:.");
    if(firstDigit == std::string::npos){
        firstDigit=displayStr.length()-3;
    }
    if(displayStr.length()-firstDigit < numDigits*3)//+3)
        firstDigit =  displayStr.length()-(numDigits)*3;//-3;

    // mode = colorCron::HHMMSS;
    if(currColorCron.timeMode!=colorCron::timeModeMap.at("AUTO") && currColorCron.timeMode < firstDigit){
        firstDigit = currColorCron.timeMode;
    }
    if(currColorCron.timeMode == colorCron::timeModeMap.at("AUTO")){



    }
    // std::string subStr=" ";
    try {
    displayStr = displayStr.substr(firstDigit,numDigits*3);
    }
    catch (std::out_of_range& e){
        displayStr = "0  0 :0  0. 0  0  ";
    }
    // maybe do this if on AM PM time, but needs more work, this isn't right
    // if(displayStr[0] == '0'){
    //     displayStr[0] = ' ';
    // }   
    for(uint8_t i = 0; i<numDigits; i++){
        std::string digitStr = displayStr.substr(displayStr.length()-3-(i*3),3);
        if(i==0){ //never show colon or decimal point on the lowest digit
            digitStr[1]=' ';
            digitStr[2]=' ';
        }
        uint8_t newBrightness;
        if(((int)currColorCron.brightness + (int)this->dispBrightness) > 255){
            newBrightness = 255;
            this->dispBrightness = 255 - this->currColorCron.brightness;
        }
        else if(((int)currColorCron.brightness + (int)this->dispBrightness) < 0){
            newBrightness = 0;
            this->dispBrightness = 0 - this->currColorCron.brightness;
        }
        else {
            newBrightness = currColorCron.brightness + this->dispBrightness;
        } 

        digit[i].updateDigit(digitStr,
            currColorCron.symbolColor,
            currColorCron.colonColor,
            currColorCron.dpColor,
            newBrightness,
            currColorCron.symbolBlink,
            currColorCron.colonBlink,
            currColorCron.dpBlink
        );
    }

};


// void Display::displayScan(void) {
//     printf("\nI2C Bus Scan\n");
//     // printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

//     // for (int addr = 0; addr < (1 << 7); ++addr) {
//     //     if (addr % 16 == 0) {
//     //         printf("%02x ", addr);
//     //     }

//     //     // Perform a 1-byte dummy read from the probe address. If a slave
//     //     // acknowledges this address, the function returns the number of bytes
//     //     // transferred. If the address byte is ignored, the function returns
//     //     // -1.

//     //     // Skip over any reserved addresses.
//     //     int ret;
//     //     uint8_t rxdata;
//     //     //if (reserved_addr(addr))
//     //         ret = PICO_ERROR_GENERIC;
//     //     //else
            
//     //         ret = Digit::i2c_write(bus, addr, &rxdata, 1, false,10000000);

//     //     printf(ret < 0 ? "." : "@");
//     //     printf(addr % 16 == 15 ? "\n" : "  ");
//     // }

// };


// // starting point of thread for this display
// void Display::operator()(uint8_t baddr, I2Cbus* inbus){
//     baseAddress = baddr;
//     ibus=inbus;


//     while(1){
//         std::this_thread::sleep_for(std::chrono::milliseconds(700));
//         printf("Hello World from thread! %s %x\n",ibus->busName.c_str(),brightness);
//     }
// };

// Display::Displayold(I2Cbus* ib,/* uint8_t sdapin, uint8_t sclpin,*/ uint8_t baddr){
//     baseAddress = baddr;
//     ibus = ib;
//     // numDigits =nd;
//     numDigits =ibus->i2cdetectNumDigits(0x00);
//     // std::cout << "initializing i2c on bus: " << bus << " at base address " << +baseAddress << " on pins " << +sdapin << " " << +sclpin << std::endl;
//     // gpio_set_function(sdapin, GPIO_FUNC_I2C);
//     // gpio_set_function(sclpin, GPIO_FUNC_I2C);
//     // gpio_pull_up(sdapin);
//     // gpio_pull_up(sclpin);
//     // gpio_set_input_hysteresis_enabled(sdapin,true);
//     // gpio_set_input_hysteresis_enabled(sclpin,true);
//     // gpio_set_slew_rate(sdapin,GPIO_SLEW_RATE_SLOW);
//     // gpio_set_slew_rate(sclpin,GPIO_SLEW_RATE_SLOW);
//     // i2c_init(bus, 1000 * 1000);
//     // colorList.assign({
//     //     colorCron(-0001, 00, -01, -01, -01, RGB(0x00,0xFF,0x00), RGB(0xFF,0x00,0x00), RGB(0x00,0xFF,0xFF),colorCron::AUTO),
//     //     // colorCron( 0007, 00,  00,  00,  00, RGB(0x00,0xFF,0x00), RGB(0xFF,0x00,0x00), RGB(0x00,0xFF,0x00)),
//     //     colorCron(-0001, 30, -01, -01, -01, RGB(0xFF,0xFF,0x00), RGB(0xFF,0x00,0xFF), RGB(0x00,0x00,0xFF),colorCron::AUTO),
//     //     colorCron(-0001, 45, -01, -01, -01, RGB(0xFF,0x00,0x0), RGB(0xFF,0x00,0xFF), RGB(0x00,0x00,0xFF),colorCron::AUTO),
//     //     // colorCron(-0001, -1, 5, -01, -01, RGB(0xFF,0x00,0x0), RGB(0xFF,0x00,0xFF), RGB(0x00,0x00,0xFF),colorCron::HHMMSS)
//     //     });
    
//     //need to use exception handling because the colorMap is not guaranteed to have the key
//     //also had to include in the CMakelists.txt a line set(PICO_CXX_ENABLE_EXCEPTIONS 1) to enable exceptions
//     //this adds 2k to the binary size(?CoPilot says 2k, but I didn't check the size before adding this line)
//     try {
//         /****************************************************************************************************************************************************************************************/
//         /** Color setup for XC ski race                                                                                                                                                       /**/
//         /** first the list of colors                                                                                                                                                          /**/
//         /**/RGB symbolGoColor = cM.rgbMap.at("LAWNGREEN");                                                                                                                                    /**/
//         /**/RGB symbolStopColor = cM.rgbMap.at("RED1");                                                                                                                                       /**/
//         /**/RGB colonStopColor = cM.rgbMap.at("BLUE");                                                                                                                                        /**/
//         /**/RGB colonGoColor = cM.rgbMap.at("RED1");                                                                                                                                          /**/
//         /**/colorList.assign({                                                                                                                                                                /**/
//         /**set up to start every 30 seconds with the go color with a +/- 3 second window                                                                                                      /**/
//         /**switch back to stop color after the go window is over                                                                                                                              /**/
//         /**                 tenths, seconds,    minutes,    hours,  days,   symbolColor,        colonColor,     dpColor,                mode,               symbolBlink,    colonBlink, dpBlink**/
//         /**/    colorCron(  +0000,  +00,        +00,        +00,    +00,    symbolGoColor,      colonGoColor,   RGB(0xFF,0x00,0x00),    colorCron::AUTO,    0x00,           0x00,       0x00),/**/
//         /**/    colorCron(  -0001,  +03,        -01,        -01,    -01,    symbolStopColor,    colonStopColor, RGB(0x00,0xFF,0x00),    colorCron::AUTO,    0x00,           0x00,       0x00),/**/
//         /**/    colorCron(  -0001,  +27,        -01,        -01,    -01,    symbolGoColor,      colonGoColor,   RGB(0xFF,0x00,0x00),    colorCron::AUTO,    0x00,           0x00,       0x00),/**/
//         /**/    colorCron(  -0001,  +33,        -01,        -01,    -01,    symbolStopColor,    colonStopColor, RGB(0x00,0xFF,0x00),    colorCron::AUTO,    0x00,           0x00,       0x00),/**/
//         /**/    colorCron(  -0001,  +57,        -01,        -01,    -01,    symbolGoColor,      colonGoColor,   RGB(0xFF,0x00,0x00),    colorCron::AUTO,    0x00,           0x00,       0x00),/**/
//         /****************************************************************************************************************************************************************************************/
//         });
//         /************************************************************************************************************************************************************************************************/
//         /** Color setup for progressing from green to yellow to red over a minute                                                                                                                     /**/                                                                                 /**/
//         /**                                                                                                                                                                                           /**/
//         /**/colorList.assign({                                                                                                                                                                        /**/
//         /**                                                                                                                                                                                           /**/
//         /**                                                                                                                                                                                           /**/
//         /**                 tenths, seconds,    minutes,    hours,  days,   symbolColor,        colonColor,           dpColor,                mode,                 symbolBlink,    colonBlink, dpBlink**/
//         /**/    colorCron(  -0000,  +00,        -00,        -00,    -00,    RGB(  0,255,0x00),  RGB(255,  0,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::AUTO  ,    0x00,           0x0A,       0x03),/**/
//         /**/    colorCron(  -0000,  +05,        -00,        -00,    -00,    RGB(  0,255,0x00),  RGB(255,  0,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x0A,       0x03),/**/
//         /**/    colorCron(  -0000,  +10,        -00,        -00,    -00,    RGB( 42,170,0x00),  RGB(170, 42,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x07,       0x03),/**/
//         /**/    colorCron(  -0000,  +20,        -00,        -00,    -00,    RGB( 84,128,0x00),  RGB(128, 84,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x05,       0x03),/**/
//         /**/    colorCron(  -0000,  +30,        -00,        -00,    -00,    RGB(128, 84,0x00),  RGB( 84,128,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x03,       0x03),/**/
//         /**/    colorCron(  -0000,  +40,        -00,        -00,    -00,    RGB(170, 42,0x00),  RGB( 42,170,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x02,       0x03),/**/
//         /**/    colorCron(  -0000,  +50,        -00,        -00,    -00,    RGB(255,  0,0x00),  RGB(  0,255,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x01,       0x03),/**/
//         /************************************************************************************************************************************************************************************************/
//         });
//     }
//     //TODO:
//     //catch the exception and assign a default color
//     catch (const std::out_of_range& e) {
//         colorList.assign({
//             //tenths, seconds, minutes, hours, days, symbolColor, colonColor, dpColor, mode
//             colorCron(-0001, 00, -01, -01, -01, RGB(0xFF,0xFF,0xFF), RGB(0xFF,0xFF,0xFF), RGB(0xFF,0xFF,0xFF),colorCron::AUTO,0x00,0x00,0x00)
//         });
//     }
    
//     symbolColor = RGB(0xFF,0xFF,0xFF);
//     colonColor = RGB(0xFF,0xFF,0xFF);
//     dpColor = RGB(0xFF,0xFF,0xFF);
//     digit = new Digit[numDigits]; // Don't forget to delete [] ; when you're done!
//     for(int i = 0;i<numDigits; i++){
//          digit[i].init(ibus,baseAddress+i);
//          //TODO:busy_wait_ms(100);
//     }
//     //TODO:busy_wait_ms(500);
//     update(0);


// try {
//     /****************************************************************************************************************************************************************************************/
//     /** Color setup for XC ski race                                                                                                                                                       /**/
//     /** first the list of colors                                                                                                                                                          /**/
//     /**/RGB symbolGoColor = cM.rgbMap.at("LAWNGREEN");                                                                                                                                    /**/
//     /**/RGB symbolStopColor = cM.rgbMap.at("RED1");                                                                                                                                       /**/
//     /**/RGB colonStopColor = cM.rgbMap.at("BLUE");                                                                                                                                        /**/
//     /**/RGB colonGoColor = cM.rgbMap.at("RED1");                                                                                                                                          /**/
//     /**/colorList.assign({                                                                                                                                                                /**/
//     /**set up to start every 30 seconds with the go color with a +/- 3 second window                                                                                                      /**/
//     /**switch back to stop color after the go window is over                                                                                                                              /**/
//     /**                 tenths, seconds,    minutes,    hours,  days,   symbolColor,        colonColor,     dpColor,                mode,               symbolBlink,    colonBlink, dpBlink**/
//     /**/    colorCron(  +0000,  +00,        +00,        +00,    +00,    symbolGoColor,      colonGoColor,   RGB(0xFF,0x00,0x00),    colorCron::AUTO,    0x00,           0x00,       0x00),/**/
//     /**/    colorCron(  -0001,  +03,        -01,        -01,    -01,    symbolStopColor,    colonStopColor, RGB(0x00,0xFF,0x00),    colorCron::AUTO,    0x00,           0x00,       0x00),/**/
//     /**/    colorCron(  -0001,  +27,        -01,        -01,    -01,    symbolGoColor,      colonGoColor,   RGB(0xFF,0x00,0x00),    colorCron::AUTO,    0x00,           0x00,       0x00),/**/
//     /**/    colorCron(  -0001,  +33,        -01,        -01,    -01,    symbolStopColor,    colonStopColor, RGB(0x00,0xFF,0x00),    colorCron::AUTO,    0x00,           0x00,       0x00),/**/
//     /**/    colorCron(  -0001,  +57,        -01,        -01,    -01,    symbolGoColor,      colonGoColor,   RGB(0xFF,0x00,0x00),    colorCron::AUTO,    0x00,           0x00,       0x00),/**/
//     /****************************************************************************************************************************************************************************************/
//     });
//     /************************************************************************************************************************************************************************************************/
//     /** Color setup for progressing from green to yellow to red over a minute                                                                                                                     /**/                                                                                 /**/
//     /**                                                                                                                                                                                           /**/
//     /**/colorList.assign({                                                                                                                                                                        /**/
//     /**                                                                                                                                                                                           /**/
//     /**                                                                                                                                                                                           /**/
//     /**                 tenths, seconds,    minutes,    hours,  days,   symbolColor,        colonColor,           dpColor,                mode,                 symbolBlink,    colonBlink, dpBlink**/
//     /**/    colorCron(  -0000,  +00,        -00,        -00,    -00,    RGB(  0,255,0x00),  RGB(255,  0,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::AUTO  ,    0x00,           0x0A,       0x03),/**/
//     /**/    colorCron(  -0000,  +05,        -00,        -00,    -00,    RGB(  0,255,0x00),  RGB(255,  0,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x0A,       0x03),/**/
//     /**/    colorCron(  -0000,  +10,        -00,        -00,    -00,    RGB( 42,170,0x00),  RGB(170, 42,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x07,       0x03),/**/
//     /**/    colorCron(  -0000,  +20,        -00,        -00,    -00,    RGB( 84,128,0x00),  RGB(128, 84,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x05,       0x03),/**/
//     /**/    colorCron(  -0000,  +30,        -00,        -00,    -00,    RGB(128, 84,0x00),  RGB( 84,128,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x03,       0x03),/**/
//     /**/    colorCron(  -0000,  +40,        -00,        -00,    -00,    RGB(170, 42,0x00),  RGB( 42,170,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x02,       0x03),/**/
//     /**/    colorCron(  -0000,  +50,        -00,        -00,    -00,    RGB(255,  0,0x00),  RGB(  0,255,0x00),    RGB(0x00,0xFF,0xFF),    colorCron::HHMMSS,    0x00,           0x01,       0x03),/**/
//     /************************************************************************************************************************************************************************************************/
//     });
// }
// //TODO:
// //catch the exception and assign a default color
// catch (const std::out_of_range& e) {
//     colorList.assign({
//         //tenths, seconds, minutes, hours, days, symbolColor, colonColor, dpColor, mode, symbolBlink, colonBlink, dpBlink
//         colorCron(-0001, 00, -01, -01, -01, RGB(0xFF,0xFF,0xFF), RGB(0xFF,0xFF,0xFF), RGB(0xFF,0xFF,0xFF),colorCron::AUTO,0x00,0x00,0x00)
//     });
// }

