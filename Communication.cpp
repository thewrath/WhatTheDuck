#include "Communication.h"

namespace Communication
{
    void read(int description, char *buffer)
    {
        ssize_t bytes_read = 0;
        memset(buffer, 0, BUFSIZE);

        buffer[bytes_read] = 0; // Null-terminate the buffer
        bytes_read = recv(description, buffer, BUFSIZE, 0);
        if (bytes_read == -1) {
            throw SocketException("Socket recv failed");
        }
    }

    void write(int description, std::string data)
    {
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
            throw SocketException("Invalid address");
        }

        if (connect(this->description, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            throw SocketException("Cannot connect to server");
        }

        this->receptionThread = std::thread(receiveData, this->description, &this->receptionChannelMutex, &this->receptionChannel);
        this->transmissionThread = std::thread(sendData, this->description, &this->transmissionChannelMutex, &this->transmissionChannel);

        // Tout est prêt, on envoi un message de connexion au serveur
        Message::Connection connection;

        // Send connection message to server
        write(this->description, connection.SerializeToString());
        std::cout << "me: envoi du message de connexion au server : " << connection.DebugString() << std::endl;
    }

    void Client::stop()
    {
        this->receptionThread.join();
        this->transmissionThread.join();
        close(this->description);
    }

    void receiveData(int description, std::mutex* receptionChannelMutex, std::queue<PMessageBase>* receptionChannel)
    {
        char buffer[BUFSIZE];

        // Le thread de reception reste toujours actif
        while(true){
            try {
                /** GESTION DE LA RECEPTION DE MESSAGE **/
                {
                    // Read socket
                    read(description, buffer);
                    // On bloque la queue pour la communication inter thread
                    std::unique_lock<std::mutex> lock(*receptionChannelMutex);
                    // Check message type
                    switch (Message::Base::GetType(std::string(buffer)))
                    {
                        // duck message
                        case Message::MessageType::duck :
                            {
                                auto duck = std::make_shared<Message::Duck>();
                                duck->ParseFromString(std::string(buffer));
                                receptionChannel->push(std::dynamic_pointer_cast<Message::Base>(duck));
                                std::cout << "server: un canard est en " << duck->DebugString() << std::endl;
                            }
                            break;
                        case Message::MessageType::win :
                            {
                                Message::Win win;
                                win.ParseFromString(std::string(buffer));
                                std::cout << "server: le joueur " << std::to_string(win.id) << " a gagné !"<< std::endl;
                            }
                            break;

                        default:
                            // Type de message inconnu
                            std::cout << "server: tentative d'envoi d'un message de type : " << std::to_string(Message::Base::GetType(std::string(buffer))) << std::endl;
                            break;
                    }

                }

            } catch(SocketException& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }

    void sendData(int description, std::mutex* transmissionChannelMutex, std::queue<PMessageBase>* transmissionChannel)
    {
      // Le thread d'envoi reste toujours actif.
        while(true){
            try {
                /** GESTION DE L'EMISSION DE MESSAGE **/
                {
                    // On bloque la queue pour la communication inter thread
                    std::unique_lock<std::mutex> lock(*transmissionChannelMutex, std::defer_lock);
                    if (lock.try_lock() && !transmissionChannel->empty()) {

                        // Récupération du message dans la file d'attente d'envoi et écriture du message sur le socket
                        std::shared_ptr<Message::Base> message = transmissionChannel->front();
                        transmissionChannel->pop();
                        // il faut déterminer le type du message, je n'arrive pas a bien faire fonctionner le polymorphiseme de C++ ...
                        // si l'on ne fait pas ça il appel la méthode Message::Base et non de Message::Found, et le message est mal serialisé
                        switch (message->type)
                        {
                            // found message
                            case Message::MessageType::found :
                                {
                                  auto found = std::dynamic_pointer_cast<Message::Found>(message);
                                  write(description, found->SerializeToString());
                                  std::cout << "me: envoi d'un message de canard trouvé au serveur " <<found->DebugString() << std::endl;
                                }
                                break;
                            
                            //position message
                            case Message::MessageType::position :
                                {
                                  auto position = std::dynamic_pointer_cast<Message::Position>(message);
                                  write(description, position->SerializeToString());
                                  std::cout << "me: envoie de ma position " <<position->DebugString() << std::endl;
                                }
                                break;
                            
                            default:
                                // Type de message inconnu
                                std::cout << "me: tentative d'envoi d'un message de type : " << std::to_string(message->type) << std::endl;
                                break;
                        }
                    }
                }

            } catch(SocketException& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
}
