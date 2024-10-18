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
		this->_name = rhs._name;
		this->_topic = rhs._topic;
		this->_inviteMode = rhs._inviteMode;
		this->_usersLimit = rhs._usersLimit;
		this->_password = rhs._password;
		this->_users = rhs._users;
		this->_op = rhs._op;
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

int Channel::getUsersLimit() const
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
	this->_users.push_back(user);
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
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i].getFd() == user.getFd())
		{
			this->_op.push_back(this->_users[i]);
			break;
		}
	}
}

void Channel::removeOpChannel(int userFd)
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

std::ostream& operator<<(std::ostream& os, const Channel& channel)
{
	os << "Channel name: " << channel.getName() << std::endl;
	os << "Channel topic: " << channel.getTopic() << std::endl;
	os << "Channel users: ";
	for (size_t i = 0; i < channel.getUsers().size(); i++)
	{
		os << channel.getUsers()[i] << " "; 
	
	}
	os << "users in channel : " << channel.getUsers().size() << std::endl;	
	os << std::endl;
	return os;
}