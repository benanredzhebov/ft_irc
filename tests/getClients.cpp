#include <iostream>
#include <vector>

class Client {
public:
    Client(int fd) : fd(fd) {}
    int getFd() const { return fd; }

private:
    int fd;
};

class Server {
public:
    Client* getClient(int fd);
    void addClient(Client client) { _clients.push_back(client); }

private:
    std::vector<Client> _clients;
};

Client* Server::getClient(int fd) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i].getFd() == fd)
            return &this->_clients[i];
    }
    return nullptr; // Explicitly handle the case where no client is found
}

int main() {
    Server server;
    server.addClient(Client(1));
    server.addClient(Client(2));

    int fd = 1;
    Client* client = server.getClient(fd);
    if (client) {
        std::cout << "Client with fd " << fd << " found." << std::endl;
    } else {
        std::cout << "Client with fd " << fd << " not found." << std::endl;
    }

    return 0;
}