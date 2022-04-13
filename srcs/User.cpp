
#include "../include/User.hpp"


User::User(int sockfd, const std::string &host, std::string &servername) :
    _hostname(host), _servername(servername), _socket_user(sockfd) { }

User::~User() { }

int		User::readMessage()
{
	std::string	text;
	// if (messages.size() > 0)
	// 	text = messages.front();
	char    buffer[MESSAGE_SIZE];
	int     bytesRead;
	while ((bytesRead = recv(_socket_user, buffer, (MESSAGE_SIZE - 1), 0)) > 0)
	{
		buffer[bytesRead] = 0;
		text += buffer;
		buffer[0] = 0;
		if (text.find('\n') != std::string::npos)
			break;
	}
	if (text.length() > 512)
		text = text.substr(0, 510) + "\r\n";
	if (bytesRead == 0)
		return (DISCONNECT);
	while (text.find("\r\n") != std::string::npos)
		text.replace(text.find("\r\n"), 2, "\n");
	// if (text.size() > 1)
	// 	messages = split(text, '\n', true);
	this->message = text;
	return 0;
}

void    User::setFlag(int _flag) {
	if (_flag == BREAKCONNECTION)
		this->_flagUser.break_connect = true;
}

const std::string	&User::getMessages() const {
	return (this->message);
}

struct flagUser	User::getFlags() const {
	return (this->_flagUser);
}

int	User::getSockfd() const {
	return(this->_socket_user);
}