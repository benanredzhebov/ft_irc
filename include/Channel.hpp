#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "./Client.hpp"
#include "./Server.hpp"

class Client;
class Channel {
	private:
		int									_invit_only;
		int									_topic;
		int									_key;
		int									_limit;
		bool								_topic_restriction;
		std::string							_name;
		std::string							_time_creation;
		std::string							_password;
		std::string							_created_at;
		std::string							_topic_name;
		std::vector<Client>	_clients;
		std::vector<Client>	_admins;
		std::vector<std::pair<char, bool> >	_modes;


	public:
		Channel();
		Channel(const Channel &other);
		~Channel();
		Channel &operator=(const Channel &other);

		/*GETTERS*/
		std::string getName();
		Client *getClientInChannel(std::string name);

};

#endif