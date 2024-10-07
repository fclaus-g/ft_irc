#include "../inc/ft_irc.hpp"

User::User()
{
    std::cout << "User constructed" << std::endl;
}

User::~User()
{
    std::cout << "User destructed" << std::endl;
}

int User::getFd()
{
    return this->_fd;
}

void User::setFd(int fd)
{
    this->_fd = fd;
}
