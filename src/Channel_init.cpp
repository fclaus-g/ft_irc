#include "ft_irc.hpp"

/*-----------------------[CONSTRUCT]------------------------*/
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

/*-----------------------[OVERLOAD]------------------------*/
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


/*-----------------------[GETTERS]------------------------*/
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
