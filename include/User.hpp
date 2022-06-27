#ifndef USER_HPP
#define USER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string>

#include "Channel.hpp"

#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define CYAN	"\033[1;36m"
#define RESET	"\033[0m"

#define MESSAGE_SIZE 1024

#define REGISTERED		1
#define BREAKCONNECTION	2
#define	DISCONNECT  -2

#define REGISTERED		1
#define INVISIBLE		2
#define RECEIVENOTICE	3
#define RECEIVEWALLOPS	4
#define IRCOPERATOR		5
#define AWAY			6
#define PINGING			7

struct flagUser
{
    bool registered;
    bool break_connect;

    flagUser(): registered(0), break_connect(0)
    {}
};

/*
** ====================================================================
** хранит даныне подключившегося клиента.
** ====================================================================
*/

class User {
private:
    std::string                     _pass;
    std::string                     _nick;
    std::string                     _user_name;
    std::string                     _real_name;
    std::string                     _hostname;
    std::string                     _servername;
    std::string                     message;
    std::string						awayMessage;
    flagUser                        _flagUser;
	std::vector<const Channel *>    channels;

    int         _socket_user;
    int         _status;

	User();
	User(const User& copy);
	User	&operator=(const User& other);
public:
	User(int sockfd, const std::string &host, std::string &servername);
	~User();

    int                 readMessage();
	void	    	    sendMessage(const std::string &msg) const;
	void				removeFlag(int flag);
    void	            addChannel(const Channel &channel);
    void	            removeChannel(const std::string &name);
    
    // getters
    const std::string	                &getMessages() const;
    struct flagUser		                getFlags() const;
	int				                    getSockfd() const;
	const std::string	                &getUsername() const;
	const std::string	                &getServername() const;
	const std::string	                &getNickname() const;
	const std::string	                &getRealname() const;
    std::string                         getPrefix() const;
    const std::vector<const Channel *>	&getChannels() const;
    const std::string	                &getPassword() const;
    const std::string	                &getAwayMessage() const;
    
    // setters
    void                setFlag(int _flag);
	void				setNickname(const std::string nickName);
	void				setUsername(const std::string userName);
	void 				setRealname(const std::string realName);
	void 				setQuitMessage(const std::string msg);
    void                setPassword(const std::string &pass);
};


#endif