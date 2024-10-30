#include "ft_irc.hpp"

Command::Command(int socketFd, const std::string msg)
{
	this->_socketFd = socketFd;
	this->_msg = msg;
	_initCommands();
}

void	Command::_initCommands()
{
	this->_commands[0] = "USER";
    this->_commands[1] = "NICK";
    this->_commands[2] = "JOIN";
    this->_commands[3] = "QUIT";
    this->_commands[4] = "PRIVMSG";
    this->_commands[5] = "KICK";
    this->_commands[6] = "INVITE";
    this->_commands[7] = "TOPIC";
    this->_commands[8] = "MODE";
}

Command::~Command()
{
}

bool Command::checkCmd(int userFd, std::string msg)
{
	for (int i = 0; i < TOTAL; i++)
	{
		if (msg.find(_commands[i]) == 0)
		{
			runCmd(userFd, i, msg);
			return (true);
		}
	}
	return (false);
}

void	Command::runCmd(int userFd, int key, std::string msg)
{
	(void)userFd;
	(void)msg;
	switch (key)
	{
		case USER:
			break;
		case NICK:
			break;
		case JOIN:
			break;
		case QUIT:
			break;
		case PRIVMSG:
			break;
		case KICK:
			break;
		case INVITE:
			break;
		case TOPIC:
			break;
		case MODE:
			break;
		default:
			break;
	}
}
