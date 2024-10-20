#include "ft_irc.hpp"

/**
 * @brief In the case in which the new connection is happening by using
 * 	HexChat client
 * 	When login using this client, it sends two separate messages sent at start
 * 	-to consecutive poll events in the same socket-
 * 		#1 = "CAP LS 302\n"
 * 		#2 = "NICK pgomez-r\nUSER pgomez-r 0 * :realname\n"
 * @param msg is the whole message string sent by the hexchat client
 * TODO: find out what has to be done with the PASS authentication in this case
 */
void	User::hexChatLogin(std::string msg)
{
	std::string	nick;
	std::string user;
	size_t		pos;

	pos = msg.find_first_of("\n");
	nick = msg.substr(5, pos - 5);
	user = msg.substr(msg.find("USER") + 5);
	pos = user.find_first_of(" ");
	user = user.substr(0, pos);
	setNick(nick);
	setUserName(user);
	setAuthenticated(true);
}

int User::getFd() const
{
	return this->_fd;
}

bool User::getAuthenticated() const
{
	return this->_authenticated;
}

void User::setAuthenticated(bool authenticated)
{
	this->_authenticated = authenticated;
}

std::string User::getNick() const
{
	return (this->_nickName);
}

void User::setNick(std::string nick)
{
	this->_nickName = nick;
}
std::string User::getUserName() const
{
	return this->_userName;
}

void User::setUserName(std::string userName)
{
	this->_userName = userName;
}

std::string User::getRealName() const
{
	return (this->_realName);
}

void User::setRealName(std::string realName)
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
