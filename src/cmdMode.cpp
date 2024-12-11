#include "ft_irc.hpp"
void execModes(const char mode, const char sign, Channel *channel, User* user)
{
	(void)user;

	switch (mode)
	{
		case 'i':// channel is invite only
			if (sign == '+')
			{
				std::cout << "Setting channel as private" << std::endl;
				if (!channel->getInviteMode())
				{
					std::cout << "Setting channel as private" << std::endl;
					channel->setInviteMode(true);
					//RPL_CHANNELMODEIS (324)
				}
			}
			else if (sign == '-')
			{
				if (channel->getInviteMode())
				{
					channel->setInviteMode(false);
					//RPL_CHANNELMODEIS (324)
				}
			}
			break;
		case 'k':// channel has a key
			if (sign == '+')
			{
				if (!channel->getKeyMode())
				{
					channel->setKeyMode(true);
					//RPL_CHANNELMODEIS (324)
				}
			}
			else if (sign == '-')
			{
				if (channel->getKeyMode())
				{
					channel->setKeyMode(false);
					//RPL_CHANNELMODEIS (324)
				}
			}
			break;
		case 'l':// channel has a user limit
			if (sign == '+')
			{
				if (!channel->getUsersLimit())
				{
					//channel->setUsersLimit(10); NECESITA PARAMETRO
					//RPL_CHANNELMODEIS (324)
				}
			}
			else if (sign == '-')
			{
				if (channel->getUsersLimit())
				{
					channel->setUsersLimit(-1);
					//RPL_CHANNELMODEIS (324)
				}
			}
			break;
		/* code */
			break;
		case 'o':
		/* code */
			break;
		case 't':
		/* code */
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
	if (args.size() == 2)
	{
		//RPL_CHANNELMODEIS (324)
		//RPL_CREATIONTIME (329)
		return;
	}
	std::string channelName = args[1];
	Channel *channel = this->_server.getChannelByName(channelName);
	if (!channel)
	{
		//ERR_NOSUCHCHANNEL (403)
		return;
	}
	//chequear si el name es correcto y si el canal existe
	//chequear si el usuario es admin
	if (!channel->isOp(this->_user))
	{
		//ERR_CHANOPRIVSNEEDED (482)
		return;
	}
	std::vector<std::string> modes;
	std::vector<std::string> params;
	for (size_t i = 2; i < args.size(); i++)
	{
		if (args[i][0] == '+' || args[i][0] == '-')
			modes.push_back(args[i]);
		else
			params.push_back(args[i]);
	}
	std::cout << "Modes:" << std::endl;
	printVector(modes);
	std::cout << "Params:" << std::endl;
	printVector(params);
	    size_t paramIndex = 0;
    for (size_t i = 0; i < modes.size(); ++i)
    {
        char sign = modes[i][0];
        for (size_t j = 1; j < modes[i].size(); ++j)
        {
            char mode = modes[i][j];
            std::cout << "Setting mode " << mode << " to channel " << channelName << std::endl;
            if (sign == '+')
            {
                if (mode == 'i')
                {
                    std::cout << "Setting channel as invite-only" << std::endl;
                    channel->setInviteMode(true);
                }
                else if (mode == 'o' && paramIndex < params.size())
                {
                    std::string userNick = params[paramIndex++];
                    User *user = this->_server.getUserByNick(userNick);
                    if (user)
                    {
                        std::cout << "Setting user " << userNick << " as operator" << std::endl;
                        //channel->addOpChannel(user);
                    }
                }
                // Agrega más modos según sea necesario
            }
            else if (sign == '-')
            {
                if (mode == 'i')
                {
                    std::cout << "Removing invite-only mode from channel" << std::endl;
                    channel->setInviteMode(false);
                }
                else if (mode == 'o' && paramIndex < params.size())
                {
                    std::string userNick = params[paramIndex++];
                    User *user = this->_server.getUserByNick(userNick);
                    if (user)
                    {
                        std::cout << "Removing operator status from user " << userNick << std::endl;
                        //channel->removeOperator(user);
                    }
                }
                // Agrega más modos según sea necesario
            }
        }
    }
}

