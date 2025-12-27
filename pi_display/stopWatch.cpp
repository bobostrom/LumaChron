#include "stopWatch.hpp"
#include <stdio.h>
#include <iostream>
//std::map <std::string, uint8_t, uint8_t>
stopWatch::stopWatch(){
    state=STOPPED;
}

// void stopWatch::lapReset(void){
//     //this is called from a interrupt callback
//     //spend as little time here as possible
//     // std::cout << "lap button pressed" << std::endl;
//     switch(state){
//     case stopWatch::STOPPED:
//         // std::cout << "Resetting the stopwatch" << std::endl;
//         start_us = get_absolute_time();
//         //display zeros?
//         state = RESET;
//         break;
//     case stopWatch::RUNNING:
//         // std::cout << "add lap here" << std::endl;
//         lap_us = get_absolute_time();
//         //sleep_ms(2000);
//         break;
//     default:
//         break;
//     }
// }

void stopWatch::start(void){
    //if(state==STOPPED){
        state = RUNNING;
        startTime= std::chrono::high_resolution_clock::now();
    //}
    
}

void stopWatch::stop(void){
    stopTime= std::chrono::high_resolution_clock::now();
    state = STOPPED;
    
    //TODO: stop_us = get_absolute_time();
}
void stopWatch::setCountdown(int64_t cd_us){
    // preLoad_us = cd_us;
    //TODO: start_us = get_absolute_time();
    // state = COUNTDOWN;
}
void stopWatch::reset(void){
    // preLoad_us = 0;
    //TODO: start_us = get_absolute_time();
    
}
void stopWatch::lapPause(void){
    //TODO: lap_us = get_absolute_time();
    state = PAUSED;
}
// void stopWatch::startStop(void){
//     //this is called from a interrupt callback
//     //spend as little time here as possible
//     switch(state){
//         case stopWatch::RESET:
//             start_us = get_absolute_time();
//             //intentionally fall through to next case we want to start counting immediately
//         case stopWatch::STOPPED:
//             // std::cout << "Starting the stopwatch" << std::endl;
//             // start_us = get_absolute_time();
//             state = RUNNING;
//             break;
//         case stopWatch::RUNNING:
//             state=STOPPED;
//             stop_us = get_absolute_time();
//             // std::cout << "Stopping the stopwatch" << std::endl;
//             break;
//         case stopWatch::LAP: //LAP should also automatically go back to RUNNING but this can do it sooner
//             state = RUNNING;
//             break;
//         default:
//             break;
//     }
// }


