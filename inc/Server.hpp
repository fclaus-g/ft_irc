
#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.hpp"

/**
 * TODO check if Forward declaration of User class is allowed
 * (If not, will have to #include "User.hpp")
 */
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
	
		static void signalHandler(int signal);
};

std::ostream& operator<<(std::ostream& out, const Server& server);

#endif
