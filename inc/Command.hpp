#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "ft_irc.hpp"

enum command
{
	CAPLS,
	PASS,
	USER,
	NICK,
	JOIN,
	QUIT,
	PRIVMSG,
	KICK,
	INVITE,
	TOPIC,
	MODE,
	TOTAL
};

class	User;
class Command
{
    private:
		std::string			_msg;
		int					_socketFd;
		std::string			_commands[TOTAL];
		void				_initCommands();
		User				&_user;
    public:
		Command(int socketFd, const std::string msg, User &user);
		~Command();

		//Commands methods
		void 			commandPrivmsg(int userFd, const std::string msg);
		void 			commandUser(User user);
		void 			commandNick(User user);
		void 			commandJoin(User user);
		void 			commandQuit(User user);
		void 			commandKick(User user);
		void 			commandInvite(User user);
		void 			commandTopic(User user);
		void 			commandMode(User user);
		bool 			checkCmd(int userFd, std::string msg);
		void			runCmd(int userFd, int key, std::string msg);
};

#endif
