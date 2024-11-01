#ifndef User_HPP
#define User_HPP

#include "ft_irc.hpp"

class User 
{
	private:
		int 			_fd;
		bool			_authenticated;
		std::string		_ip;
		bool			_hexChatClient;
		bool			_hexChatPass;
		std::string		_nickName;
		std::string		_userName;
		std::string 	_realName;		

	public:
		User();
		User(int fd);
		User(const User& rhs);
		User& operator=(const User& rhs);
		~User();

		bool operator==(const User& rhs);
		bool operator==(const int userFd);

		int					getFd() const;
		bool				getAuthenticated() const;
		std::string			getIp() const;
		std::string			getNick() const;
		std::string			getUserName() const;
		std::string			getRealName() const;

		void				setFd(int fd);
		void				setIp(std::string ip);
		void				setAuthenticated(bool authenticated);
		void				setNick(std::string nick);
		void				setUserName(std::string userName);
		void				setRealName(std::string realName);
		bool				getHexClient() const;
		void				setHexClient(bool state);
		void				hexChatUser(std::string msg);
		bool				getHexStat() const;
		void				setHexStat(bool state);
};

std::ostream& operator<<(std::ostream& out, const User& user);

#endif