#include "Communication.h"

namespace Communication
{
    void read(int description, char *buffer)
    {
        ssize_t bytes_read = 0;
        memset(buffer, 0, BUFSIZE);

        buffer[bytes_read] = 0; // Null-terminate the buffer
        bytes_read = recv(description, buffer, BUFSIZE, 0);
        std::cout << std::to_string(bytes_read) << std::endl;
        if (bytes_read == -1) {
            throw SocketException("Socket recv failed");
        }

        std::cout << buffer << std::endl;
    }

    void write(int description, std::string data)
    {
        std::cout << "Send message to server :" << data << std::endl;
        int n = send(description, data.c_str(), data.length(), 0);
        if (n < 0) {
            throw SocketException("ERROR writing to socket");
        }
    }

    Client::Client(const std::string& server_address, int port)
    {
        this->description = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in serv_addr;

        if (this->description < 0) {
            throw SocketException("Erreur initialisation socket.");
        }

        // Structure contenant les informations "reseaux" de la socket
        serv_addr.sin_port = htons(port); // on precise le port d'ecoute (htons permet de convertir le int du c++ en int reseaux)
        serv_addr.sin_family = AF_INET; // on precise qu'il s'agit d'un socket reseau et non inter-processus (AF_UNIX)
        if(inet_pton(AF_INET, server_address.c_str(), &serv_addr.sin_addr)<=0)  
        { 
            throw SocketException("Invalide address");
        }

        if (connect(this->description, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        {
            throw SocketException("Cannot connect to server");  
        }

        this->receptionThread = std::thread(receiveData, this->description, &this->receptionChannelMutex, &this->receptionChannel);
        this->transmissionThread = std::thread(sendData, this->description, &this->transmissionChannelMutex, &this->transmissionChannel);
    }

    void Client::stop()
    {
        this->receptionThread.join();
        this->transmissionThread.join();
        close(this->description);
    }

    void receiveData(int description, std::mutex* receptionChannelMutex, std::queue<Message::Duck>* receptionChannel)
    {
        char buffer[BUFSIZE];
        Message::Connection connection;

        // Send connection message to server
        write(description, connection.SerializeToString());
        std::cout << connection.SerializeToString() << std::endl;

        // Process message
        while(true){
            try {     
                /** GESTION DE LA RECEPTION DE MESSAGE **/
                {
                    // Read socket 
                    read(description, buffer);
                    // On bloque la queue pour la communication inter thread
                    std::unique_lock<std::mutex> lock(*receptionChannelMutex, std::defer_lock);
                    if(lock.try_lock()) {
                        // Check message type
                        switch (Message::Base::GetType(std::string(buffer)))
                        {
                            // duck message
                            case Message::MessageType::duck :
                                {
                                    Message::Duck duck;
                                    // std::cout << "Duck received ! " << std::endl;
                                    duck.ParseFromString(std::string(buffer));
                                    receptionChannel->push(duck);
                                    std::cout << duck.DebugString() << std::endl;   
                                }
                                break;

                            default:
                                // Unknow message type
                                std::cout << "package type " << std::to_string(Message::Base::GetType(std::string(buffer))) << std::endl;
                                break;
                        }
                        lock.unlock();
                    }
                    
                }
                
            } catch(SocketException& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }

    void sendData(int description, std::mutex* transmissionChannelMutex, std::queue<Message::Found>* transmissionChannel)
    {
        while(true){
            try {     
                /** GESTION DE L'EMISSION DE MESSAGE **/
                {
                    // On bloque la queue pour la communication inter thread
                    std::unique_lock<std::mutex> lock(*transmissionChannelMutex, std::defer_lock);
                    if (lock.try_lock() && !transmissionChannel->empty()) {
                        Message::Found message = transmissionChannel->front();
                        std::cout << message.DebugString() << std::endl;
                        transmissionChannel->pop();
                        lock.unlock();
                        write(description, message.SerializeToString());
                    }
                }
                
            } catch(SocketException& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
}