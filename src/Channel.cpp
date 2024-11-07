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
bool Channel::isOp(User& user)
{
	std::map<User *, bool>::iterator i;

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
		this->_usersMap.erase(i);
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
 * @param message the message to be sent
 * @param userFd the user file descriptor to avoid sending the message to the sender
 * TODO: later, check each user for bans, mutes, etc before sending the message
 */
void Channel::broadcastMessage(const std::string& message, int userFd)
{
	std::map<User *, bool>::iterator	i;

	for (i = this->_usersMap.begin(); i != this->_usersMap.end(); ++i)
	{
		if (i->first->getFd() != userFd)
			send(i->first->getFd(), message.c_str(), message.size(), 0);
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

/*-----------------------[GETTER]------------------------*/

const std::string& Channel::getName() const
{
	return this->_name;
}

const std::string& Channel::getTopic() const
{
	return this->_topic;
}

const std::string Channel::getUsersChannelStr() const
{
	std::string usersStr;
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		usersStr += this->_users[i].getNick() + " ";
	}
	return usersStr;
}

const std::vector<User>& Channel::getUsers() const
{
	return this->_users;
}

bool Channel::getInviteMode() const
{
	return this->_inviteMode;
}

bool Channel::getTopicMode() const
{
	return this->_topicMode;
}

bool Channel::getKeyMode() const
{
	return this->_keyMode;
}

int Channel::getUsersInChannel() const
{
	return this->_usersInChannel;
}

int Channel::getUsersLimit() const
{
	return this->_usersLimit;
}

const std::string& Channel::getPassword() const
{
	return this->_password;
}
/*-----------------------[CHECK METHODS]------------------------*/
/**
 * @brief Check if a user is in the channel
 * 
 * @param user 
 * @return true 
 * @return false 
 */
bool Channel::isUserInChannel(User& user)
{
	std::cout << "User fd on isUserInChannel: " << user.getFd() << std::endl;
	printUsersInChannel();
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		std::cout << "User fd in channel: " << this->_users[i].getFd() << std::endl;
		if (this->_users[i].getFd() == user.getFd())
		{
			std::cout << this->_users[i].getNick() << " is in channel" << std::endl;
			std::cout << "User fd: " << user.getFd() << std::endl;
			std::cout << "User fd in channel: " << this->_users[i].getFd() << std::endl;
			return true;
		}
			
	}
	return false;
}

/**
 * @brief Check if a user is an op in the channel
 * 
 * @param user 
 * @return true 
 * @return false 
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

/**
 * @brief Check if the channel is full
 * static cast the int to size_t to compare the size of the vector
 * @return true 
 * @return false 
 */
bool Channel::channelIsFull()
{
	if (this->_usersLimit == -1)
		return false;
	if (this->_users.size() >= static_cast<size_t>(this->_usersLimit))
		return true;
	return false;
}
/*-----------------------[METHODS]------------------------*/

/**
 * @brief add a user to the channel after checking if the user is already in the channel and if the channel is full
 * send a message to the user if the user is already in the channel or if the channel is full.
 * If the user is not in the channel and the channel is not full, add the user to the channel and send a message to the user
 * for joining the channel and create the channel window on client side
 * 
 * @param user 
 */
void Channel::addUserChannel(User& user)
{
	std::cout << user.getNick() << "getNick" << std::endl;	
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
	printUsersInChannel();
	std::cout << "addUserChannel: " << user.getNick() << std::endl;
}
/**
 * @brief Remove a user from the channel checking if the user is in the vector of users
 * 
 * @param user 
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

/**
 * @brief add a user to the op vector after checking if the user is already an op
 * 
 * @param user 
 */
void Channel::addOpChannel(User& user)
{
	if (this->isOp(user))
	{
		std::cout << "User is already op" << std::endl;
		send(user.getFd(), "User is already op", 18, 0);
		return;
	}
	if (this->isUserInChannel(user))
	{
		this->_op.push_back(user);
		if (this->_usersMap[user.getFd()] == false)
		{
			this->_usersMap[user.getFd()] = true;
		}
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
/**
 * @brief send a message to the user with the channel topic if the topic is empty send a message to the user that no topic is set
 * 
 * @param user 
 */
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

std::ostream& operator<<(std::ostream& os, const Channel& channel)
{
	os << "Channel name: " << channel.getName() << std::endl;
	os << "Channel topic: " << channel.getTopic() << std::endl;
	os << "Channel users: ";
	for (size_t i = 0; i < channel.getUsers().size(); i++)
	{
		os << channel.getUsers()[i] << " "; 
	
	}
	os << "users in channel : " << channel.getUsersInChannel() << std::endl;	
	os << std::endl;
	return os;
}
