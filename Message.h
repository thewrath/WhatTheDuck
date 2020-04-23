/**
 * \file Message.h
 * \brief Package contenant les différents messages
 * \author thewrath
 * \version 1.0
 * \date 20/04/2020
 *
 * Package contenant les différents messages et les méthodes de sérializations.
 *
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>

namespace Message 
{

    struct MessageException : public std::exception
    {
        std::string message;
        MessageException(std::string m) : message(m) {}
        ~MessageException() throw () {}
        const char* what() const throw() { return message.c_str(); }
    };

    std::vector<std::string> extractArguments(std::string);

    enum MessageType {
        base = 0,
        connection = 1, 
        deconnection = 2,
        found = 3,
        duck = 4,
        win = 5
    };

    class Base 
    {
        protected:
            MessageType type = MessageType::base;
            

        public: 
            std::string SerializeToString();
            std::string DebugString();
            void ParseFromString(std::string);

            static MessageType GetType(std::string);
    };

    class Connection : Base
    {
        public:
            Connection();
            std::string SerializeToString() { return Base::SerializeToString(); };
            std::string DebugString() { return Base::DebugString(); };
            void ParseFromString(std::string data) { return Base::ParseFromString(data); };
    };

    class Deconnection : Base
    {
        public:
            Deconnection();
            std::string SerializeToString() { return Base::SerializeToString(); };
            std::string DebugString() { return Base::DebugString(); };
            void ParseFromString(std::string data) { return Base::ParseFromString(data); };
    };

    class Found : Base
    {
        public:
            int id = 0;

            Found();
            Found(int);
            std::string SerializeToString();
            std::string DebugString() { return this->SerializeToString(); };
            void ParseFromString(std::string data);
    };

    class Duck : Base
    {
        public:
            std::string sound = "default.wav";
            int id = 0;
            float x = 0;
            float y = 0;
            float z = 0;
            float ax = 0;
            float ay = 0;
            float az = 0;

            Duck();
            std::string SerializeToString();
            std::string DebugString() { return this->SerializeToString(); };
            void ParseFromString(std::string data);

            void set_sound(std::string);
            void set_id(int);
            void set_x(float);
            void set_y(float);
            void set_z(float);
            void set_ax(float);
            void set_ay(float);
            void set_az(float);
    };

    class Win : Base
    {
        public:
            int id = 0;

            Win();
            Win(int);
            std::string SerializeToString();
            std::string DebugString() { return Base::DebugString(); };
            void ParseFromString(std::string data);
    };
};
#endif