
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
	this->server_fd = -1;
	_initCommands();
}

Server::~Server()
{
	std::cout << "Server destroyed" << std::endl;
}

void	Server::_initCommands()
{
	this->_commands[0] = "USER";
    this->_commands[1] = "NICK";
    this->_commands[2] = "JOIN";
    this->_commands[3] = "QUIT";
    this->_commands[4] = "PRIVMSG";
    this->_commands[5] = "KICK";
    this->_commands[6] = "INVITE";
    this->_commands[7] = "TOPIC";
    this->_commands[8] = "MODE";
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
