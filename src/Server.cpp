#include "../inc/Server.hpp"

Server::Server()
{
	this->_server_fd = -1;
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
				if (_fds[i].fd == _server_fd)
					acceptUser();
				else
					readUser(_fds[i].fd);
					std::cout << "Client reading" << std::endl;
			}
		}
	}
	close(_server_fd);
	//EL SERVER SE QUEDA ESCUCHANDO, NECESITAMOS UNA FUNCION PARA QUE ACEPTE CONEXIONES
}

void Server::stop()
{
	this->_isRunning = false;
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
	
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_server_fd < 0)
	{
		perror("socket");
		return;
	}
	// Set the socket options
	int opt = 1;
	//NONBLOCK: Set the O_NONBLOCK file status flag on the open file description
	if (setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
	{
		perror("setsockopt");
		close(this->_server_fd);
		return;
	}
	if (fcntl(this->_server_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("fcntl");
		close(this->_server_fd);
		return;
	}
	if (bind(this->_server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		close(this->_server_fd);
		return;
	}
	if (listen(this->_server_fd, SOMAXCONN) < 0)//SOMAXCONN is the maximum number of pending connections
	{
		perror("listen");
		close(this->_server_fd);
		return;
	}
	poll_fd.fd = this->_server_fd;//file descriptor of the socket
	poll_fd.events = POLLIN;//POLLIN: There is data to read
	poll_fd.revents = 0;//revents is an output parameter, filled by the kernel with the events that actually occurred
	_fds.push_back(poll_fd);//add the server socket to the pollfd vector

	this->_isRunning = true;
	std::cout << "Server listening on port " << this->_port << std::endl;
	std::cout << "Server started" << std::endl;
}
void Server::acceptUser()
{
	int user_fd;
	struct sockaddr_in user_addr;
	socklen_t user_addr_len = sizeof(user_addr);
    // Accept a connection
    user_fd = accept(_server_fd, (struct sockaddr *)&user_addr, &user_addr_len);
    if (user_fd < 0) {
        perror("accept");
        close(_server_fd);
        return ;
    }
	if (fcntl(user_fd, F_SETFL, O_NONBLOCK) < 0)
		std::cout << "Error setting non-blocking" << std::endl;
	std::cout << "Client connected" << std::endl;
	struct pollfd user_poll_fd;//poll struct
	
	user_poll_fd.fd = user_fd;
	user_poll_fd.events = POLLIN;
	user_poll_fd.revents = 0;
	_fds.push_back(user_poll_fd);//add the client socket to the pollfd vector
	//clients[client_fd] = "";//add the client socket to the clients map
	addUser(user_fd, user_addr);//add the client to the clients map
    //close(client_fd);
}

void Server::addUser(int user_fd, struct sockaddr_in user_addr)
{
	User newUser;

	newUser.setFd(user_fd);
	newUser.setIp(inet_ntoa(user_addr.sin_addr));
	//PENDIENTE DE RELLENAR TODOS LOS ATRIBUTOS DE CLIENT
	_users[user_fd] = newUser;//add the client to the clients map
	std::cout << "User added" << std::endl;
	
}

void Server::readUser(int client_fd)
{
	char buffer[1024];
	int bytes_read;

	memset(buffer, 0, sizeof(buffer));
	bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_read < 0)
	{
		perror("recv");
		close(client_fd);
		return;
	}
	if (bytes_read == 0)
	{
		close(client_fd);
		return;
	}
	std::cout << "Received message: " << buffer << std::endl;
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
