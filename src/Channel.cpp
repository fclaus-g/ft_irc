#include "ft_irc.hpp"

/*-----------------------[CHECK METHODS]------------------------*/
bool Channel::isUserInChannel(User& user)
{
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i].getFd() == user.getFd())
			return true;
	}
	return false;
}

/**
 * @brief Check if a user is administrator in the current channel
 */
bool Channel::isOp(User& user)
{
	for (size_t i = 0; i < this->_op.size(); i++)
	{
		if (this->_op[i].getFd() == user.getFd())
			return true;
	}
	return false;
}

bool Channel::channelIsFull()
{
	if (this->_usersLimit == -1)
		return false;
	if (this->_users.size() >= static_cast<size_t>(this->_usersLimit))
		return true;
	return false;
}
/*-----------------------[METHODS]------------------------*/

void Channel::addUserChannel(User& user)
{
	if (this->isUserInChannel(user))
	{
		std::cout << "User already in channel" << std::endl;
		send(user.getFd(), "User already in channel", 21, 0);
		return;
	}
	if (this->channelIsFull())
	{
		std::cout << "Channel is full" << std::endl;
		send(user.getFd(), "Channel is full", 15, 0);
		return;
	}
	else
	{
		this->_usersInChannel++;
		this->_usersMap[user.getFd()] = false;
		this->_users.push_back(user);
		std::string msg = ":" + user.getNick() + "!" + user.getUserName() + "@127.0.0.1 JOIN :" + this->getName() + "\n";
		send(user.getFd(), msg.c_str(), msg.length(), 0);
	}
}

/**
 * @brief Remove a user from the channel
 * @param user the user to be removed
 * TODO: what if the user is not in the channel? maybe make return bool?
 */
void Channel::removeUserChannel(User& user)
{
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i].getFd() == user.getFd())
		{
			this->_users.erase(this->_users.begin() + i);
			break;
		}
	}
}

void Channel::addOpChannel(User& user)
{
	if (this->isOp(user))
	{
		std::cout << "User is already op" << std::endl;
		return;
	}
	if (this->isUserInChannel(user))
	{
		this->_op.push_back(user);
	}
	else
	{
		std::cout << "User is not in channel" << std::endl;
	}
}

void Channel::removeOpChannel(int userFd)
{
	if (this->_op.empty())
	{
		std::cout << "No op in channel" << std::endl;
		return;
	}
	if (this->isOp(this->_users[userFd]))
	{
		for (size_t i = 0; i < this->_op.size(); i++)
		{
			if (this->_op[i].getFd() == userFd)
			{
				this->_op.erase(this->_op.begin() + i);
				break;
			}
		}
	}
	else
	{
		std::cout << "User is not op" << std::endl;
	}
}

/**
 * @brief Broadcast a message to all users in the channel except the one who sent the message
 * @param message the message to be sent
 * @param userFd the user file descriptor to avoid sending the message to the sender
 * TODO: later, check each user for bans, mutes, etc before sending the message
 */
void Channel::broadcastMessage(const std::string& message, int userFd)
{
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i].getFd() != userFd)
			send(this->_users[i].getFd(), message.c_str(), message.size(), 0);
	}
}

void Channel::sendTopicMessage(User& user)
{
	std::string topicMsg;
	if (this->_topic.empty())
	{
		topicMsg = ":server 331" + user.getNick() + " " + this->_name + " :No topic is set";
	}
	else
	{
		topicMsg = ":server 332" + user.getNick() + " " + this->_name + " :" + this->_topic;
	}
	std::cout << topicMsg << std::endl;
	send(user.getFd(), topicMsg.c_str(), topicMsg.size(), 0);
}

void Channel::setName(const std::string& name)
{
	this->_name = name;
}

void Channel::setTopic(const std::string& topic)
{
	this->_topic = topic;
}

void Channel::setInviteMode(const bool inviteMode)
{
	this->_inviteMode = inviteMode;
}

void Channel::setTopicMode(const bool topicMode)
{
	this->_topicMode = topicMode;
}

void Channel::setKeyMode(const bool keyMode)
{
	this->_keyMode = keyMode;
}

void Channel::setUsersInChannel(const int usersInChannel)
{
	this->_usersInChannel = usersInChannel;
}

void Channel::setUsersLimit(const int usersLimit)
{
	this->_usersLimit = usersLimit;
}

void Channel::setPassword(const std::string& password)
{
	this->_password = password;
}
