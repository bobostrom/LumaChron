#ifndef QUEUE
#define QUEUE
#include "pico/util/queue.h"



const int QUEUE_MAX_DATA_LENGTH = 128;
typedef struct {
    uint8_t command;
    uint8_t size; //data current packet size (not command or size values)
    int16_t data[QUEUE_MAX_DATA_LENGTH];

} queueItem_t;







#endif