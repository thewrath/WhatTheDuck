#include "Communication.h"

namespace Communication
{
    void read(int description, char *buffer)
    {
        ssize_t bytes_read = 0;
        memset(buffer, 0, BUFSIZE);

        // buffer[bytes_read] = 0; // Null-terminate the buffer
        bytes_read = recv(description, buffer, BUFSIZE, 0);

        if (bytes_read == -1) {
            throw SocketException("Socket recv failed");
        }
    }

    void write(int description, std::string data)
    {
        std::cout << "Send message to server :" << data << std::endl;
        int n = send(description, data.c_str(), data.length(), 0);
        if (n < 0) {
            throw SocketException("ERROR writing to socket");
        }
    }

    Client::Client(std::string address, int port)
    {
        this->thread = std::thread(communicate, address, port, &this->duckCreationRequestsCondition, &this->duckCreationRequestsMutex, &this->duckCreationRequests); 
    }

    void Client::stop()
    {
        this->thread.join();
    }

    void communicate(std::string server_address, int port, std::condition_variable* duckCreationRequestsCondition, std::mutex* duckCreationRequestsMutex, std::queue<Message::Duck>* duckCreationRequests)
    {
        char buffer[BUFSIZE];

        int description = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in serv_addr;

        if (description < 0) {
            throw SocketException("Erreur initialisation socket.");
        }

        // Structure contenant les informations "reseaux" de la socket
        serv_addr.sin_port = htons(port); // on precise le port d'ecoute (htons permet de convertir le int du c++ en int reseaux)
        serv_addr.sin_family = AF_INET; // on precise qu'il s'agit d'un socket reseau et non inter-processus (AF_UNIX)
        if(inet_pton(AF_INET, server_address.c_str(), &serv_addr.sin_addr)<=0)  
        { 
            throw SocketException("Invalide address");
        }

        if (connect(description, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        {
            throw SocketException("Cannot connect to server");  
        }

        Message::Connection connection;

        // Send connection message to server
        write(description, connection.SerializeToString());
        std::cout << connection.SerializeToString() << std::endl;

        // Process message
        while(true){
            try {

                // Read socket 
                read(description, buffer);

                // On bloque la queue pour la communication inter thread
                std::unique_lock<std::mutex> lock(*duckCreationRequestsMutex);
                // Check message type
                switch (Message::Base::GetType(std::string(buffer)))
                {
                    // duck message
                    case Message::MessageType::duck :
                        {
                            Message::Duck duck;
                            // std::cout << "Duck received ! " << std::endl;
                            duck.ParseFromString(std::string(buffer));
                            duckCreationRequests->push(duck);
                            std::cout << duck.DebugString() << std::endl;   
                        }
                        break;

                    default:
                        // Unknow message type
                        std::cout << "package type " << std::to_string(Message::Base::GetType(std::string(buffer))) << std::endl;
                        break;
                }
                // duckCreationRequestsCondition->notify_one();
                
            } catch(SocketException& e) {
                std::cout << e.what() << std::endl;
            }
            
        }

        close(description);
    }
}