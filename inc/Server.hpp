
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
		int							_port;
		std::string					_password;
		std::string					_name;
		bool						_isRunning;
		int							_serverFd;
		std::string					_message;
		std::vector<Channel>		_channels;//vector of channels
		std::map<std::string, Channel> _channelsMap;//map of channels
		std::map<int, User*>		_users;//map of clients file descriptors and their objects
		//std::map<int, std::string> clients;//map of clients file descriptors and their names
		std::vector<struct pollfd> 	_fds;//pollfd used for monitoring file descriptors
		std::string					_tempNick;
		std::string					_tempPass;
		std::string					_commands[TOTAL];
		void						_initCommands();
	public:
		Server();
		Server(int port, std::string password);
		~Server();
		//Getters and setters
		int			getPort() const;
		std::string	getName() const;
		bool		getIsRunning() const;
		//Server engine methods
		void		start();
		void		run();
		void		stop();
		void		prepareSocket();
		void 		newConnection();
		//User methods
		void		deleteUser(int socketFd);
		void		welcomeUser(int userFd);
		void		msgHandler(int socketFd);
		bool		firstMessage(int userFd, std::string msg);
		bool		checkHexChatPass(int socketFd);
		bool		loginFormat(std::string msg);
		void		sendWarning(int userFd, std::string str);
		void		checkPass(int userFd);
		void		parseMsg(int userFd, std::string msg);
		bool		checkCmd(int userFd, std::string msg);
		void		runCmd(int userFd, int key, std::string cmd);
		//Channel methods
		void		createChannel(const std::string& name);
		void		addUserToChannel(const std::string& channelName, User& user);
		void		removeChannel(const std::string& name);
		
		//Command methods
		
		void		checkCommand(User user);
		void		commandUser(User& user);
		void		commandNick(User& user);
		void		commandJoin(User& user);
		void 		sendTopicMessage(int userFd, const std::string& channel);
		void		commandQuit(User user);
		void		commandPrivmsg(User user);
		void		commandKick(User user);
		void		commandInvite(User user);
		void		commandTopic(User user);
		void		commandMode(User user);

		static void	signalHandler(int signal);
		void		printMap(const std::map<int, User>& map);
		void		printVector(const std::vector<Channel>& vector);
};

std::ostream& operator<<(std::ostream& out, const Server& server);

#endif

