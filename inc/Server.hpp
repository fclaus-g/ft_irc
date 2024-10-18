
#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.hpp"

enum command
{
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

class User;
class Server
{
	private:
		int							port;
		std::string					password;
		std::string					name;
		bool						isRunning;
		int							server_fd;
		std::map<int, User*> 		_users;
		std::vector<struct pollfd> 	_socketsPoll;
		std::string					_tempNick;
		std::string					_tempPass;
		std::string					_commands[TOTAL];
		void						_initCommands();
	public:
		Server();
		Server(int port, std::string password);
		~Server();
		
		int 		getPort() const;
		std::string getName() const;
		bool 		getIsRunning() const;

		void 		start();
		void		run();
		void 		stop();
	
		void 		prepareSocket();
		void 		newConnection();
		void		deleteUser(int socketFd);
		void		welcomeUser(int userFd);
		void		msgHandler(int socketFd);
		bool		firstMessage(int userFd, std::string msg);
		bool		loginFormat(std::string msg);
		void		sendWarning(int userFd, std::string str);
		void		checkPass(int userFd);
		void		parseMsg(int socketFd, std::string msg);
		bool		checkCmd(int userFd, std::string msg);
		void		runCmd(int userFd, int key, std::string cmd);
	
		static void signalHandler(int signal);
};

std::ostream &operator<<(std::ostream &out, const Server &server);

#endif
