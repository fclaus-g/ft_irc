#include "ft_irc.hpp"

/**
 * @brief Command to check the password sent by the user
 * TODO: What if the user sends PASS command not in the first message?
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
		return (kickNonAuthenticatedUser(this->_user.getFd()));

	std::string	nick;
	size_t		pos;

	pos = this->_msg.find_first_of("\n");
	nick = this->_msg.substr(5, pos - 6);
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
		return (kickNonAuthenticatedUser(this->_user.getFd()));

	std::string user;
	size_t		pos;

	user = this->_msg.substr(this->_msg.find("USER") + 5);
	pos = user.find_first_of(" ");
	user = user.substr(0, pos);
	this->_user.setUserName(user);
	this->_user.setRealName(user);
}

/**
 * @brief Command to join a channel, modification of channel_fclaus-g branch Server method
 *	- if the channel does not exist, create it and add the user as operator
 *	- if the channel exists, add the user to the channel
 * (!) Diff from original: Command._msg is the message received to parse
 * (!) Diff from original: Added first check if user is authenticated
 * (!) Diff from original: Addeded getter methods for channels vector and map
 * TODO: Channel related methods have been left in Server as original; move them to Command?
 */
void Command::commandJoin(User& user)
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));

	size_t iPos = this->_msg.find_first_not_of(" \t") + 5;
	size_t fPos = this->_msg.find_first_of(" \t", iPos);
	std::string channel = this->_msg.substr(iPos, fPos - iPos);
	
	if (channel[0] != '#')
	{
		std::cout << "Error: Invalid channel name" << std::endl;
		return ;
	}
	bool channelExists = false;
	for(std::vector<Channel>::iterator it = this->_server.getChannels().begin();
		it < this->_server.getChannels().end(); ++it)
	{
		std::cout << "Channel: " << it->getName() << std::endl;
		if (it->getName() == channel)
		{
			channelExists = true;
			this->_server.addUserToChannel(channel, user);
			break ;
		}
	}
	if (!channelExists)
	{
		this->_server.createChannel(channel);
		this->_server.addUserToChannel(channel, user);
		this->_server.getChannelsMap()[channel].addOpChannel(user);
	}
}

/**
 * @brief Command to send a message to a user or channel
 *	(!) First - if the user is not authenticated, kick the user
 *	- Check if the message is to a channel or user
 *	- In both cases, find if the user or channel exists
 *	- In case of channel, check if the user is in the channel
 *	TODO: check :<msg> format, do the : comes with hexChat? add formatCheck to user nc?
 */
void Command::cmdPrivmsg()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));

	std::string target;
	std::string msg_text;

	msg_text = this->_msg.substr(this->_msg.find("PRIVMSG") + 8);
	target = msg_text.substr(0, msg_text.find_first_of(" "));
	msg_text = msg_text.substr(msg_text.find_first_of(" ") + 1);
	if (target[0] == '#')
	{
		//# is stored at the channel name? if not, remove it from target variable here
		Channel	*target_channel = this->_server.getChannelByName(target);
		if (!target_channel)
		{
			std::string	error = ":MyServer " + target + " :No such nick/channel\n";
			send(this->_user.getFd(), error.c_str(), error.length(), 0);
			return ;
		}
		if (target_channel->isUserInChannel(this->_user))
			target_channel->broadcastMessage(msg_text, this->_user.getFd());
		else
		{
			std::string	error = ":MyServer " + target + " :You're not in that channel\n";
			send(this->_user.getFd(), error.c_str(), error.length(), 0);
		}
	}
	else
	{
		User	*target_user = this->_server.getUserByNick(target);
		if (!target_user)
		{
			std::string	error = ":MyServer " + target + " :No such nick/channel\n";
			send(this->_user.getFd(), error.c_str(), error.length(), 0);
			return ;
		}
		send(target_user->getFd(), msg_text.c_str(), msg_text.length(), 0);
	}
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
