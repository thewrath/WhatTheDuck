#ifndef MESSAGE_H
#define MESSAGE_H

#include <fstream>
#include <iostream>
#include <string.h>

namespace Message 
{

    enum MessageType {
        base = 0,
        connection = 1, 
        deconnection = 2, 
        duck = 3 
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

    class Duck : Base
    {
        private:
            std::string sound;
            float x;
            float y;
            float z;
            float ax;
            float ay;
            float az;
        public:
            Duck();
            std::string SerializeToString() { return Base::SerializeToString(); };
            std::string DebugString() { return Base::DebugString(); };
            void ParseFromString(std::string data) { return Base::ParseFromString(data); };

            void set_sound(std::string);
            void set_x(float);
            void set_y(float);
            void set_z(float);
            void set_ax(float);
            void set_ay(float);
            void set_az(float);
    };
};
#endif