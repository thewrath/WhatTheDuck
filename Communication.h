#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <iostream>
#include <string.h>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <utils.h>
#include <gl-matrix.h>

#include "Duck.h"

#include "Message.h"

namespace Communication
{
    const int BUFSIZE = 4096;

    struct SocketException : public std::exception
    {
        std::string message;
        SocketException(std::string m) : message(m) {}
        ~SocketException() throw () {}
        const char* what() const throw() { return message.c_str(); }
    };

    void read(int, char *);
    void write(int, std::string);

    class Client
    {
        private:
            int description;

            std::thread transmissionThread;
            std::thread receptionThread;

        public:
            std::mutex receptionChannelMutex;
            std::queue<Message::Duck> receptionChannel;

            std::mutex transmissionChannelMutex;
            std::queue<Message::Found> transmissionChannel;

            Client(const std::string&, int);
            void stop();
    };

    void receiveData(int, std::mutex*, std::queue<Message::Duck>*);
    void sendData(int, std::mutex*, std::queue<Message::Found>*);
};

#endif