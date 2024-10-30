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
	char		buffer[BUFF_SIZE];
	int			read_bytes;

	this->_message.clear();
	read_bytes = read(socketFd, buffer, sizeof(buffer) - 1);
	if (read_bytes <= 0)
	{
		if (read_bytes == 0)
			std::cout << RED << "User disconnected" << RES << std::endl;
		else
			perror("read");
		deleteUser(socketFd);
		return ;
	}
	buffer[read_bytes] = '\0';
	this->_message = buffer;
	if (this->_message.find("CAP LS") != std::string::npos)
		this->_users[socketFd]->setHexClient(true);
	else if (this->_users[socketFd]->getHexClient() && !this->_users[socketFd]->getAuthenticated())
		this->_users[socketFd]->setHexStat(checkHexChatPass(socketFd));
	else if (this->_users[socketFd]->getHexStat() && this->_users[socketFd]->getAuthenticated())
	{
		this->_users[socketFd]->hexChatUser(this->_message);
		this->_users[socketFd]->setHexStat(false);
		sendWarning(socketFd, ":MyServer 001 * :Welcome to the Pollitas Internet Relay Network\n");
	}
	else if (!firstMessage(socketFd, this->_message))
		parseMsg(socketFd, this->_message);
}

bool	Server::firstMessage(int userFd, std::string msg)
{

	if (this->_users[userFd]->getAuthenticated() == true)
		return (false);
	welcomeUser(userFd);
	if (!loginFormat(msg))
	{
		std::cout << "New connection with socket fd " << userFd << " tried to login with wrong login format" << std::endl;
		std::cout << RED << "Connection rejected and socket closed" << RES << std::endl;
		sendWarning(userFd, "Wrong format for login authentication, your are being disconnected\n");
		deleteUser(userFd);
	}
	else
		checkPass(userFd);
	return (true);
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