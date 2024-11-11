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
	int			i;
	std::string	token;
	std::istringstream tokens(message);

	std::cout << "DEBUG: Start split" << std::endl;
	i = 0;
	while (tokens >> token)
	{
		std::cout << "DEBUG: " << token << " ___ ";
		res[i] = token;
		i++;
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

	//obtenemos el nombre del canal
	size_t iPos = this->_msg.find_first_of(" \t");
	iPos = this->_msg.find_first_not_of(" \t", iPos);
	size_t fPos = this->_msg.find_first_of(" \t", iPos);
	channelName = this->_msg.substr(iPos, fPos - iPos);
	//args = ft_split(this->_msg);
	//channelName = args[1];
	Channel *channel = this->_server.getChannelByName(channelName);
	std::cout << "DEBUG: " << channelName << std::endl;
	if (!channel)
	{
		/*
		Devolver respuesta ERR_NOSUCHCHANNEL al cliente
		*/
		return ;
	}	

	// Si el usuario no es admin no hacer nada
	//if (!channel->isOp(user))
	//{
		/*
		Devolver respuesta ERR_CHANOPRIVSNEEDED al cliente
		*/
	//	return ;
	//}
	
	// Parsear el resto del mensaje para obtener los modos a cambiar
	
}
