#ifndef CORE1
#define CORE1

#include "Digit.hpp"
#include "RGB.hpp"
#include "pico/util/queue.h"
#include "pico/stdlib.h"
#include "../lib/constants.hpp"
#include "seeed_XIAO_details.h"
#include "Queue.hpp"
#include "Registers.hpp"



#define CORE1_HB XIAO_RED_LED_PIN
extern  Registers regs;
extern queue_t coreQueue;
bool core1_digitUpdate_callback(struct repeating_timer *t);
bool core1_heartbeat_callback(struct repeating_timer *t);
void core1_entry();

#endif