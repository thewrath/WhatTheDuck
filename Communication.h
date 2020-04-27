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

    typedef std::shared_ptr<Message::Base> PMessageBase;

    class Client
    {
        private:
            int description;

            std::thread transmissionThread;
            std::thread receptionThread;

        public:
            std::mutex receptionChannelMutex;
            std::queue<PMessageBase> receptionChannel;

            std::mutex transmissionChannelMutex;
            std::queue<PMessageBase> transmissionChannel;

            Client(const std::string&, int);
            void stop();
    };

    void receiveData(int, std::mutex*, std::queue<PMessageBase>*);
    void sendData(int, std::mutex*, std::queue<PMessageBase>*);
};

#endif
