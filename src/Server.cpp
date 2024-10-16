#include "../inc/Server.hpp"

Server::Server()
{

}

Server::Server(Server const &s)
{
	if (this != &s)
		*this = s;
}

Server& Server::operator=(Server const &s)
{
	if (this != &s)
	{
		this->_socket = s._socket;
		this->_port = s._port;
		this->_pass = s._pass;
	}
	return (*this);
}

Server::Server(int port, std::string pass)
{
	struct sockaddr_in	serverAddress;
	
	this->_port = port;
	this->_pass = pass;
	this->_serverName = "MyServer";
	
	//Create socket
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == -1)
		throw std::runtime_error("ERROR: Can't create socket");
	else //Debug only
		std::cout << "Socket has been created" << std::endl;
	
	//Setup the struct
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);

	//Binding
	if (bind(this->_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		close(this->_socket);
		throw std::runtime_error("ERROR: Address is already in use");
	}
	else //Debug only
		std::cout << "Socket has been binded" << std::endl;
	//Server not blocking
	fcntl(this->_socket, F_SETFL, O_NONBLOCK);

	//Listen
	if (listen(this->_socket, MAX_CONNECT_NBR) < 0)
	{
		close(this->_socket);
		throw std::runtime_error("ERROR: Can't listen on a socket");
	}
	else //Debug only
		std::cout << "Socket is listening" << std::endl;

	std::cout << "SERVER is running... Port: " << this->_port;
	std::cout << ", password: " << this->_pass << std::endl;
}

Server::~Server()
{
	close(this->_socket);
	//more code
	/*
	Eliminar lista/vector de canales y de usuarios
	*/
}

int Server::getPort() const
{
	return (this->_port);
}

std::string Server::getName() const
{
	return (this->_serverName);
}
		
bool Server::getIsRunning() const
{
	return (this->_isRunning);
}
		
/*
std::map<int, User *> &Server::getUsers() const
{
	return (this->_users);
}
*/

void Server::startIRCServer()
{
	struct pollfd	poll_fd;

	poll_fd.fd = this->_socket;
	poll_fd.events = POLLIN;
	poll_fd.revents = 0;
	this->_fds.push_back(poll_fd);
	this->_isRunning = true;
	while (this->_isRunning)
	{
		if ((poll(&this->_fds[0], this->_fds.size(), -1)) < 0)
			throw std::runtime_error("ERROR: Can't add to poll");
		for (size_t i = 0; i < this->_fds.size(); i++)//iterate over the pollfd vector
		{
			if (this->_fds[i].revents & POLLIN)//POLLIN: There is data to read
			{
				if (this->_fds[i].fd == this->_socket)
					acceptUser();
				else
				{
					readUser(this->_fds[i].fd);
					std::cout << "Client reading" << std::endl;
				}
			}
		}
	}
	close(this->_socket);
}

void Server::acceptUser()
{
	int userFd;
	struct sockaddr_in userAddr;
	socklen_t userAddr_len = sizeof(userAddr);
	struct pollfd user_poll_fd;//poll struct
    
	// Accept a connection
    userFd = accept(this->_socket, (struct sockaddr *)&userAddr, &userAddr_len);
    if (userFd < 0) {
        //perror("accept");
		std::cout << "ERROR: Server can't accept user" << std::endl;
        close(this->_socket);
        return ;
    }
	if (fcntl(userFd, F_SETFL, O_NONBLOCK) < 0)
		std::cout << "Error setting non-blocking" << std::endl;
	std::cout << "Client connected" << std::endl;
	
	user_poll_fd.fd = userFd;
	user_poll_fd.events = POLLIN;
	user_poll_fd.revents = 0;
	this->_fds.push_back(user_poll_fd);//add the client socket to the pollfd vector
	//clients[client_fd] = "";//add the client socket to the clients map
	addUser(userFd, userAddr);//add the client to the clients map
    //close(client_fd);
}

void Server::readUser(int userFd)
{
	char buffer[1024];
	int bytes_read;

	memset(buffer, 0, sizeof(buffer));
	bytes_read = recv(userFd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_read < 0)
	{
		//perror("recv");
		std::cout << "ERROR: Message hasn't been received" << std::endl;
		close(userFd);
		return;
	}
	if (bytes_read == 0)
	{
		close(userFd);
		return;
	}
	std::cout << "Received message: " << buffer << std::endl;
	std::cout << "From client: " << userFd << std::endl;
	std::cout << this->_users[userFd] << std::endl;
	send(userFd, "Su mensaje ha sido recibido\n", strlen("Su mensaje ha sido recibido\n"), 0);
}

void Server::addUser(int userFd, struct sockaddr_in userAddr)
{
	User	newUser;

	newUser.setSocket(userFd);
	newUser.setHost(inet_ntoa(userAddr.sin_addr));
	newUser.setNickName("");
	newUser.setUserName("");
	newUser.setRealName("");
	newUser.setAuthent(false);
	this->_users[userFd] = newUser;//add the client to the clients map
	std::cout << "Datos del cliente:" << std::endl;
	std::cout << _users[userFd] << std::endl;
	std::cout << "User added" << std::endl;
}
