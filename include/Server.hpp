#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdio.h>  //fprintf()
#include <string.h> // memset()
#include <stdlib.h> //exit()


class Server {
private:
    int     port;
    int     pass;

    int                 status;     // проверка возвращаемого рез.
    struct addrinfo     hints;
    struct addrinfo     *servinfo;  // указатель на результаты

public:
	Server(const char *, const char *);
    
    int start(void);
};


#endif