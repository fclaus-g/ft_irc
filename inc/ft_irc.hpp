#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <map>
#include <vector>
#include <cstdlib>
#include <signal.h>
#include <exception>
#include <fcntl.h>
#include <arpa/inet.h>
#include <ctime>
#include <algorithm>

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Command.hpp"

#define RED		"\033[31m"
#define GRE		"\033[32m"
#define YEL		"\033[33m"
#define BLU		"\033[34m"
#define RES 	"\033[0m"
#define	TIMEOUT	5000

const int MAX_CLIENTS = 10;
const int BUFF_SIZE = 1024;

/*Errors and respones enum*/

enum response
{
	MOD_USER = 0,
	MOD_CAST = 1,
	MOD_ALL = 2,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	ERR_NOSUCHCHANNEL = 403,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANOPRIVSNEEDED = 482
};

/*Utils.cpp functions*/
bool						readFromSocket(int socketFd, std::string &store);
std::vector<std::string>	ft_split(std::string message);
std::string 				toString(int num);
std::vector<std::string> 	customSplit(const std::string &str, char delimiter);

#endif