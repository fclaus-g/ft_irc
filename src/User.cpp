#include "ft_irc.hpp"

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