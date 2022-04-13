

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

#define REGISTERED		1
#define BREAKCONNECTION	2
#define	DISCONNECT  -2

struct flagUser
{
    bool break_connect;

    flagUser(): break_connect(0) 
    {}
};


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
    
    // от levensta
    std::string _hostname; // зачем?)
    std::string _servername; // зачем?)
    std::string message;
    flagUser    _flagUser;

    int         _socket_user;
    int         _status;

	User();
	User(const User& copy);
	User	&operator=(const User& other);
public:
	User(int sockfd, const std::string &host, std::string &servername);
	~User();

    int     readMessage();

    // setters
    void    setFlag(int _flag);

    // getters
    const std::string	&getMessages() const;
    struct flagUser		getFlags() const;
	int				    getSockfd() const;
};


#endif