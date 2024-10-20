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

std::ostream& operator<<(std::ostream& out, const User& user)
{
	out << "User: " << user.getFd() << std::endl;
	out << "IP: " << user.getIp() << std::endl;
	out << "Nick: " << user.getNick() << std::endl;
	out << "UserName: " << user.getUserName() << std::endl;
	out << "RealName: " << user.getRealName() << std::endl;
	return out;
}
