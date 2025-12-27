#ifndef SOCKETCOMM_HPP
#define SOCKETCOMM_HPP
#include <thread>
#include <atomic>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

#include <errno.h>

#include <sys/types.h>

#include <netdb.h>


#include <arpa/inet.h>


// #include <iostream>
#include "MessageBuffer.hpp"
// #include "ServerSocket.hpp"
// #include "SocketException.hpp"

class fdComm{
 
    uint32_t PORT = 8080;
    const uint32_t MAXMSG = 1024;
    // std::vector<MessageBuffer::Message> inMessages;
    // MessageBuffer::Message inMessages[FD_SETSIZE]; //buffer for the incoming messages
    // std::string inBuffer[FD_SETSIZE]; //buffer for the incoming data
    // std::string outBuffer[FD_SETSIZE];   
    std::uint16_t read_from_client (int filedes);
    std::uint16_t make_socket(uint16_t port);
    std::thread msgProcessorThread;
    void msgProcessorThreadFunc();
    std::atomic<bool> msgProcessorRunning = false;
    std::thread fdCommThread;
    void fdCommThreadFunc();
    MessageBuffer* msgBuffer;
    public:
    std::atomic<bool> fdCommRunning = false;
    fdComm(MessageBuffer*);
    ~fdComm() {
        fdCommRunning = false;
        fdCommThread.join();
    }
};



#endif