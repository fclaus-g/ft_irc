#include "inc/Server.hpp"

int main (int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}
	if (std::atoi(av[1]) < 1024 || std::atoi(av[1]) > 49151)
	{
		//CREO QUE NO SE PUEDE USAR STD::ATOI PORQUE ES DEL STD::11
		std::cerr << "Port must be between 1024 and 49151" << std::endl;
		return 1;
	}
	Server server(std::atoi(av[1]), av[2]);

}