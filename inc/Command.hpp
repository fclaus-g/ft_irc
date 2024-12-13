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
		int							_socketFd;
		std::string					_msg;
		std::string					_errorMsg;
		std::string					_commands[TOTAL];
		void						_initCommands();
		User						&_user;
		Server						&_server;
		Channel						*_currChannel;
		std::vector<std::string>	_splitCmd;
    public:
		Command(int socketFd, const std::string msg, User &user, Server &server);
		~Command();
		//Aux Methods	
		bool 						checkCmd(int userFd);
		void						runCmd(int userFd, int key);
		void						kickNonAuthenticatedUser(int userFd);
		void						sendResponse(int code, int mode);
		std::string					composeResponse(int code);
		std::vector<std::string>	splitMessage(const std::string &msg, char delim);
		void 						printVector(const std::vector<std::string> args);
		//Commands methods
		void 			cmdNick();
		void 			cmdPass();
		void 			cmdUser();
		void 			cmdPrivmsg();
		void 			cmdJoin();
		void 			cmdMode();
		void 			commandKick();
		void 			commandInvite();
		void 			commandQuit();
		void 			commandTopic();
		void 			commandMode();
};

#endif
	/*AL CREAR EL CMD SE CREA CON EL FD DEL CLIENTE, EL MENSAJE, REFERENCIA DEL USUARIO Y REF DEL SERVER*/