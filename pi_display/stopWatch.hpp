#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>
#include <iostream>
#include <map>


class stopWatch{
    public:
    // enum Mode {STOPWATCH, CLOCK, COUNTDOWN, RAW} mode;
    enum State {RUNNING, STOPPED, PAUSED} state;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> stopTime;
    std::map  <std::string, uint8_t, uint8_t> buttons;
//std::map <std::string, uint8_t, uint8_t>
    stopWatch();
    void start(void);
    void lapPause(void);
    void stop(void);
    void reset(void);
    void setCountdown(int64_t pl_us = 0);

};



#endif