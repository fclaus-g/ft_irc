
#include "ft_irc.hpp"

/*
IRC usa una serie de códigos numéricos para indicar el estado de los comandos. Para JOIN, estos son algunos de los mensajes que deberás enviar desde el servidor al cliente:

001 (RPL_WELCOME): Al usuario al conectarse al servidor.
332 (RPL_TOPIC): Devuelve el tema del canal si tiene uno.
353 (RPL_NAMREPLY): Muestra la lista de usuarios en el canal.
366 (RPL_ENDOFNAMES): Indica el fin de la lista de usuarios.*/
/*  JOIN #foobar                    ; join channel #foobar.

  JOIN &foo fubar                 ; join channel &foo using key "fubar".

  JOIN #foo,&bar fubar            ; join channel #foo using key "fubar"
								  and &bar using no key.

  JOIN #foo,#bar fubar,foobar     ; join channel #foo using key "fubar".
								  and channel #bar using key "foobar".

  JOIN #foo,#bar                  ; join channels #foo and #bar.
Para ejecutar el join se debe enviar un mensaje al servidor con el siguiente formato:
*/

void Command::printVector(const std::vector<std::string> args)
{
	std::cout << "Printing vector" << std::endl;
	for (size_t i = 0; i < args.size(); i++)
	{
		std::string formattedString;
		for (size_t j = 0; j < args[i].size(); j++)
		{
			if (args[i][j] == '\n')
				formattedString += "\\n";
			else if (args[i][j] == '\r')
				formattedString += "\\r";
			else
				formattedString += args[i][j];
		}
		std::cout << formattedString << std::endl;
	}
}

/**
 * @brief Command to join a channel
 * 	- if the user is not authenticated, kick the user
 * 	- split the message into the channel name and key
 * 	- if the channel name is empty or does not start with #, send an error message
 * 	- check if the channel exists
 *	- if the channel does not exist, create it and add the user as operator
 *	- if the channel exists, add the user to the channel
 * (?) DOUBTS - MARKED AS TODO - Are these mandatory?
 * TODO: [?] standard init values of channel according to documentation (userLimit, modes)
 * TODO: [?] inviteMode + keyMode? - search in documentation of IRC and HexChat 
 * TODO: [?] ERR_BANNEDFROMCHAN (474)
 * TODO: [?] ERR_TOOMANYCHANNELS (405) - add server.maxChannelPerUser and compare with user.ChannelList.size()
 */
void Command::cmdJoin()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	if (this->_splitCmd.size() < 2)
		return (this->sendResponse(ERR_NEEDMOREPARAMS, MOD_USER, 0));

	std::vector<std::string> args = splitMessage(this->_msg, ' ');
	std::vector<std::string> channels = splitMessage(args[1], ',');
	std::vector<std::string> keys = (args.size() > 2) ? splitMessage(args[2], ',') : std::vector<std::string>();

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string channelName = channels[i];
		if (!channelName.empty())
			channelName.erase(channelName.find_last_not_of(" \n\r\t") + 1);
	
		this->_currChannel = this->_server.getChannelByName(channelName);
		if (channelName.empty() || channelName[0] != '#')
		{
			this->_errorMsg = channelName;
			this->sendResponse(ERR_BADCHANMASK, MOD_USER, 0);
			continue ;
		}
		if (!this->_currChannel)
		{
			this->_server.createChannel(channelName);
			this->_currChannel = this->_server.getChannelByName(channelName);
			this->_currChannel->addUserChannel(this->_user);
			this->_currChannel->addOpChannel(this->_user);
			this->_user.addChannelToList(this->_currChannel);
			std::string topic = this->_currChannel->getTopic();
			this->sendResponse(RPL_NAMREPLY, MOD_USER, 0);
			this->sendResponse(RPL_ENDOFNAMES, MOD_USER, 0);
		}
		else
		{
			if (this->_currChannel->isUserInChannel(this->_user))
				continue;
			if (this->_currChannel->getInviteMode())
			{
				this->sendResponse(ERR_INVITEONLYCHAN, MOD_USER, 0);
				continue;
			}
			if (this->_currChannel->getKeyMode() && (keys.size() <= i || keys[i] != this->_currChannel->getPassword()))
			{
				this->sendResponse(ERR_BADCHANNELKEY, MOD_USER, 0);
				continue;
			}
			if (this->_currChannel->channelIsFull())
			{
				this->sendResponse(ERR_CHANNELISFULL, MOD_USER, 0);
				continue;
			}
			this->_currChannel->addUserChannel(this->_user);
			this->_user.addChannelToList(this->_currChannel);
			std::string msg = "JOIN " + channelName;
			this->_currChannel->broadcastMessage(msg, this->_user, 0);
			std::string topic = this->_currChannel->getTopic();
			if (!topic.empty())
			{
				this->sendResponse(RPL_TOPIC, MOD_USER, 0);
				this->sendResponse(RPL_TOPICWHOTIME, MOD_USER, 0);
			}
			this->sendResponse(RPL_NAMREPLY, MOD_USER, 0);
			this->sendResponse(RPL_ENDOFNAMES, MOD_USER, 0);
		}
	}
}

/**
 * @brief Command WHO, currently just using to handle informartion about a Channel
 * 	Otherwise, the command silenlty gets ignored
 */
void	Command::cmdWho()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	if (this->_splitCmd.size() != 2 || (this->_splitCmd.size() == 2 && this->_splitCmd[1][0] != '#'))
		return;
	
	std::string	channel_name = this->_splitCmd[1];
	channel_name.erase(channel_name.find_last_not_of(" \n\r\t") + 1);
	this->_currChannel = this->_server.getChannelByName(channel_name);
	if (!this->_currChannel || this->_currChannel->getUsersInChannel() < 1)
		return (this->sendResponse(ERR_NOSUCHCHANNEL, MOD_USER, 0));

	std::map<User*, bool>			users = this->_currChannel->getUsers();
	std::map<User*, bool>::iterator	i = users.begin();
	while (i != users.end())
	{
		User* curr_user = i->first;
		this->_errorMsg =  curr_user->getUserName() + " " + curr_user->getHost() + " " + this->_server.getName() + " " + curr_user->getNick() + " H :0 " + curr_user->getRealName() + "\r\n";
		this->sendResponse(RPL_WHOREPLY, MOD_USER, 0);
		i++;
	}
	this->sendResponse(RPL_ENDOFWHO, MOD_USER, 0);
}

//user->getUserName() + " " + user->getHost() + " " + this->_server.getServerName() + " " + user->getNick() + " H :0 " + user->getRealName() + "\r\n";