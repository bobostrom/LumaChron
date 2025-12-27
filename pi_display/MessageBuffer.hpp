#ifndef MESSAGEBUFFER_HPP
#define MESSAGEBUFFER_HPP
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
#include <queue>

class MessageBuffer {

    uint64_t IDcounter=0;
    public:
    std::mutex mtx;
    // Message struct to hold the message data
    struct Message {
        //timestamp for the command
        std::chrono::time_point<std::chrono::system_clock> timestamp;
        std::string source = "";
        int64_t channel = -1;
        std::string command = "";
        std::string data = "";
        int32_t dataSize = -1;
        std::string response = "";
        std::string buffer = "";
        // std::queue<char> bufferQueue;

        // int32_t bufferSize = -1;
        int64_t ID = -1;
        bool ready = false;
        
    };
    MessageBuffer() = default;
    ~MessageBuffer() = default;
    
    std::vector<Message> messages;

    // void addMessage(const std::string& source, const bool& ready, const std::string& command, const std::string& data, const std::string& buff, const int64_t chan);
    void addMessage(){
        this->messages.push_back(Message());
        this->messages.back().timestamp = std::chrono::system_clock::now();
        this->messages.back().ID = IDcounter++;
    }

    void deleteMessage(uint64_t ID);

    void printMessages();

    void clearMessages();

};


#endif