#include "Message.h"

namespace Message 
{

    std::vector<std::string> extractArguments(std::string data)
    {
        std::vector<std::string> arguments;
        std::string argument;
        for(char& c : data) {
            if(c == ':' || c == ';'){
                arguments.push_back(argument);
                argument = "";
            } else {
                argument += c;
            }
        }

        return arguments;
    }

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
        return (MessageType) std::stoi(data_type);
    }

    Connection::Connection()
    {
        this->type = MessageType::connection;
    }

    Deconnection::Deconnection()
    {
        this->type = MessageType::deconnection;
    }

    Found::Found(int id)
    {
        this->type = MessageType::found;
        this->id = id;
    }

    std::string Found::SerializeToString()
    {
        return std::to_string(this->type)
        +":"+std::to_string(this->id)
        +";";
    }

    void Found::ParseFromString(std::string data)
    {
        // Call this method only if the message type is validate before
        std::vector<std::string> arguments = extractArguments(data);
        if(arguments.size() < 2) {
            throw MessageException("Not enought argument to parse duck message");
        }

        try {
            this->id = std::stoi(arguments[1]);
        } catch (std::exception const & e) {
            throw MessageException("One argument type doesn't match");
        }
        
    }

    Duck::Duck()
    {
        this->type = MessageType::duck;
    }

    std::string Duck::SerializeToString()
    {
        return std::to_string(this->type)
        +":"+this->sound
        +":"+std::to_string(this->id)
        +":"+std::to_string(this->x)
        +":"+std::to_string(this->y)
        +":"+std::to_string(this->z)
        +":"+std::to_string(this->ax)
        +":"+std::to_string(this->ay)
        +":"+std::to_string(this->az)
        +";";
    }

    void Duck::ParseFromString(std::string data)
    {
        // Call this method only if the message type is validate before
        std::vector<std::string> arguments = extractArguments(data);
        if(arguments.size() < 9) {
            throw MessageException("Not enought argument to parse duck message");
        }

        try {
            this->sound = arguments[1];
            this->id = std::stoi(arguments[2]);
            this->x = std::stof(arguments[3]);
            this->y = std::stof(arguments[4]);
            this->z = std::stof(arguments[5]);
            this->ax = std::stof(arguments[6]);
            this->ay = std::stof(arguments[7]);
            this->az = std::stof(arguments[8]);
        } catch (std::exception const & e) {
            throw MessageException("One argument type doesn't match");
        }
        
    }

    void Duck::set_sound(std::string sound){this->sound = sound;}
    void Duck::set_id(int id){this->id = id;}
    void Duck::set_x(float x){this->x = x;}
    void Duck::set_y(float y){this->y = y;}
    void Duck::set_z(float z){this->z = z;}
    void Duck::set_ax(float ax){this->ax = ax;}
    void Duck::set_ay(float ay){this->ay = ay;}
    void Duck::set_az(float az){this->az = az;}

    Win::Win()
    {
        this->type = MessageType::win;
    }
}