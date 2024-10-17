#include "../inc/Channel.hpp"

Channel::Channel()
{
	std::cout << "Channel object created" << std::endl;
	this->_name = "Channel";
	this->_topic = "";
	this->_inviteMode = false;
	this->_topicMode = false;
	this->_keyMode = false;
	this->_usersLimit = -1;
	this->_password = "";
}

Channel::Channel(std::string name)
{
	std::cout << "Channel object created" << std::endl;
	this->_name = name;
	this->_topic = "";
	this->_inviteMode = false;
	this->_topicMode = false;
	this->_keyMode = false;
	this->_usersLimit = -1;
	this->_password = "";
}

Channel::~Channel()
{
	std::cout << "Channel object destroyed" << std::endl;
}

Channel::Channel(const Channel &rhs)
{
	std::cout << "Channel copy constructor" << std::endl;
	*this = rhs;
}

Channel& Channel::operator=(const Channel &rhs)
{
	std::cout << "Channel assignment operator" << std::endl;
	if (this != &rhs)
	{
		this->_channelName = rhs._channelName;
		this->_topic = rhs._topic;
		this->_isPrivate = rhs._isPrivate;
		this->_maxUsers = rhs._maxUsers;
		this->_channelPass = rhs._channelPass;
		this->_channelUsers = rhs._channelUsers;
		this->_channelOp = rhs._channelOp;
	}
	return *this;
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


void Channel::setUsersLimit(const int usersLimit)
{
	this->_usersLimit = usersLimit;
}

void Channel::setPassword(const std::string& password)
{
	this->_password = password;
}

/*-----------------------[GETTER]------------------------*/

const bool& Channel::getIsPrivate() const
{
	return this->_isPrivate;
}
const size_t& Channel::getMaxUsers() const
{
	return this->_maxUsers;
}
const std::string& Channel::getName() const
{
	return this->_channelName;
}
const std::string& Channel::getPass() const
{
	return this->_channelPass;
}
const std::string& Channel::getTopic() const
{
	return this->_topic;
}

const std::vector<int>& Channel::getUsers() const
{
	return this->_users;
}

const bool Channel::getInviteMode() const
{
	return this->_inviteMode;
}

const bool Channel::getTopicMode() const
{
	return this->_topicMode;
}

const bool Channel::getKeyMode() const
{
	return this->_keyMode;
}

const int Channel::getUsersLimit() const
{
	return this->_usersLimit;
}

const std::string& Channel::getPassword() const
{
	return this->_password;
}

/*-----------------------[METHODS]------------------------*/

void Channel::addUserChannel(User& user)
{
	this->_channelUsers.push_back(user);
	std::cout << *this << std::endl;
}

void Channel::removeUserChannel(User& user)
{
	for (size_t i = 0; i < this->_channelUsers.size(); i++)
	{
		if (this->_channelUsers[i].getFd() == user.getFd())
		{
			this->_channelUsers.erase(this->_channelUsers.begin() + i);
			break;
		}
	}
}

void Channel::addOpChannel(User& user)
{
	for (size_t i = 0; i < this->_channelUsers.size(); i++)
	{
		if (this->_channelUsers[i].getFd() == user.getFd())
		{
			this->_channelOp.push_back(this->_channelUsers[i]);
			break;
		}
	}
}

void Channel::removeOpChannel(int userFd)
{
	for (size_t i = 0; i < this->_channelOp.size(); i++)
	{
		if (this->_channelOp[i].getFd() == userFd)
		{
			this->_channelOp.erase(this->_channelOp.begin() + i);
			break;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Channel& channel)
{
	os << "Channel name: " << channel.getName() << std::endl;
	os << "Channel topic: " << channel.getTopic() << std::endl;
	os << "Channel users: ";
	for (size_t i = 0; i < channel._channelUsers.size(); i++)
	{
		os << channel._channelUsers[i] << " "; 
	
	}
	os << "users in channel : " << channel._channelUsers.size() << std::endl;	
	os << std::endl;
	return os;
}