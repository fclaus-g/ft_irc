#include "ft_irc.hpp"

User::User()
{
}

User::User(int socket_fd)
{
	this->_fd = socket_fd;
	this->_authenticated = false;
	this->_hexChatClient = false;
	this->_hexChatPass = false;
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
bool User::getHexStat() const
{
	return this->_hexChatPass;
}

int User::getFd() const
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
/**
 * @brief When login using this client, it sends three separate messages at start
 * 	-three consecutive poll events in the same socket- this method checks #3
 * 		#1 = "CAP LS 302\n" - skipeed first time and user->_hexChat = TRUE
 *		#2 = "PASS <password>\n" - checked in server.checkPassHexChat()
 * 		#3 = "NICK pgomez-r\nUSER pgomez-r 0 * :realname\n" - let's do it!
 *	(!)Hexchat needs to have the server password in the network config,
 *		otherwise, it won't send message #2
 */
void	User::hexChatUser(std::string msg)
{
	std::string	nick;
	std::string user;
	size_t		pos;

	pos = msg.find_first_of("\n");
	nick = msg.substr(5, pos - 6);
	user = msg.substr(msg.find("USER") + 5);
	pos = user.find_first_of(" ");
	user = user.substr(0, pos);
	this->_nickName = nick;
	this->_userName = user;
}

bool	User::getHexClient() const
{
	return (this->_hexChatClient);
}

void	User::setHexClient(bool state)
{
	this->_hexChatClient = state;
}

void	User::setHexStat(bool state)
{
	this->_hexChatPass = state;
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
