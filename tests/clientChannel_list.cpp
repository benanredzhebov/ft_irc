#include <iostream>
#include <vector>
#include <string>

class Client {
public:
    Client(std::string nickname) : nickname(nickname) {}
    std::string getNickName() const { return nickname; }

private:
    std::string nickname;
};

class Channel {
public:
    std::string clientChannel_list();
    void addClient(Client client) { _clients.push_back(client); }
    void addAdmin(Client admin) { _admins.push_back(admin); }

private:
    std::vector<Client> _clients; // List of clients in the channel
    std::vector<Client> _admins; // List of admins in the channel
};

std::string Channel::clientChannel_list() {
    std::string list;
    for (size_t i = 0; i < _admins.size(); i++) {
        list += "@" + _admins[i].getNickName();
        if ((i + 1) < _admins.size())
            list += " ";
    }
    if (_clients.size())
        list += " ";
    for (size_t i = 0; i < _clients.size(); i++) {
        list += _clients[i].getNickName();
        if ((i + 1) < _clients.size())
            list += " ";
    }
    return list;
}

int main() {
    Channel channel;
    channel.addAdmin(Client("admin1"));
    channel.addClient(Client("user1"));
    channel.addClient(Client("user2"));

    std::string clientList = channel.clientChannel_list();
    std::cout << "Client list: " << clientList << std::endl;

    return 0;
}