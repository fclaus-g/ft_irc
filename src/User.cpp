#include "../inc/User.hpp"

User::User()
{

}

User::User(User const &u)
{
	if (this != &u)
		*this = u;    
}

User &User::operator=(User const &u)
{
	if (this != &u)
	{
		this->_userName = u._userName;
		this->_nickName = u._nickName;
		this->_realName = u._realName;
        this->_host = u._host;
        this->_port = u._port;
        this->_clientSocket = u._clientSocket;
	}
	return (*this);
}

User::User(int serverSocket)
{
    //need to grow and do many things
    (void) serverSocket;
}
		
User::~User()
{
    //Borrar la lista de canales en los que está el usuario
}

std::string User::getUserName() const
{
    return (this->_userName);
}

std::string User::getNickName() const
{
    return (this->_nickName);
}

std::string User::getRealName() const
{
    return (this->_realName);
}

std::string User::getHost() const
{
    return (this->_host);
}

int User::getPort() const
{
    return (this->_port);
}

int User::getSocket() const
{
    return (this->_clientSocket);
}

bool User::getAuthent() const
{
	return (this->_authent);
}

void User::setUserName(std::string name)
{
	this->_userName = name;
}

void User::setNickName(std::string nick)
{
	this->_nickName = nick;
}

void User::setRealName(std::string realName)
{
	this->_realName = realName;
}

void User::setHost(std::string host)
{
	this->_host = host;
}

void User::setPort(int port)
{
	this->_port = port;
}

void User::setSocket(int sock)
{
	this->_clientSocket = sock;
}

void User::setAuthent(bool auth)
{
	this->_authent = auth;
}

std::ostream& operator<<(std::ostream& out, const User& user)
{
    out << "User: " << user.getSocket() << std::endl;
    out << "IP: " << user.getHost() << std::endl;
    out << "Nick: " << user.getNickName() << std::endl;
    out << "UserName: " << user.getUserName() << std::endl;
    out << "RealName: " << user.getRealName() << std::endl;
    return out;
}