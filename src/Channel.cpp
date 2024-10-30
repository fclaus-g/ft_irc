#include "ft_irc.hpp"

Channel::Channel()
{
	this->_name = "Channel";
	this->_topic = "";
	this->_inviteMode = false;
	this->_topicMode = false;
	this->_keyMode = false;
	this->_usersInChannel = 0;
	this->_usersLimit = -1;
	this->_password = "";
}

Channel::Channel(const std::string& name)
{
	this->_name = name;
	this->_topic = "";
	this->_inviteMode = false;
	this->_topicMode = false;
	this->_keyMode = false;
	this->_usersInChannel = 0;
	this->_usersLimit = -1;
	this->_password = "";
}

Channel::~Channel()
{
}

Channel::Channel(const Channel &rhs)
{
	*this = rhs;
}

Channel& Channel::operator=(const Channel &rhs)
{
	if (this != &rhs)
	{
		this->_name = rhs._name;
		this->_topic = rhs._topic;
		this->_users = rhs._users;
		this->_op = rhs._op;
		this->_usersMap = rhs._usersMap;
		this->_inviteMode = rhs._inviteMode;
		this->_topicMode = rhs._topicMode;
		this->_keyMode = rhs._keyMode;
		this->_usersInChannel = rhs._usersInChannel;
		this->_usersLimit = rhs._usersLimit;
		this->_password = rhs._password;
	}
	return *this;
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
bool Channel::isUserInChannel(User& user)
{
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i].getFd() == user.getFd())
			return true;
	}
	return false;
}

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
	std::cout << RED << user << std::endl;
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
	std::cout << *this << std::endl;
}

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

void Channel::broadcastMessage(const std::string& message)
{
	for (size_t i = 0; i < this->_users.size(); i++)
	{
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
