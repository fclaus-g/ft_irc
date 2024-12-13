#include "ft_irc.hpp"

/**
 * @brief Command to send a message to a user or channel
 *	(!) First - if the user is not authenticated, kick the user
 *	- Check if the message is to a channel or user
 *	- In both cases, find if the user or channel exists
 *	- In case of channel, check if the user is in the channel
 * (?) DOUBTS - MARKED AS TODO -
 * TODO: Multiple targets user/channel: sending but not showing in HexChat
 * 	(when executing from terminal, it does work and received in HexChat too)
 * TODO: ERR_NOTONCHANNEL implemented but not needed according to the doc
 * TODO: ERR_NEEDMOREPARAMS implemented but not needed according to the doc
 * TODO: RPL_AWAY (301) - mandatory? USER AWAY STATUS
 * TODO: ERR_CANNOTSENDTOCHAN (404) -  mandatory? BANS and MODERATED MODE
 */
void Command::cmdPrivmsg()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	if (this->_splitCmd.size() < 2)
		return (this->sendResponse(ERR_NEEDMOREPARAMS, MOD_USER));
	if ((this->_splitCmd.size() == 2 && this->_splitCmd[1][0] != ':' )
			|| (this->_splitCmd.size() > 2 && this->_splitCmd[2].empty()))
		return (this->sendResponse(ERR_NOTEXTTOSEND, MOD_USER));
	std::string msg_text = this->_splitCmd[2];
	if (this->_splitCmd.size() > 3)
	{
		for (size_t i = 3; i < this->_splitCmd.size(); i++)
		{
			msg_text.append(" ");
			msg_text.append(this->_splitCmd[i]);
		}
		msg_text.erase(msg_text.find_last_not_of(" \n\r\t") + 1);
	}

	std::vector<std::string> targets = this->splitMessage(this->_splitCmd[1], ',');
	for (size_t i = 0; i < targets.size(); i++)
	{
		targets[i].erase(targets[i].find_last_not_of(" \n\r\t") + 1);
		if (targets[i][0] == ':')
			return (this->sendResponse(ERR_NORECIPIENT, MOD_USER));
		if (targets[i][0] == '#')
		{
			this->_currChannel = this->_server.getChannelByName(targets[i]);
			if (!this->_currChannel)
			{
				this->_errorMsg = targets[i];
				this->sendResponse(ERR_NOSUCHNICK, MOD_USER);
				continue;
			}
			if (!this->_currChannel->isUserInChannel(this->_user))
			{
				this->sendResponse(ERR_NOTONCHANNEL, MOD_USER);
				continue;
			}
			std::string	channel_msg = this->_splitCmd[0] + " " + targets[i] + " " + msg_text;
			this->_currChannel->broadcastMessage(channel_msg, this->_user, 0);
		}
		else
		{
			User	*target_user = this->_server.getUserByNick(targets[i]);
			if (!target_user)
			{
				this->_errorMsg = targets[i];
				this->sendResponse(ERR_NOSUCHNICK, MOD_USER);
				continue;
			}
			std::string	priv_msg = this->_splitCmd[0] + " " + targets[i] + " " + msg_text;
			this->_server.messageToClient(priv_msg, this->_user, *target_user);
		}
	}
}

/**
 * @brief Command to kick a user from a channel - Parameters: <channel> <user> *( "," <user> ) [<comment>]
 *	(!) First - if the user is not authenticated, kick the user
 *	- Check if enough parameters are given in the message
 *	- Get the nickName and channelName from the message
 *	- Check if the channel exists, if so, check if the user is operator
 *	- Check if the user to be kicked exists in the channel; if so, remove it
 * TODO: ERR_NEEDMOREPARAMS shown in main server window, not channel
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
	
	std::vector<std::string> nicks = this->splitMessage(this->_splitCmd[2], ',');
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
			this->_msg = this->_msg + " :for no specific reason";
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
 */
void Command::commandInvite()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	if (this->_splitCmd.size() < 3)
		return (this->sendResponse(ERR_NEEDMOREPARAMS, MOD_USER));
	
	std::string	nick = this->_splitCmd[1];
	std::string	channel_name = this->_splitCmd[2];
	nick.erase(nick.find_last_not_of(" \n\r\t") + 1);
	channel_name.erase(channel_name.find_last_not_of(" \n\r\t") + 1);
	this->_currChannel = this->_server.getChannelByName(channel_name);
	
	if (!this->_currChannel || this->_currChannel->getUsersInChannel() < 1)
		return (this->sendResponse(ERR_NOSUCHCHANNEL, MOD_USER));
	if (!this->_currChannel->isUserInChannel(this->_user))
		return (this->sendResponse(ERR_NOTONCHANNEL, MOD_USER));
	if (this->_currChannel->getInviteMode() && !this->_currChannel->isOp(this->_user))
		return (this->sendResponse(ERR_CHANOPRIVSNEEDED, MOD_USER));

	User *invitedUser = this->_server.getUserByNick(nick);
	if (!invitedUser)
	{
		this->_errorMsg = ":User does not exists";
		return (this->sendResponse(ERR_CUSTOM_CHANNEL, MOD_USER));
	}
	if (this->_currChannel->isUserInChannel(*invitedUser))
		return (this->sendResponse(ERR_USERONCHANNEL, MOD_USER));
	if (this->_currChannel->channelIsFull())
	{
		this->_errorMsg = ":Channel is full";
		return (this->sendResponse(ERR_CUSTOM_CHANNEL, MOD_USER));
	}
	this->_server.addUserToChannel(channel_name, *invitedUser);
	return (this->sendResponse(RPL_INVITING, MOD_USER));
}

/**
 * @brief The Quit command is used to disconnect from the server and close the socket connection 
 * 	- The user is removed from the server and the socket is closed
 * 	- The user is removed from all channels
 * 	- The user is removed from the pollfd vector
 * 	- The user is removed from the users map
 * The client send to server the QUIT command with a message, but the server does not need to process it
 * TODO: for correctly work must have the localhost on the client for correct message
 */
void Command::commandQuit()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	
	std::cout << "User with fd " << this->_user.getFd() << " has quit the server" << std::endl;
	std::string quitMessage = ":" + this->_user.getNick() + "! QUIT :Client quit\n";
	send(this->_user.getFd(), quitMessage.c_str(), quitMessage.length(), 0);
	std::map<std::string, Channel*>::iterator it = this->_server.getChannelsMap().begin();
	while (it != this->_server.getChannelsMap().end())
	{
		it->second->removeUserChannel(this->_user);
		it->second->broadcastMessage("QUIT :Quit: Bye for now!\n", this->_user, 0);
		it++;
	}
	std::vector<Channel*>::iterator it2 = this->_server.getChannels().begin();
	while (it2 != this->_server.getChannels().end())
	{
		if ((*it2)->isUserInChannel(this->_user))
		{
			(*it2)->removeUserChannel(this->_user);
		}
		it2++;
	}
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
