
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
		User(const User &rhs);
		~User();
	
		User &operator=(const User &rhs);
	
		int 			getFd();
		bool			getAuth();
		std::string		getNick();
		void			setNick(std::string newNick);
		void			setAuth(bool state);
};


#endif
