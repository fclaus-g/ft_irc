#ifndef User_HPP
#define User_HPP

#include <iostream>
#include <string>

class User 
{
	private:
		int 			_fd;
		bool			_authenticated;
		std::string		_nick;
		std::string		_userName;
		std::string 	_realName;
	public:
		User();
		~User();

		int getFd();
		std::string getIp();

		void setFd(int fd);
		void setIp(std::string ip);
};

#endif