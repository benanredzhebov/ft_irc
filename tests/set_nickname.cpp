#include <iostream>
#include <vector>
#include <string>

class Client {
public:
    Client(int fd, std::string nickname = "") : fd(fd), nickname(nickname), registered(false), loggedIn(false) {}
    std::string getNickName() const { return nickname; }
    void setNickName(const std::string &nick) { nickname = nick; }
    bool getRegistered() const { return registered; }
    void setRegistered(bool reg) { registered = reg; }
    bool getLogedIn() const { return loggedIn; }
    void setLogedIn(bool log) { loggedIn = log; }
    std::string getUserName() const { return username; }
    void setUserName(const std::string &user) { username = user; }

private:
    int fd;
    std::string nickname;
    std::string username;
    bool registered;
    bool loggedIn;
};

class Channel {
public:
    Client* getClientInChannel(const std::string &name) {
        for (auto &client : clients) {
            if (client.getNickName() == name)
                return &client;
        }
        return nullptr;
    }
    void addClient(Client client) { clients.push_back(client); }

private:
    std::vector<Client> clients;
};

class Server {
public:
    void set_nickname(std::string cmd, int fd);
    void sendResponse(const std::string &response, int fd) {
        std::cout << "Response to fd " << fd << ": " << response << std::endl;
    }
    Client* getClient(int fd) {
        for (auto &client : clients) {
            if (client.getNickName() == std::to_string(fd)) // Simplified for example
                return &client;
        }
        return nullptr;
    }
    bool nickNameInUse(const std::string &nickname) {
        for (const auto &client : clients) {
            if (client.getNickName() == nickname)
                return true;
        }
        return false;
    }
    bool is_validNickname(const std::string &nickname) {
        if (!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
            return false;
        for (size_t i = 1; i < nickname.size(); i++) {
            if (!std::isalnum(nickname[i]) && nickname[i] != '_')
                return false;
        }
        return true;
    }
    void addClient(Client client) { clients.push_back(client); }
    void addChannel(Channel channel) { channels.push_back(channel); }

private:
    std::vector<Client> clients;
    std::vector<Channel> channels;
};

void Server::set_nickname(std::string cmd, int fd) {
    std::string inuse;
    cmd = cmd.substr(4);
    size_t pos = cmd.find_last_not_of("\t\v ");
    if (pos < cmd.size()) {
        cmd = cmd.substr(pos);
        if (cmd[0] == ':')
            cmd.erase(cmd.begin());
    }
    Client *cli = getClient(fd);
    if (pos == std::string::npos || cmd.empty()) {
        sendResponse("ERR_NOTENOUGHPARAM *", fd);
        return;
    }
    if (nickNameInUse(cmd) && cli->getNickName() != cmd) {
        inuse = "*";
        if (cli->getNickName().empty())
            cli->setNickName(inuse);
        sendResponse("ERR_NICKINUSE " + cmd, fd);
        return;
    }
    if (!is_validNickname(cmd)) {
        sendResponse("ERR_ERRONEUSNICK " + cmd, fd);
        return;
    } else {
        if (cli && cli->getRegistered()) {
            std::string oldnick = cli->getNickName();
            cli->setNickName(cmd);
            for (size_t i = 0; i < channels.size(); i++) {
                Client *cl = channels[i].getClientInChannel(oldnick);
                if (cl)
                    cl->setNickName(cmd);
            }
            if (!oldnick.empty() && oldnick != cmd) {
                if (oldnick == "*" && !cli->getUserName().empty()) {
                    cli->setLogedIn(true);
                    sendResponse("RPL_CONNECTED " + cli->getNickName(), fd);
                    sendResponse("RPL_NICKCHANGE " + cli->getNickName() + " " + cmd, fd);
                } else {
                    sendResponse("RPL_NICKCHANGE " + oldnick + " " + cmd, fd);
                }
                return;
            }
        } else if (cli && !cli->getRegistered()) {
            sendResponse("ERR_NOTREGISTERED " + cmd, fd);
        }
    }
    if (cli && cli->getRegistered() && !cli->getUserName().empty() && !cli->getNickName().empty() && cli->getNickName() != "*" && !cli->getLogedIn()) {
        cli->setLogedIn(true);
        sendResponse("RPL_CONNECTED " + cli->getNickName(), fd);
    }
}

int main() {
    Server server;
    server.addClient(Client(1, "user1"));
    server.addClient(Client(2, "user2"));

    Channel channel;
    channel.addClient(Client(1, "user1"));
    channel.addClient(Client(2, "user2"));
    server.addChannel(channel);

    server.set_nickname("NICK newuser1", 1);
    server.set_nickname("NICK newuser2", 2);

    return 0;
}