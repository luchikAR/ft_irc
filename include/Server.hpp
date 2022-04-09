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


#include "../include/User.hpp"
#define BACKLOG 10
#define COUT_COMMAND 7

#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define CYAN	"\033[1;36m"
#define RESET	"\033[0m"


	// std::string _g_cmd_name[COUT_COMMAND] = {	"NICK",
	// 								  		"PASS",
	// 								  		"USER",
	// 								  		"NOTICE",
	// 								  		"JOIN",
	// 								  		"KICK",
	// 								  		"PRIVMSG"
	// 								  	};

class Server {
private:
	
	std::string _g_cmd_name[COUT_COMMAND]; // команды которые мы обрабатываем
	const char*	_port_ch; 			// тест для структуры addrinfo
    int     	_port;
    int     	_pass;

	// map<std::string, User> 	user;	// список пользователей, где string это имя пользователя
    int                 socket_fd;     // проверка возвращаемого рез.
    struct addrinfo     hints;
    struct addrinfo     *servinfo;  // указатель на результаты

	void _print_error(std::string str);
	void _system_mess(std::string str);
public:
	Server(const char *, const char *);
	~Server();

    int start(void);
};


#endif