#include "../inc/Server.hpp"
Server::Server()
{
	this->server_fd = -1;
	//PENDIENTE DE VER SI HAY QUE TENER CONSTRUCTOR POR DEFECTO O NO
}
Server::Server(int port, std::string password)
{
	std::cout << "Server object created" << std::endl;
	this->port = port;
	this->password = password;
	this->name = "MyServer";
	this->isRunning = false;
}

Server::~Server()
{
	std::cout << "Server object destroyed" << std::endl;
}

/*-----------------------[SETTER]------------------------*/

/*-----------------------[GETTER]------------------------*/
int Server::getPort() const
{
	return this->port;
}
std::string Server::getName() const
{
	return this->name;
}
bool Server::getIsRunning() const
{
	return this->isRunning;
}

/*-----------------------[METHODS]------------------------*/
void Server::start()
{
	prepareSocket();
	while (this->isRunning)
	{
		if ((poll(&fds[0], fds.size(), -1)) < 0)
			throw std::runtime_error("poll");
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == server_fd)
					acceptClient();
				else
					readClient(fds[i].fd);
					std::cout << "Client reading" << std::endl;
			}
		}
	}
	close(server_fd);
	//EL SERVER SE QUEDA ESCUCHANDO, NECESITAMOS UNA FUNCION PARA QUE ACEPTE CONEXIONES
}

void Server::stop()
{
	this->isRunning = false;
	std::cout << "Server stopped" << std::endl;
}
void Server::prepareSocket()
{
	struct sockaddr_in server_addr;//server address struct
	struct pollfd poll_fd;//poll struct
	
	memset(&server_addr, 0, sizeof(server_addr));//initialize the server address struct
	server_addr.sin_family = AF_INET;// Set the address family to IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;// Bind to all available interfaces
	server_addr.sin_port = htons(this->port);// Convert the port to network byte order// Create the server socket
	
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_fd < 0)
	{
		perror("socket");
		return;
	}
	// Set the socket options
	int opt = 1;
	//NONBLOCK: Set the O_NONBLOCK file status flag on the open file description
	if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
	{
		perror("setsockopt");
		close(this->server_fd);
		return;
	}
	if (fcntl(this->server_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("fcntl");
		close(this->server_fd);
		return;
	}
	if (bind(this->server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		close(this->server_fd);
		return;
	}
	if (listen(this->server_fd, SOMAXCONN) < 0)//SOMAXCONN is the maximum number of pending connections
	{
		perror("listen");
		close(this->server_fd);
		return;
	}
	poll_fd.fd = this->server_fd;//file descriptor of the socket
	poll_fd.events = POLLIN;//POLLIN: There is data to read
	poll_fd.revents = 0;//revents is an output parameter, filled by the kernel with the events that actually occurred
	fds.push_back(poll_fd);//add the server socket to the pollfd vector

	this->isRunning = true;
	std::cout << "Server listening on port " << this->port << std::endl;
	std::cout << "Server started" << std::endl;
}
void Server::acceptClient()
{
	int client_fd;
	char buffer[1024];
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
    // Accept a connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return ;
    }
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
		std::cout << "Error setting non-blocking" << std::endl;
	
	struct pollfd client_poll_fd;
	
	client_poll_fd.fd = client_fd;
	client_poll_fd.events = POLLIN;
	client_poll_fd.revents = 0;
	fds.push_back(client_poll_fd);//add the client socket to the pollfd vector
	
	clients[client_fd] = "";
	std::cout << "Received message: " << buffer << std::endl;
    //close(client_fd);
}

void Server::readClient(int client_fd)
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
