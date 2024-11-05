#ifndef User_HPP
#define User_HPP

#include "ft_irc.hpp"

class User 
{
	private:
		int 				_fd;
		std::string			_buffer;
		bool				_authenticated;
		bool				_hexChatClient;
		bool				_hexChatPass;
		std::string			_nickName;
		std::string			_userName;
		std::string 		_realName;
	public:
		User();
		User(int fd);
		User(const User& rhs);
		User& operator=(const User& rhs);
		~User();
		//Get-set
		int 				getFd() const;
		bool 				getAuthenticated() const;
		void 				setAuthenticated(bool authenticated);
		std::string 		getNick() const;
		void 				setNick(std::string nick);
		std::string 		getUserName() const;
		void 				setUserName(std::string userName);
		std::string 		getRealName() const;
		void 				setRealName(std::string realName);
		bool				getHexClient() const;
		void				setHexClient(bool state);
};

std::ostream& operator<<(std::ostream& out, const User& user);

#endif