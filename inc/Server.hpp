
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <fstream>

#define RED "\033[31m"
#define GRE "\033[32m"
#define YEL "\033[33m"
#define BLU "\033[34m"
#define RES "\033[0m"

class Server
{
	private:
		std::string host;
		std::string ip;
		int port;
		bool isRunning;
	public:
		Server(std::string host, std::string ip, int port);
		~Server();

		
		void setHost(std::string host);
		void setIp(std::string ip);
		void setPort(int port);
		
		std::string getHost() const;
		std::string getIp() const;
		int getPort() const;
		bool getIsRunning() const;
};

std::ostream& operator<<(std::ostream& out, const Server& server);

#endif