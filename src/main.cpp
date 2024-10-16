#include "../inc/ircserv.h"

static bool ft_isnbr(std::string str)
{
	bool	res;

	res = true;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str.at(i)))
		{
			res = false;
			break;
		}
	}
	return (res);
}

static int ft_stoi(std::string s) 
{
    int i;

    std::istringstream(s) >> i;
    return i;
}


int main(int argc, char **argv)
{
	int			port;
	std::string	pass;

	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv port password" << std::endl;
		exit(1);
	}
	if (!ft_isnbr(argv[1]))
	{
        std::cerr << "ERROR: Port is invalid." << std::endl;
		exit(1);
    }
	port = ft_stoi(argv[1]);
	if (port < 1024  || port > 65535)
	{
		std::cerr << "ERROR: Port is invalid." << std::endl;
		exit(1);
	}
	pass = argv[2];
	try
    {
		Server server(port, pass);
		server.startIRCServer();
        return (0);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
}
