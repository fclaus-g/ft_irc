
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
/**
 * @brief take the message and split it into a vector of strings
 * 
 * @param msg 
 * @return std::vector<std::string> 
 */
std::vector<std::string> splitMessage(const std::string &msg, char delim)
{
	std::vector<std::string> args;
	std::string word;
	std::stringstream ss(msg);
	while (getline(ss, word, delim))
		args.push_back(word);
	return args;
}

void printVector(std::vector<std::string> args)
{
	std::cout << "Printing vector" << std::endl;
	for (size_t i = 0; i < args.size(); i++)
	{
		std::cout << args[i] << std::endl;
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
 * TODO: Channel related methods have been left in Server as original; move them to Command?
 * TODO: VERIFICAR SI EL CANAL ESTA EN MODO PRIVADO - mirar como esta MODE y como se gestiona
 * TODO: VERIFICAR SI EL USUARIO HA SIDO INVITADO - mirar como esta INVITE y como se gestiona
 */
void Command::cmdJoin()
{
	if (this->_msg.find("JOIN") == std::string::npos)
		//ERR_NEEDMOREPARAMS (461)
		return;
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	/*vamos a separar del mensaje los argumentos, para eso vamos a hacer un split
	y vamos a guardarlo de la siguiente manera si:
	JOIN #channel1,#channel2 clave1,clave2 partimos en el espacio y dejamos las claves en vector[1]
	pero si JOIN #channel1, #channel2 en este caso solo tenemos que tratar el primer canal
	*/
	std::vector<std::string> args = splitMessage(this->_msg, ' ');
	printVector(args);
	std::vector<std::string> channels = splitMessage(args[1], ',');
	printVector(channels);
	std::vector<std::string> keys = (args.size() > 2) ? splitMessage(args[2], ',') : std::vector<std::string>();
	printVector(keys);

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string channelName = channels[i];
		channelName.erase(channelName.find_last_not_of(" \n\r\t") + 1);

		if (channelName.empty() || channelName[0] != '#'){
			//ERR_BADCHANNELKEY (475)
			return (this->_server.sendWarning(this->_user.getFd(),
					"JOIN: Error: No such nick/channel\n"));}
		Channel *channel = this->_server.getChannelByName(channelName);
		if (!channel)
		{
			std::cout << "Channel does not exist, creating new channel" << std::endl;
			this->_server.createChannel(channelName);
			channel = this->_server.getChannelByName(channelName);
			if (!channel)
			{
				std::cerr << "Failed to create or retrieve channel" << std::endl;
				return;
			}
			channel->addUserChannel(this->_user);
			channel->addOpChannel(this->_user);
			/*RPL_TOPIC (332)
			RPL_NAMREPLY (353)
			RPL_ENDOFNAMES (366)*/
		}
		else
		{
			std::cout << "Channel exists, adding user to channel" << std::endl;
			if (channel->isUserInChannel(this->_user))
				return (this->_server.sendWarning(this->_user.getFd(),
					"JOIN: Error: You are already in that channel\n"));
			channel->addUserChannel(this->_user);
			std::string msg = "JOIN " + channelName + "\n";
			channel->broadcastMessage(msg, this->_user);
			/*RPL_NOTOPIC (331) = "<client> <channel> :No topic is set"
			RPL_TOPIC (332) =   "<client> <channel> :<topic>"
			RPL_NAMREPLY (353) = "<client> <symbol/canalstate no neccessary?> <channel> :[prefix(space)]<nick>{ [prefix]<nick>}"
			RPL_ENDOFNAMES (366) = "<client> <channel> :End of /NAMES list"*/
		}
	}
}	
