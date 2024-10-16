#pragma once

#include "ircserv.h"
#include "User.hpp"

class User;

class Channel
{
	private:
        std::string	            _name;
		std::string	            _pass;
        std::string	            _topic;
        std::vector <User *>    _users;
        std::vector <User *>    _admins;
		Channel();
		Channel(Channel const &c);
		Channel &operator=(Channel const &c);

	public:
		Channel(std::string name, std::string pass, User *admin);
		~Channel();

        std::string getName();
        std::string getPass();
        std::string getTopic();
        std::string getUsers();
        size_t      getNumUsers();
};
