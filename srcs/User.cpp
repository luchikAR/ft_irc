
#include "../include/User.hpp"


User::User(int sockfd, const std::string &host, std::string &servername) :
    _socket_user(sockfd), _hostname(host), _servername(servername) { }

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
	return 0;
}