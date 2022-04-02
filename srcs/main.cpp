
// c masandilov.ru
// ветка gvenonat

#include <iostream>

#include "../include/Server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3) {
		std::cout << "Wrong number of arguments" << std::endl;
		exit(-1);
	}

    Server serv(argv[1], argv[2]);

	serv.start();
    
    return 0;
}