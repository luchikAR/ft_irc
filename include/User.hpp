

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

#define MESSAGE_SIZE 1024

/*
** ====================================================================
** хранит даныне подключившегося клиента.
** ====================================================================
*/

class User {
private:
    std::string _pass;
    std::string _nick;
    std::string user_name;
    
    std::string _hostname; // зачем?)
    std::string _servername; // зачем?)
    std::string message;

    int         _socket_user;
    int         _status;

	User();
	User(const User& copy);
	User	&operator=(const User& other);
public:
	User(int sockfd, const std::string &host, std::string &servername);
	~User();

    int readMessage();
};


#endif