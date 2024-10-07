
#ifndef USER_HPP
#define USER_HPP

#include "ft_irc.hpp"
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
		//Getters
		int getFd();
		void setFd(int fd);
		//Setters
		//Methods
};

#endif
