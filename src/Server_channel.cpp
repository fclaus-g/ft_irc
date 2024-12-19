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
		if (this->_channels[i]->getName() == name)
		{
			std::cout << "Channel already exists" << std::endl;
			return;
		}
	}
	Channel *newChannel = new Channel(name);
	this->_channels.push_back(newChannel);
	this->_channelsMap[name] = newChannel;
}

void Server::addUserToChannel(const std::string& channelName, User& user)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i]->getName() == channelName)
		{
			this->_channels[i]->addUserChannel(user);
			return;
		}
	}
}

/**
 * @brief Remove a channel from containers (vector, map) and also delete the object
 * @param name The name of the channel to remove
 * TODO: Check if the order delete-erase is correct and not access invalid memory
 */
void Server::removeChannel(const std::string& name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i]->getName() == name)
		{
			delete this->_channels[i];
			this->_channels.erase(this->_channels.begin() + i);
			this->_channelsMap.erase(name);
			return ;
		}
	}
}

bool Server::channelExists(const std::string& name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i]->getName() == name)
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
	std::map<std::string, Channel*>::iterator i = this->_channelsMap.find(name);

	if (i != this->_channelsMap.end())
		channel_ptr = i->second;
	return (channel_ptr);
}
