#ifndef FT_IRC_HPP
#define FT_IRC_HPP


#include <iostream>
#include <string>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <map>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <signal.h>
#include <exception>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sstream>

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

bool						readFromSocket(int socketFd, std::string &store);
std::vector<std::string>	ft_split(std::string message);

#endif