

#ifndef USER_HPP
#define USER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>

#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define CYAN	"\033[1;36m"
#define RESET	"\033[0m"


class User {
private:

    std::string   _pass, _nick, _user_name;

    // int                 socket_fd;     // проверка возвращаемого рез.
    // struct addrinfo     hints;
    // struct addrinfo     *servinfo;  // указатель на результаты

public:
	User();
	~User();

    int setNick(char* nick);
    int setUsername(char* username);
};


#endif