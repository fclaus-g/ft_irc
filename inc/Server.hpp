#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.hpp"

class Channel;
class User;

class Server
{
	private:
		int								_port;
		std::string						_password;
		std::string						_name;
		bool							_isRunning;
		int								_serverFd;
		std::string						_message;
		std::vector<Channel *>			_channels;
		std::map<std::string, Channel*>	_channelsMap;
		std::map<int, User *>			_users;
		std::vector<struct pollfd> 		_fds;
		std::string						_tempNick;
		std::string						_tempPass;
	public:
		Server();
		Server(int port, std::string password);
		~Server();
		
		//Get-set
		int 							getPort() const;
		std::string 					getName() const;
		bool 							getIsRunning() const;
		User							*getUserByNick(std::string nick);
		User							*getUserByFd(int fd);
		const std::string				getPassword() const;
		std::vector<Channel*>			&getChannels();
    	std::map<std::string, Channel*>	&getChannelsMap();
		Channel							*getChannelByName(std::string name);
		//Server engine methods
		static void						signalHandler(int signal);
		void 							prepareSocket();
		void 							start();
		void 							run();
		void 							stop();
		void 							newConnection();
		void							deleteUser(int socketFd);
		//Message-user handle methods
		void							welcomeUser(int userFd);
		void							sendWarning(int userFd, std::string str);
		void							msgHandler(int socketFd);
		//Debug methods
		//void 							printMap(const std::map<int, User>& map) const;
		void 							printChannelMap(const std::map<std::string, Channel*>& map) const;	
		void					printVector(const std::vector<Channel>& vector);
		//Channel methods
		void							createChannel(const std::string& name);
		void							addUserToChannel(const std::string& channelName, User& user);
		void							removeChannel(const std::string& name);
		bool 							channelExists(const std::string& name);
};

std::ostream& operator<<(std::ostream& out, const Server& server);

#endif
