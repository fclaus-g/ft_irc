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
}

Server::~Server()
{
}

int Server::getPort() const
{
	return this->_port;
}
std::string Server::getName() const
{
	return this->_name;
}
bool Server::getIsRunning() const
{
	return this->_isRunning;
}

/**
 * @brief Getter to the server password, for future pass check
 */
const std::string Server::getPassword() const
{
	return this->_password;
}
