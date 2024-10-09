
#ifndef USER_HPP
#define USER_HPP

#include "ft_irc.hpp"
class User 
{
	private:
		int 			_socketFd;
		bool			_authenticated;
		std::string		_nickName;
		std::string		_userName;
		std::string 	_realName;
	public:
		User();
		User(int socket_fd);
		~User();
	
		int getFd();
};

#endif
