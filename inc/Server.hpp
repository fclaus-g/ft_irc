
#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.hpp"
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
		static void signalHandler(int signal);
};

std::ostream& operator<<(std::ostream& out, const Server& server);

#endif
