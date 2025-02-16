#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "./Client.hpp"
#include "./Server.hpp"

class Client;

class Channel {
	private:
		int									_invit_only;
		int									_key;
		int									_limit;
		bool								_topic_restriction;
		std::string							_topic;
		std::string							_name;
		std::string							_time_creation;
		std::string							_password;
		std::string							_created_at;
		std::string							_topic_name;
		std::vector<Client>					_clients;
		std::vector<Client>					_admins;
		std::vector<std::pair<char, bool> >	_modes;



	public:
		Channel();
		Channel(const Channel &other);
		~Channel();
		Channel &operator=(const Channel &other);

		/*SETTERS*/
		void		setInvitOnly(int invit_only);
		void		setTopic(std::string topic);
		void		setKey(int key);
		void		setLimit(int limit);
		void		setTopicName(std::string topic_name);
		void		setPassword(std::string password);
		void		setName(std::string name);
		void		setTime(std::string time);
		void		set_topicRestriction(bool value);
		void		setModeAtindex(size_t index, bool mode);
		void		set_createiontime();

		
		
		/*GETTERS*/
		int			getAdminSize();
		int			getInvitOnly();
		int			getKey();
		int			getLimit();
		int			getClientsNumber();
		bool		gettopic_restriction() const;
		bool		getModeAtindex(size_t index);
		bool		clientInChannel(const std::string &nick);
		Client		*get_client(int fd);
		Client		*get_admin(int fd);
		Client		*getClientInChannel(std::string name);
		std::string	getTopic();
		std::string	getTopicName();
		std::string	GetPassword();
		std::string	getName();
		std::string	getTime();
		std::string	get_creationtime();
		std::string	getModes();
		std::string	clientChannel_list();

		/*METHODS*/
		void		add_client(Client newClient);
		void		add_admin(Client newClient);
		int			remove_client(int fd);
		int			remove_admin(int fd);
		bool		change_clientssToAdmin(std::string nick);
		bool		change_clientToAdmin(std::string& nick);
		bool		change_adminToClient(std::string& nick);
		void		sendTo_all(std::string rpl1);
		void		sendTo_all(std::string rpl1, int fd);
		int			checkClientExistence(int fd);

};

#endif