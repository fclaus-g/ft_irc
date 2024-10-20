#include "ft_irc.hpp"

Server::Server()
{
}

Server::Server(int port, std::string password)
{
	this->_port = port;
	this->_password = password;
	this->_name = "MyServer";
	this->_isRunning = false;
	this->_serverFd = -1;
	_initCommands();
}

Server::~Server()
{
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
