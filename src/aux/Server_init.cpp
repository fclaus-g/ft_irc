
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
	std::cout << "Server created" << std::endl;
}

Server::~Server()
{
	std::cout << "Server destroyed" << std::endl;
}

std::ostream& operator<<(std::ostream& out, const Server& server)
{
	out << GRE << "Server: " << server.getName()<< RES << std::endl;
	out << "Port :" << server.getPort() << std::endl;
	if(server.getIsRunning())
	{
		out << "Server is running" << std::endl;
	}
	else
	{
		out << "Server is not running" << std::endl;
	}
	return out;
}

/*-----------------------[SETTERS]------------------------*/

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
