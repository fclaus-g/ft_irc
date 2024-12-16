#include "ft_irc.hpp"
/*
El comando `MODE` en IRC se utiliza para cambiar o consultar los modos de un canal o de un usuario. Los modos de canal pueden cambiar varias configuraciones del canal, como si es privado, si tiene una clave, si tiene un límite de usuarios, etc. Los modos de usuario pueden cambiar privilegios específicos del usuario.

### Resumen de los Modos de Canal

1. **Modo `i` (Invite Only)**:
   - **Descripción**: El canal es solo por invitación.
   - **Argumento**: No lleva argumento.
   - **Uso**: `MODE #canal +i` o `MODE #canal -i`.

2. **Modo `k` (Key)**:
   - **Descripción**: El canal tiene una clave (password).
   - **Argumento**: Lleva un argumento que es la clave del canal.
   - **Uso**: `MODE #canal +k clave` o `MODE #canal -k clave`.

3. **Modo `l` (User Limit)**:
   - **Descripción**: El canal tiene un límite de usuarios.
   - **Argumento**: Lleva un argumento que es el número máximo de usuarios permitidos en el canal.
   - **Uso**: `MODE #canal +l número` o `MODE #canal -l`.

4. **Modo `t` (Topic Settable by Ops Only)**:
   - **Descripción**: Solo los operadores del canal pueden cambiar el tópico.
   - **Argumento**: No lleva argumento.
   - **Uso**: `MODE #canal +t` o `MODE #canal -t`.

5. **Modo `o` (Operator)**:
   - **Descripción**: Otorga o quita privilegios de operador a un usuario.
   - **Argumento**: Lleva un argumento que es el nick del usuario.
   - **Uso**: `MODE #canal +o nick` o `MODE #canal -o nick`.
*/
void Command::execModes(const char sign, const char mode)
{
	switch (mode)
	{
		case 'i':// channel is invite only
			if (sign == '+')
			{
				this->_currChannel->setInviteMode(true);
				this->currChannel->uptadeMode('i', 0);
				//RPL_CHANNELMODEIS (324)
			}
			else if (sign == '-')
			{
				this->_currChannel->setInviteMode(false);
				this->currChannel->uptadeMode('i', 1);
				//RPL_CHANNELMODEIS (324)
			}
			break;
		case 'k':// channel has a key
			if (!this->params[this->_paramCount])
				return; 
			if (sign == '+')
			{
				this->_currChannel->setKeyMode(true);
				this->_currChannel->setPassword(this->_params[this->_paramCount]);
				this->_paramCount++;
				this->currChannel->uptadeMode('k', 0);
				//RPL_CHANNELMODEIS (324)
	
			}
			else if (sign == '-')
			{
				if (this->_currChannel->getKeyMode())
				{
					if (this->_currChannel->getPassword() != this->_params[this->_paramCount])
					{
						//ERR_BADCHANNELKEY (475)
						return;
					}
					this->_paramCount++;
					this->_currChannel->setKeyMode(false);
					this->currChannel->uptadeMode('k', 1);
					this->_currChannel->setPassword("");
					//RPL_CHANNELMODEIS (324)
				}
			}
			break;
		case 'l':// channel has a user limit
			if (sign == '+')
			{
				if (!this->_currChannel->getUsersLimit())
				{
					//channel->setUsersLimit(10); NECESITA PARAMETRO
					//RPL_CHANNELMODEIS (324)
				}
			}
			else if (sign == '-')
			{
				if (this->_currChannel->getUsersLimit())
				{
					this->_currChannel->setUsersLimit(-1);
					//RPL_CHANNELMODEIS (324)
				}
			}
			break;
		case 'o':
		/* code */
			break;
		case 't':
			if (sign == '+')
			{
				this->_currChannel->setTopicMode(true);
				this->currChannel->uptadeMode('t', 0);
				//RPL_CHANNELMODEIS (324)
			}
			else if (sign == '-')
			{
				this->_currChannel->setTopicMode(false);
				this->currChannel->uptadeMode('t', 1);
				//RPL_CHANNELMODEIS (324)
			}
			break;
		default:
			break;
	}
}
void Command::cmdMode()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	std::vector<std::string> args = splitMessage(this->_msg, ' ');
	printVector(args);
	if (args.size() < 2)
	{
		//ERR_NEEDMOREPARAMS (461)
		return;
	}
	std::string channelName = args[1];
	this->_currChannel = this->_server.getChannelByName(channelName);
	if (!this->_currChannel)
	{
		//ERR_NOSUCHCHANNEL (403)
		return;
	}
	if (args.size() == 2)
	{
		//RPL_CHANNELMODEIS (324)
		//RPL_CREATIONTIME (329)
		return;
	}
	//chequear si el usuario es admin
	if (!this->_currChannel.isOp(this->_user))
	{
		//ERR_CHANOPRIVSNEEDED (482)
		return;
	}
	this->_modes = splitMessage(args[2], ',');
	this->_params = splitMessage(args[3], ' ');
	for (size_t i = 3; i < args.size(); i++)
	{
		if (args[i])
			params.push_back(args[i]);
	}
	std::cout << "Modes:" << std::endl;
	printVector(modes);
	std::cout << "Params:" << std::endl;
	printVector(params);

	for (size_t i = 0; i < modes.size(); i++)
	{
		for (size_t j = 0; j < modes[i].size(); j++)
		{
			char sign = '+';
			if ((modes[i][j] == '+' || modes[i][j] == '-') && sign != modes[i][j])
			{
				sign = modes[i][j];
				
			}
			else if (modes[i][j] == 'i' || modes[i][j] == 'k' || modes[i][j] == 'l' || modes[i][j] == 'o' || modes[i][j] == 't')
			{
				execModes(modes[i][j], sign);
			}
			else
			{
				//ERR_UMODEUNKNOWNFLAG (501)
				continue;
			}
			j++;
		}
	}

}

