#pragma once

#include "ircserv.h"
#include "Channel.hpp"

class User
{
	private:
        std::string     _userName;
		std::string     _nickName;
        std::string     _realName;
        std::string     _host;
        int             _port;
        int             _clientSocket;
		bool			_authent;
        //lista/vector de canales

	public:
        User();
		User(int serverSocket);
		User(User const &u);
		User &operator=(User const &u);
		~User();

        std::string getUserName() const;
        std::string getNickName() const;
        std::string getRealName() const;
        std::string getHost() const;
        int         getPort() const;
        int         getSocket() const;
		bool		getAuthent() const;
		void setUserName(std::string name);
        void setNickName(std::string nick);
        void setRealName(std::string realName);
        void setHost(std::string host);
        void setPort(int port);
        void setSocket(int sock);
		void setAuthent(bool auth);
};

std::ostream& operator<<(std::ostream& out, const User& user);