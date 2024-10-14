
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
	msg.append("Usage \"NICK <your_nickname> PASS <server_password>\"\n");
	send(userFd, msg.c_str(), msg.length(), 0);
}

void	Server::deleteUser(int socketFd)
{
	delete (this->_users[socketFd]);
	close(socketFd);
}

void	Server::sendWarning(int userFd, std::string str)
{
	std::string	msg = "";
	msg.append(str);
	send(userFd, msg.c_str(), msg.length(), 0);

}

bool	Server::loginFormat(std::string msg)
{
	size_t		nick_pos = msg.find("NICK ");
	size_t		pass_pos = msg.find(" PASS ");
	std::string	nick;
	std::string	pass;

	if (nick_pos == std::string::npos || pass_pos == std::string::npos)
		return (false);
	if (nick_pos != 0 || nick_pos > pass_pos)
		return (false);
	nick = msg.substr(nick_pos + 5, pass_pos - (nick_pos + 5));
	pass = msg.substr(pass_pos + 6);
	if (nick.empty() || pass.empty())
		return (false);
	return (true);
}
