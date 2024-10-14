
#include "ft_irc.hpp"

Server::Server()
{
	this->server_fd = -1;
}

Server::Server(int port, std::string password)
{
	this->port = port;
	this->password = password;
	this->name = "MyServer";
	this->isRunning = false;
}

Server::~Server()
{
	std::cout << "Server destroyed" << std::endl;
}

/*-----------------------[GETTERS]------------------------*/
int Server::getPort() const
{
	return this->port;
}
std::string Server::getName() const
{
	return this->name;
}
bool Server::getIsRunning() const
{
	return this->isRunning;
}
