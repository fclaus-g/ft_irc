#include "ft_irc.hpp"

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
		channel = this->_server.getChannelByName(channelName);
		this->_user.addChannelToList(channel);
	}
	else
	{
		if (channel->isUserInChannel(this->_user))
			return (this->_server.sendWarning(this->_user.getFd(),
				"JOIN: Error: You are already in that channel\n"));
		channel->addUserChannel(this->_user);
		//TODO: Duda, y si falla addUserToChannel (full, already in channel, etc)?
		this->_user.addChannelToList(channel);
		std::string msg = "JOIN " + channelName + "\n";
		channel->broadcastMessage(msg, this->_user, 0);
	}
}

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
			target_channel->broadcastMessage(this->_msg, this->_user, 0);
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
 * TODO: if user was the only one left in channel, remove channel?
 * 	- if(this->_currChannel.size() < 1){this->_server.removeChannel(channelName)}
 * 		+update removeChannel to also update each user channelList
 */
void Command::commandKick()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	if (this->_splitCmd.size() < 3)
	{
		if (this->_splitCmd.size() == 2)
		{
			this->_splitCmd[1].erase(this->_splitCmd[1].find_last_not_of(" \n\r\t") + 1);
			this->_currChannel = this->_server.getChannelByName(this->_splitCmd[1]);
		}
		return (this->sendResponse(ERR_NEEDMOREPARAMS, MOD_USER));
	}
	std::string channel_name = this->_splitCmd[1];
	channel_name.erase(channel_name.find_last_not_of(" \n\r\t") + 1);
	this->_currChannel = this->_server.getChannelByName(channel_name);
	if (!this->_currChannel)
		return (this->sendResponse(ERR_NOSUCHCHANNEL, MOD_USER));
	if (!this->_currChannel->isUserInChannel(this->_user))
		return (this->sendResponse(ERR_NOTONCHANNEL, MOD_USER));
	if (!this->_currChannel->isOp(this->_user))
		return (this->sendResponse(ERR_CHANOPRIVSNEEDED, MOD_USER));
	
	std::string	comment = "";
	if (this->_splitCmd.size() > 3)
	{
		for (size_t i = 3; i < this->_splitCmd.size(); i++)
		{
			comment.append(this->_splitCmd[i]);
			comment.append(" ");
		}
		comment.erase(comment.find_first_not_of(" \n\r\t") + 1);
	}
	
	std::vector<std::string> nicks = customSplit(this->_splitCmd[2], ',');
	for (size_t i = 0; i < nicks.size(); i++)
	{
		nicks[i].erase(nicks[i].find_last_not_of(" \n\r\t") + 1);
		this->_splitCmd[2] = nicks[i];
		User *deleteUser = this->_server.getUserByNick(nicks[i]);
		if (!this->_currChannel->isUserInChannel(*deleteUser))
		{
			this->sendResponse(ERR_USERNOTINCHANNEL, MOD_USER);
			continue ;
		}
		if (comment.empty())
			this->_msg = this->_msg + " :for no specific reason\r\n";
		this->_currChannel->broadcastMessage(this->_msg, this->_user, 1);
		this->_currChannel->removeUserChannel(*deleteUser);
		deleteUser->delChannelFromList(this->_currChannel);
	}
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
 * 	TODO:  RPL_INVITING (341)
 *	TODO:  ERR_NEEDMOREPARAMS (461)
 *	TODO:  ERR_NOSUCHCHANNEL (403)
 *	TODO:  ERR_NOTONCHANNEL (442)
 *	TODO:  ERR_CHANOPRIVSNEEDED (482)
 *	TODO:  ERR_USERONCHANNEL (443)
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
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	
	std::cout << "User with fd " << this->_user.getFd() << " has quit the server" << std::endl;
	std::map<std::string, Channel*>::iterator it = this->_server.getChannelsMap().begin();
	while (it != this->_server.getChannelsMap().end())
	{
		it->second->removeUserChannel(this->_user);
		it->second->broadcastMessage("QUIT: " + this->_user.getNick() + " has quit the server\n", this->_user, 0);
		it++;
	}
	std::vector<Channel*>::iterator it2 = this->_server.getChannels().begin();
	while (it2 != this->_server.getChannels().end())
	{
		if ((*it2)->isUserInChannel(this->_user))
		{
			(*it2)->removeUserChannel(this->_user);
			(*it2)->broadcastMessage("QUIT: " + this->_user.getNick() + " has quit the server\n", this->_user, 0);}
		it2++;
	}
	std::string msg = "QUIT :Client quit\n";
	send(this->_user.getFd(), msg.c_str(), msg.length(), 0);
	this->_server.deleteUser(this->_user.getFd());
}

/**
 * @brief Command to check the current topic or change it
 * (!) First - if the user is not authenticated, kick the user
 * (!) Using ft_split to check how many ARGS
 * - 1 ARGS - ERR_NEEDMOREPARAMS (461)
 * - 2 ARGS - Show current topic or alert that no topic is set
 * - 3+ ARGS - Change the current topic using arg[2] to arg[last]
 * TODO: test ERR_CHANOPRIVSNEEDED response when MODE is ready
 */
void Command::commandTopic()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	
	std::vector<std::string>	cmd_args = ft_split(this->_msg);
	size_t						ac = cmd_args.size();

	if (ac == 1)
		return (this->sendResponse(ERR_NEEDMOREPARAMS, MOD_USER));
	
	this->_currChannel = this->_server.getChannelByName(cmd_args[1]);
	if (!this->_currChannel)
		return (this->sendResponse(ERR_NOSUCHCHANNEL, MOD_USER));
	if (!this->_currChannel->isUserInChannel(this->_user))
		return (this->sendResponse(ERR_NOTONCHANNEL, MOD_USER));
	
	if (ac == 2)
	{
		if (this->_currChannel->getTopic().empty())
			this->sendResponse(RPL_NOTOPIC, MOD_USER);
		else
		{
			this->sendResponse(RPL_TOPIC, MOD_USER);
			this->sendResponse(RPL_TOPICWHOTIME, MOD_USER);
		}
	}
	else
	{
		if (this->_currChannel->getTopicMode() && !this->_currChannel->isOp(this->_user))
			return (this->sendResponse(ERR_CHANOPRIVSNEEDED, MOD_USER));
		std::string	new_topic = "";
		for (size_t i = 2; i < ac; i++)
		{
			new_topic.append(cmd_args[i]);
			new_topic.append(" ");
		}
		new_topic.erase(new_topic.find_last_not_of(" \r\t\n") + 1);
		if (new_topic[0] == ':')
			new_topic.erase(0, 1);
		this->_currChannel->updateTopic(new_topic, this->_user.getNick());
		this->_currChannel->broadcastMessage(this->_msg, this->_user, 1);
	}
}
