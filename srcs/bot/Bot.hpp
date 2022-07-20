#ifndef BOT_HPP
# define BOT_HPP

#include <string>
#include <deque>
#include <iostream>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <fcntl.h>
#include <functional>
#include <unistd.h>
#include <sstream>
#include "Message.hpp"

#define BUFF_SIZE 512

# define BOT_ERROR_MSG "Bot error: please check your input and try again."

enum Status {
	READ,
	WRITE,
	DISCONNECT
};

class Bot {

public:
	Bot(int port, std::string password);
	~Bot();

	void	run();

protected:
	Bot(std::string name, int port, std::string password);

private:
	Bot() {};
	Bot(const Bot & copy);
	Bot &operator=(const Bot & copy);

	void	_setup(int port);
	void	_login(std::string password);

	void	_sendMsg();
	void	_readMsg();
	void	_handleMsg(std::string msg);

	void 	_pongMsg(std::string servName);
	void	_botMsg(Message &message);

	virtual std::string	_botReply(std::string msg) = 0;

	std::string	_name;
	int			_fd;
	Status		_status;

	std::deque<std::string> _msgQueue;
	std::string 			_msgBuff;
};

#endif
