#include "Bot.hpp"

Bot::Bot(int port, std::string password) : _name("bot"), _status(READ) {
	_setup(port);
	_login(password);
}

Bot::Bot(std::string name, int port, std::string password) : _name(name) , _status(READ) {
	_setup(port);
	_login(password);
}

Bot::~Bot() {}

void	Bot::run() {
	while(true) {
		struct pollfd fds;
		/*
		 * struct pollfd {
			* int fd; описатель файла
			* short events; запрошенные события
			* short revents; возвращенные события
		 * }
		 */
		fds.fd = _fd;
		fds.revents = 0;
		if (READ == _status)
			fds.events = POLLIN; //Можно считывать данные
		else
			fds.events = POLLOUT; //Запись не будет блокирована
		
		int r = poll(&fds, 1, 10);
		if (r < 0) {
			char buff[256];
			strerror_r(errno, buff, 256);
			throw std::runtime_error(buff);
		}
		if (r > 0) {
			if (fds.revents == POLLIN) {
				_readMsg();
			}
			else if (fds.revents == POLLOUT) {
				sleep(1);
				_sendMsg();
			}
			else if (fds.revents & (POLLERR|POLLHUP)) { //Произошла ошибка | "Положили трубку"
				close(_fd);
				return;
			}
			else if (fds.revents == POLLNVAL) { //Неверный запрос: fd не открыт
				std::cerr << "File descriptor " << _fd << " is invalid " << std::endl;
				return;
			}
		}
	}
}

void Bot::_setup(int port) {
	struct sockaddr_in saddr;
	std::memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = (inet_addr("127.0.0.1"));
	saddr.sin_port = htons(port);

	char buff[256];

	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1) {
		strerror_r(errno, buff, 256);
		throw std::runtime_error(buff);
	}
	if (connect(_fd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        strerror_r(errno, buff, 256);
		close(_fd);
		throw std::runtime_error(buff);
    }

	std::cout << "Bot listening on port " << port << std::endl;
}

void	Bot::_login(std::string password) {
	std::string passMsg = "PASS " + password;
	std::string nickMsg = "NICK " + _name;
	std::string userMsg = "USER " + _name + " " + _name + " " + _name + " " + _name;
	
	_msgQueue.push_back(passMsg);
	_msgQueue.push_back(nickMsg);
	_msgQueue.push_back(userMsg);
	_status = WRITE;
}

void	Bot::_sendMsg() {
	std::cout << "sending..." << std::endl;
	if (_msgQueue.empty()) {
		_status = READ;
		return;
	}
	std::string msg = _msgQueue.front() + "\r\n";
	_msgQueue.pop_front();
	std::cout << "msg: " << msg << std::endl;
	int r = send(_fd, msg.c_str(), msg.size(), 0);
	if (r < 0) {
		std::cerr << "Send error: " << r << std::endl;
	}
}

void	Bot::_readMsg() {
	std::cout << "reading..." << std::endl;
	char buff[BUFF_SIZE + 1];
 	bzero(buff, BUFF_SIZE + 1);
	int r = recv(_fd, buff, BUFF_SIZE, 0);
	if (r <= 0) {
		std::cerr << "\rPoll: recv error" << std::endl;
		return;
	}
	buff[r] ='\0';
	std::string b(buff);
	_handleMsg(b);
}

void Bot::_handleMsg(std::string msg) {
	_msgBuff.append(msg);
	size_t i = 0;
    while (true) {
		i = _msgBuff.find("\r\n");
		if (i == std::string::npos) { //если ничего не нашли
			i = _msgBuff.find("\n");
			if (i != std::string::npos)
				_msgBuff.insert(i, "\r");
		}
		if (i != std::string::npos) { //если нашли такой символ
        	std::string mess = _msgBuff.substr(0, i);
			_msgBuff.erase(0, i + 2);
			if (mess.length() > 512) {
				mess.erase(511, mess.length() - 510);
				mess.append("\r\n");
			}

        	Message message(mess);
			std::string command = message.getCommand();
			if ("PRIVMSG" == command)
				_botMsg(message);
			else if ("PING" == command)
				_pongMsg(message.getParams()[0]);
		}
		else
			break ;
    }
}

void Bot::_pongMsg(std::string servName) {
	std::string msg = "PONG :" + servName;
	_msgQueue.push_back(msg);
	_status = WRITE;
}

std::vector<std::string> split(std::string & s)
{
	std::vector<std::string> vect;
	std::stringstream ss(s);
  	std::string buff;

	while(std::getline(ss, buff,'\n'))
		vect.push_back(buff);
	return vect;
}

void Bot::_botMsg(Message &message) {
	std::string text;
	std::string prefix = message.getPrefix();
	int i = prefix.find("!");
	std::string toNick = prefix.substr(0, i);
	if (message.getParams().size() != 2 && !message.getParams()[1].empty())
		text = BOT_ERROR_MSG;
	else {
		text = _botReply(message.getParams()[1]);
	}
	std::vector<std::string> lines = split(text);
	for (size_t i = 0; i < lines.size(); i++)
	{
		std::string response = "PRIVMSG " + toNick + " :" + lines[i];
		_msgQueue.push_back(response);
		std::cout << response << std::endl;
	}
	_status = WRITE;
}