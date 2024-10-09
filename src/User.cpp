#include "../inc/User.hpp"

User::User()
{
    std::cout << "User constructor" << std::endl;
}

User::~User()
{
    std::cout << "User destructor" << std::endl;
}

/*Get*/
int User::getFd()
{
    return this->User_fd;
}

std::string User::getIp()
{
    return this->User_ip;
}

/*Set*/

void User::setFd(int fd)
{
    this->User_fd = fd;
}

void User::setIp(std::string ip)
{
    this->User_ip = ip;
}
