#include "ft_irc.hpp"

/**
 * @brief Construct a new User:: User object, default constructor
 * TODO: check if needs to be private/protected to avoid use
 */

User::User()
{
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
    return this->_socketFd;
}
