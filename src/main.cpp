#include "ft_irc.hpp"

/**
 *	@brief Aux function to read from a socket and store it message into a _strClassAttribute
 * 	- Return bool to protect errors when called
 * 	- If error reading, sends corresponding log message and returns false
 * 	- If everything goes OK, it updates the string passed as attribute
 * 	- Need to be aware to treat the stored string before and after call this function as needed
 *	@param store: the string to be updated appending the buffer read from the socket,
 *		passed as reference so changes remains after leaving the function scope
 *	@param pollfd: need to use the poll function and struct to protect reading from happening
 *		before data is available
 */
bool	readFromSocket(int socketFd, std::string &store)
{
	char			buffer[BUFF_SIZE];
	int				read_bytes;
	struct pollfd 	pollfd;
	
	pollfd.fd = socketFd;
	pollfd.events = POLLIN;
	int	poll_result = poll(&pollfd, 1, TIMEOUT);
	if (poll_result == -1)
		return (perror("poll"), false);
	else if (poll_result == 0)
	{
		std::cout << "poll: Timeout occurred, no data available" << std::endl;
		return (false);
	}
	read_bytes = read(socketFd, buffer, sizeof(buffer) - 1);
	if (read_bytes <= 0)
	{
		if (read_bytes == 0)
			std::cout << RED << "User disconnected" << RES << std::endl;
		else
			perror("read");
		return (false);
	}
	buffer[read_bytes] = '\0';
	store += buffer;
	return (true);
}

int main (int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}
	if (std::atoi(av[1]) < 1024 || std::atoi(av[1]) > 49151)
	{
		std::cerr << "Port must be between 1024 and 49151" << std::endl;
		return 1;
	}
	Server server(std::atoi(av[1]), av[2]);
	try
	{
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		server.start();
	}
	catch(const std::exception& e)
	{
		server.stop();
		std::cerr << e.what() << '\n';
	}
	return 0;
}
