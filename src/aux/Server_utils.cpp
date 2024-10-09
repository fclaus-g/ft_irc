
#include "ft_irc.hpp"

void Server::stop()
{
	this->isRunning = false;
	close(server_fd);
	std::cout << "Server stopped" << std::endl;
}

void Server::signalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
	{
		std::cout << "Signal received: " << signal << std::endl;
		throw std::runtime_error("Server stopped by signal");
	}
}
