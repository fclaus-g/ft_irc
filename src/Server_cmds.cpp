#include "ft_irc.hpp"

void Server::commandUser(User user)
{
	std::string username;
	std::string realname;

	size_t iPos = this->_message.find_first_not_of(" \t");
	size_t fPos = this->_message.find_first_of(" \t", iPos);
	if (fPos == std::string::npos)
	{
		std::cout << "Error: Not enough parameters" << std::endl;
		return;
	}
	username = this->_message.substr(iPos, fPos - iPos);

	this->_message = this->_message.substr(fPos + 1);
	iPos = this->_message.find_first_not_of(" \t", fPos);
	fPos = this->_message.find_first_of(" \t", iPos);
	realname = this->_message.substr(iPos, fPos - iPos);

	this->_message.clear();
	(void)iPos;
	(void)fPos;
	(void)user;
}

void Server::commandNick(User user)
{
	size_t iPos = this->_message.find_first_not_of(" \t");

	(void)iPos;
	(void)user;

}

void Server::commandJoin(User user)
{
	size_t iPos = this->_message.find_first_not_of(" \t");
	size_t fPos = this->_message.find_first_of(" \t", iPos);

	std::string channel = this->_message.substr(iPos, fPos - iPos);
	if (channel[0] != '#')
	{
		std::cout << "Error: Invalid channel name" << std::endl;
		return;
	}
	std::vector<Channel> channels = this->_channels;
	for(std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getName() == channel)
		{
			it->addUserChannel(user);
			return;
		}
	}
	this->createChannel(channel);
	this->addUserToChannel(channel, user);
}

void Server::commandQuit(User user)
{
	(void)user;
}

void Server::commandPrivmsg(User user)
{
	(void)user;
}

void Server::commandKick(User user)
{
	(void)user;
}

void Server::commandInvite(User user)
{
	(void)user;
}

void Server::commandTopic(User user)
{
	(void)user;
}

void Server::commandMode(User user)
{
	(void)user;
}
