#include "ft_irc.hpp"

/**
 * @brief Command constructor
 * @param socketFd the socket file descriptor - stored just in case, not sure yet if needed
 * @param msg the message to be checked - won't need this when we implement User_buffer
 * @param user reference to the user that sent the message, so we can use its methods
 * @param server reference to the server, so we can use its methods
 */
Command::Command(int socketFd, const std::string msg, User &user, Server &server)
	: _socketFd(socketFd), _msg(msg), _user(user), _server(server)
{
	this->_currChannel = NULL;
	this->_splitCmd = ft_split(msg);
	this->_initCommands();
}

/**
 * @brief Initialize the commands array with the possible commands
 * (!) Has to be updated when new commands are added to enum and match its order
 */
void	Command::_initCommands()
{
	this->_commands[0] = "CAP LS";
	this->_commands[1] = "PASS";
	this->_commands[2] = "USER";
	this->_commands[3] = "NICK";
	this->_commands[4] = "JOIN";
	this->_commands[5] = "QUIT";
	this->_commands[6] = "PRIVMSG";
	this->_commands[7] = "KICK";
	this->_commands[8] = "INVITE";
	this->_commands[9] = "TOPIC";
	this->_commands[10] = "MODE";
}

/**
 * @brief Command destructor
 */
Command::~Command()
{
}

/**
 * @brief Aux function to check if the message is a valid command, then calls runCmd
 * @param userFd the user file descriptor - propably won't need it later when user class is more developed
 * @return bool just in case can use it to runs check or conditions later
 *	true if the message is a valid command, false otherwise
 */
bool Command::checkCmd(int userFd)
{
	for (int i = 0; i < TOTAL; i++)
	{
		if (this->_msg.find(_commands[i]) == 0)
		{
			//std::cout << "Command found: " << _commands[i] << "en el indice" << i << "checCmd 54" << std::endl;
			runCmd(userFd, i);
			return (true);
		}
	}
	return (false);
}

/**
 * @brief Run the command if found; each case calls the corresponding command method
 * @param userFd the user file descriptor
 * @param key the command key
 * TODO: Implement the command methods =)
 * TODO: If not used, remove USER from here, enum, _initCommands()
 */
void	Command::runCmd(int userFd, int key)
{
	(void)userFd;
	switch (key)
	{
		case CAPLS:
			this->_user.setHexClient(true);
			break;
		case PASS:
			cmdPass();
			break;
		case USER:
			cmdUser();
			break;
		case NICK:
			cmdNick();
			break;
		case JOIN:
			cmdJoin();
			break;
		case QUIT:
			commandQuit();
			break;
		case PRIVMSG:
			cmdPrivmsg();
			break;
		case KICK:
			commandKick();
			break;
		case INVITE:
			commandInvite();
			break;
		case TOPIC:
			commandTopic();
			break;
		case MODE:
			cmdMode();
			break;
		default:
			break;
	}
}

/**
 * @brief Aux function to kick the user from the server when trying to use a command before authentication
 *  - sends a message to the userm a log to the server and call server.deleteUser()
 * TODO: send the proper response to the user according to IRC protocol documentation
 */
void	Command::kickNonAuthenticatedUser(int userFd)
{
	std::cout << "User with socket fd " << userFd << " tried to use a command before authentication" << std::endl;
	this->_server.sendWarning(userFd, "You need to authenticate before using commands\n");
	this->_server.sendWarning(userFd, "You are being disconnected\n");
	this->_server.deleteUser(userFd);
	std::cout << RED << "User kicked and socket closed" << RES << std::endl;
}

/**
 * @brief Function to send a response from the server to one or several clients with the
 * 	proper IRC protocol format and code
 * (!) First it generates the prefix of all responses, then completes it according to the
 * 	code of the response
 * @var response is the "prefix" message of the response
 * @var detail is the specific message for each response code
 * @param code the response code to be checked later in the response enum
 * @param mode value to send the response in different ways
 * 		MOD_CAST(1) - send response to all but the user that executed the command
 * 		MOD_ALL(2) - send response to all users including the one executing the command
 * 		MOD_USER(0) or ANY - send response ONLY to the user executing the command
 */
void	Command::sendResponse(int code, int mode)
{
	std::string	response = ":" + this->_server.getName() + " " + toString(code);
	std::string	detail = this->composeResponse(code);
	response.append(detail);
	if (mode == MOD_CAST)
	{
		std::map<User*, bool>				users = this->_currChannel->getUsers();
		std::map<User *, bool>::iterator	i;
		for (i = users.begin(); i != users.end(); ++i)
		{
			if (i->first->getFd() != this->_user.getFd())
				send(i->first->getFd(), response.c_str(), response.size(), 0);
		}
	}	
	else if (mode == MOD_ALL)
	{
		std::map<User*, bool>				users = this->_currChannel->getUsers();
		std::map<User *, bool>::iterator	i;
		for (i = users.begin(); i != users.end(); ++i)
			send(i->first->getFd(), response.c_str(), response.size(), 0);
	}	
	else
		send(this->_user.getFd(), response.c_str(), response.size(), 0);
}

/**
 * @brief Aux function to complete the response message depending of the given code
 * @param code passed as enum definition or int value, will be the key in the switch
 * @return std::string detail, the message to be appended to the response
 */
std::string	Command::composeResponse(int code)
{
	std::string	detail = "";
	switch (code)
	{
		case ERR_NEEDMOREPARAMS:
			detail = " " + this->_user.getNick() + " " + this->_splitCmd[0] + " :Not enough parameters\r\n";
			break;
		case ERR_NOSUCHCHANNEL:
			detail = " " + this->_user.getNick() + " " + this->_splitCmd[1] + " :No such channel\r\n";
			break;
		case ERR_NOTONCHANNEL:
			detail = " " + this->_user.getNick() + " " + this->_currChannel->getName() + " :You're not on that channel\r\n";
			break;
		case ERR_CHANOPRIVSNEEDED:
			detail = " " + this->_user.getNick() + " " + this->_currChannel->getName() + " :You're not channel operator\r\n";
			break;
		case RPL_NOTOPIC:
			detail = " " + this->_user.getNick() + " " + this->_currChannel->getName() + " :No topic is set\r\n";
			break;
		case RPL_TOPIC:
			detail = " " + this->_user.getNick() + " " + this->_currChannel->getName() + " :" + this->_currChannel->getTopic() + "\r\n";
			break;
		case RPL_TOPICWHOTIME:
			detail = " " + this->_user.getNick() + " " + this->_currChannel->getName() + " " + this->_currChannel->getTopicCreator() + " " + this->_currChannel->getTimeStamp() + "\r\n";
			break;
		default:
			break;
	}
	return (detail);
}

//<client> <channel> 	<nick> 										<setat>
//Topic for #test 		set by pgomez-r!pgomez-r@212.170.131.219 (Tue Dec 10 15:32:22 2024)