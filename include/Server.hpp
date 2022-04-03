#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdio.h>  //fprintf()
#include <string.h> // memset()
#include <stdlib.h> //exit()
#include <unistd.h> // close()

#define BACKLOG 10

std::string g_cmd_name[10] = {"NICK",
									  "PASS",
									  "USER",
									  "SERVER",
									  "JOIN",
									  "QUIT",
									  "KICK",
									  "PRIVMSG",
									  "CONNECT",
									  "KILL",
									  };

class Server {
private:
	const char*	port_ch; // тест для структуры addrinfo
    int     port;
    int     pass;

    int                 socket_fd;     // проверка возвращаемого рез.
    struct addrinfo     hints;
    struct addrinfo     *servinfo;  // указатель на результаты

public:
	Server(const char *, const char *);
	~Server();

    int start(void);
};


#endif