#include "ft_irc.hpp"

/**
 * @brief Function to parse, set and init the MODE command environment
 * 	1 - Check if user is authenticated, if not, finish here (only to protect nc connections)
 *	2 - Check if enough params (MIN "MODE #channelName" - show modes list string)
 *	3 - Check if channel exists
 *	4 - Check if user isOp
 *	(!) IF EVERYTHING ABOVE IS OK
 *	5 - Parse and store modes (i-k,t+ol) in string vector splitted by ','
 *	6 - Parse and store params ("param1 param2 param3") in string vector splitted by ' '
 *	7 - Iterate the modes vector -> check each char in each string to update sign, execute
 *			mode command or skip if none
 *	TODO: implement RESPONSES
 */
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
	if (!this->_currChannel->isOp(this->_user))
	{
		//ERR_CHANOPRIVSNEEDED (482)
		return;
	}
	this->_modes = splitMessage(args[2], ',');
	this->_params = splitMessage(args[3], ' ');
	for (size_t i = 3; i < args.size(); i++)
	{
		this->_params.push_back(args[i]);
	}
	std::cout << "Modes:" << std::endl;
	printVector(this->_modes);
	std::cout << "Params:" << std::endl;
	printVector(this->_params);

	for (size_t i = 0; i < this->_modes.size(); i++)
	{
		for (size_t j = 0; j < this->_modes[i].size(); j++)
		{
			char sign = '+';
			if ((this->_modes[i][j] == '+' || this->_modes[i][j] == '-') && sign != this->_modes[i][j])
			{
				sign = this->_modes[i][j];
				
			}
			else if (this->_modes[i][j] == 'i' || this->_modes[i][j] == 'k' || this->_modes[i][j] == 'l' || this->_modes[i][j] == 'o' || this->_modes[i][j] == 't')
			{
				execModes(this->_modes[i][j], sign);
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

/**
 * @brief Function to execute the mode command according to the character and sign
 * @param sign + / -
 * @param mode iktol - if not one of those, error
 * TODO: Test all usage +/- IKTOL!
 * TODO: Implement RESPONSES
 * TODO: [?] When no params and needed - error or silence?
 */
void Command::execModes(const char sign, const char mode)
{
	User	*target_user = NULL;
	switch (mode)
	{
		case 'i':
			if (sign == '+')
			{
				this->_currChannel->setInviteMode(true);
				this->_currChannel->updateMode('i', 0);
				//RPL_CHANNELMODEIS (324)
			}
			else if (sign == '-')
			{
				this->_currChannel->setInviteMode(false);
				this->_currChannel->updateMode('i', 1);
				//RPL_CHANNELMODEIS (324)
			}
			break;
		case 'k':
			if (this->_paramCount < this->_params.size() && this->_params[this->_paramCount].empty())
				return; 
			if (sign == '+')
			{
				this->_currChannel->setKeyMode(true);
				this->_currChannel->setPassword(this->_params[this->_paramCount]);
				this->_currChannel->updateMode('k', 0);
				this->_paramCount++;
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
					this->_currChannel->setKeyMode(false);
					this->_currChannel->setPassword("");
					this->_currChannel->updateMode('k', 1);
					this->_paramCount++;
					//RPL_CHANNELMODEIS (324)
				}
			}
			break;
		case 'l':
			if (sign == '+')
			{
				if (this->_paramCount < this->_params.size() && this->_params[this->_paramCount].empty())
					return;
				if (this->_currChannel->getUsersLimit() != atoi(this->_params[this->_paramCount].c_str()))
				{	
					this->_currChannel->setUsersLimit(atoi(this->_params[this->_paramCount].c_str()));
					this->_currChannel->updateMode('l', 0);
					this->_paramCount++;
					//RPL_CHANNELMODEIS (324)
				}
			}
			else if (sign == '-')
			{
				if (this->_currChannel->getUsersLimit() != -1)
				{
					this->_currChannel->setUsersLimit(-1);
					this->_currChannel->updateMode('l', 1);
					//RPL_CHANNELMODEIS (324)
				}
			}
			break;
		case 'o':
			if (this->_paramCount < this->_params.size() && this->_params[this->_paramCount].empty())
				return;
			target_user = this->_server.getUserByNick(this->_params[this->_paramCount]);
			if (!target_user)
				return ;//RESPONSE
			if (sign == '+' && !this->_currChannel->isOp(*target_user))
			{
				this->_currChannel->addOpChannel(*target_user);
				//RESPONSE
			}
			else if (sign == '-' && this->_currChannel->isOp(*target_user))
			{
				this->_currChannel->deleteOpChannel(*target_user);
				//RESPONSE
			}
			break;
		case 't':
			if (sign == '+' && !this->_currChannel->getTopicMode())
			{
				this->_currChannel->setTopicMode(true);
				this->_currChannel->updateMode('t', 0);
				//RPL_CHANNELMODEIS (324)
			}
			else if (sign == '-' && this->_currChannel->getTopicMode())
			{
				this->_currChannel->setTopicMode(false);
				this->_currChannel->updateMode('t', 1);
				//RPL_CHANNELMODEIS (324)
			}
			break;
		default:
			//NOT_MODE_OR_MODEFORMAT_VALID?
			break;
	}
}
