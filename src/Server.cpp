#include "../inc/Server.hpp"

Server::Server()
{
	this->_serverFd = -1;
	//PENDIENTE DE VER SI HAY QUE TENER CONSTRUCTOR POR DEFECTO O NO
}
Server::Server(int port, std::string password)
{
	std::cout << "Server object created" << std::endl;
	this->_port = port;
	this->_password = password;
	this->_name = "MyServer";
	this->_isRunning = false;
}

Server::~Server()
{
	std::cout << "Server object destroyed" << std::endl;
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

const std::map<int, User>& Server::getUsers() const
{
	return this->_users;
}
/*-----------------------[METHODS]------------------------*/
void Server::start()
{
	prepareSocket();
	while (this->_isRunning)
	{
		if ((poll(&_fds[0], _fds.size(), -1)) < 0)
			throw std::runtime_error("poll");
		for (size_t i = 0; i < _fds.size(); i++)//iterate over the pollfd vector
		{
			if (_fds[i].revents & POLLIN)//POLLIN: There is data to read
			{
				if (_fds[i].fd == _serverFd)
					acceptUser();
				else
				{
					readUser(_fds[i].fd);
					std::cout << "Client reading" << std::endl;
				}
			}
		}
	}
	close(_serverFd);
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
	std::cout << "Server listening on port " << this->_port << std::endl;
	std::cout << "Server started" << std::endl;
}
void Server::acceptUser()
{
	int userFd;
	struct sockaddr_in user_addr;
	socklen_t user_addr_len = sizeof(user_addr);
    // Accept a connection
    userFd = accept(_serverFd, (struct sockaddr *)&user_addr, &user_addr_len);
    if (userFd < 0) {
        perror("accept");
        close(_serverFd);
        return ;
    }
	if (fcntl(userFd, F_SETFL, O_NONBLOCK) < 0)
		std::cout << "Error setting non-blocking" << std::endl;
	std::cout << "Client connected" << std::endl;
	struct pollfd user_poll_fd;//poll struct
	
	user_poll_fd.fd = userFd;
	user_poll_fd.events = POLLIN;
	user_poll_fd.revents = 0;
	_fds.push_back(user_poll_fd);//add the client socket to the pollfd vector
	//clients[client_fd] = "";//add the client socket to the clients map
	addUser(userFd, user_addr);//add the client to the clients map
    //close(client_fd);
}

void Server::addUser(int userFd, struct sockaddr_in user_addr)
{
	User newUser;
	std::cout << "Adding user--------constructo 1-------------------" << std::endl;

	newUser.setFd(userFd);
	newUser.setIp(inet_ntoa(user_addr.sin_addr));
	newUser.setNick("");
	newUser.setUserName("");
	newUser.setRealName("");
	newUser.setAuthenticated(false);
	_users[userFd] = newUser;//add the client to the clients map
	std::cout << "Adding user--------constructo 2-------------------" << std::endl;
	std::cout << _users[userFd] << "imprimido" << std::endl;
	std::cout << "User added" << std::endl;
	//printMap(_users);
}

void Server::printMap(const std::map<int, User>& map)
{
	for (std::map<int, User>::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		std::cout << "Key: " << it->first << std::endl;
		std::cout << it->second << std::endl;
	}
}

void Server::removeUser(int userFd)
{
	_users.erase(userFd);
	_fds.erase(_fds.begin() + userFd);
	std::cout << "User removed" << std::endl;
	//MUY SIMPLE LO VEO QUIZAS HAY QUE RECORRER LOS CONTENEDORES Y COMPROBAR SI ES IGUAL 
//ANTES PARA LUEGO BORRARLO
}

void Server::readUser(int client_fd)
{
	char buffer[1024];
	int bytes_read;

	memset(buffer, 0, sizeof(buffer));
	bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_read <= 0)
	{
		perror("recv");
		std::cout << "Client " << client_fd << " disconnected" << std::endl;
		//AQUI HAY QUE ELIMINAR EL CLIENTE DE LA LISTA DE CLIENTES Y DE LA LISTA DE POLL
		close(client_fd);
		return;
	}
	else
	{
		buffer[bytes_read] = '\0';
		std::cout << "Received message: " << buffer << std::endl;
		this->_message = buffer;
		checkCommand(client_fd);
		std::cout << "From client: " << client_fd << std::endl;
		std::cout << this->_users[client_fd] << std::endl;
		send(client_fd, "Su mensaje ha sido resibido\n", strlen("Su mensaje ha sido resibido\n"), 0);
	}
}

void Server::checkCommand(int userFd)
{
	std::string commands[9] = {"USER", "NICK", "JOIN", "QUIT", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"};
	std::cout << "Checking command: " << this->_message << std::endl;
	size_t iPos = this->_message.find_first_not_of(" \t");
	size_t fPos = this->_message.find_first_of(" \t", iPos);
	std::string command = this->_message.substr(iPos, fPos - iPos);
	std::cout << "Command: " << command << std::endl;
	size_t i = -1;
	while(commands[++i] != "")
		if (command == commands[i])
			break;
	if (i < commands->size())
		this->_message = this->_message.substr(fPos + 1);
	std::cout << "Message: " << this->_message << std::endl;
	switch (i)
	{
		case USER:
			this->commandUser(userFd);
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

void Server::commandUser(int userFd) //Modificar para que acepte USER jsaavedr 0 * :realname
{
	std::string username;
	std::string realname;

	std::cout << "Message USER: " << this->_message << std::endl;
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

	this->_users[userFd].setUserName(username);
	this->_users[userFd].setRealName(realname);
	this->_message.clear();
}

void Server::commandNick(int userFd)
{
	size_t iPos = this->_message.find_first_not_of(" \t");

	this->_users[userFd].setNick(this->_message.substr(iPos));
}

void Server::commandJoin(int userFd)
{
	size_t iPos = this->_message.find_first_not_of(" \t");
	size_t fPos = this->_message.find_first_of(" \t", iPos);

	std::string channel = this->_message.substr(iPos, fPos - iPos);
	if (channel[0] != '#')
	{
		std::cout << "Error: Invalid channel name" << std::endl;
		return;
	}
	channel.erase(0, 1);
	std::vector<Channel> channels = this->_channels;
	for(std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getName() == channel)
		{
			it->addUser(userFd);
			return;
		}
	}
	Channel newChannel(channel);
	newChannel.addUser(userFd);
	this->_channels.push_back(newChannel);
}

void Server::commandQuit(int userFd)
{
	(void)userFd;
}

void Server::commandPrivmsg(int userFd)
{
	(void)userFd;
}

void Server::commandKick(int userFd)
{
	(void)userFd;
}

void Server::commandInvite(int userFd)
{
	(void)userFd;
}

void Server::commandTopic(int userFd)
{
	(void)userFd;
}

void Server::commandMode(int userFd)
{
	(void)userFd;
}

void Server::signalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
	{
		std::cout << "Signal received: " << signal << std::endl;
		throw std::runtime_error("Server stopped by signal");
	}
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
