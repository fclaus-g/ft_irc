#ifndef User_HPP
#define User_HPP

#include <iostream>
#include <string>

class User 
{
	private:
		int 			_fd;
		bool			_authenticated;
		std::string		_ip;
		std::string		_nick;
		std::string		_userName;
		std::string 	_realName;
	public:
		User();
		User(const User& rhs);
		User& operator=(const User& rhs);
		~User();
		int getFd() const;
		std::string getIp() const;
		bool getAuthenticated() const;
		std::string getNick() const;
		std::string getUserName() const;
		std::string getRealName() const;


		void setFd(int fd);
		void setIp(std::string ip);
		void setAuthenticated(bool authenticated);
		void setNick(std::string nick);
		void setUserName(std::string userName);
		void setRealName(std::string realName);
};

std::ostream& operator<<(std::ostream& out, const User& user);

#endif