#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "./Client.hpp"
#include "./Server.hpp"

class Client;
class Channel {
	private:
		int									_invit_only; // Invitation-only flag
		int									_topic; // Topic of the channel
		int									_key; // Key for the channel (password)
		int									_limit; // Limit on the number of clients
		bool								_topic_restriction; // Topic restriction flag
		std::string							_name; // Name of the channel
		std::string							_time_creation; // Time of channel creation
		std::string							_password; // Password for the channel
		std::string							_created_at; // Creation timestamp
		std::string							_topic_name; // Name of the topic

		std::vector<Client>					_clients; // List of clients in the channel

		
		std::vector<Client>					_admins; // List of admins in the channel
		std::vector<std::pair<char, bool> >	_modes; // List of modes and their statuses


	public:
		Channel();
		Channel(const Channel &other);
		~Channel();
		Channel &operator=(const Channel &other);

		/*SETTERS*/
		void		setInvitOnly(int invit_only); // Set invitation-only flag
		void		setTopic(int topic); // Set topic
		void		setKey(int key); // Set key (password)
		void		setLimit(int limit); // Set client limit
		void		setTopicName(std::string topic_name); // Set topic name
		void		setPassword(std::string password); // Set password
		void		setName(std::string name); // Set channel name
		void		setTime(std::string time); // Set time
		void		set_topicRestriction(bool value); // Set topic restriction flag
		void		setModeAtindex(size_t index, bool mode); // Set mode at index
		void		set_createiontime(); // Set creation time

		
		
		int			checkClientExistence(int fd);
		/*GETTERS*/
		int			getInvitOnly(); // Get invitation-only flag
		int			getTopic(); // Get topic
		int			getKey(); // Get key (password)
		int			getLimit(); // Get client limit
		int			getClientsNumber(); // Get number of clients
		bool		gettopic_restriction() const; // Get topic restriction flag
		bool		getModeAtindex(size_t index); // Get mode at index
		bool		clientInChannel(std::string &nick); // Check if client is in channel
		std::string	getTopicName(); // Get topic name
		std::string	GetPassword(); // Get password
		std::string	getName(); // Get channel name
		std::string	getTime(); // Get time
		std::string	get_creationtime(); // Get creation time
		std::string	getModes(); // Get modes
		std::string	clientChannel_list(); // Get client channel list
		Client		*get_client(int fd); // Get client by file descriptor
		Client		*get_admin(int fd); // Get admin by file descriptor
		Client		*getClientInChannel(std::string name); // Get client in channel by name

		/*METHODS*/
		void		add_client(Client newClient); // Add client to channel
		void		add_admin(Client newClient); // Add admin to channel
		void		remove_client(int fd); // Remove client by file descriptor
		void		remove_admin(int fd);  // Remove admin by file descriptor
		bool		change_clientToAdmin(std::string& nick); // Change client to admin
		bool		change_adminToClient(std::string& nick); // Change admin to client

		/*SEND TO ALL*/
		void		sendTo_all(std::string rpl1); // Send message to all clients
		void		sendTo_all(std::string rpl1, int fd); // Send message to all clients except one
};

#endif