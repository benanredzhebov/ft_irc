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
    bool change_clientToAdmin(std::string &nick);
    void addClient(Client client) { _clients.push_back(client); }
    void addAdmin(Client admin) { _admins.push_back(admin); }
    void printClientsAndAdmins();

private:
    std::vector<Client> _clients; // List of clients in the channel
    std::vector<Client> _admins; // List of admins in the channel
};

bool Channel::change_clientToAdmin(std::string &nick) {
    size_t i = 0;
    for (; i < _clients.size(); i++) {
        if (_clients[i].getNickName() == nick)
            break;
    }
    if (i < _clients.size()) {
        std::cout << "Client size: " << _clients.size() << std::endl;
        std::cout << "i= " << i << std::endl;
        _admins.push_back(_clients[i]);
        std::cout << "Client nickname: " << _clients[i].getNickName() << std::endl;
        _clients.erase(i + _clients.begin());
        return true;
    }
    return false;
}

void Channel::printClientsAndAdmins() {
    std::cout << "Clients: ";
    for (const auto &client : _clients) {
        std::cout << client.getNickName() << " ";
    }
    std::cout << "\nAdmins: ";
    for (const auto &admin : _admins) {
        std::cout << admin.getNickName() << " ";
    }
    std::cout << std::endl;
}

int main() {
    Channel channel;
    channel.addClient(Client("user1"));
    channel.addClient(Client("user2"));
    channel.addClient(Client("user3"));

    std::cout << "Before promotion:" << std::endl;
    channel.printClientsAndAdmins();

    std::string nickname = "user3";
    if (channel.change_clientToAdmin(nickname)) {
        std::cout << nickname << " has been promoted to admin." << std::endl;
    } else {
        std::cout << nickname << " not found in clients." << std::endl;
    }

    std::cout << "After promotion:" << std::endl;
    channel.printClientsAndAdmins();

    return 0;
}