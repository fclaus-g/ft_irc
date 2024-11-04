#include "ft_irc.hpp"

/**
 * @brief User default constructor
 * TODO: check if we need to have it, otherwise delete it
 */
User::User()
{
}

/**
 * @brief User constructor, called when a new connection is established,
 *	it sets all attributes to false, null or empty except the socket fd
 * @param socket_fd the socket file descriptor passed when new connection is established
 */
User::User(int socket_fd)
{
	this->_fd = socket_fd;
	this->_authenticated = false;
	this->_hexChatClient = false;
	this->_nickName = "";
	this->_userName = "";
	this->_realName = "";
	this->_buffer = "";
}

/**
 * @brief User copy constructor, not using it yet but may be useful in the future
 */
User::User(const User& rhs)
{
	*this = rhs;
}

/**
 * @brief User assignment operator, needed by the copy constructor
 */
User& User::operator=(const User& rhs)
{
	if (this != &rhs)
	{
		this->_fd = rhs._fd;
		this->_authenticated = rhs._authenticated;
		this->_nickName = rhs._nickName;
		this->_userName = rhs._userName;
		this->_realName = rhs._realName;
	}
	return *this;
}

/**
 * @brief User destructor
*/
User::~User()
{
}

/**
 * @brief Overload of the << operator to print the user attributes
 */
std::ostream& operator<<(std::ostream& out, const User& user)
{
	out << "User: " << user.getFd() << std::endl;
	out << "Nick: " << user.getNick() << std::endl;
	out << "UserName: " << user.getUserName() << std::endl;
	out << "RealName: " << user.getRealName() << std::endl;
	return out;
}
