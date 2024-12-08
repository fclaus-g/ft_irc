#include "ft_irc.hpp"

/**
 * @brief Command to check the password sent by the user
 * TODO: What if the user sends PASS command not in the first message?
 */
void	Command::cmdPass()
{
	std::string	pass;

	pass = this->_msg.substr(this->_msg.find("PASS") + 5);
	std::cout << "On cmdPass 12: " << pass << std::endl;
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
 */
void	Command::cmdNick()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));

	std::string	nick;

	nick = this->_msg.substr(5);
	nick.erase(nick.find_last_not_of(" \r\t\n") + 1);
	this->_user.setNick(nick);
	if (!this->_user.getHexClient())
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
 * TODO: protect USER command when already logged - working weird now (hexChat sending lowercase)
 * If a client tries to send the USER command after they have already completed registration with server
 * ERR_ALREADYREGISTERED reply should be sent and the attempt should fail.
 */
void Command::cmdUser()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	if (!this->_user.getUserName().empty())
	{
		std::string	err_alreadyreg = this->_user.getNick() + " :You may not reregister\n";
		send(this->_user.getFd(), err_alreadyreg.c_str(), err_alreadyreg.length(), 0);
		return ;
	}

	std::string	user;
	size_t		pos;

	user = this->_msg.substr(5);
	pos = user.find_first_of(" ");
	user = user.substr(0, pos);
	user.erase(user.find_last_not_of(" \r\t\n") + 1);
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
 * 
  JOIN #foobar                    ; join channel #foobar.

  JOIN &foo fubar                 ; join channel &foo using key "fubar".

  JOIN #foo,&bar fubar            ; join channel #foo using key "fubar"
                                  and &bar using no key.

  JOIN #foo,#bar fubar,foobar     ; join channel #foo using key "fubar".
                                  and channel #bar using key "foobar".

  JOIN #foo,#bar                  ; join channels #foo and #bar.
 * 
 * TODO: Channel related methods have been left in Server as original; move them to Command?
 */
/*
IRC usa una serie de códigos numéricos para indicar el estado de los comandos. Para JOIN, estos son algunos de los mensajes que deberás enviar desde el servidor al cliente:

001 (RPL_WELCOME): Al usuario al conectarse al servidor.
332 (RPL_TOPIC): Devuelve el tema del canal si tiene uno.
353 (RPL_NAMREPLY): Muestra la lista de usuarios en el canal.
366 (RPL_ENDOFNAMES): Indica el fin de la lista de usuarios.*/
// void Command::cmdJoin()
// {
// 	if (!this->_user.getAuthenticated())
// 		return (kickNonAuthenticatedUser(this->_user.getFd()));
// 	std::string channelName = this->_msg.substr(this->_msg.find("JOIN") + 5);
// 	channelName.erase(channelName.find_last_not_of(" \n\r\t") + 1);

// 	if (channelName.empty() || channelName[0] != '#'){
// 		return (this->_server.sendWarning(this->_user.getFd(),
// 				"JOIN: Error: No such nick/channel\n"));}
// 	Channel *channel = this->_server.getChannelByName(channelName);
// 	if (!channel)
// 	{
// 		std::cout << "Channel does not exist, creating new channel" << std::endl;
// 		this->_server.createChannel(channelName);
// 		channel = this->_server.getChannelByName(channelName);
// 		if (!channel)
// 		{
// 			std::cerr << "Failed to create or retrieve channel" << std::endl;
// 			return;
// 		}
// 		//this->_server.addUserToChannel(channelName, this->_user);
// 		channel->addUserChannel(this->_user);
// 		channel->addOpChannel(this->_user);
// 	}
// 	else
// 	{
// 		std::cout << "Channel exists, adding user to channel" << std::endl;
// 		if (channel->isUserInChannel(this->_user))
// 			return (this->_server.sendWarning(this->_user.getFd(),
// 				"JOIN: Error: You are already in that channel\n"));
// 		channel->addUserChannel(this->_user);
// 		std::string msg = "JOIN " + channelName + "\n";
// 		channel->broadcastMessage(msg, this->_user);
// 	}
// }


/**
 * @brief Command to send a message to a user or channel
 *	(!) First - if the user is not authenticated, kick the user
 *	- Check if the message is to a channel or user
 *	- In both cases, find if the user or channel exists
 *	- In case of channel, check if the user is in the channel
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
		Channel	*target_channel = this->_server.getChannelByName(target);
		if (!target_channel)
			return (this->_server.sendWarning(this->_user.getFd(), "Error: No such nick/channel\n"));
		if (target_channel->isUserInChannel(this->_user))
			target_channel->broadcastMessage(this->_msg, this->_user);
		else
			this->_server.sendWarning(this->_user.getFd(), "Error: You are not channel member\n");
	}
	else
	{
		User	*target_user = this->_server.getUserByNick(target);
		if (!target_user)
			return (this->_server.sendWarning(this->_user.getFd(), "Error: No such nick/channel\n"));
		this->_server.messageToClient(this->_msg, this->_user, *target_user);
	}
}

/**
 * @brief Command to kick a user from a channel - Parameters: <channel> <user> *( "," <user> ) [<comment>]
 *	(!) First - if the user is not authenticated, kick the user
 *	- Check if enough parameters are given in the message
 *	- Get the nickName and channelName from the message
 *	- Check if the channel exists, if so, check if the user is operator
 *	- Check if the user to be kicked exists in the channel; if so, remove it
 * TODO: Check/protect not enough parameters (what about more than needed?)
 * TODO: SERVER NEEDS TO SEND MESSAGE TO KICKED USER (and all users in channel?)
 * TODO: Add comment - if passed by command, else default comment
 * TODO: check if user was the only one left in channel, remove channel?
 * TODO: check error messages - to the client or server? ERROR responses?
 */
void Command::commandKick()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	//KICK #channel nick comment_reason(optional)
	std::string nick;
	std::string channel_name;
	//std::string	comment = "";

	channel_name = this->_msg.substr(5);
	nick = channel_name.substr(channel_name.find_first_of(" ") + 1);
	channel_name = channel_name.substr(0, channel_name.find_first_of(" "));
	// if (nick.find(" "))
	nick.erase(nick.find_last_not_of(" \n\r\t") + 1);
	channel_name.erase(channel_name.find_last_not_of(" \n\r\t") + 1);

	Channel *channel;
	channel = this->_server.getChannelByName(channel_name);
	if (!channel)
		return (this->_server.sendWarning(this->_user.getFd(), "Error: Channel does not exist\n"));
	if (!channel->isOp(this->_user))
		return (this->_server.sendWarning(this->_user.getFd(), "Error: You are not channel admin\n"));
	
	User *deleteUser = this->_server.getUserByNick(nick);
	if (!deleteUser)
		return (this->_server.sendWarning(this->_user.getFd(), "Error: Nick not found in server\n"));
	this->_msg.erase(this->_msg.find_last_not_of(" \n\r\t") + 1);
	this->_msg = this->_msg + " for no reason\n";
	channel->broadcastMessage(this->_msg, this->_user);
	this->_server.messageToClient(this->_msg, this->_user, this->_user);
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
 * TODO: protect not enough params (what about more than needed?)
 * TODO: check error messages - to the client or server? ERROR/RESPONSES?
 */
void Command::commandInvite()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));

	std::string	nick;
	std::string	channel_name;

	nick = this->_msg.substr(7);
	channel_name = nick.substr(nick.find_first_of(" ") + 1);
	nick = nick.substr(0, nick.find_first_of(" "));
	nick.erase(nick.find_last_not_of(" \n\r\t") + 1);
	channel_name.erase(channel_name.find_last_not_of(" \n\r\t") + 1);

	Channel *channel;
	channel = this->_server.getChannelByName(channel_name);
	if (!channel)
		return (this->_server.sendWarning(this->_user.getFd(), "Error: Channel does not exist\n"));

	User *invitedUser = NULL;
	invitedUser = this->_server.getUserByNick(nick);
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
	this->_server.addUserToChannel(channel_name, *invitedUser);
}

/**
 * @brief The Quit command is used to disconnect from the server and close the socket connection 
 * 	- The user is removed from the server and the socket is closed
 * 	- The user is removed from all channels
 * 	- The user is removed from the pollfd vector
 * 	- The user is removed from the users map
 * The client send to server the QUIT command with a message, but the server does not need to process it
 * 
 */
void Command::commandQuit()
{
	std::cout << "User with fd " << this->_user.getFd() << " has quit the server" << std::endl;
	std::map<std::string, Channel*>::iterator it = this->_server.getChannelsMap().begin();
	while (it != this->_server.getChannelsMap().end())
	{
		it->second->removeUserChannel(this->_user);
		it->second->broadcastMessage("QUIT: " + this->_user.getNick() + " has quit the server\n", this->_user);
		it++;
	}
	std::vector<Channel*>::iterator it2 = this->_server.getChannels().begin();
	while (it2 != this->_server.getChannels().end())
	{
		if ((*it2)->isUserInChannel(this->_user))
		{
			(*it2)->removeUserChannel(this->_user);
			(*it2)->broadcastMessage("QUIT: " + this->_user.getNick() + " has quit the server\n", this->_user);}
		it2++;
	}
	std::string msg = "QUIT :Client quit\n";
	send(this->_user.getFd(), msg.c_str(), msg.length(), 0);
	this->_server.deleteUser(this->_user.getFd());
}

void Command::commandTopic(User user)
{
	std::cout << RED << this->_msg << RES << std::endl;
	size_t  iPos = this->_msg.find_first_not_of(" \t", 5);
	size_t  cPos = this->_msg.find_first_of(" \t", iPos);
	std::cout << iPos <<" , " << cPos << std::endl;
	std::string tmpMsg = this->_msg.substr(iPos, cPos - iPos);
	Channel* channel = this->_server.getChannelByName(tmpMsg);
	std::cout << BLU << "Hola" << RES << std::endl;
	if (!channel->isOp(user) && channel->getTopicMode())
		return ;
	size_t  fPos = this->_msg.find_first_not_of(" \t", cPos);
	std::cout << fPos << ", " << tmpMsg << std::endl;
	if (fPos == std::string::npos)
	{
		std::cout << RED << "Hola" << RES << std::endl;
		channel->sendTopicMessage(user);
		return ;
	}
	std::string topic = this->_msg.substr(fPos + 1, this->_msg.size() - fPos);
	std::cout << BLU << topic << RES << std::endl;
	channel->setTopic(topic);
	std::cout << YEL << this->_server.getChannelByName(tmpMsg)->getTopic() << RES << std::endl;
}
