#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <iostream>
#include <string.h>
#include <thread>
#include <vector>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

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

    void communicate(std::string, int);

    class Client
    {
        std::thread thread;

        public:
            void start(std::string, int);
            void stop();
    };
};

#endif