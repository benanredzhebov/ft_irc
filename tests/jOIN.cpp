#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>

class Client {
public:
    std::string getNickName() const { return "user"; }
    int getFd() const { return 1; }
};

class Server {
public:
    int splitJoin(std::vector<std::pair<std::string, std::string>> &token, std::string cmd, int fd);
    void senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg);
    Client* getClient(int fd) { return &client; }

private:
    Client client;
};

int	Server::splitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd) {
	std::vector<std::string>	tmp;
	std::string					chStr, passStr, buff;
	std::istringstream			iss(cmd);
	
	//Split command into tokens and store in the tmp vector
	while(iss >> cmd)
		tmp.push_back(cmd);
    std::cout << "Command: " << cmd << std::endl;
    std::cout << "Size of tmp: " << tmp.size() << std::endl;
	if (tmp.size() < 2) {token.clear(); return 0;}
	//extract channel string and password string from the tmp vector
    std::cout << "TEMP0: " << tmp[0] << std::endl;
    std::cout << "TEMP1: " << tmp[1] << std::endl;
    std::cout << "TEMP2: " << tmp[2] << std::endl;
	tmp.erase(tmp.begin());
	chStr = tmp[0];
    std::cout << "Channel string: " << chStr << std::endl;
	tmp.erase(tmp.begin());
    std::cout << "TEMP0: " << tmp[0] << std::endl;
    std::cout << "TEMP1: " << tmp[1] << std::endl;
    std::cout << "TEMP2: " << tmp[2] << std::endl;
	if (!tmp.empty()) {
		passStr = tmp[0];
        std::cout << "passStr1: " << passStr << std::endl;
		tmp.clear();
	}
	
	//Splits the channel string by commas ans stores each channel name in the token vector as a pair with an empty password
	std::cout << "Channel string :" << chStr << std::endl;
    for (size_t i = 0; i < chStr.size(); i++){
		if (chStr[i] == ',')
				{token.push_back(std::make_pair(buff, "")); buff.clear();}
		else buff += chStr[i];
        std::cout << "Buff :" << buff << std::endl;
	}
	token.push_back(std::make_pair(buff, ""));

	//Splits the password string by commas and assings each password to the corresponding channel in the token vector
	if (!passStr.empty()){
		size_t j = 0; buff.clear();
		for (size_t i = 0; i < passStr.size(); i++){
			if (passStr[i] == ',')
				{token[j].second = buff; j++; buff.clear();}
			else buff += passStr[i];
		}
		token[j].second = buff;
	}
	for (size_t i = 0; i < token.size(); i++) // erase the empty channel names
		{if (token[i].first.empty())token.erase(token.begin() + i--);}
	for (size_t i = 0; i < token.size(); i++) { //ERR_NOSUCHCHANNEL (403) // if the channel doesn't exist
		if (*(token[i].first.begin()) != '#')
			{senderror(403, getClient(fd)->getNickName(), token[i].first, getClient(fd)->getFd(), " :No such channel\r\n"); token.erase(token.begin() + i--);}
		else
			token[i].first.erase(token[i].first.begin());
	}
	return 1;
}

void Server::senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg) {
    std::cerr << "Error " << code << " for client " << clientname << " on channel " << channelname << ": " << msg << std::endl;
}

int main() {
    Server server;
    std::vector<std::pair<std::string, std::string>> token;
    std::string cmd = "JOIN #channel1,#channel2 key1,key2";
    int fd = 1;

    if (server.splitJoin(token, cmd, fd)) {
        std::cout << "Parsed JOIN command successfully:" << std::endl;
        for (const auto &pair : token) {
            std::cout << "Channel: " << pair.first << ", Password: " << pair.second << std::endl;
        }
    } else {
        std::cout << "Failed to parse JOIN command." << std::endl;
    }

    return 0;
}