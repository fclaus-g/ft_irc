#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "ft_irc.hpp"

class Command
{
    private:
		std::string		_msg;
    public:
		Command(const std::string msg);
		~Command();
	
		cmdNick();
};

#endif
