#include "fdComm.hpp"

void fdComm::msgProcessorThreadFunc(){
  uint32_t dataSize = 0;
  // std::cout << "Hello from msgProcessorThreadFunc" << std::endl;
  while (msgProcessorRunning) {
    this->msgBuffer->mtx.lock();
    //step through the messages
    for(auto msg = this->msgBuffer->messages.begin(); msg != this->msgBuffer->messages.end(); ++msg){
      // check timestamp
      // if the message is older than 5 seconds, delete it
      // if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - msg->timestamp).count() > 5) {
      //   std::cout << "Message is older than 5 seconds, deleting it" << std::endl;
      //   this->msgBuffer->deleteMessage(msg->ID);
      //   continue;
      // }
      // if the message is ready, process it
      // if this is a socketComm message and it is not marked as ready, process it
      if(msg->source == "fdComm" && msg->ready == false){
        // std::cout << "*1*";
        // if the command is empty, and a new command is complete in the buffer, fill it in
        if(msg->command.empty() && msg->buffer.find('\n') != std::string::npos){
          // std::cout << "*2*";
          msg->command = msg->buffer.substr(0, msg->buffer.find('\n'));
          // clear out the buffer that was used to get the command
          msg->buffer.erase(0, msg->buffer.find('\n') + 1);
          msg->buffer.shrink_to_fit();
          // std::cout << msg->buffer.length() << "Command: " << msg->command << std::endl;
          continue;
        }
        // if the data size is not set, and a new data size is complete in the buffer, fill it in
        if(msg->dataSize < 0 && msg->buffer.find('\n') != std::string::npos) {
          // std::cout << "*3*" << msg->command << msg->buffer << std::endl;
          // std::cout << msg->buffer.find('\n') << "." << msg->buffer.length()<< std::endl;
          //get the data size from the buffer, if the data size is not there, set it to 0
          // if it is out of range print an error message and set it to 0
          // any other errors set it to 0
          try{
          msg->dataSize = std::stol(msg->buffer.substr(0,msg->buffer.length()-1),nullptr, 0);
          } catch (const std::invalid_argument& e) {
            // std::cerr << "no data size entered: " << e.what() << std::endl;
            //get this when the dataSize is not entered
            msg->dataSize = 0;
          } catch (const std::out_of_range& e) {
            std::cerr << "Out of range, setting the data size to 0: " << e.what() << std::endl;
            msg->dataSize = 0;
          } catch (...){
            std::cerr << "Unknown data size error, setting the data size to 0: " << std::endl;
            msg->dataSize = 0;
          }
          //clear out the buffer that was used to get the data size
          msg->buffer.erase(0, msg->buffer.find('\n') + 1);
          // msg->buffer.shrink_to_fit();
          // continue; //not sure if this is needed
        }
        //don't continue on to process the data if the dataSize is not set
        // else {/*std::cout << msg->dataSize << ":" << msg->buffer << ":"<< std::endl; */continue;} 
        // if the data collected so far is less than the data size, and there is data in the buffer, append it to the data
        if((int32_t)msg->data.length() < msg->dataSize && msg->buffer.length() > 0) {
          // std::cout << "*4*";
          uint32_t bytesToRead = msg->dataSize - msg->data.length();
          msg->data.append(msg->buffer.substr(0, bytesToRead));
          // msg->buffer.erase(0, bytesToRead);
          // msg->buffer.erase(0, msg->buffer.find('\n') + 1);
          // msg->buffer.clear();
          // msg->buffer.shrink_to_fit();
          std::string tmp = msg->buffer.substr(msg->buffer.find('\n') + 1, msg->buffer.length());
          msg->buffer = tmp;
          // msg->buffer.shrink_to_fit();
        } 
        //if the data length is equal to the data size, set the message to ready
        if(msg->data.length() == msg->dataSize) {
          msg->ready = true;
          // std::cout << "Message is ready" << std::endl;
          // 
        }
      }
    }
    this->msgBuffer->mtx.unlock();
 }
}


fdComm::fdComm(MessageBuffer* msgBufferIn){
  this->msgBuffer = msgBufferIn;

  
  try{
  this->fdCommRunning = true;
  fdCommThread= std::thread(&fdComm::fdCommThreadFunc, this);
  this->msgProcessorRunning = true;
  msgProcessorThread = std::thread(&fdComm::msgProcessorThreadFunc, this);
  
  }
  catch (const std::system_error& e) {
    std::cerr << "Error creating socket thread: " << e.what() << std::endl;
    std::exit(1);
  }
  catch (const std::exception& e) {
    std::cerr << "Error creating socket thread: " << e.what() << std::endl;
    std::exit(1);
  }
  catch (...) {
    std::cerr << "Unknown socket thread error" << std::endl;
    std::exit(1);
  }
  // std::cout << "Socket thread started" << std::endl;
}

uint16_t fdComm::read_from_client (int filedes)
{
  char buffer[MAXMSG];
  int nbytes;
  
  nbytes = read (filedes, buffer, MAXMSG);
  if (nbytes < 0)
    {
      /* Read error. */
      perror ("read");
      exit (EXIT_FAILURE);
    }
  else if (nbytes == 0)
    /* End-of-file. */
    return -1;
  else
    {
      /* Data read. */
      
      bool found = false;
      // std::cout << "Data read: " << nbytes << std::endl;
      // std::cout << "Buffer: " << buffer << std::endl;
      // std::cout << "Buffer length: " << strlen(buffer) << std::endl;
      for(auto inMessage = this->msgBuffer->messages.begin(); inMessage != this->msgBuffer->messages.end(); ++inMessage){
        // std::cout << "Checking message" << std::endl;
        // std::cout << "Source: " << inMessage->source << std::endl;
        // std::cout << "Channel: " << inMessage->channel << std::endl;
        // std::cout << "Ready: " << inMessage->ready << std::endl;
        // std::cout << "Buffer: " << inMessage->buffer << std::endl;
        // std::cout << "Filedes: " << filedes << std::endl;
        if(inMessage->source == "fdComm" && inMessage->channel == filedes && inMessage->ready == false){
          found = true;
          this->msgBuffer->mtx.lock();
          // std::cout << "Found message length of data:  " << inMessage->data.length() << std::endl;
          inMessage->buffer.append(buffer, nbytes);
          inMessage->timestamp = std::chrono::system_clock::now(); // update the timestamp
          // inMessage->bufferSize += nbytes;
          // std::cout << "Buffer: " << inMessage->buffer << std::endl;
          // fprintf (stderr, "Server: got message from %d: `%s'\n", filedes, inBuffer[filedes].c_str());)
          this->msgBuffer->mtx.unlock();
          return 0;
        }
      }
      if(found == false){
        // std::cout << "Message not found" << std::endl;
        // if the message is not found, create a new message
        // std::cout << "Creating new message" << std::endl;
        this->msgBuffer->mtx.lock();
        this->msgBuffer->addMessage();
        this->msgBuffer->messages.back().source = "fdComm";
        this->msgBuffer->messages.back().ready = false;
        this->msgBuffer->messages.back().command = "";
        this->msgBuffer->messages.back().data = "";
        this->msgBuffer->messages.back().buffer.append(buffer,nbytes);
        this->msgBuffer->messages.back().channel = filedes;
        this->msgBuffer->messages.back().dataSize = -1;
        this->msgBuffer->mtx.unlock();
        
      }
      // fprintf (stderr, "Server: got message from %d: `%s'\n", filedes, inBuffer[filedes].c_str());)
      return 0;
    }
}

uint16_t fdComm::make_socket(uint16_t port) {
  int sock;
  struct sockaddr_in name;

  /* Create the socket. */
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  /* Give the socket a name. */
  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);

  int reuse = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
      perror("setsockopt(SO_REUSEADDR) failed");


  if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
      perror("setsockopt(SO_REUSEPORT) failed");




  if (bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  return sock;
};




void fdComm::fdCommThreadFunc(){

  int sock;
  fd_set active_fd_set, read_fd_set, stdio_fd_set;
  int i;
  struct sockaddr_in clientname;
  socklen_t size;

  /* Create the socket and set it up to accept connections. */
  sock = make_socket (PORT);
  if (listen (sock, 1) < 0)
    {
      perror ("listen");
      exit (EXIT_FAILURE);
    }

  /* Initialize the set of active sockets. */
  FD_ZERO (&active_fd_set);
  FD_SET (sock, &active_fd_set);
  FD_SET (STDIN_FILENO, &active_fd_set);

  while (1)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      /* Block until input arrives on one or more active sockets. */
      read_fd_set = active_fd_set;
      int junk = FD_SETSIZE;
      if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
        {
          perror("select");
          exit(EXIT_FAILURE);
        }

      /* Service all the sockets with input pending. */
      for(i = 0; i < FD_SETSIZE; ++i)
        
        if(FD_ISSET (i, &read_fd_set))
          {
            if (i == sock)
              {
                /* Connection request on original socket. */
                int newConn;
                size = sizeof (clientname);
                newConn = accept(sock,(struct sockaddr *) &clientname, &size);
                if (newConn < 0)
                  {
                    perror ("accept");
                    exit (EXIT_FAILURE);
                  }
                // fprintf (stderr,
                //          "Server: connect from host %s, port %hd.\n",
                //          inet_ntoa (clientname.sin_addr),
                //          ntohs (clientname.sin_port));
                FD_SET (newConn, &active_fd_set);
              }
            else
              {
                /* Data arriving on an already-connected socket. */
                
                if (read_from_client (i) < 0)
                  {
                    close (i);
                    FD_CLR (i, &active_fd_set);
                  }
              }
          }
    }


// std::cout << "Hello from socketComm" << std::endl;
}

