#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "ft_irc.hpp"

class	User;

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

class Command
{
    private:
		int					_socketFd;
		std::string			_msg;
		std::string			_commands[TOTAL];
		void				_initCommands();
		User				&_user;
		Server				&_server;
    public:
		Command(int socketFd, const std::string msg, User &user, Server &server);
		~Command();
		//Aux Methods	
		bool 			checkCmd(int userFd);
		void			runCmd(int userFd, int key);
		void			kickNonAuthenticatedUser(int userFd);
		//Commands methods
		void 			cmdNick();
		void 			cmdPass();
		void 			cmdUser();
		void 			cmdPrivmsg();
		void 			commandJoin(User &user);
		void 			commandKick();
		void 			commandInvite();
		//Command methods (empty yet)
		void 			commandQuit(User user);
		void 			commandTopic(User user);
		void 			commandMode(User user);
};

#endif
