#ifndef BUTTONS_HPP
#define BUTTONS_HPP
#include <map>
#include <stdlib.h>
// #include "GPIOaccess.hpp"
#include <string>
#include <gpiod.h>
#include <vector>


class Buttons{
    private:
    
    public:

    Buttons(std::map<std::string,uint8_t>);
    ~Buttons();

    

    
    std::map<std::string,uint8_t> keymap;

    



    

};




#endif