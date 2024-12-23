#include <iostream>
#include <vector>
#include <string>

class Channel {
public:
    Channel();
    std::string getModes();

private:
    std::vector<std::pair<char, bool>> _modes; // List of modes and their statuses
};

Channel::Channel() {
    _modes = {{'i', true}, {'t', false}, {'k', true}, {'o', false}, {'l', true}};
}

std::string Channel::getModes() {
    std::string mode;
    for (size_t i = 0; i < _modes.size(); i++) {
        if (_modes[i].first != 'o' && _modes[i].second) {
            mode.push_back(_modes[i].first);
            std::cout << "First mode: " << _modes[i].first << std::endl;
            std::cout << "Second mode: " << _modes[i].second << std::endl;
        }
            
    }
    if (!mode.empty())
        mode.insert(mode.begin(), '+');
    return mode;
}

int main() {
    Channel channel;
    std::string modes = channel.getModes();
    std::cout << "Active modes: " << modes << std::endl;
    return 0;
}