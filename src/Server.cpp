#include "../inc/Server.hpp"

Server::Server(std::string host, std::string ip, int port)
{
	std::cout << "Server object created" << std::endl;
	this->host = host;
	this->ip = ip;
	this->port = port;
	this->isRunning = false;
}

Server::~Server()
{
	std::cout << "Server object destroyed" << std::endl;
}

void Server::setHost(std::string host)
{
	this->host = host;
}

void Server::setIp(std::string ip)
{
	this->ip = ip;
}

void Server::setPort(int port)
{
	this->port = port;
}

std::string Server::getHost() const
{
	return this->host;
}

std::string Server::getIp() const
{
	return this->ip;
}

int Server::getPort() const
{
	return this->port;
}

bool Server::getIsRunning() const
{
	return this->isRunning;
}

std::ostream& operator<<(std::ostream& out, const Server& server)
{
	out << GRE << "Server: " << server.getHost()<< RES << std::endl;
	out << "IP : " << server.getIp() << std::endl;
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