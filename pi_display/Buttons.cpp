#include "Buttons.hpp"

// Buttons::Buttons(uint8_t junk){
//     printf("starting buttons\n");

// }

Buttons::Buttons(std::map<std::string,uint8_t> inmap){

    //populate the keymap from the key name and number incoming map
    //add the value as zero
    //also build the array of pin numbers
    // std::vector<uint8_t> pins;
    // int linesize = inmap.size();
    // int i = 0;
    // this->gpios.lines = new uint[linesize];
    // for (auto const& [key, val] : inmap){
    //     this->keymap.insert({key,{val,0}});
    //     this->gpios.lines[i] = val;
    //     i++;
    // }
    // this->gpios.direction = GPIOD_LINE_DIRECTION_INPUT;
    // this->gpios.consumer = "Display Clock";
    
    // this->gpios.init();
    
    // for (;;) {
    //     /* Blocks until at least one event is available. */
    //     //request.read_edge_events(this->gpios.edgeEventBuffer);
    //     if(this->gpios.edgeEventBuffer.num_events() != 0){
    //         for (const auto &event : this->gpios.edgeEventBuffer)
    //             ::std::cout << "line: " << event.line_offset()
    //                     << "  type: " << (int)event.type()
    //                     << "  event #" << event.line_seqno()
    //                     << ::std::endl;
    //     }
    // }
    
    // keymap["START"].val=this->gpios.getPinValue(25);
    // keymap["START"].val=this->gpios.getPinValue(25);
    // keymap["START"].val=this->gpios.getPinValue(keymap["START"].gpioNum);
    // keymap["START"].val=this->gpios.getPinValue(keymap["START"].gpioNum);

    // update the keymap values with the current value
    // for (auto const& [key, val] : this->keymap){
    //     this->keymap[key].val=this->gpios.getPinValue(this->keymap[key].gpioNum);
    // }
};

Buttons::~Buttons(){
    printf("Destroying buttons");

}