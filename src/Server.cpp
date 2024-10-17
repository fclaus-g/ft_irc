
#include "ft_irc.hpp"

void Server::start()
{
	prepareSocket();
	run();
}

void Server::prepareSocket()
{
	struct sockaddr_in 	server_addr;
	struct pollfd 		poll_fd;
	int 				opt = 1;

	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_fd < 0)
	{
		perror("socket");
		return;
	}
	if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
	{
		perror("setsockopt");
		close(this->server_fd);
		return;
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(this->port);
	if (bind(this->server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		close(this->server_fd);
		return;
	}
	if (listen(this->server_fd, SOMAXCONN) < 0)
	{
		perror("listen");
		close(this->server_fd);
		return;
	}
	poll_fd.fd = this->server_fd;
	poll_fd.events = POLLIN;
	poll_fd.revents = 0;
	_socketsPoll.push_back(poll_fd);
	this->isRunning = true;
	std::cout << *this << std::endl;
}

void	Server::run()
{
	while (this->isRunning)
	{
		if ((poll(&_socketsPoll[0], _socketsPoll.size(), -1)) < 0)
			throw std::runtime_error("poll");
		for (size_t i = 0; i < _socketsPoll.size(); i++)
		{
			if (_socketsPoll[i].revents & POLLIN)
			{
				if (_socketsPoll[i].fd == server_fd)
					newConnection();
				else
					msgHandler(_socketsPoll[i].fd);
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

	client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
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
	_socketsPoll.push_back(new_pollfd);
	_users[client_socket] = new User(client_socket);
	std::cout << GRE << "New connection established with socket fd " << client_socket << RES << std::endl;
	welcomeUser(client_socket);
}

void	Server::msgHandler(int socketFd)
{
	char		buffer[BUFF_SIZE];
	std::string	msg;
	int			read_bytes;

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
	msg = buffer;
	if (!firstMessage(socketFd, msg))
	{
		parseMsg(socketFd, msg);
	}
}

void	Server::parseMsg(int userFd, std::string msg)
{
	if (!checkCmd(userFd, msg))
	{
		std::string	user_msg = "@" + this->_users[userFd]->getNick() + ": " + msg;

		for (size_t i = 0; i < this->_socketsPoll.size(); i++)
		{
			if (this->_socketsPoll[i].fd != this->server_fd
				&& this->_socketsPoll[i].fd != userFd)
			{
				send(this->_socketsPoll[i].fd, user_msg.c_str(), user_msg.length(), 0);
			}
		}
	}
}

bool Server::checkCmd(int userFd, std::string msg)
{
	std::string commands[9] = {"USER", "NICK", "JOIN", "QUIT",
		"PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"};

	for (int i = 0; commands[i] != ""; i++)
	{
		if (msg.find(commands[i]) == 0)
		{
			runCmd(userFd, commands[i], msg);
			return (true);
		}
	}
	return (false);
}

void	Server::runCmd(int userFd, std::string key, std::string msg)
{
	std::string commands[9] = {"USER", "NICK", "JOIN", "QUIT",
		"PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"};
	int i = -1;

	while (commands[++i] != "")
	{
		if (commands[i] == key)
			break ;
	}
	switch (i)
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
