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
	while (42)
	{
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
		if (store.find("\n") != std::string::npos)
			break ;
	}
	return (true);
}

/**
 * @brief Helper function to generate a vector of strings out of one string
 * @param message the string to split into vector of strings
 * @return std::vector<std::string> 
 */
std::vector<std::string> ft_split(std::string message)
{
	std::vector<std::string> res;
	std::string	token;

	if (message.find('\n') != std::string::npos)
		message.at(message.find('\n')) = ' ';
	if (message.find('\r') != std::string::npos)
		message.at(message.find('\r')) = ' ';
	
	std::istringstream tokens(message);

	while (std::getline(tokens, token, ' '))
	{
		if (token.length() > 0 && token[0] != '\n')
			res.push_back(token);
	}
	return (res);
}

/**
 * @brief Aux function to convert a int value into a string
 * -ft_itoa from Aliexpress-
 * @param num the int value to convert
 * @return std::string the string obtained from the value
 */
std::string toString(int num)
{
	std::ostringstream	str;
	str << num;
	return str.str();
}

