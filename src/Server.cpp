#include "../inc/Server.hpp"

Server::Server()
{
	this->_serverFd = -1;
	//PENDIENTE DE VER SI HAY QUE TENER CONSTRUCTOR POR DEFECTO O NO
}
Server::Server(int port, std::string password)
{
	this->_port = port;
	this->_password = password;
	this->_name = "MyServer";
	this->_isRunning = false;
	this->_serverFd = -1;
	_initCommands();

}

void	Server::_initCommands()
{
	this->_commands[0] = "USER";
    this->_commands[1] = "NICK";
    this->_commands[2] = "JOIN";
    this->_commands[3] = "QUIT";
    this->_commands[4] = "PRIVMSG";
    this->_commands[5] = "KICK";
    this->_commands[6] = "INVITE";
    this->_commands[7] = "TOPIC";
    this->_commands[8] = "MODE";
}

Server::~Server()
{
}

/*-----------------------[SETTER]------------------------*/

/*-----------------------[GETTER]------------------------*/
int Server::getPort() const
{
	return this->_port;
}
std::string Server::getName() const
{
	return this->_name;
}
bool Server::getIsRunning() const
{
	return this->_isRunning;
}

/*-----------------------[METHODS]------------------------*/
void Server::start()
{
	prepareSocket();
	run();
	stop();
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

void Server::stop()
{
	this->_isRunning = false;
	for (size_t i = 1; i < _fds.size(); i++)
		close(_fds[i].fd);
	std::cout << "Server stopped" << std::endl;
}
void Server::prepareSocket()
{
	struct sockaddr_in server_addr;//server address struct
	struct pollfd poll_fd;//poll struct
	
	memset(&server_addr, 0, sizeof(server_addr));//initialize the server address struct
	server_addr.sin_family = AF_INET;// Set the address family to IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;// Bind to all available interfaces
	server_addr.sin_port = htons(this->_port);// Convert the port to network byte order// Create the server socket
	
	this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverFd < 0)
	{
		perror("socket");
		return;
	}
	// Set the socket options
	int opt = 1;
	//NONBLOCK: Set the O_NONBLOCK file status flag on the open file description
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
	if (listen(this->_serverFd, SOMAXCONN) < 0)//SOMAXCONN is the maximum number of pending connections
	{
		perror("listen");
		close(this->_serverFd);
		return;
	}
	poll_fd.fd = this->_serverFd;//file descriptor of the socket
	poll_fd.events = POLLIN;//POLLIN: There is data to read
	poll_fd.revents = 0;//revents is an output parameter, filled by the kernel with the events that actually occurred
	_fds.push_back(poll_fd);//add the server socket to the pollfd vector

	this->_isRunning = true;
	std::cout << *this << std::endl;
}

void Server::printMap(const std::map<int, User>& map)
{
	for (std::map<int, User>::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		std::cout << "Key: " << it->first << std::endl;
		std::cout << it->second << std::endl;
	}
}

void Server::checkCommand(User user)
{
	std::string commands[9] = {"USER", "NICK", "JOIN", "QUIT", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"};
	size_t iPos = this->_message.find_first_not_of(" \t");
	size_t fPos = this->_message.find_first_of(" \t", iPos);
	std::string command = this->_message.substr(iPos, fPos - iPos);
	size_t i = -1;
	while(commands[++i] != "")
		if (command == commands[i])
			break;
	if (i < commands->size())
		this->_message = this->_message.substr(fPos + 1);
	switch (i)
	{
		case USER:
			this->commandUser(user);
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

void Server::commandUser(User user) //Modificar para que acepte USER jsaavedr 0 * :realname
{
	std::string username;
	std::string realname;

	size_t iPos = this->_message.find_first_not_of(" \t");
	size_t fPos = this->_message.find_first_of(" \t", iPos);
	if (fPos == std::string::npos)
	{
		std::cout << "Error: Not enough parameters" << std::endl;
		return;
	}
	username = this->_message.substr(iPos, fPos - iPos);

	this->_message = this->_message.substr(fPos + 1);
	iPos = this->_message.find_first_not_of(" \t", fPos);
	fPos = this->_message.find_first_of(" \t", iPos);
	realname = this->_message.substr(iPos, fPos - iPos);

	this->_message.clear();
	(void)iPos;
	(void)fPos;
	(void)user;
}

void Server::commandNick(User user)
{
	size_t iPos = this->_message.find_first_not_of(" \t");

	(void)iPos;
	(void)user;

}

void Server::commandJoin(User user)
{
	size_t iPos = this->_message.find_first_not_of(" \t");
	size_t fPos = this->_message.find_first_of(" \t", iPos);

	std::string channel = this->_message.substr(iPos, fPos - iPos);
	if (channel[0] != '#')
	{
		std::cout << "Error: Invalid channel name" << std::endl;
		return;
	}
	std::vector<Channel> channels = this->_channels;
	for(std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getName() == channel)
		{
			it->addUserChannel(user);
			return;
		}
	}
	this->createChannel(channel);
	this->addUserToChannel(channel, user);
}

void Server::commandQuit(User user)
{
	(void)user;
}

void Server::commandPrivmsg(User user)
{
	(void)user;
}

void Server::commandKick(User user)
{
	(void)user;
}

void Server::commandInvite(User user)
{
	(void)user;
}

void Server::commandTopic(User user)
{
	(void)user;
}

void Server::commandMode(User user)
{
	(void)user;
}

void Server::signalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
	{
		std::cout << "Signal received: " << signal << std::endl;
		throw std::runtime_error("Server stopped by signal");
	}
}

void Server::createChannel(const std::string& name)
{
	if (name == "")
	{
		std::cout << "Channel name can't be empty" << std::endl;
		return;
	}
	if (name[0] != '#')
	{
		std::cout << "Channel name must start with #" << std::endl;
		return;
	}
	if (name.size() < 2)
	{
		std::cout << "Channel name must have at least 2 characters" << std::endl;
		return;
	}
	if (name.size() > 50)
	{
		std::cout << "Channel name must have at most 50 characters" << std::endl;
		return;
	}
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == name)
		{
			std::cout << "Channel already exists" << std::endl;
			return;
		}
	}
	Channel *newChannel = new Channel(name);
	this->_channels.push_back(*newChannel);
	printVector(_channels);
	std::cout << "Channel created" << std::endl;
}

void Server::printVector(const std::vector<Channel>& vector)
{
	for (size_t i = 0; i < vector.size(); i++)
	{
		std::cout << vector[i] << std::endl;
	}
}

void Server::addUserToChannel(const std::string& channelName, User& user)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == channelName)
		{
			this->_channels[i].addUserChannel(user);
			std::cout << "User added to channel" << std::endl;
			send(user.getFd(), "You have been added to the channel\n", strlen("You have been added to the channel\n"), 0);
			return;
		}
	}
	std::cout << "Channel not found" << std::endl;
}

void Server::removeChannel(const std::string& name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == name)
		{
			this->_channels.erase(this->_channels.begin() + i);
			std::cout << "Channel removed" << std::endl;
			return;
		}
	}
	std::cout << "Channel not found" << std::endl;
}

std::ostream& operator<<(std::ostream& out, const Server& server)
{
	out << GRE << "Server: " << server.getName()<< RES << std::endl;
	out << "Port :" << server.getPort() << std::endl;
	if(server.getIsRunning())
	{
		out << "Server is running" << std::endl;
	}
	else
	{
		out << "Server is not running" << std::endl;
	}
	return out;
}

void Server::welcomeUser(int userFd)
{
	std::string msg;
	msg = "Wecolme to The Pollitas Server!\n";
	msg.append("Insert user nick and server password\n");
	msg.append("Usage \"NICK <your_nickname> PASS <server_password>\"\n");
	send(userFd, msg.c_str(), msg.length(), 0);
}

void	Server::deleteUser(int socketFd)
{
	std::vector<struct pollfd>::iterator i = this->_fds.begin();

	while (i != this->_fds.end())
	{
		if (i->fd == socketFd)
		{
			_fds.erase(i);
			break ;
		}
		i++;
	}
	delete (this->_users[socketFd]);
	this->_users.erase(socketFd);
	close(socketFd);
}

void	Server::sendWarning(int userFd, std::string str)
{
	std::string	msg = "";
	msg.append(str);
	send(userFd, msg.c_str(), msg.length(), 0);

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

bool	Server::loginFormat(std::string msg)
{
	size_t		nick_pos = msg.find("NICK ");
	size_t		pass_pos = msg.find(" PASS ");

	this->_tempNick.clear();
	this->_tempPass.clear();
	if (nick_pos == std::string::npos || pass_pos == std::string::npos)
		return (false);
	if (nick_pos != 0 || nick_pos > pass_pos)
		return (false);
	this->_tempNick = msg.substr(nick_pos + 5, pass_pos - (nick_pos + 5));
	this->_tempPass = msg.substr(pass_pos + 6);
	if (this->_tempNick.empty() || this->_tempPass.empty())
		return (false);
	if (this->_tempNick.find(" ") != std::string::npos
		|| this->_tempPass.find(" ") != std::string::npos)
			return (false);
	return (true);
}

void	Server::checkPass(int userFd)
{
	if (this->_tempPass[this->_tempPass.length() - 1] == '\n')
		this->_tempPass.erase(this->_tempPass.length() - 1);
	if (this->_tempPass != this->_password)
	{
		std::cout << "New user with socket fd " << userFd << " tried to login with wrong password" << std::endl;
		std::cout << RED << "Connection rejected and socket closed" << RES << std::endl;
		sendWarning(userFd, "Wrong password, your are being disconnected\n");
		deleteUser(userFd);
	}
	else
	{
		this->_users[userFd]->setNick(this->_tempNick);
		this->_users[userFd]->setAuthenticated(true);
		std::cout << GRE << "New user with socket fd " << userFd << " has joined the server" << RES << std::endl;
		sendWarning(userFd, "Login succesfull\n");
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
	if (!firstMessage(socketFd, this->_message))
		parseMsg(socketFd, this->_message);
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

bool Server::checkCmd(int userFd, std::string msg)
{
	for (int i = 0; i < TOTAL; i++)
	{
		if (msg.find(_commands[i]) == 0)
		{
			runCmd(userFd, i, msg);
			return (true);
		}
	}
	return (false);
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