#include "../inc/Client.hpp"

Client::Client()
{
    std::cout << "Client constructor" << std::endl;
}

Client::~Client()
{
    std::cout << "Client destructor" << std::endl;
}

/*Get*/
int Client::getFd()
{
    return this->client_fd;
}

std::string Client::getIp()
{
    return this->client_ip;
}

/*Set*/

void Client::setFd(int fd)
{
    this->client_fd = fd;
}

void Client::setIp(std::string ip)
{
    this->client_ip = ip;
}
