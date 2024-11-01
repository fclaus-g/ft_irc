#include "ft_irc.hpp"

void Server::start()
{
	prepareSocket();
	run();
	stop();
}

void Server::prepareSocket()
{
	struct sockaddr_in server_addr;
	struct pollfd poll_fd;
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(this->_port);
	
	this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverFd < 0)
	{
		perror("socket");
		return;
	}
	int opt = 1;
	if (setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
	{
		perror("setsockopt");
		close(this->_serverFd);
		return;
	}
	if (fcntl(this->_serverFd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("fcntl");
		close(this->_serverFd);
		return;
	}
	if (bind(this->_serverFd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		close(this->_serverFd);
		return;
	}
	if (listen(this->_serverFd, SOMAXCONN) < 0)
	{
		perror("listen");
		close(this->_serverFd);
		return;
	}
	poll_fd.fd = this->_serverFd;
	poll_fd.events = POLLIN;
	poll_fd.revents = 0;
	_fds.push_back(poll_fd);
	this->_isRunning = true;
	std::cout << *this << std::endl;
}

void	Server::run()
{
	while (this->_isRunning)
	{
		if ((poll(&_fds[0], _fds.size(), -1)) < 0)
			throw std::runtime_error("poll");
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _serverFd)
					newConnection();
				else
					msgHandler(_fds[i].fd);
			}
		}
	}
}

void	Server::newConnection()
{
	sockaddr_in 		client_addr;
	socklen_t 			client_addr_len = sizeof(client_addr);
	int 				client_socket;
	int					error;
	struct pollfd		new_pollfd;

	client_socket = accept(_serverFd, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_socket < 0)
	{
		perror("accept");
		return ;
	}
	error = fcntl(client_socket, F_GETFL, 0);
	if (error == -1 || fcntl(client_socket, F_SETFL, error | O_NONBLOCK) == -1)
	{
		perror("fcntl");
		close(client_socket);
		return;
	}
	new_pollfd.fd = client_socket;
	new_pollfd.events = POLLIN | POLLOUT;
	new_pollfd.revents = 0;
	_fds.push_back(new_pollfd);
	_users[client_socket] = new User(client_socket);
	std::cout << GRE << "New connection established with socket fd " << client_socket << RES << std::endl;
}

/**
 * @brief Aux function to read from a socket and store it message into a _strClassAttribute
 * 	- Return bool to protect errors when called
 * 	- If error reading, sends corresponding log message and returns false
 * 	- If everything goes OK, it updates the string passed as attribute
 * 	- Need to be aware to treat the stored string before and after call this function as needed
 * @param store: the string to be updated appending the buffer read from the socket,
 *	passed as reference so changes remains after leaving the function scope
 */
bool	readFromSocket(int socketFd, std::string &store)
{
	char		buffer[BUFF_SIZE];
	int			read_bytes;

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

/**
 * @brief This function handles the message -poll event- sent by the current socket
 * 	Logic process:
 * 		- If nothing or error read, kick and delete user
 * 		- Check if the client is HexChat -> if so, we asumme there are three consecutive
 * 			fixed messages -poll events- in this socket -> we check three conditions
 * 		- If not, the socket is for a client using a terminal, then check if this is the
 * 			first message of the user (extract NICK and PASS)
 * 		- If nothing above matches and there has not been any errors, we procceed to parse
 * 			the message (the user is already authenticated and the buffer read is either 
 * 			a message to send or a command to execute)
 */
void	Server::msgHandler(int socketFd)
{
	this->_message.clear();
	if (!readFromSocket(socketFd, this->_message))
		return (deleteUser(socketFd));
	std::cout << "Mensaje recogido msgHanlder (x)" << std::endl;
	if (this->_users[socketFd]->getAuthenticated() == true)
		parseMsg(socketFd, this->_message);
	else if (this->_message.find("CAP LS") != std::string::npos)
		hexChatLogin(socketFd);
	else
		ncLogin(socketFd);
}

/**
 * @brief When login using HexChat, this client sends 3 separate messages at start
 * 	-three consecutive poll events in the same socket-
 * 		#1 = "CAP LS 302\n" - user->_hexChat = TRUE
 *		#2 = "PASS <password>\n" - checked in server.checkPassHexChat()
 * 		#3 = "NICK pgomez-r\nUSER pgomez-r 0 * :realname\n" - parsed in user.hexChatUser() 
 *	(!)Hexchat needs to have the server password in the network config,
 *		otherwise, it won't send message #2
 */
void	Server::hexChatLogin(int socketFd)
{
	std::cout << "Entramos en hexChatLogin" << std::endl;
	this->_users[socketFd]->setHexClient(true);
	this->_message.clear();
	if (!readFromSocket(socketFd, this->_message))
		return (deleteUser(socketFd));
	std::cout << "Mensaje recogido en hexChatLogin" << std::endl;
	if (checkHexChatPass(socketFd))
	{
		std::cout << "Pasamos hexChatPass" << std::endl;
		this->_message.clear();
		if (!readFromSocket(socketFd, this->_message))
			return (deleteUser(socketFd));
		std::cout << "Mensaje recogido después de hexChatLogin" << std::endl;
		this->_users[socketFd]->hexChatUser(this->_message);
		sendWarning(socketFd, ":MyServer 001 * :Welcome to the Pollitas Internet Relay Network\n");
	}
}

void	Server::ncLogin(int userFd)
{
	//welcome message - usage?
	if (!loginFormat(this->_message))
	{
		std::cout << "New connection with socket fd " << userFd << " tried to login with wrong login format" << std::endl;
		std::cout << RED << "Connection rejected and socket closed" << RES << std::endl;
		sendWarning(userFd, "Wrong format for login authentication, your are being disconnected\n");
		deleteUser(userFd);
	}
	else
		checkPass(userFd);
}

void	Server::parseMsg(int userFd, std::string msg)
{
	//Later on, we will first handle user->_buffer before handling the command
	//	this->_users[userFd].updateBuffer(msg);
	//	if (this->_users[userFd].getBuffer().find(\n, \r...) != npos)
	//This is only an example, we will find a way to do it correctly later, now just using msg
	Command	cmd(userFd, msg);
	cmd.checkCmd(userFd, msg);
}

bool Server::channelExists(const std::string& name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == name)
			return (true);
	}
	return (false);
}