
#include <iostream>

#include "../include/Server.hpp"

#define EXIT_FAILURE 1

int main(int argc, char *argv[])
{
    if (argc != 3) {
		std::cout << RED << "Wrong arguments: use ./ircserv <port> <password>" << RESET << std::endl;
		return (EXIT_FAILURE);
	}

	int port = atoi(argv[1]);

	if (port < 1024 || port > 49151) {
		std::cout << RED << "Wrong port! use after 1024 and defore 49151" << RESET << std::endl;
		return (EXIT_FAILURE);
	}

    Server serv(argv[1], argv[2]);

	serv.start();
    
    return 0;
}