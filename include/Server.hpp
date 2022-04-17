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
#include <vector>
#include <map>
#include <poll.h>
#include <fcntl.h> // для работы с флагами файла
#include <csignal>
#include <arpa/inet.h> // переводет ip в имя

#include "../include/User.hpp"
#include "../include/utils.hpp"
#include "../include/Channel.hpp"
#include "../include/send_modul.hpp"

#define COUT_COMMAND 7
#define TIMEOUT 1000 // 1 секунда

#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define PURPLE	"\033[1;35m"
#define CYAN	"\033[1;36m"
#define RESET	"\033[0m"

class Server;

typedef  int (Server::*Method) (const std::vector<std::string> &, User &);

class Server {
private:
/* 
************************************************************
** Основные настройки сервера
************************************************************
*/
	std::vector<User *>				_users;
	std::vector<struct pollfd>		_usersFD;
	const char*						_port_ch;
    int     						_port;
    int     						_pass;
	std::map<std::string, Method>	commands;
	static const unsigned long		maxChannels = 20;

/* 
************************************************************
** Данные для подключения
************************************************************
*/
    int                 socket_fd;
    struct addrinfo     hints;
    struct addrinfo     *servinfo;
	
	void	_initializationServ();
	void	_sigHandler(int signum);
	void	deleteBrokenConnections();
	void    _ft_correct(std::vector<std::string> *str);
	int		makeCommand(User &user);

	void	_print_error(std::string str) const;
	void	_system_mess(std::string str) const;
	void	_client_mess(std::string str) const;

	// command
	int				connectToChannel(const User &user, const std::string &name, const std::string &key);
	int				passCmd(const std::vector<std::string> &msg, User &user);
	int				nickCmd(const std::vector<std::string> &msg, User &user);
	int				userCmd(const std::vector<std::string> &msg, User &user);
	int				quitCmd(const std::vector<std::string> &msg, User &user);
//	int				privmsgCmd(const std::vector<std::string> &msg, User &user);
//	int				noticeCmd(const std::vector<std::string> &msg, User &user);
	int				joinCmd(const std::vector<std::string> &msg, User &user);
	int				kickCmd(const std::vector<std::string> &msg, User &user);

	Server();
	Server(const Server& copy);
	Server	&operator=(const Server& other);
public:
	Server(const char *port, const char *pass);
	~Server();

    int		start(void);

	std::vector<User *> getUsers() const {
		return _users;
	}

	bool nickIsExist(const std::string nick);
};


#endif