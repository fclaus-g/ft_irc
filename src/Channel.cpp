#include "../inc/Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(Channel const &c)
{
    if (this != &c)
		*this = c;
}
Channel &Channel::operator=(Channel const &c)
{
	if (this != &c)
	{
		this->_name = c._name;
		this->_pass = c._pass;
		this->_topic = c._topic;
        this->_users = c._users;
        this->_admins = c._admins;
	}
	return (*this);
}

Channel::Channel(std::string name, std::string pass, User *admin)
{
    this->_name = name;
    this->_pass = pass;
    this->_topic = "";
    this->_admins.push_back(admin);
}
Channel::~Channel()
{

}

std::string Channel::getName()
{
    return (this->_name);
}

std::string Channel::getPass()
{
    return (this->_pass);
}

std::string Channel::getTopic()
{
    return (this->_topic);
}

size_t Channel::getNumUsers()
{
    return (this->_users.size());
}
std::string Channel::getUsers()
{
    std::string userLst = "";
    size_t      size;

    size = this->_users.size();
    for (size_t i = 0; i < size; i++)
    {
        userLst.append("@");
        userLst.append(this->_users[i]->getNickName());
        userLst.append(" ");
    }
    return (userLst);
}