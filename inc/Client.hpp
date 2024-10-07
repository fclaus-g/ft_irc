#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>

class Client 
{
	private:
		int client_fd;
		std::string client_ip;
		std::string client_name;
	public:
		Client();
		~Client();

		int getFd();
		std::string getIp();

		void setFd(int fd);
		void setIp(std::string ip);
};

#endif