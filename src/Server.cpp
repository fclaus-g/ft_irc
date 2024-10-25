#include "ft_irc.hpp"

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
	//std::cout << "Command: " << command << std::endl;
	//std::cout << user << std::endl;
	//std::cout << "Message: OUT OF COMMAND "<< std::endl;
}

void Server::commandUser(User& user) //Modificar para que acepte USER jsaavedr 0 * :realname
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

void Server::commandNick(User& user)
{
	size_t iPos = this->_message.find_first_not_of(" \t");

	(void)iPos;
	(void)user;

}

void Server::commandJoin(User& user)
{
	std::cout << "Command JOIN" << std::endl;
	//std::cout << user << std::endl;
	size_t iPos = this->_message.find_first_not_of(" \t");
	size_t fPos = this->_message.find_first_of(" \t", iPos);
	//std::cout << iPos << fPos << std::endl;

	//std::cout << this->_message.substr(fPos, this->_message.size() - 1); 
	/*LA SIGUIENTE LINEA DEBE LLEGAR HASTA EL SIGUIENTE ESPACIO NO HASTA EL FINAL*/
	std::string channel = this->_message.substr(fPos + 1, this->_message.size() - 1);
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
			this->addUserToChannel(channel, user);
			return;
		}
	}
	this->createChannel(channel);
	this->addUserToChannel(channel, user);
	//std::cout << "post add user to channel" << std::endl;
	//std::cout << user << "------" << std::endl;
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
	this->_channelsMap[name] = *newChannel;
	//printVector(_channels); for check the vector channel is created correctly
	std::cout << "Channel created" << std::endl;
}

void Server::printVector(const std::vector<Channel>& vector)
{
	for (size_t i = 0; i < vector.size(); i++)
	{
		std::cout << YEL << vector[i] << RES << std::endl;
	}
}

void Server::addUserToChannel(const std::string& channelName, User& user)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == channelName)
		{
			this->_channels[i].addUserChannel(user);
			std::cout<< GRE << "User added to channel " << channelName << std::endl;
			std::string  message = "You have been added to the channel " + channelName + "\n";
			send(user.getFd(), message.c_str(), message.size(), 0);
			return;
		}
		// if (i == this->_channels.size() - 1)
		// {
		// 	std::cout << "Channel not found" << std::endl;
		// 	send(user.getFd(), "Channel not found\n", strlen("Channel not found\n"), 0);
		// }
	}
}

void Server::removeChannel(const std::string& name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].getName() == name)
		{
			this->_channels.erase(this->_channels.begin() + i);
			this->_channelsMap.erase(name);
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
	{
		std::cout << "User with socket fd " << userFd << " is already authenticated" << std::endl;
		return (false);
	}
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
	std::cout << RED << "RAW MESSAGE = " << RES << this->_message << std::endl;
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
	else if (this->_users[socketFd]->getHexClient() && this->_users[socketFd]->getAuthenticated())
	{
		this->_users[socketFd]->hexChatUser(this->_message);
		std::cout << "este es el mensje NICK" << std::endl;
	}	
}

void	Server::parseMsg(int userFd, std::string msg)
{
	std::cout << "Parsing message" << std::endl;
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

/**
 * @brief When login using HexChat, this client sends 3 separate messages at start
 * 	-three consecutive poll events in the same socket- this method checks #2 to see
 * 	if the password sent by the client matches the one we set for the server
 * 		#1 = "CAP LS 302\n" - skipeed first time and user->_hexChat = TRUE
 *		#2 = "PASS <password>\n" - checked in server.checkPassHexChat()
 * 		#3 = "NICK pgomez-r\nUSER pgomez-r 0 * :realname\n" - let's do it!
 *	(!)Hexchat needs to have the server password in the network config,
 *		otherwise, it won't send message #2
 */
bool	Server::checkHexChatPass(int socketFd)
{
	std::string	pass;
	bool		verified = true;

	if (this->_message.find("PASS ") != 0)
		verified = false;
	if (verified)
	{
		pass = this->_message.substr(this->_message.find("PASS") + 5);
		pass.erase(pass.find_last_not_of(" \n\r\t") + 1);
		if (pass != this->_password)
			verified = false;
	}
	if (!verified)
	{
		std::cout << "New HexChat connection with socket fd " << socketFd << " tried to login with wrong password or whithout any" << std::endl;
		std::cout << RED << "Connection rejected and socket closed" << RES << std::endl;
		sendWarning(socketFd, ":MyServer 464 * :Password incorrect\n");
		deleteUser(socketFd);
		return (false);
	}
	this->_users[socketFd]->setAuthenticated(true);
	return (true);
}

bool Server::checkCmd(int userFd, std::string msg)
{
	std::cout << "Checking command" << std::endl;
	for (int i = 0; i < TOTAL; i++)
	{
		//std::cout << "Checking command " << _commands[i] << std::endl;
		if (msg.find(_commands[i]) == 0)
		{
			std::cout << "Command found" << std::endl;
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
			commandJoin(*_users[userFd]);
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
			std::cout << RED << "MODE" << RES << std::endl;
			break;
		default:
			break;
	}
}
