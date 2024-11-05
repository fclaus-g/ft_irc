#include "ft_irc.hpp"

void Server::createChannel(const std::string& name)
{
	std::cout << "Creating channel" << RED << name << std::endl;
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
	std::cout << name << std::endl;
	this->_channels.push_back(*newChannel);
	this->_channelsMap[name] = *newChannel;
	//printVector(_channels); for check the vector channel is created correctly
	std::cout << "Channel created" << std::endl;
}

void Server::addUserToChannel(const std::string& channelName, User& user)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == channelName)
		{
			this->_channels[i].addUserChannel(user);
			return;
		}
	}
}

void Server::removeChannel(const std::string& name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == name)
		{
			this->_channels.erase(this->_channels.begin() + i);
			this->_channelsMap.erase(name);
			std::cout << "Channel removed" << std::endl;
			return;
		}
	}
	std::cout << "Channel not found" << std::endl;
}

bool Server::channelExists(const std::string& name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == name)
			return (true);
	}
	return (false);
}

/**
 * @brief Returns a [pointer to a] channel by its name if found, NULL otherwise
 * TODO: Check if _channelsMap should be <std::string, Channel*> instead
 */
Channel	*Server::getChannelByName(std::string name)
{
	Channel	*channel_ptr = NULL;
	std::map<std::string, Channel>::iterator i = this->_channelsMap.find(name);

	if (i != this->_channelsMap.end())
		channel_ptr = &i->second;
	return (channel_ptr);
}
