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
	_initCommands();
}

/**
 * @brief Initialize the commands array with the possible commands
 * (!) Has to be updated when new commands are added to enum and match its order
 */
void	Command::_initCommands()
{
	this->_commands[0] = "CAPLS";
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
			break;
		case NICK:
			cmdNick();
			break;
		case JOIN:
			commandJoin(this->_user);
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
			commandTopic(this->_user);
			break;
		case MODE:
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
	this->_server.sendWarning(userFd, ":MyServer: You need to authenticate before using commands\n:MyServer: You are being disconnected\n");
	this->_server.deleteUser(userFd);
	std::cout << RED << "User kicked and socket closed" << RES << std::endl;
}
