#ifndef GPIOACCESS_HPP
#define GPIOACCESS_HPP
#include <gpiod.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <thread>
#include <atomic>

class GPIOaccess {
    private:
    std::filesystem::path chip_path;
    gpiod::line_settings lineSettings;
    gpiod::line::offsets line_offsets;
    
    
    public:
    std::vector<gpiod::edge_event> eventVector;
    // stopWatch stopwatch;
    std::atomic<bool> eventRunning = false;;
    std::thread eventThread;
    void eventThreadFunc();
    /*
    * A larger buffer is an optimisation for reading bursts of events from
    * the kernel, but that is not necessary in this case, so 1 is fine.
    */
    // edgeEventBuffer(64);
    gpiod::edge_event_buffer edgeEventBuffer;

    std::string consumer = "GPIO";
    uint* lines;
    // gpiod_line_direction direction;
    GPIOaccess();
    int init(gpiod::line::offsets);
    ~GPIOaccess();

    int8_t getPinValue(uint);
    uint getEdgeEventCount();

    // void operator = (const GPIOaccess &G ) { 
    //     this->chip = G.chip;
    //     this->lineSettings = G.lineSettings;
    //     this->lineConfig = G.lineConfig;
    //     this->lines = G.lines;
    //     this->requestConfig = G.requestConfig;
    //     this->request = G.request;
    //     this->direction = G.direction;
    //  }
};
#endif