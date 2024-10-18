#include "ft_irc.hpp"

User::User()
{
}

User::User(int socket_fd)
{
	this->_fd = socket_fd;
	this->_authenticated = false;
	this->_nickName = "";
	this->_userName = "";
	this->_realName = "";
}

User::User(const User& rhs)
{
	*this = rhs;
}

User& User::operator=(const User& rhs)
{
	if (this != &rhs)
	{
		this->_fd = rhs._fd;
		this->_authenticated = rhs._authenticated;
		this->_ip = rhs._ip;
		this->_nickName = rhs._nickName;
		this->_userName = rhs._userName;
		this->_realName = rhs._realName;
	}
	return *this;
}

User::~User()
{
}

/*Get*/
int User::getFd() const
{
	return this->_fd;
}

bool User::getAuthenticated() const
{
	return this->_authenticated;
}


std::string User::getIp() const
{
	return this->_ip;
}

std::string User::getNick() const
{
	return this->_nickName;
}

std::string User::getUserName() const
{
	return this->_userName;
}

std::string User::getRealName() const
{
	return this->_realName;
}

/*Set*/

void User::setFd(int fd)
{
	this->_fd = fd;
}

void User::setIp(std::string ip)
{
	this->_ip = ip;
}

void User::setAuthenticated(bool authenticated)
{
	this->_authenticated = authenticated;
}

void User::setNick(std::string nick)
{
	this->_nickName = nick;
}

void User::setUserName(std::string userName)
{
	this->_userName = userName;
}

void User::setRealName(std::string realName)
{
	this->_realName = realName;
}

std::ostream& operator<<(std::ostream& out, const User& user)
{
	out << "User: " << user.getFd() << std::endl;
	out << "IP: " << user.getIp() << std::endl;
	out << "Nick: " << user.getNick() << std::endl;
	out << "UserName: " << user.getUserName() << std::endl;
	out << "RealName: " << user.getRealName() << std::endl;
	return out;
}