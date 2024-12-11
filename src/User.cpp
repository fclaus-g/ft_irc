#include "ft_irc.hpp"

int	User::getFd() const
{
	return this->_fd;
}

bool	User::getAuthenticated() const
{
	return this->_authenticated;
}

void	User::setAuthenticated(bool authenticated)
{
	this->_authenticated = authenticated;
}

std::string User::getNick() const
{
	return (this->_nickName);
}

void	User::setNick(std::string nick)
{
	this->_nickName = nick;
}
std::string User::getUserName() const
{
	return this->_userName;
}

void	User::setUserName(std::string userName)
{
	this->_userName = userName;
}

std::string User::getRealName() const
{
	return (this->_realName);
}

void	User::setRealName(std::string realName)
{
	this->_realName = realName;
}

bool	User::getHexClient() const
{
	return (this->_hexChatClient);
}

void	User::setHexClient(bool state)
{
	this->_hexChatClient = state;
}

bool	User::getLoginStat() const
{
	return (this->_logged);
}

void	User::setLoginStat(bool stat)
{
	this->_logged = stat;
}

std::vector<Channel*>	User::getChannelList()
{
	return (this->_channelList);
}

void	User::addChannelToList(Channel *channel)
{
	this->_channelList.push_back(channel);
}

void	User::delChannelFromList(Channel *channel)
{
	if (this->_channelList.empty())
		return;
	std::vector<Channel*>::iterator	pos; 
	pos = std::find(this->_channelList.begin(), this->_channelList.end(), channel);
	if (pos != this->_channelList.end())
		this->_channelList.erase(pos);
}

std::string	User::getHost() const
{
	return (this->_host);
}

std::string	User::getHostName() const
{
	char hostname[1024];
	if (gethostname(hostname, sizeof(hostname)) == 0)
		return (std::string(hostname));
	else
	{
		perror("gethostname");
		return "";
	}
}
