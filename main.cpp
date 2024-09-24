#include "inc/Server.hpp"


int main(void)
{
	Server server("localhost", "192.168.0.1", 6667);

	std::cout << server << std::endl;
	
	
	return 0;
}