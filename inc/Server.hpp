
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

#define RED "\033[31m"
#define GRE "\033[32m"
#define YEL "\033[33m"
#define BLU "\033[34m"
#define RES "\033[0m"

class Server
{
	private:
		int			port;
		std::string	password;
		std::string	name;
		bool		isRunning;
		int			server_fd;
		std::map<int, std::string> clients;
		std::vector<struct pollfd> fds;//pollfd used for monitoring file descriptors
	public:
		Server();
		Server(int port, std::string password);
		~Server();
		
		int getPort() const;
		std::string getName() const;
		bool getIsRunning() const;

		void start();
		void stop();
		void prepareSocket();
		void acceptClient();
		void readClient(int client_fd);
		static void signalHandler(int signal);

};

std::ostream& operator<<(std::ostream& out, const Server& server);

#endif
