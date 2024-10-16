
#pragma once

#include "ircserv.h"

#define MAX_CONNECT_NBR	128

class User;

class Server
{
	private:
		int			_socket;
		int			_port;
		std::string	_pass;
		std::string	_serverName;
		bool		_isRunning;
		//std::vector<Channels *>	_channels;
		std::map<int, User>			_users;
		std::vector<struct pollfd>	_fds;
		Server();
		Server(Server const &s);
		Server &operator=(Server const &s);

	public:
		Server(int port, std::string pass);
		~Server();

		int getPort() const;
		std::string getName() const;
		bool getIsRunning() const;
		//std::map<int, User *> &getUsers() const;

		void startIRCServer();
		void acceptUser();
		void readUser(int userFd);
		void addUser(int userFd, struct sockaddr_in userAddr);
};
std::ostream& operator<<(std::ostream& out, const Server& server);
