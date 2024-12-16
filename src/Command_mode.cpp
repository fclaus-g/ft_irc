#include "../inc/ft_irc.hpp"
#include <sstream>

bool	isModeValid(char m)
{
	std::string validModes = "itokl";

	if (validModes.find(m) == std::string::npos)
		return (false);
	return (true);
}

bool	isASign(char s)
{
	switch (s)
	{
	case '+':
	case '-':
		return (true);
		break;
	
	default:
		return (false);
		break;
	}
}

std::vector<std::string> ft_split(std::string message)
{
	std::vector<std::string> res;
	std::string	token;

	//  Quitamos los caracteres especiales al mensaje y los sustituimos por
	// un espacio para que la función ft_split no tenga problemas ¿mejor while?
	if (message.find('\n') != std::string::npos)
		message.at(message.find('\n')) = ' ';
	if (message.find('\r') != std::string::npos)
		message.at(message.find('\r')) = ' ';
	
	std::istringstream tokens(message);

	while (std::getline(tokens, token, ' '))
	{
		if (token.length() > 0 && token[0] != '\n')
		{
			std::cout << "Token: @" << token << "@" << std::endl;
			res.push_back(token);
		}
	}
	return (res);
}

bool	needParam(char m)
{
	switch (m)
	{
	case 'l':
	case 'k':
	case 'o':
		return (true);
		break;
	
	default:
		return (false);
		break;
	}
}

void	readModes(std::vector<std::string> const args, std::vector<std::string> &m, std::vector<std::string> &p)
{
	std::string modeArg;
	size_t		argCount;

	argCount = args.size();
	for (size_t index = 2; index < argCount; index++)
	{
		modeArg = args[index];
		if (modeArg.length() > 0 && isASign(modeArg[0]))
		{
			if (modeArg.length() == 2)
				m.push_back(modeArg);
		}
		else if (modeArg.length() > 0)
			p.push_back(modeArg);
	}
}

//////////////////////////////DEBUG FUNCTIONS////////////////////

void	ftShowVector(std::string msg, std::vector<std::string> &v, size_t index)
{
	for (size_t i = index; i < v.size(); i++)
		std::cout << msg << ": " << v[i] << std::endl;
}

/**
* @brief Command to change channel/user modes - not implemented yet
* (!) Moved to its own .cpp file to keep the code organized, will add here all related aux functions
*/
void Command::commandMode(/*User user*/)
{
	/*
	/MODE canal [modestring [modearguments]]
	
	modestring 	= 1*( modeset )
	modeset 	= plusminus *( modechar )
	plusminus	= %x2B / %x2D
				; + or -
	modechar = ALPHA
	modearguments : argumentos de la opción (no todas tienen)
	
	Example:
	/MODE #canal +i				-> establece el canal como privado
	/MODE #canal +k password	-> establece la contraseña especificada al canal
	/MODE #canal -k				-> elimina la contraseña del canal 

 	*/

 	std::vector<std::string> args;
	std::vector<std::string> modes;
	std::vector<std::string> params;
	std::string channelName;
	size_t	argCount;

	//obtenemos el nombre del canal
	args = ft_split(this->_msg);
	argCount = args.size();
	//std::cout << "DEBUG. Argument count: " << argCount << std::endl; 
	
	//DEBUG
	ftShowVector("Args comando MODE", args, 2);
	//FIN DEBUG
	if (argCount < 2)
	{
		/*
		No hay suficientes argumentos
		*/
		std::cout << "DEBUG: Faltan parámetros en MODE\n";
		return ;
	}
	channelName = args[1];
	Channel *channel = this->_server.getChannelByName(channelName);
	if (!channel)
	{
		/*
		Devolver respuesta ERR_NOSUCHCHANNEL al cliente
		*/
		std::cout << "DEBUG: No existe el canal\n";
		return ;
	}	
	if (argCount == 2)
	{
		/*
		Enviar mensaje RPL_CHANNELMODEIS al cliente con los modos actuales del canal
		Opcionalmente enviar mensaje RPL_CREATIONTIME tras el anterior
		*/

		std::cout << "DEBUG: MODE sin parámetros:\n";
		std::cout << "FIN DEBUG\n";
		return ;
	}
	else
	{
		// Si el usuario no es admin no hacer nada
		if (!channel->isOp(this->_user))
		{
			/*
			Devolver respuesta ERR_CHANOPRIVSNEEDED al cliente
			*/
			std::cout << "DEBUG: El usuario no es operador del canal\n";
			return ;
		}
		// Parsear el resto del mensaje para obtener los modos a cambiar
		readModes(args, modes, params);

		//Vemos lo que hemos parseado
		ftShowVector("DEBUG: Modos de MODE", modes, 0);
		ftShowVector("DEBUG: Parámetros de MODE", params, 0);
		std::cout << "FIN DEBUG\n";

		char sign;
		char mode;
		for (size_t i = 0; i < modes.size(); i++)
		{
			sign = modes[i].at(0);
			mode = modes[i].at(1);
			if (!isASign(sign) || !isModeValid(mode))
			{
				// invalid mode
				continue ;
			}
			switch (mode)
			{
				case 'i':
					switch (sign)
					{
						case '+':
							channel->setInviteMode(true);
							break;
						case '-':
							channel->setInviteMode(false);
							break;
						default:
							break;
					}
					break;
				
				case 't':
					switch (sign)
					{
						case '+':
							channel->setTopicMode(true);
							break;
						case '-':
							channel->setTopicMode(false);
							break;
						default:
							break;
					}
					break;
				
				case 'o':
					switch (sign)
					{
						case '+':
						{
							if (this->_user.getNick() == params[i])
								break ;
							User *newUser = this->_server.getUserByNick(params[i]);
							if (channel->isUserInChannel(*newUser))
								channel->addOpChannel(*newUser);
							else
								//Mensaje no existe el usuario
								break ;
							break;
						}
						
						case '-':
						{
							if (this->_user.getNick() == params[i])
								break ;
							User *newUser = this->_server.getUserByNick(params[i]);
							if (channel->isUserInChannel(*newUser))
								channel->removeUserChannel(*newUser); // debe ser removeOpChannel, pero no existe
							else
								//Mensaje no existe el usuario
								break ;
							break;
						}
						default:
							break;
					}
					break;

				case 'k':
					switch (sign)
					{
						case '+':
							if (!params[i].empty())
							{
								channel->setKeyMode(true);
								channel->setPassword(params[i]);
							}
							else
								continue ;
							break;
						case '-':
							channel->setKeyMode(false);
							channel->setPassword("");
							break;
						default:
							break;
					}
					break;

				case 'l':
					switch (sign)
					{
						case '+':
							int nbr;
							nbr = std::atoi(params[i].c_str());
							if (nbr < 0 && nbr > channel->getUsersLimit())
								continue ;
							else
								channel->setUsersLimit(nbr);
							break;
						case '-':
							channel->setUsersLimit(channel->getUsersLimit());
							break;
						default:
							break;
					}
					break;

				default:
				break;
			}
		}
	}
	
}
