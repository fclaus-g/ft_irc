#include "ft_irc.hpp"

void Server::createChannel(const std::string& name)
{
	if (name == "")
	{
		std::cout << "Channel name can't be empty" << std::endl;
		return;
	}
	if (name[0] != '#')
	{
		std::cout << "Channel name must start with #" << std::endl;
		return;
	}
	if (name.size() < 2)
	{
		std::cout << "Channel name must have at least 2 characters" << std::endl;
		return;
	}
	if (name.size() > 50)
	{
		std::cout << "Channel name must have at most 50 characters" << std::endl;
		return;
	}
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == name)
		{
			std::cout << "Channel already exists" << std::endl;
			return;
		}
	}
	Channel *newChannel = new Channel(name);
	this->_channels.push_back(*newChannel);
	printVector(_channels);
	std::cout << "Channel created" << std::endl;
}

void Server::addUserToChannel(const std::string& channelName, User& user)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == channelName)
		{
			this->_channels[i].addUserChannel(user);
			std::cout << "User added to channel" << std::endl;
			send(user.getFd(), "You have been added to the channel\n", strlen("You have been added to the channel\n"), 0);
			return;
		}
	}
	std::cout << "Channel not found" << std::endl;
}

void Server::removeChannel(const std::string& name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == name)
		{
			this->_channels.erase(this->_channels.begin() + i);
			std::cout << "Channel removed" << std::endl;
			return;
		}
	}
	std::cout << "Channel not found" << std::endl;
}
