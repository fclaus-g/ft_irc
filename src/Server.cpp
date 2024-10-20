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
	welcomeUser(client_socket);
}

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
	std::cout << "RAW MESSAGE = " << this->_message << std::endl;
	if (this->_message.find("CAP LS") != std::string::npos)
	{
		this->_users[socketFd]->setHexClient(true);
		return ;
	}
	else if (this->_users[socketFd]->getHexClient() && !this->_users[socketFd]->getAuthenticated())
	{
		this->_users[socketFd]->hexChatLogin(this->_message);
		this->_users[socketFd]->setAuthenticated(true);
		return ;
	}
	else if (!firstMessage(socketFd, this->_message))
		parseMsg(socketFd, this->_message);
}

bool	Server::firstMessage(int userFd, std::string msg)
{
	if (this->_users[userFd]->getAuthenticated() == true)
		return (false);
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
	if (!checkCmd(userFd, msg))
	{
		std::string	user_msg = "@" + this->_users[userFd]->getNick() + ": " + msg;

		for (size_t i = 0; i < this->_fds.size(); i++)
		{
			if (this->_fds[i].fd != this->_serverFd
				&& this->_fds[i].fd != userFd)
			{
				send(this->_fds[i].fd, user_msg.c_str(), user_msg.length(), 0);
			}
		}
	}
}

void	Server::runCmd(int userFd, int key, std::string msg)
{
	(void)userFd;
	(void)msg;
	switch (key)
	{
		case USER:
			break;
		case NICK:
			break;
		case JOIN:
			break;
		case QUIT:
			break;
		case PRIVMSG:
			break;
		case KICK:
			break;
		case INVITE:
			break;
		case TOPIC:
			break;
		case MODE:
			break;
		default:
			break;
	}
}
