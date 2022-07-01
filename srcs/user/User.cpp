
#include "../include/User.hpp"


User::User(int sockfd, const std::string &host, std::string &servername) :
    _hostname(host), _servername(servername), _socket_user(sockfd) { }

User::~User() { }

int		User::readMessage()
{
	std::string	text;
	char    	buffer[MESSAGE_SIZE];
	int     	bytesRead;
	
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
		text.replace(text.find("\r\n"), 2, " ");
	this->message = text;
	std::cerr << YELLOW << "[reading]: " << RESET << text << "\n";
	return 0;
}

void	User::sendMessage(const std::string &msg) const
{
	if (msg.size() > 0)
		send(this->_socket_user, msg.c_str(), msg.size(), 0);
}

void    User::setFlag(int _flag) {
	switch (_flag)
	{
	case REGISTERED:
		this->_flagUser.registered = true;
		break;
	case BREAKCONNECTION:
		this->_flagUser.break_connect = true;
		break;
	default:
		break;
	}
}

void	User::setPassword(const std::string &pass) {
	_pass = pass;
}

void	User::removeFlag(int _flag) {
	switch (_flag)
	{
	case REGISTERED:
		this->_flagUser.registered = false;
		break;
	case BREAKCONNECTION:
		this->_flagUser.break_connect = false;
		break;
	default:
		break;
	}
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

const std::string	&User::getUsername() const {
	return (this->_user_name);
}

const std::string	&User::getServername() const {
	return(this->_servername);
}

const std::string	&User::getNickname() const {
	return(this->_nick);
}

const std::string	&User::getRealname() const {
	return(this->_real_name);
}

const std::string	&User::getPassword() const
{
	return _pass;
}

const std::string	&User::getAwayMessage() const
{
	return awayMessage;
}

std::string	User::getPrefix() const {
	return std::string(this->_nick + "!" + this->_user_name + "@" + this->_hostname);
}

const std::vector<const Channel *>	&User::getChannels() const
{
	return this->channels;
}

void	User::addChannel(const Channel &channel)
{
	channels.push_back(&channel);
}

void	User::removeChannel(const std::string &name)
{
	std::vector<const Channel *>::iterator	begin = channels.begin();
	std::vector<const Channel *>::iterator	end = channels.end();
	for (; begin != end; ++begin)
		if ((*begin)->getName() == name)
			break ;
	channels.erase(begin);
}

void User::setNickname(const std::string nickName) {
	this->_nick = nickName;
}

void User::setUsername(const std::string userName) {
	this->_user_name = userName;
}

void User::setRealname(const std::string realName) {
	this->_real_name = realName;
}

void User::setQuitMessage(const std::string msg) {
	std::cout << "Quit - " << msg << std::endl;
}