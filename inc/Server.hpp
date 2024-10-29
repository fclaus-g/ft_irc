
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

class Channel;
class User;

class Server
{
	private:
		int								_port;
		std::string						_password;
		std::string						_name;
		bool							_isRunning;
		int								_serverFd;
		std::string						_message;
		std::vector<Channel>			_channels;
		std::map<int, User *>			_users;
		std::vector<struct pollfd> 		_fds;
		std::string						_tempNick;
		std::string						_tempPass;
		std::string						_commands[TOTAL];
		void							_initCommands();
	public:
		Server();
		Server(int port, std::string password);
		~Server();
		
		//Get-set
		int 			getPort() const;
		std::string 	getName() const;
		bool 			getIsRunning() const;
		User*			getUserByNick(std::string nick);
		User*			getUserByFd(int fd);
		//Server engine methods
		static void		signalHandler(int signal);
		void 			prepareSocket();
		void 			start();
		void 			run();
		void 			stop();
		void 			newConnection();
		//Message-user handle methods
		void			deleteUser(int socketFd);
		void			welcomeUser(int userFd);
		void			msgHandler(int socketFd);
		bool			firstMessage(int userFd, std::string msg);
		bool			loginFormat(std::string msg);
		void			sendWarning(int userFd, std::string str);
		void			checkPass(int userFd);
		bool			checkHexChatPass(int socketFd);
		void			parseMsg(int userFd, std::string msg);
		bool			checkCmd(int userFd, std::string msg);
		void			runCmd(int userFd, int key, std::string cmd);
		//Debug methods
		void 			printMap(const std::map<int, User>& map);
		void 			printVector(const std::vector<Channel>& vector);
		//Channel methods
		void 			createChannel(const std::string& name);
		void 			addUserToChannel(const std::string& channelName, User& user);
		void			removeChannel(const std::string& name);
		//Commands methods
		void 			commandUser(User user);
		void 			commandNick(User user);
		void 			commandJoin(User user);
		void 			commandQuit(User user);
		void 			commandPrivmsg(User user);
		void 			commandKick(User user);
		void 			commandInvite(User user);
		void 			commandTopic(User user);
		void 			commandMode(User user);
};

std::ostream& operator<<(std::ostream& out, const Server& server);

#endif
