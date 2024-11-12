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
	std::istringstream tokens(message);

	while (std::getline(tokens, token, ' '))
		res.push_back(token);
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
	std::string channelName;
	size_t	argCount;

	//obtenemos el nombre del canal
	//size_t iPos = this->_msg.find_first_of(" \t");
	//iPos = this->_msg.find_first_not_of(" \t", iPos);
	//size_t fPos = this->_msg.find_first_of(" \t", iPos);
	//channelName = this->_msg.substr(iPos, fPos - iPos);
	//std::cout << "DEBUG. Message: " << this->_msg << std::endl;
	args = ft_split(this->_msg);
	//std::cout << "DEBUG. Argument count: " << args.size() << std::endl; 
	argCount = args.size();
	if (argCount < 2)
	{
		/*
		No hay suficientes argumentos
		*/
		return ;
	}
	channelName = args[1];
	Channel *channel = this->_server.getChannelByName(channelName);
	if (!channel)
	{
		/*
		Devolver respuesta ERR_NOSUCHCHANNEL al cliente
		*/
		return ;
	}	
	if (argCount == 2)
	{
		/*
		Enviar mensaje RPL_CHANNELMODEIS al cliente con los modos actuales del canal
		Opcionalmente enviar mensaje RPL_CREATIONTIME tras el anterior
		*/
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
			return ;
		}
		
		// Parsear el resto del mensaje para obtener los modos a cambiar
	}
	
}
