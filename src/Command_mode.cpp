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
 *	TODO: Test cases, errors and RESPONSES
 *	TODO: Check if _errorMsg is used in any response during execModes, if so, use new attribute instead
 */
void Command::cmdMode()
{
	if (!this->_user.getAuthenticated())
		return (kickNonAuthenticatedUser(this->_user.getFd()));
	
	std::vector<std::string> args = splitMessage(this->_msg, ' ');
	if (args.size() < 2)
		return (this->sendResponse(ERR_NEEDMOREPARAMS, MOD_USER, 0));
		
	std::string channelName = args[1];
	this->_currChannel = this->_server.getChannelByName(channelName);
	if (!this->_currChannel)
		return (this->sendResponse(ERR_NOSUCHCHANNEL, MOD_USER, 0));
	if (args.size() == 2)
		return (this->sendResponse(RPL_CHANNELMODEIS, MOD_USER, 0),
			this->sendResponse(RPL_CREATIONTIME, MOD_USER, 0));
	if (!this->_currChannel->isOp(this->_user))
		return (this->sendResponse(ERR_CHANOPRIVSNEEDED, MOD_USER, 0));
	
	this->_modes = splitMessage(args[2], ',');
	for (size_t i = 3; i < args.size(); i++)
		this->_params.push_back(args[i]);

	for (size_t i = 0; i < this->_modes.size(); i++)
	{
		for (size_t j = 0; j < this->_modes[i].size(); j++)
		{
			char sign = '+';
			if ((this->_modes[i][j] == '+' || this->_modes[i][j] == '-') && sign != this->_modes[i][j])
				sign = this->_modes[i][j];
			else if (this->_modes[i][j] == 'i' || this->_modes[i][j] == 'k' || this->_modes[i][j] == 'l' || this->_modes[i][j] == 'o' || this->_modes[i][j] == 't')
				execModes(sign, this->_modes[i][j]);
			else
			{
				this->_unknowFlags += this->_modes[i][j] + " ";
				continue;
			}
			j++;
		}
	}
	std::cout << "modeStr: " << this->_currChannel->getModeStr() << std::endl;
	// if (!this->_currChannel->getModeStr().empty())
	this->sendResponse(RPL_CHANNELMODEIS, MOD_USER, 0);
	if (!this->_unknowFlags.empty())
	{
		this->_unknowFlags.erase(this->_unknowFlags.find_last_not_of(" \n\r\t")  + 1);
		this->sendResponse(ERR_UMODEUNKNOWNFLAG, MOD_USER, 0);
	}
}

/**
 * @brief Function to execute the mode command according to the character and sign
 * @param sign + / -
 * @param mode iktol - if not one of those, error
 * TODO: Test all usage +/- IKTOL!
 * TODO: Implement RESPONSES
 * TODO: [?] When no params and needed - error or silence? currently: silence
 * TODO: get iktol check in aux function
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
			}
			else if (sign == '-')
			{
				this->_currChannel->setInviteMode(false);
				this->_currChannel->updateMode('i', 1);
			}
			break;
		case 'k':
			if (this->_paramCount > this->_params.size() || this->_params.size() == 0)
			{
				this->_errorMsg = " :'k'";
				return (this->sendResponse(ERR_NEEDMOREPARAMS, MOD_USER, 0));
			} 
			if (sign == '+')
			{
				this->_currChannel->setPassword(this->_params[this->_paramCount]);
				this->_currChannel->setKeyMode(true);
				this->_currChannel->updateMode('k', 0);
				this->_paramCount++;
			}
			else if (sign == '-')
			{
				if (this->_currChannel->getKeyMode())
				{
					if (this->_currChannel->getPassword() != this->_params[this->_paramCount])
						return (this->sendResponse(ERR_BADCHANNELKEY, MOD_USER, 0));
					this->_currChannel->setKeyMode(false);
					this->_currChannel->setPassword("");
					this->_currChannel->updateMode('k', 1);
					this->_paramCount++;
				}
			}
			break;
		case 'l':
			if (sign == '+')
			{
				if (this->_paramCount < this->_params.size() && this->_params[this->_paramCount].empty())
				{
					this->_errorMsg = " :'l'";
					return (this->sendResponse(ERR_NEEDMOREPARAMS, MOD_USER, 0));
				}
				if (this->_currChannel->getUsersLimit() != atoi(this->_params[this->_paramCount].c_str()))
				{	
					this->_currChannel->setUsersLimit(atoi(this->_params[this->_paramCount].c_str()));
					this->_currChannel->updateMode('l', 0);
					this->_paramCount++;
				}
			}
			else if (sign == '-')
			{
				if (this->_currChannel->getUsersLimit() != -1)
				{
					this->_currChannel->setUsersLimit(-1);
					this->_currChannel->updateMode('l', 1);
				}
			}
			break;
		case 'o':
			if (this->_paramCount < this->_params.size() && this->_params[this->_paramCount].empty())
			{
				this->_errorMsg = " :'o'";
				return (this->sendResponse(ERR_NEEDMOREPARAMS, MOD_USER, 0));
			}
			target_user = this->_server.getUserByNick(this->_params[this->_paramCount]);
			if (this->_currChannel->isUserInChannel(*target_user))
			{
				this->_errorMsg = this->_params[this->_paramCount];
				this->_paramCount++;
				return (this->sendResponse(ERR_USERNOTINCHANNEL, MOD_USER, 0));
			}
			if (sign == '+' && !this->_currChannel->isOp(*target_user))
				this->_currChannel->addOpChannel(*target_user);
			else if (sign == '-' && this->_currChannel->isOp(*target_user))
				this->_currChannel->deleteOpChannel(*target_user);
			this->_paramCount++;
			break;
		case 't':
			if (sign == '+' && !this->_currChannel->getTopicMode())
			{
				this->_currChannel->setTopicMode(true);
				this->_currChannel->updateMode('t', 0);
			}
			else if (sign == '-' && this->_currChannel->getTopicMode())
			{
				this->_currChannel->setTopicMode(false);
				this->_currChannel->updateMode('t', 1);
			}
			break;
		default:
			break;
	}
}
