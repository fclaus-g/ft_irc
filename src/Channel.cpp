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

/*-----------------------[SETTER]------------------------*/

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

void Channel::addUser(int userFd)
{
	this->_users.push_back(userFd);
}

void Channel::removeUser(int userFd)
{
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i] == userFd)
		{
			this->_users.erase(this->_users.begin() + i);
			return ;
		}
	}
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
	os << std::endl;
	return os;
}