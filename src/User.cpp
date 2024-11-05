#include "ft_irc.hpp"

int	User::getFd() const
{
	return this->_fd;
}

bool	User::getAuthenticated() const
{
	return this->_authenticated;
}

void	User::setAuthenticated(bool authenticated)
{
	this->_authenticated = authenticated;
}

std::string User::getNick() const
{
	return (this->_nickName);
}

void	User::setNick(std::string nick)
{
	this->_nickName = nick;
}
std::string User::getUserName() const
{
	return this->_userName;
}

void	User::setUserName(std::string userName)
{
	this->_userName = userName;
}

// void	User::setHexChatPass(bool state)
// {
// 	this->_hexChatPass = state;
// }

std::string User::getRealName() const
{
	return (this->_realName);
}

void	User::setRealName(std::string realName)
{
	this->_realName = realName;
}

bool	User::getHexClient() const
{
	return (this->_hexChatClient);
}

void	User::setHexClient(bool state)
{
	this->_hexChatClient = state;
}
