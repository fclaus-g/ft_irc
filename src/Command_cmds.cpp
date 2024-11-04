#include "ft_irc.hpp"

/**
 * @brief Command to check the password sent by the user
 */
void	Command::cmdPass()
{
	std::string	pass;

	pass = this->_msg.substr(this->_msg.find("PASS") + 5);
	pass.erase(pass.find_last_not_of(" \n\r\t") + 1);
	if (pass != this->_server.getPassword())
	{
		std::cout << "New connection with socket fd " << this->_user.getFd() << " tried to login with wrong password" << std::endl;
		std::cout << RED << "Connection rejected and socket closed" << RES << std::endl;
		this->_server.sendWarning(this->_user.getFd(), ":MyServer 464 * :Password incorrect\n");
		this->_server.deleteUser(this->_user.getFd());
		return ;
	}
	this->_user.setAuthenticated(true);
}

/**
 * @brief Command to set or change the user nickName
 *	- if the user is HexChat client and this is first connection message
 *		("NICK <nick>\nUSER <user> 0 * :<realname>\n") cmdUser is called
 *	- if the user is 'nc' client, the nickName is set as userName and realName (provisional)
 * (!) First check if user is already authenticated - if not, kick the user
 * TODO: Check if nickName is already in use
 * TODO: what if spaces after NICK or before \n?
 */
void	Command::cmdNick()
{
	if (!this->_user.getAuthenticated())
		kickNonAuthenticatedUser(this->_user.getFd());
	std::string	nick;
	size_t		pos;

	pos = this->_msg.find_first_of("\n");
	nick = this->_msg.substr(5, pos - 5);
	this->_user.setNick(nick);
	if (this->_user.getHexClient() && this->_msg.find("USER") != std::string::npos)
		cmdUser();
	else
	{
		this->_user.setUserName(nick);
		this->_user.setRealName(nick);
	}
}

/**
 * @brief Command to set the user userName and realName, it will always be sent
 *	at connection process, otherwise it will be treated as non-command message
 *	- if the user is HexChat client this is the nick-user message
 *		("NICK <nick>\nUSER <user> 0 * :<realname>\n")
 * (!) Need to check if user is already authenticated FIRST - if not, kick the user
 * TODO: Check if userName or realName is already in use
 * TODO: implment the realName parsing (currently setting the same as userName)
 */
void Command::cmdUser()
{
	if (!this->_user.getAuthenticated())
		kickNonAuthenticatedUser(this->_user.getFd());
	std::string user;
	size_t		pos;

	user = this->_msg.substr(this->_msg.find("USER") + 5);
	pos = user.find_first_of(" ");
	user = user.substr(0, pos);
	this->_user.setUserName(user);
	this->_user.setRealName(user);
}

void Command::commandPrivmsg(int userFd, const std::string msg)
{
	(void)userFd;
	(void)msg;
}


void Command::commandNick(User user)
{
	(void)user;
}

void Command::commandJoin(User user)
{
	(void)user;
}

void Command::commandQuit(User user)
{
	(void)user;
}

void Command::commandKick(User user)
{
	(void)user;
}

void Command::commandInvite(User user)
{
	(void)user;
}

void Command::commandTopic(User user)
{
	(void)user;
}

void Command::commandMode(User user)
{
	(void)user;
}
