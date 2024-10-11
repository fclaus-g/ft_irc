#include "../inc/Channel.hpp"

Channel::Channel()
{
	this->_channelName = "default";
	this->_topic = "";
	this->_isPrivate = false;
	this->_maxUsers = 0;
	this->_channelPass = "";	
	std::cout << "Channel object created" << std::endl;
}

Channel::Channel(std::string name)
{
	std::cout << "Channel object created" << std::endl;
	this->_channelName = name;
	this->_topic = "";
	this->_isPrivate = false;
	this->_maxUsers = 0;
	this->_channelPass = "";
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
const std::vector<User>& Channel::getUsers() const
{
	return this->_channelUsers;
}
const std::vector<User>& Channel::getOp() const
{
	return this->_channelOp;
}

/*-----------------------[SETTER]------------------------*/

void Channel::setIsPrivate(bool isPrivate)
{
	this->_isPrivate = isPrivate;
}

void Channel::setMaxUsers(size_t maxUsers)
{
	this->_maxUsers = maxUsers;
}
void Channel::setName(const std::string& name)
{
	this->_channelName = name;
}
void Channel::setPass(const std::string& pass)
{
	this->_channelPass = pass;
}
void Channel::setTopic(const std::string& topic)
{
	this->_topic = topic;
}

/*-----------------------[METHODS]------------------------*/




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