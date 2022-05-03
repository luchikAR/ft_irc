

#include "../include/Server.hpp"

bool	work = true; // для сигналов

void    sigHandler(int signum) {
	(void)signum;
	work = false;
}

Server::Server(const char *port, const char *pass) {
    this->_port = atoi(port);
	this->_pass = atoi(pass);
    this->_port_ch = port;

    memset(&hints, 0, sizeof(hints));   // убедимся, что структура пуста
    hints.ai_family     = AF_UNSPEC;    // неважно, IPv4 или IPv6
    hints.ai_socktype   = SOCK_STREAM;  // TCP stream-sockets
    hints.ai_flags      = AI_PASSIVE;   // заполните мой IP-адрес за меня

    // хардкод
	commands["PASS"] = &Server::passCmd;
	commands["NICK"] = &Server::nickCmd;
	commands["USER"] = &Server::userCmd;
	commands["QUIT"] = &Server::quitCmd;
	commands["PRIVMSG"] = &Server::privmsgCmd;
	commands["NOTICE"] = &Server::noticeCmd;
	commands["JOIN"] = &Server::joinCmd;
	commands["KICK"] = &Server::kickCmd;
	// commands["PING"] = &Server::pingCmd;
	// commands["PONG"] = &Server::pongCmd;
	// commands["KILL"] = &Server::killCmd;
}

Server::~Server() {
    // Очистить _users массив 
    // картинку вывести :)
    void();
}

void Server::_print_error(std::string str) const {
    std::cerr << CYAN << "[SERVER]: " << RED << str << RESET << "\r\n";
}

void Server::_system_mess(std::string str) const {
    std::cerr << CYAN << "[SERVER]: " << GREEN << str << RESET << "\r\n";
}

void Server::_client_mess(std::string str) const {
    std::cerr << PURPLE << "[CLIENT]: " << RESET << str << "\r\n";
}

bool	Server::containsNickname(const std::string &nickname) const
{
	size_t	usersCount = _users.size();
	for (size_t i = 0; i < usersCount; i++)
	{
		if (_users[i]->getNickname() == nickname)
			return (true);
	}
	return (false);
}

User	*Server::getUserByName(const std::string &name)
{
	User	*ret;
	size_t	usersCount = _users.size();
	for (size_t i = 0; i < usersCount; i++)
		if (_users[i]->getNickname() == name)
			ret = _users[i];
	return ret;
}

// убирает сигналы типа (^D) (PA^https://github.com/levensta/IRC-ServerDSS) в сообщении как просит сабджект.
void    Server::_ft_correct(std::vector<std::string> *str) {
    std::vector<std::string>::iterator i = str->begin();
    for ( ; i < str->end(); i++) {
        for (char comp = 1; comp <= 31; comp++) {
            while (i->find(comp) != std::string::npos)
		        i->replace(i->find(comp), 1, "");
        }
    }
	char comp = ' ';
	i = str->begin();
	for ( ; i < str->end(); i++) {
		while (i->find(comp) != std::string::npos)
			i->replace(i->find(comp), 1, "");
	}
}

int	Server::makeCommand(User &user)
{
    std::vector<std::string> comm = split(user.getMessages(), ' ');
    _ft_correct(&comm);
    _client_mess(user.getMessages()); // для сервера
    // std::cout << "comm = '" << comm[0] << "' " << "len = " << comm[0].length() << std::endl;

	if (user.getFlags().registered == false && comm[0] != "QUIT" && comm[0] != "PASS" \
			&& comm[0] != "USER" && comm[0] != "NICK")
	    sendError(user, ERR_NOTREGISTERED);
	 else
	 {
	 	try
	 	{
	 		int ret = (this->*(commands.at(comm[0])))(comm, user);
	 		if (ret == DISCONNECT)
	 			return (DISCONNECT);
	 	}
	 	catch(const std::exception& e)
	 	{
	 		sendError(user, ERR_UNKNOWNCOMMAND, comm[0]);
	 	}
	 }
    // send(clientSocket, buf, bytesReceived + 1, 0);
	return (0);
}

/*
*************************************************************
** Технически стартуем сервер: Создаём, биндим и слушаем сокет
*************************************************************
*/
void    Server::_initializationServ()
{
    int status = getaddrinfo(NULL, this->_port_ch, &hints, &servinfo);
    if (status != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        exit (EXIT_FAILURE);
    }

    this->socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (this->socket_fd == -1) {
        _print_error("error on server: socket");
        exit (EXIT_FAILURE);
    }
    // для перезапуска и нового использования того же порта
	const int trueFlag = 1;
	if (setsockopt(this->socket_fd , SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
	{
       _print_error("setsockopt failed");
		exit(EXIT_FAILURE);
	}
    if ( bind(this->socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1 ) {
        close(this->socket_fd);
        _print_error("error on server: bind");
        exit (EXIT_FAILURE);
    }
    freeaddrinfo(servinfo); // и освобождаем связанный список
    
    if ( listen(this->socket_fd, SOMAXCONN) == -1 ) // SOMAXCONN вроде 128, default value
    {
        close(this->socket_fd);
        _print_error("error on server: listen");
        exit (EXIT_FAILURE);
    }
}

void	Server::deleteBrokenConnections()
{
	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (_users[i]->getFlags().break_connect == true)
		{
			// this->nicknamesHistory.addUser(*(_users[i]));
			// notifyUsers(*(_users[i]), ":" + _users[i]->getPrefix() + " QUIT :" + _users[i]->getQuitMessage() + "\n");
			close(_users[i]->getSockfd());
			// std::map<std::string, Channel *>::iterator	beg = channels.begin();
			// std::map<std::string, Channel *>::iterator	end = channels.end();
			// for (; beg != end; ++beg)
			// 	if ((*beg).second->containsNickname(_users[i]->getNickname()))
			// 		(*beg).second->disconnect(*(_users[i]));
			delete _users[i];
			_users.erase(_users.begin() + i);
			_usersFD.erase(_usersFD.begin() + i);
			--i;
		}
	}
}

int Server::start(void)
{
    // Создаём, биндим и слушаем сокет
	_initializationServ();

    // чтобы не блокировать сокет сервера
	fcntl(this->socket_fd, F_SETFL, O_NONBLOCK);
    _system_mess("server: waiting for connections…");

	signal(SIGINT, sigHandler);

/*
************************************************************
** Начинаем работать с сообщением от пользователей
************************************************************
*/
    while (work)
    {
        /* 
        ************************************************************
        ** grabConnection
        ************************************************************
        */
        struct sockaddr_in their_addr; // только для accept
        socklen_t addr_size = sizeof (their_addr);
        int clientSocket = accept(this->socket_fd, (struct sockaddr *)&their_addr, &addr_size);
        if (clientSocket >= 0)
        {
            char	host[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(their_addr.sin_addr), host, INET_ADDRSTRLEN);
            struct pollfd	pfd;
            pfd.fd = clientSocket;
            pfd.events = POLLIN;
            pfd.revents = 0;
            this->_usersFD.push_back(pfd);
            std::string name("ft_irc"); // servername
            this->_users.push_back(new User(clientSocket, host, name) );
        }
        /* 
        ************************************************************
        ** processMessages
        ************************************************************
        */

        // Убрать _usersFD.data() так как это C++ 11
        // &_usersFD
        int	pret = poll(_usersFD.data(), _usersFD.size(), TIMEOUT);
        std::vector<int>	toErase;
        // протестить > 0, так как ещё и меньше может вернуть
        if (pret != 0)
        {
            for (size_t i = 0; i < _usersFD.size(); i++)
            {
                if (_usersFD[i].revents & POLLIN)
                {
                    if (_users[i]->readMessage() == DISCONNECT)
                        _users[i]->setFlag(BREAKCONNECTION);
                    else if (makeCommand(*(_users[i])) == DISCONNECT)
                        _users[i]->setFlag(BREAKCONNECTION);
                }
                _usersFD[i].revents = 0;
            }
        }
        /* 
        ************************************************************
        ** checkConnectionWithUsers
        ************************************************************
        */
            // проверка соединения посылая ping
        /* 
        ************************************************************
        ** deleteBrokenConnections
        ************************************************************
        */
        deleteBrokenConnections();
        /* 
        ************************************************************
        ** deleteEmptyChannels
        ************************************************************
        */
        // deleteEmptyChannels();
    }
    
   return (0);
}

bool Server::nickIsExist(const std::string nick) {
	std::vector<User *> users = getUsers();
	for (size_t i = 0; i < users.size(); ++i) {
		if (users[i]->getNickname() == nick) {
			return true;
		}
	}
	return false;
}

bool Server::containsChannel(std::string &channel) const {
	try {
		channels.at(channel);
		return true;
	}
	catch (std::exception &e) {
		return false;
	}
}

int 	Server::privmsgCmd(const std::vector<std::string> &msg, User &user)
{
	if (msg.size() == 1)
		return (sendError(user, ERR_NORECIPIENT, msg[0]));
	if (msg.size() == 2)
		return (sendError(user, ERR_NOTEXTTOSEND));

	std::vector<std::string> receivers = split(msg[1], ',');
	std::set<std::string> uniqReceivers;

	for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end(); ++it)
	{
//		 checking if there contains dublicate receiver
//		if (uniqReceivers.find(receivers.front()) != uniqReceivers.end())     <--
//																				 |	эти строки лишние, т к в set не допускаются повторения
//			return (sendError(user, ERR_TOOMANYTARGETS, receivers.front()));  <--
//		 if receiver is channel
		if (receivers.front()[0] == '#' || receivers.front()[0] == '$')
		{
//			 checking if there such a channel
			if (!this->containsChannel(receivers.front()))
				return (sendError(user, ERR_NOSUCHNICK, receivers.front()));
//			 check that the current user is in the channel
			if (!this->channels[receivers.front()]->containsNickname(user.getNickname()))
				return (sendError(user, ERR_CANNOTSENDTOCHAN, receivers.front()));
		}
//			 checking if there such a nickname
		else if (this->nickIsExist(*it) == false)
			return (sendError(user, ERR_NOSUCHNICK, msg[1]));
		uniqReceivers.insert(*it);
//		receivers.pop_back();
	}
	for (std::set<std::string>::iterator it = uniqReceivers.begin(); it != uniqReceivers.end(); ++it)
	{
		if ((*it)[0] == '#' || (*it)[0] == '$')
		{
			Channel *receiverChannel = this->channels[*it];
			// check that user can send message to channel (user is operator or speaker on moderated channel)
			if (receiverChannel->getFlags().nomsgout & MODERATED && (!receiverChannel->isOperator(user) && !receiverChannel->isSpeaker(user)))
				sendError(user, ERR_CANNOTSENDTOCHAN, *it);
			else
				receiverChannel->sendMessage(msg[0] + " " + *it + " :" + msg[1] + "\n", user, false);
		}
		else
		{
//			if (msg[0] == "PRIVMSG" && (this->getUserByName(*it)->getFlags() & AWAY))																	 <--
//																																							| этот чеккер нужен для автоответа с флагом AWAY ??
//				sendReply(user.getServername(), user, RPL_AWAY, *it, this->getUserByName(*it)->getAwayMessage()); //для чего тут нужен getAwayMessage??? <--
			if (msg[0] != "NOTICE" || (this->getUserByName(*it)->getFlags().registered & RECEIVENOTICE))
				this->getUserByName(*it)->sendMessage(":" + user.getPrefix() + " " + msg[0] + " " + *it + " :" + msg[1] + "\n");
		}
	}
	return 0;
}

int		Server::noticeCmd(const std::vector<std::string> &msg, User &user)
{
	int size = msg.size();
	std::string user1 = user.getNickname();
	return size;
}