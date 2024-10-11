#include "ft_irc.hpp"

/**
 * @brief Construct a new User:: User object, default constructor
 * TODO: check if needs to be private/protected to avoid use
 */

User::User()
{
	std::cout << "User default constructor called" << std::endl;
}
/**
 * @brief Construct a new User object and set all attributes to NULL
 * @param socket_fd, to save its socket fd for later use with poll
 */

User::User(int socket_fd)
{
	this->_socketFd = socket_fd;
	this->_authenticated = false;
	this->_nickName = "";
	this->_userName = "";
	this->_realName = "";
	std::cout << "User by fd constructor called" << std::endl;
}

User::User(const User& rhs)
{
	std::cout << "User copy constructor called" << std::endl;
	*this = rhs;
}

User &User::operator=(const User &rhs)
{
	if (this != &rhs)
	{
		this->_socketFd = rhs._socketFd;
		this->_authenticated = rhs._authenticated;
		this->_nickName = rhs._nickName;
		this->_userName = rhs._userName;
		this->_realName = rhs._realName;
	}
	return (*this);
}

/**
 * @brief Destroy the User:: User object, default destructor
 */

User::~User()
{
	std::cout << "User has been destructed" << std::endl;
}


int User::getFd()
{
	return (this->_socketFd);
}
