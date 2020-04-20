#include "Message.h"

namespace Message 
{
    std::string Base::SerializeToString()
    {
        return std::to_string(this->type)+";";
    }

    std::string Base::DebugString()
    {
        return this->SerializeToString();
    }

    void Base::ParseFromString(std::string data)
    {

    }

    MessageType Base::GetType(std::string data)
    {
        std::string data_type = "";
        for(char& c : data) {
            if(c == ':' || c == ';'){
                break;    
            }
            data_type += c;
        }
        return (MessageType) atoi(data_type.c_str());
    }

    Connection::Connection()
    {
        this->type = MessageType::connection;
    }

    Deconnection::Deconnection()
    {
        this->type = MessageType::deconnection;
    }

    Duck::Duck()
    {
        this->type = MessageType::duck;
    }

    void Duck::set_sound(std::string sound){this->sound = sound;}
    void Duck::set_x(float x){this->x = x;}
    void Duck::set_y(float y){this->y = y;}
    void Duck::set_z(float z){this->z = z;}
    void Duck::set_ax(float ax){this->ax = ax;}
    void Duck::set_ay(float ay){this->ay = ay;}
    void Duck::set_az(float az){this->az = az;}
}