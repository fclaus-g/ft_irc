#include "ft_irc.hpp"

std::ostream& operator<<(std::ostream& out, const Server& server)
{
	out  << "Server: " << server.getName() << std::endl;
	out << "Port: " << server.getPort() << std::endl;
	if(server.getIsRunning())
	{
		out << GRE << "Server is running" << RES << std::endl;
	}
	else
	{
		out << RED << "Server is not running" << RES << std::endl;
	}
	return out;
}

void Server::printMap(const std::map<int, User>& map)
{
	for (std::map<int, User>::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		std::cout << "Key: " << it->first << std::endl;
		std::cout << it->second << std::endl;
	}
}

void Server::printVector(const std::vector<Channel>& vector)
{
	for (size_t i = 0; i < vector.size(); i++)
	{
		std::cout << vector[i] << std::endl;
	}
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
	msg.append("Registration process usage:\n");
	msg.append("Message #1 - \"PASS <server_password>\" Message #2\"NICK <server_password>\"\n");
	send(userFd, msg.c_str(), msg.length(), 0);
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

	this->_tempNick.clear();
	this->_tempPass.clear();
	if (nick_pos == std::string::npos || pass_pos == std::string::npos)
		return (false);
	if (nick_pos != 0 || nick_pos > pass_pos)
		return (false);
	this->_tempNick = msg.substr(nick_pos + 5, pass_pos - (nick_pos + 5));
	this->_tempPass = msg.substr(pass_pos + 6);
	if (this->_tempNick.empty() || this->_tempPass.empty())
		return (false);
	if (this->_tempNick.find(" ") != std::string::npos
		|| this->_tempPass.find(" ") != std::string::npos)
			return (false);
	return (true);
}

void	Server::checkPass(int userFd)
{
	if (this->_tempPass[this->_tempPass.length() - 1] == '\n')
		this->_tempPass.erase(this->_tempPass.length() - 1);
	if (this->_tempPass != this->_password)
	{
		std::cout << "New user with socket fd " << userFd << " tried to login with wrong password" << std::endl;
		std::cout << RED << "Connection rejected and socket closed" << RES << std::endl;
		sendWarning(userFd, "Wrong password, your are being disconnected\n");
		deleteUser(userFd);
	}
	else
	{
		this->_users[userFd]->setNick(this->_tempNick);
		this->_users[userFd]->setAuthenticated(true);
		std::cout << GRE << "New user with socket fd " << userFd << " has joined the server" << RES << std::endl;
		sendWarning(userFd, "Login succesfull\n");
	}
}

bool	Server::checkHexChatPass(int socketFd)
{
	std::string	pass;
	bool		verified = true;

	if (this->_message.find("PASS ") != 0)
		verified = false;
	if (verified)
	{
		pass = this->_message.substr(this->_message.find("PASS") + 5);
		pass.erase(pass.find_last_not_of(" \n\r\t") + 1);
		if (pass != this->_password)
			verified = false;
	}
	if (!verified)
	{
		std::cout << "New HexChat connection with socket fd " << socketFd << " tried to login with wrong password or whithout any" << std::endl;
		std::cout << RED << "Connection rejected and socket closed" << RES << std::endl;
		sendWarning(socketFd, ":MyServer 464 * :Password incorrect\n");
		deleteUser(socketFd);
		return (false);
	}
	this->_users[socketFd]->setAuthenticated(true);
	return (true);
}

void	Server::deleteUser(int socketFd)
{
	std::vector<struct pollfd>::iterator i = this->_fds.begin();

	while (i != this->_fds.end())
	{
		if (i->fd == socketFd)
		{
			_fds.erase(i);
			break ;
		}
		i++;
	}
	delete (this->_users[socketFd]);
	this->_users.erase(socketFd);
	close(socketFd);
}

void Server::stop()
{
	this->_isRunning = false;
	for (size_t i = 1; i < _fds.size(); i++)
		close(_fds[i].fd);
	std::cout << "Server stopped" << std::endl;
}

User*	Server::getUserByNick(std::string nick)
{
	User	*user_ptr = NULL;
	std::map<int, User*>::iterator i = this->_users.begin();

	while (i != this->_users.end())
	{
		if (i->second->getNick() == nick)
		{
			user_ptr = i->second;
			break ;
		}
		i++;
	}
	return (user_ptr);
}

User*	Server::getUserByFd(int fd)
{
	User	*user_ptr = NULL;
	std::map<int, User*>::iterator i = this->_users.find(fd);

	if (i != this->_users.end())
		user_ptr = i->second;
	return (user_ptr);
}
