#ifndef RGB_HPP
#define RGB_HPP
#include "pico/stdlib.h"
// #include <string.h>
// #include <string>
// using namespace std;

class RGB {
    public:
    int16_t r;
    int16_t g;
    int16_t b;

    RGB(int16_t red,int16_t green,int16_t blue) {r=red;g=green;b=blue;};
    //RGB(uint8_t red,uint8_t green,uint8_t blue);
    RGB(uint32_t c) { r=c>>16&0xFF; g=c>>8&0xFF; b=c>>0&0xFF; };
    RGB(void) {r=0xFF;g=0xFF;b=0xFF;};
    uint32_t urgb_u32(void){ return ((uint32_t) (r) << 8) | ((uint32_t) (g) << 16) | (uint32_t) (b);};
    RGB operator*(const int s){
    return RGB((int16_t)(r*s),(int16_t)(g*s),(int16_t)(b*s));
    };
    RGB operator*(const float s){
    return RGB((int16_t)((float)r*s),(int16_t)((float)g*s),(int16_t)((float)b*s));
    };
    RGB operator*(const RGB s){
    return RGB((int16_t)(r*s.r),(int16_t)(g*s.r),(int16_t)(b*s.r));
    };
    RGB operator/(const float s); //untested
    RGB operator-(const RGB in){
    return RGB(r-in.r, g-in.g,b-in.b);
    };
    RGB operator+(const RGB in){
    return RGB(r+in.r, g+in.g,b+in.b);
    };
    
    bool operator!=(const RGB in){
        //cout << "r: " << r << "_" << in.r <<   " g: " << g << "_" << in.g  << " b: " << b << "_" << in.b << endl;
        if(r!=in.r || g!=in.g || b!=in.b) return true;
        else return false;
    };
    void operator=(const RGB in){
        r = in.r;
        g = in.g;
        b = in.b;
    };
    void operator=(uint8_t in[3]){
        r = in[0];
        g = in[1];
        b = in[2];
    };
    //operator float() const { return uint32_t; };//
    operator uint32_t () const {
         return (g<<16 | r<<8 | b);
      }

    bool operator==(const RGB in){
        //cout << "r: " << r << "_" << in.r <<   " g: " << g << "_" << in.g  << " b: " << b << "_" << in.b << endl;
        if(r==in.r && g==in.g && b==in.b) return true;
        else return false;
    };
    //bool operator<=(const RGB in);
};


#endif