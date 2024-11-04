#ifndef FT_IRC_HPP
#define FT_IRC_HPP


#include <iostream>//for cout
#include <string>//for string
#include <fstream>//for file
#include <sys/socket.h>//for socket
#include <sys/types.h>//for socket
#include <netinet/in.h>//for sockaddr_in
#include <unistd.h> //for close
#include <poll.h>//for poll
#include <map>//for map
#include <vector>//for vector
#include <cstring>//for memset
#include <cstdlib>//for atoi
#include <signal.h>//for signal
#include <exception>//for exception
#include <fcntl.h>//for fcntl
#include <arpa/inet.h>//for inet_ntoa
#include <algorithm>

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

#define RED "\033[31m"
#define GRE "\033[32m"
#define YEL "\033[33m"
#define BLU "\033[34m"
#define RES "\033[0m"

const int MAX_CLIENTS = 10;
const int BUFF_SIZE = 1024;

#endif