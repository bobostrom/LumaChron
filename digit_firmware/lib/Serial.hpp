#ifndef SERIAL_HPP
#define SERIAL_HPP
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include <string>
#include <queue>


class Serial {
    uart_inst_t *uart = nullptr;
    uint8_t rxPin,txPin;
    
    public:
    uint32_t baud;
    Serial(uart_inst_t *uartIn, uint8_t rxPinIN, uint8_t txPinIN,uint32_t baudIN);
    ~Serial();
    char getchar_timeout_us(uint32_t);
    char getchar();
    void putc(char);
    void puts(const char *cin);
    int getline(std::string& buf);
    std::string family;
    int available();
    static void on_uart_rx();
    static Serial* instance;
    std::queue<char> rxBuffer;
 



};

#endif