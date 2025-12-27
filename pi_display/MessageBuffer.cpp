#include "MessageBuffer.hpp"


// Add a message to the buffer
// The message is a struct with a timestamp, source, command, data and response
// The timestamp is the time when the message was added to the buffer    
// void MessageBuffer::addMessage(const std::string& source, const bool& ready, const std::string& command , const std::string& data,const std::string& buff, const int64_t chan) {
//     // this->mtx.lock();
//     this->messages.push_back(Message());
//     this->messages.back().timestamp = std::chrono::system_clock::now();
//     this->messages.back().source = source;
//     this->messages.back().command = command;
//     this->messages.back().data = data;
//     this->messages.back().ID = IDcounter++;
//     this->messages.back().channel = chan;
//     this->messages.back().dataSize = data.length();
//     this->messages.back().response = "";
//     this->messages.back().buffer = buff;
//     this->messages.back().bufferSize = buff.length();
//     this->messages.back().ready = ready;
//     // for(uint32_t i=0;i<buff.length();i++){
//     //     this->messages.back().bufferQueue.push(buff[i]);
//     // }

//     // this->mtx.unlock();
//     // Print the message to the console
//     std::cout << "****************************************" << std::endl;
//     std::cout << "Message added: " << source << " " << command << " " << data << std::endl;
//     std::cout << "ID: " << IDcounter - 1 << std::endl;
//     std::cout << "Source: " << this->messages.back().source << std::endl;
//     std::cout << "Command: " << this->messages.back().command << std::endl;
//     std::cout << "Data: " << this->messages.back().data << std::endl;
//     std::cout << "Buffer: " << this->messages.back().buffer << std::endl;
//     std::cout << "Channel: " << this->messages.back().channel << std::endl;
//     std::cout << "Data size: " << this->messages.back().dataSize << std::endl;
//     std::cout << "Ready: " << this->messages.back().ready << std::endl;
//     std::cout << "Timestamp: " << this->messages.back().timestamp.time_since_epoch().count() << std::endl;
//     std::cout << "Buffer size: " << messages.size() << std::endl;
//     std::cout << "----------------------------------------" << std::endl;
// };

// Delete a message from the buffer
// The message is deleted by its ID
void MessageBuffer::deleteMessage(uint64_t ID) {

    auto it = std::remove_if(messages.begin(), messages.end(), [ID](const Message& msg) {
        return msg.ID == ID;
    });

    /*if(it->buffer.empty() == false) {
        it->command = "";
        it->data = "";
        it->dataSize = -1;
        it->ready = false;
        it->timestamp = std::chrono::system_clock::now();

    }
    else */messages.erase(it, it+1);
    
};


void MessageBuffer::printMessages() {
    for (const auto& msg : this->messages) {
        std::cout << "Timestamp: " << std::chrono::system_clock::to_time_t(msg.timestamp)
                  << ", Source: " << msg.source
                  << ", Command: " << msg.command
                  << ", Data: " << msg.data
                  << ", Response: " << msg.response << std::endl;
    }
};

void MessageBuffer::clearMessages() {
    messages.clear();
}
