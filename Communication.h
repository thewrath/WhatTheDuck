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
    const int BUFSIZE = 15;

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
            std::thread thread;

        public:
            std::mutex duckCreationRequestsMutex;
            std::condition_variable duckCreationRequestsCondition;
            std::queue<Message::Duck> duckCreationRequests;

            Client(std::string, int);
            void stop();
    };

    void communicate(std::string, int, std::condition_variable*, std::mutex*, std::queue<Message::Duck>*);
};

#endif