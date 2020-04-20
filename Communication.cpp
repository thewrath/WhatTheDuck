#include "Communication.h"

namespace Communication
{
    void read(int description, char *buffer)
    {
        ssize_t bytes_read = 0;
        memset(buffer, 0, BUFSIZE);
        //testing stuff, BUFSIZE = 4096
        printf("Received Socket # %d\nBuffer Size = %d\n", description, BUFSIZE);

        // buffer[bytes_read] = 0; // Null-terminate the buffer
        bytes_read = recv(description, buffer, BUFSIZE, 0);

        if (bytes_read == -1) {
            throw SocketException("Socket recv failed");
        }

        //Turn into a sting
        // std::string dataStr(buffer);

        std::cout << buffer << std::endl;

        // std::cout << "Fin de reception" << std::endl;
    }

    void write(int description, std::string data)
    {
        std::cout << "Send message to server :" << data << std::endl;
        int n = send(description, data.c_str(), data.length(), 0);
        if (n < 0) {
            throw SocketException("ERROR writing to socket");
        }
    }

    void Client::start(std::string address, int port)
    {
        this->thread = std::thread(communicate, address, port);
    }

    void Client::stop()
    {
        this->thread.join();
    }

    void communicate(std::string server_address, int port)
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
        write(description, connection.SerializeToString());
        std::cout << connection.SerializeToString() << std::endl;
        while(true){

            try {
                read(description, buffer); 
                Message::Duck duck;
                duck.ParseFromString(std::string(buffer));
                
                std::cout << duck.DebugString();   
                
            } catch(SocketException& e) {
                std::cout << e.what() << std::endl;
            }
        }

        close(description);
    }
}