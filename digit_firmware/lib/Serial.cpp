#include "Serial.hpp"

Serial* Serial::instance = nullptr;

// RX interrupt handler
void Serial::on_uart_rx() {
    uint16_t size;
    uint8_t ch;
    instance->rxBuffer.push(uart_getc(instance->uart)); 
    return;
    while (uart_is_readable(instance->uart)) {
        ch = instance->getchar_timeout_us(1000);if(ch==PICO_ERROR_TIMEOUT)return;
        instance->rxBuffer.push(ch);
        size=0;
        ch = instance->getchar_timeout_us(1000);if(ch==PICO_ERROR_TIMEOUT)return;
        instance->rxBuffer.push(ch);
        size = (ch-'0')*100;
        ch = instance->getchar_timeout_us(1000);if(ch==PICO_ERROR_TIMEOUT)return;
        instance->rxBuffer.push(ch);
        size += (ch-'0')*10;
        ch = instance->getchar_timeout_us(1000);if(ch==PICO_ERROR_TIMEOUT)return;
        instance->rxBuffer.push(ch);
        size += (ch-'0');
        if(size>255)return;//just picket a number, could be any number
        for(int i=0;i<size;i++){
            ch = instance->getchar_timeout_us(1000);if(ch==PICO_ERROR_TIMEOUT)return;
            instance->rxBuffer.push(ch);
        }
        ch = instance->getchar_timeout_us(1000);if(ch==PICO_ERROR_TIMEOUT)return;
        instance->rxBuffer.push(ch); //get the checksum
        ch = instance->getchar_timeout_us(1000);if(ch==PICO_ERROR_TIMEOUT)return;
        instance->rxBuffer.push(ch); //get the newline
        return;
        // }
    }
}
Serial::Serial(uart_inst_t *uartIn, uint8_t rxPinIN, uint8_t txPinIN,uint32_t baudIN) : uart(uartIn) {
    instance = this;
    uart = uartIn;
    if(uart == uart0 || uart == uart1){
        //could check that the pins given are valid uart pins
        // uart = uartIn;
        txPin=txPinIN;
        rxPin=rxPinIN;
        baud=baudIN;
        gpio_set_function(rxPin,GPIO_FUNC_UART);
        gpio_set_function(txPin, GPIO_FUNC_UART);
        uart_set_format(uart,8,1,UART_PARITY_NONE);
        uart_init(uart, baud);//115200);//921600);
        // uart_puts(uart,"UART is ALIVE");

        // Set UART flow control CTS/RTS, we don't want these, so turn them off
        uart_set_hw_flow(uart, false, false);
        // Turn on FIFO's
        uart_set_fifo_enabled(uart, true);
        // Set up a RX interrupt
        // We need to set up the handler first
        // Select correct interrupt for the UART we are using
        int UART_IRQ = uart == uart0 ? UART0_IRQ : UART1_IRQ;

        // And set up and enable the interrupt handlers
        irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
        irq_set_enabled(UART_IRQ, true);
        while (uart_is_readable(uart)) {
            uart_getc(uart); // flush the buffer
        }
        // Now enable the UART to send interrupts - RX only
        uart_set_irq_enables(uart, true, false);

    }
}
Serial::~Serial(){
    if(uart == uart0 || uart == uart1){
        uart_deinit(uart);
        gpio_set_function(rxPin,GPIO_FUNC_NULL);
        gpio_set_function(txPin,GPIO_FUNC_NULL);
    }
}
int Serial::available(){
    if(uart == uart0 || uart == uart1){
        return uart_is_readable(uart);
    } else return PICO_ERROR_NOT_PERMITTED;
} 
char Serial::getchar_timeout_us(uint32_t timeout_us){
    if(uart == uart0 || uart == uart1){
        uint8_t buf;
        if(!uart_is_readable_within_us(uart, timeout_us)) return PICO_ERROR_TIMEOUT;
        return uart_getc(uart);
    } else return PICO_ERROR_NOT_PERMITTED;
}
char Serial::getchar(){
    if(uart == uart0 || uart == uart1){
        return uart_getc(uart);
    } else return PICO_ERROR_NOT_PERMITTED;
}

int Serial::getline(std::string &buf){
    char inchar=0x00;
    int i;
    buf.clear();
    do{
        inchar = uart_getc(uart);
        buf = buf + inchar;
    }while(inchar != '\n' && inchar != '\r');
    
    return 0;
}

void Serial::puts(const char *cin){
    if(uart == uart0 || uart == uart1){
        uart_puts(uart,cin);
    }
}

void Serial::putc(char cin){
    if(uart == uart0 || uart == uart1){
        uart_putc(uart,cin);
    }
}