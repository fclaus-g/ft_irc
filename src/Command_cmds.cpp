#include "../inc/ft_irc.hpp"

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
		this->_server.sendWarning(this->_user.getFd(), "Password incorrect\n");
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
	nick = this->_msg.substr(5, pos - 5);
	nick.erase(nick.find_last_not_of(" \r\t\n") + 1);
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
void Command::cmdJoin()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	std::string channelName = this->_msg.substr(this->_msg.find("JOIN") + 5);
	channelName.erase(channelName.find_last_not_of(" \n\r\t") + 1);
	if (channelName[0] != '#')
		return (this->_server.sendWarning(this->_user.getFd(),
			"JOIN: Error: No such nick/channel\n"));
	Channel	*channel = this->_server.getChannelByName(channelName);
	if (!channel)
	{
		this->_server.createChannel(channelName);
		this->_server.addUserToChannel(channelName, this->_user);
		this->_server.getChannelsMap()[channelName]->addOpChannel(this->_user);
	}
	else
	{
		if (channel->isUserInChannel(this->_user))
			return (this->_server.sendWarning(this->_user.getFd(),
				"JOIN: Error: You are already in that channel\n"));
		channel->addUserChannel(this->_user);
		std::string msg = ":" + this->_user.getNick() + "!" + this->_user.getUserName() + " JOIN " + channelName + "\n";
		channel->broadcastMessage(msg, this->_user);
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
			return (this->_server.sendWarning(this->_user.getFd(), "Error: No such nick/channel\n"));
		if (target_channel->isUserInChannel(this->_user))
			target_channel->broadcastMessage(msg_text, this->_user);
		else
			this->_server.sendWarning(this->_user.getFd(), "Error: You are not channel member\n");
	}
	else
	{
		User	*target_user = this->_server.getUserByNick(target);
		if (!target_user)
			return (this->_server.sendWarning(this->_user.getFd(), "Error: No such nick/channel\n"));
		send(target_user->getFd(), msg_text.c_str(), msg_text.length(), 0);
	}
}

/**
 * @brief Command to kick a user from a channel - Parameters: <channel> <user> *( "," <user> ) [<comment>]
 *	(!) First - if the user is not authenticated, kick the user
 *	- Check if enough parameters are given in the message
 *	- Get the nickName and channelName from the message
 *	- Check if the channel exists, if so, check if the user is operator
 *	- Check if the user to be kicked exists in the channel; if so, remove it
 * TODO: PARAMS ORDER WRONG! swap nick-channelName -> order changed
 * TODO: check error messages - to the client or server? ERROR responses?
 * TODO: check if user is in channel, and if it has been found and removed
 * TODO: send message both success and error
 * TODO: check if user was the only one left in channel, remove channel?
 */
void Command::commandKick()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));

	std::string nickName;
	std::string channelName;
	size_t iPos = this->_msg.find_first_not_of(" \t");
	size_t fPos = this->_msg.find_first_of(" \t", iPos);

	if (fPos == std::string::npos)
		return (this->_server.sendWarning(this->_user.getFd(), "Error: Not enough parameters\n"));
	
	channelName = this->_msg.substr(iPos, fPos - iPos);
	this->_msg = this->_msg.substr(fPos + 1);
	iPos = this->_msg.find_first_not_of(" \t", fPos);
	fPos = this->_msg.find_first_of(" \t", iPos);
	nickName = this->_msg.substr(iPos, fPos - iPos);
	
	Channel *channel;
	channel = this->_server.getChannelByName(channelName);
	if (!channel)
		return (this->_server.sendWarning(this->_user.getFd(), "Error: Channel does not exist\n"));
	if (!channel->isOp(this->_user))
		return (this->_server.sendWarning(this->_user.getFd(), "Error: You are not channel admin\n"));
	
	User *deleteUser = this->_server.getUserByNick(nickName);
	if (!deleteUser)
		return (this->_server.sendWarning(this->_user.getFd(), "Error: Nick not found in server\n"));
	channel->removeUserChannel(*deleteUser);
}

/**
 * @brief Command to invite a user to a channel
 *	(!) First - if the user is not authenticated, kick the user
 *	- Check if enough parameters are given in the message
 *	- Get the nickName and channelName from the message
 *	- Check if the channel exists in server
 *	- Check if the user to be invited exists in the server
 *	- Check if channel is invite only
 *	- Check if command user is currently in channel or admin if invite only
 *	- Check if invite target user is already in channel
 *	- Check if channel is full
 *	- Of all checks passed, add user to channel
 * TODO: check error messages - to the client or server? ERROR responses?
 */
void Command::commandInvite()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));

	std::string	nickName;
	std::string	channelName;
	size_t iPos = this->_msg.find_first_not_of(" \t");
	size_t fPos = this->_msg.find_first_of(" \t", iPos);
	if (fPos == std::string::npos)
		return (this->_server.sendWarning(this->_user.getFd(), "Error: Not enough parameters\n"));

	nickName = this->_msg.substr(iPos, fPos - iPos);
	this->_msg = this->_msg.substr(fPos + 1);
	iPos = this->_msg.find_first_not_of(" \t", fPos);
	fPos = this->_msg.find_first_of(" \t", iPos);
	channelName = this->_msg.substr(iPos, fPos - iPos);

	Channel *channel;
	channel = this->_server.getChannelByName(channelName);
	if (!channel)
		return (this->_server.sendWarning(this->_user.getFd(), "Error: Channel does not exist\n"));

	User *invitedUser = NULL;
	invitedUser = this->_server.getUserByNick(nickName);
	if (!invitedUser)
		return (this->_server.sendWarning(this->_user.getFd(), "Error: Nick not found in server\n"));
	if (channel->getInviteMode())
	{
		if (!channel->isOp(this->_user))
			return (this->_server.sendWarning(this->_user.getFd(), "Error: You are not channel admin\n"));
	}
	else
	{
		if (!channel->isUserInChannel(this->_user))
			return (this->_server.sendWarning(this->_user.getFd(), "Error: You are not channel member\n"));
	}
	if (channel->isUserInChannel(*invitedUser))
		return (this->_server.sendWarning(this->_user.getFd(), "Error: User is already in channel\n"));
	if (channel->channelIsFull())
		return (this->_server.sendWarning(this->_user.getFd(), "Error: Channel is full\n"));
	this->_server.addUserToChannel(channelName, *invitedUser);
}

void Command::commandQuit(User user)
{
	(void)user;
}

void Command::commandTopic(User user)
{
	(void)user;
}
