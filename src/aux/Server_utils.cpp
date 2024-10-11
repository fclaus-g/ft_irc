
#include "ft_irc.hpp"

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
	return (out);
}

void Server::stop()
{
	this->isRunning = false;
	close(server_fd);
	std::cout << "Server stopped" << std::endl;
}

void Server::signalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
	{
		std::cout << "Signal received: " << signal << std::endl;
		throw std::runtime_error("Server stopped by signal");
	}
}

void Server::welcomeUser(int userFd)
{
	std::string msg;
	msg = "Wecolme to The Pollitas Server!\n";
	msg.append("Insert user nick and server password\n");
	msg.append("Usage (NICK <your_nick> PASS <server_pass>)\n");
	send(userFd, msg.c_str(), msg.length(), 0);
}
