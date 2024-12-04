#include "ft_irc.hpp"

/*-----------------------[CHECK METHODS]------------------------*/
bool Channel::isUserInChannel(User& user)
{
	if (this->_usersMap.find(&user) != this->_usersMap.end())
		return (true);
	return (false);
}

/**
 * @brief Check if a user is administrator in the current channel
 * - Creates an iterator which finds an aim to the user passed as argument
 * - Then checks (if the user exists in list) if the user is op or not
 */
bool Channel::isOp(User& user) const
{
	std::map<User *, bool>::const_iterator i;

	i = this->_usersMap.find(&user);
	if (i != this->_usersMap.end() && i->second == true)
		return (true);
	return (false);
}

/**
 * @brief Check if the channel is full before a new user can join
 * (!) IF userLimit == -1 NO USERS LIMIT
 */
bool Channel::channelIsFull()
{
	if (this->_usersLimit == -1)
		return false;
	if (this->_usersMap.size() >= static_cast<size_t>(this->_usersLimit))
		return true;
	return false;
}
/*-----------------------[METHODS]------------------------*/
/**
 * @brief add a user to the channel before checking if the user is already in the channel
 * - If the user is already in the channel, send a message to the user
 * - If the channel is full, send a message to the user
 * TODO: if channel is full must send a message ERR_CHANNELISFULL to the client
 * @param user 
 */
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
		this->_usersMap[&user] = false;
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
	std::map<User *, bool>::iterator i;

	i = this->_usersMap.find(&user);
	if (i != this->_usersMap.end())
	{
		this->_usersMap.erase(i);
		this->_usersInChannel--;
	}
}

/**
 * @brief add a user to the op vector after checking if the user is already an op
 * (!) Changed, since no op_vector if user is in channel and not op, set bool TRUE
 * Now is more like a setOpUser than addOp since it does not add to any vector
 * @param user 
 */
void Channel::addOpChannel(User& user)
{
	std::map <User*, bool>::iterator	aux;

	aux = this->_usersMap.find(&user);
	if (isUserInChannel(user))
	{
		if (isOp(user))
		{
			std::cout << "addOpChannel: User is already op" << std::endl;
			send(user.getFd(), "User is already op", 18, 0);
			return ;
		}
	}
	else
	{
		std::cout << "addOpChannel: User is not in channel" << std::endl;
		return ;	
	}
	aux->second = true;
}

/**
 * @brief Broadcast a message to all users in the channel except the one who sent the message
 * @param message the text message to be sent
 * @param command_msg the message to be sent formatted as irc protocol needs
 * @param userFd the user file descriptor to avoid sending the message to the sender
 * TODO: later, check each user for bans, mutes, etc before sending the message
 */
void Channel::broadcastMessage(const std::string& message, User &sender)
{
	std::map<User *, bool>::iterator	i;
	std::string							command_msg;

	command_msg.clear();
	command_msg = ":" + sender.getNick() + "!" + sender.getUserName() + " " + message;
	for (i = this->_usersMap.begin(); i != this->_usersMap.end(); ++i)
	{
		if (i->first->getFd() != sender.getFd())
			send(i->first->getFd(), command_msg.c_str(), command_msg.size(), 0);
	}
}

/**
 * @brief send a message to the user with the channel topic
 * - If the topic is empty, send a message with no topic
 * - If the topic is not empty, send a message with the topic
 * 
 * @param user 
 */
void Channel::sendTopicMessage(User& user)
{
	std::string topicMsg;
	if (this->_topic.empty())
	{
		topicMsg = ":server 331 " + user.getNick() + " " + this->_name + " :No topic is set";
	}
	else
	{
		topicMsg = ":server 332 " + user.getNick() + " " + this->_name + " :" + this->_topic;
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
