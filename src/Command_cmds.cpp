#include "ft_irc.hpp"

void Command::commandPrivmsg(int userFd, const std::string msg)
{
	(void)userFd;
	(void)msg;
}

void Command::commandUser(User user)
{
	(void)user;
}

void Command::commandNick(User user)
{
	(void)user;
}

void Command::commandJoin(User user)
{
	(void)user;
}

void Command::commandQuit(User user)
{
	(void)user;
}

void Command::commandKick(User user)
{
	(void)user;
}

void Command::commandInvite(User user)
{
	(void)user;
}

void Command::commandTopic(User user)
{
	(void)user;
}

void Command::commandMode(User user)
{
	(void)user;
}
