#include <iostream>
#include <vector>
#include <string>

class Channel {
public:
    Channel(std::string name) : name(name) {}
    std::string getName() const { return name; }

private:
    std::string name;
};

class Server {
public:
    Channel* getChannel(std::string name);
    void addChannel(Channel channel) { _channels.push_back(channel); }

private:
    std::vector<Channel> _channels;
};

Channel* Server::getChannel(std::string name) {
    for (size_t i = 0; i < this->_channels.size(); i++) {
        if (this->_channels[i].getName() == name)
            return &_channels[i];
    }
    return NULL;
}

int main() {
    Server server;
    server.addChannel(Channel("general"));
    server.addChannel(Channel("random"));

    std::string channelName1 = "general";
    std::string channelName2 = "music";

    Channel* channel1 = server.getChannel(channelName1);
    if (channel1) {
        std::cout << "Channel: " << channelName1 << " found." << std::endl;
    } else {
        std::cout << "Channel: " << channelName1 << " not found." << std::endl;
    }

    Channel* channel2 = server.getChannel(channelName2);
    if (channel2) {
        std::cout << "Channel: " << channelName2 << " found." << std::endl;
    } else {
        std::cout << "Channel: " << channelName2 << " not found." << std::endl;
    }

    return 0;
}