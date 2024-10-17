
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>//for cout
#include <string>//for string
#include <fstream>//for file
#include <sys/socket.h>//for socket
#include <sys/types.h>//for socket
#include <netinet/in.h>//for sockaddr_in
#include <unistd.h> //for close
#include <poll.h>//for poll
#include <map>//for map
#include <vector>//for vector
#include <cstring>//for memset
#include <cstdlib>//for atoi
#include <signal.h>//for signal
#include <exception>//for exception
#include <fcntl.h>//for fcntl
#include <arpa/inet.h>//for inet_ntoa

#include "User.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"

#define RED "\033[31m"
#define GRE "\033[32m"
#define YEL "\033[33m"
#define BLU "\033[34m"
#define RES "\033[0m"

class Channel;
class Client;
class Channel;

class Server
{
	private:
		int				_port;
		std::string		_password;
		std::string		_name;
		bool			_isRunning;
		int				_serverFd;
		std::string		_message;
		std::vector<Channel> _channels;//vector of channels
		std::map<int, User> _users;//map of clients file descriptors and their objects
		//std::map<int, std::string> clients;//map of clients file descriptors and their names
		std::vector<struct pollfd> _fds;//pollfd used for monitoring file descriptors
	public:
		Server();
		Server(int port, std::string password);
		~Server();
		
		int getPort() const;
		std::string getName() const;
		bool getIsRunning() const;
		const std::map<int, User>& getUsers() const;
		void addChannel(std::string &name);
		void start();
		void stop();
		void prepareSocket();
		//User methods
		void acceptUser();
		void readUser(int userFd);
		//void parseMessage(const std::string& message, int userFd);
		void printMap(const std::map<int, User>& map);
		void printVector(const std::vector<Channel>& vector);
		void addUser(int userFd, struct sockaddr_in user_addr);
		void removeUser(int userFd);
		//Channel methods
		void createChannel(const std::string& name);
		void addUserToChannel(const std::string& channelName, User& user);
		void removeChannel(const std::string& name);

		
		void checkCommand(int userFd);
		void commandUser(int userFd);
		void commandNick(int userFd);
		void commandJoin(int userFd);
		void commandQuit(int userFd);
		void commandPrivmsg(int userFd);
		void commandKick(int userFd);
		void commandInvite(int userFd);
		void commandTopic(int userFd);
		void commandMode(int userFd);

		static void signalHandler(int signal);

};

std::ostream& operator<<(std::ostream& out, const Server& server);

#endif
