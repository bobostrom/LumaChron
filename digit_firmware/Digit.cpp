#include "Digit.hpp"
#include "Pixels.hpp"

// #include <iostream>
// #include <stdio.h>
// #include "constants.hpp"
// #include "charactermap.hpp"

// #include <bitset>


// using namespace std;
extern Registers regs;

Digit::Digit(void){
    
    uint8_t dt = 0;
    uint8_t temp = 0;
    //get ready to read an input pin to tell which type of device this is


    for(int i=0;i<sizeof(TYPEINPUTPINS);i++){
        gpio_init(TYPEINPUTPINS[i]);
        
        gpio_set_dir(TYPEINPUTPINS[i],GPIO_IN);
        //gpio_pull_up(TYPEINPUTPINS[i]);
        gpio_set_pulls(TYPEINPUTPINS[i],true,false);
        
    }

    sleep_ms(1000); //this needed to settle inputs after these changes???
    dt = 0;
    for (int i=0; i<sizeof(TYPEINPUTPINS);i++){
        //first and it with 0x01, then shift it left by # of bit then | it back into devType    
        dt = dt + ((((uint8_t)gpio_get(TYPEINPUTPINS[i])) & 0x01)<<i);
        // printf("input pin %d type is 0x%02x it's value is 0x%02x\n",i,gpio_get_function(TYPEINPUTPINS[i]),gpio_get(TYPEINPUTPINS[i]));
    }
    

     regs.deviceType=(devType)dt;
    //  printf("dt is 0x%02x\n",regs.devType);
    //  sleep_ms(1000);
    
     //regs.bootsel = dt; //THIS IS BOOTSEL FOR INFO ONLY!!!!!!

    switch (regs.deviceType){
        case MONOCOLORDIGIT:
            initMONOCOLORDIGIT();
            break;
        case NEOPIXELDIGIT:
            initNEOPIXELDIGIT();
            break;
        default:
            printf("Invalid Device Type, check input pins %d and %d read as 0x%02x\n",TYPEINPUTPINS[0],TYPEINPUTPINS[1], regs.deviceType);
    }

}
/******************************************************************************
 * Function to initialize for a one color 7 segment display                   *
 * This is run by the class initializer                                       *
 * The I/O pins are set up, the blinking colon and decimal point are set      * 
*/
void Digit::initMONOCOLORDIGIT(void){
    descr = "One color LED seven segment display";
    regs.deviceType = MONOCOLORDIGIT;
    // regs.numPixels = 0;

    regs.colon = ' ';
    regs.decimalpt = ' ';
    regs.colonBlink = false;
    regs.decimalptBlink = false;
    //set up the output pins and set them to zero
    for(uint8_t i; i< sizeof(segmentPinArray)/sizeof(uint8_t); i++){
        gpio_init(segmentPinArray[i]);
        gpio_put(segmentPinArray[i],0);
        gpio_set_dir(segmentPinArray[i], GPIO_OUT);
        gpio_set_drive_strength(segmentPinArray[i], GPIO_DRIVE_STRENGTH_2MA);
    }
    
    gpio_init(SEGMENT_COLON);
    gpio_put(SEGMENT_COLON,0);
    gpio_set_dir(SEGMENT_COLON, GPIO_OUT);
    gpio_set_drive_strength(SEGMENT_COLON,GPIO_DRIVE_STRENGTH_2MA);
    
    gpio_init(SEGMENT_DECPT);
    gpio_put(SEGMENT_DECPT,0);
    gpio_set_dir(SEGMENT_DECPT, GPIO_OUT);
    gpio_set_drive_strength(SEGMENT_DECPT,GPIO_DRIVE_STRENGTH_2MA);
};


void Digit::initNEOPIXELDIGIT(void){
    descr = "Multi-color neopixel seven segment display";
    // description = "Pixel Display";
    printf("Setting up Pixel Display\n");
    regs.deviceType = NEOPIXELDIGIT;
    //regs.numPixels = NUMPIXELS;
    regs.colonBlink = false;
    regs.decimalptBlink = false;
    //cout << "init pixels with " << NUMPIXELS << " pixels" << endl;
    pixels.init(DGPIXELPIN, regs.numPixels, 0);
    //demo();
 
};
Digit::~Digit(){

};

// };
/*!
 * \brief Light up all the pixels in the chain
 * \ingroup Digit
 *
 * Light up all the pixels with an alternating red white and blue colors
 *
 * \note ...
 * no parameters and returns void
 * @return void
 */
// void Digit::demo(){
//     //cout << "doing demo" << std::endl;
//     for(int i=0; i<NUMPIXELS; i+=3){
//         //pixels.leds[i-1]=RGB(0,0,0);
//         pixels.leds[i]=RGB(0xff,0x00,0x00);
//         pixels.leds[i+1]=RGB(0xff,0xff,0xff);
//         pixels.leds[i+2]=RGB(0x00,0x00,0xff);
//         pixels.show();
//         sleep_ms(50);
//     }
// //    sleep_ms(1000);
//     // pixels.clear();
// };

/**
 * @brief Update the display with the elapsed time given.
 * 
 * @param et_us elapsed time in microseconds
 * @type uint64_t
.... */

void Digit::updateDigit(){  
    string segs;
    characterMap map;
    int segmentonoff;
    
    segs = map.segmentMap[regs.symbol];
    // printf("char: %c segs: %s\n",regs.symbol,segs.c_str());
    uint8_t k=0;
    for(std::string::iterator it=segs.end()-1; it!=segs.begin()-1; --it){
        //segmentonoff = (int)*it - (int)'0';
        if(!regs.symbol.blinker) {segmentonoff=0;}
        else if(*it=='1') {segmentonoff=1;}
        else {segmentonoff=0;}

        switch (regs.deviceType){
            case MONOCOLORDIGIT:
                gpio_put(segmentPinArray[k],segmentonoff);
                break;
       
            case NEOPIXELDIGIT:
                //set each pixel within segment k
                for(int i=0;i<NUMPIXELSPERSEGMENT;i++){
                    int lednum = segmentsStart+i+k*NUMPIXELSPERSEGMENT;
                    pixels.leds[lednum].r=regs.symbolColor.data.r*segmentonoff;
                    pixels.leds[lednum].g=regs.symbolColor.data.g*segmentonoff;
                    pixels.leds[lednum].b=regs.symbolColor.data.b*segmentonoff;
                    // pixels.leds[lednum].r=((((float)regs.symbolColor.data.r)/(255-regs.brightness)))*segmentonoff;
                    // pixels.leds[lednum].g=((((float)regs.symbolColor.data.g)/(255-regs.brightness)))*segmentonoff;
                    // pixels.leds[lednum].b=((((float)regs.symbolColor.data.b)/(255-regs.brightness)))*segmentonoff;
                    //printf("%d\n",regs.symbolColor.r);
                }
                break;
        }
        k++;
    }
    switch (regs.deviceType){
        case MONOCOLORDIGIT:   
            //printf("_%X",regs.colonBlink);      
            if(regs.colon == ':' && regs.colon.blinker){gpio_put(SEGMENT_COLON,1);} 
            else {gpio_put(SEGMENT_COLON,0);}
            if(regs.colon == ' '){gpio_put(SEGMENT_COLON,0);}
            
            if(regs.decimalpt == '.' && regs.decimalpt.blinker){gpio_put(SEGMENT_DECPT,1);} 
            else {gpio_put(SEGMENT_DECPT,0);}
            if(regs.decimalpt == ' '){gpio_put(SEGMENT_DECPT,0);}
            break;
        case NEOPIXELDIGIT:
            if(regs.colon == ':'){
                for(int i=colonStart;i<=colonEnd;i++){

                    pixels.leds[i] = regs.colonColor*(float)regs.colon.blinker; 
                    // pixels.leds[i] = pixels.leds[i]*(uint8_t)regs.brightness;
                }
            }else{
                for(int i=colonStart;i<=colonEnd;i++){
                    pixels.leds[i] = RGB(0,0,0); 
                }
            }
            if(regs.decimalpt == '.'){
                for(int i=decimalPtStart;i<=decimalPtEnd;i++){
                    pixels.leds[i] = regs.decimalptColor.data*(float)regs.decimalpt.blinker;
                    // pixels.leds[i] = pixels.leds[i]*(uint8_t)regs.brightness;
                }
            }else{
                for(int i=decimalPtStart;i<=decimalPtEnd;i++){
                    pixels.leds[i] = RGB(0,0,0);
                }
            }

            // now send out to the pixels
            pixels.show();
            break;
    }

};


// void Digit::allColorDemo(){
//     RGB color(1,1,1);
//     while(true){
//         for (int i=0;i<=this->pixels.numpix;i++){
//             this->pixels.leds[i]=color;
//             color.b+=255/pixels.numpix;

//         }
//         color.b=0;
//         color.g+=10;
//         if(color.g>240) {color.r+=10; color.g=0;}
//         if(color.r>240) return;
//         this->pixels.show();
//         sleep_ms(50);
    
//     }
// }

// void Digit::fullColorDemo(){
//     //auto iter = colorMap.begin();
//     for (auto iter = colorMap.begin(); iter != colorMap.end(); /*++iter*/){
//         for (int i=0;i<=this->pixels.numpix;i++){
//             this->pixels.leds[i]=iter->second;
//             this->pixels.show();
//             sleep_ms(50);
//             //cout << iter->first << endl;
//             iter++;
//             if(iter == colorMap.end()) break;
//         }
//     }
