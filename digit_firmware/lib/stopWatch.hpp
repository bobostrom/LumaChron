#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include "pico/stdlib.h"
class stopWatch{
    public:
    enum Mode {STOPWATCH, CLOCK, COUNTDOWN, RAW} mode;
    enum State {RUNNING, STOPPED, PAUSED} state;
    absolute_time_t start_us;
    absolute_time_t lap_us;
    absolute_time_t stop_us;
    absolute_time_t elapsed_time;
    absolute_time_t lap_time;
    absolute_time_t countdown_us;
    int64_t preLoad_us;
    
    stopWatch();
    void start(void);
    void lapPause(void);
    void stop(void);
    void reset(void);
    void setCountdown(int64_t pl_us = 0);

};



#endif