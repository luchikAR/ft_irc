

#include "../include/Server.hpp"

bool	work = true;

void    sigHandler(int signum) {
	(void)signum;
	work = false;
}

Server::Server(const char *port, const char *pass) {
    this->_port = atoi(port);
	this->_pass = std::string(pass);
    this->_port_ch = port;
    this->_name_server = "localhost"; // was ft_irc
    this->_operators.push_back("gvenonat");
    this->_operators.push_back("ubolt");
    this->_operators.push_back("fldelena");

    memset(&hints, 0, sizeof(hints));
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_flags      = AI_PASSIVE;

	commands["PASS"] = &Server::passCmd;
	commands["NICK"] = &Server::nickCmd;
	commands["USER"] = &Server::userCmd;
	commands["QUIT"] = &Server::quitCmd;
	commands["PRIVMSG"] = &Server::privmsgCmd;
	commands["NOTICE"] = &Server::noticeCmd;
	commands["JOIN"] = &Server::joinCmd;
	commands["KICK"] = &Server::kickCmd;
    commands["PING"] = &Server::pingCmd;
    commands["PONG"] = &Server::pongCmd;
}

Server::~Server() {
    std::map<std::string, Channel *>::const_iterator	beg = channels.begin();
    std::map<std::string, Channel *>::const_iterator	end = channels.end();
    for (; beg != end; ++beg)
        delete (*beg).second;
    void();
}

void Server::_print_error(std::string str) const {
    std::cerr << CYAN << "[SERVER]: " << RED << str << RESET << "\r\n";
}

void Server::_system_mess(std::string str) const {
    std::cerr << CYAN << "[SERVER]: " << GREEN << str << RESET << "\n";
}

void Server::_client_mess(std::string str, User const &user, std::vector<std::string> comm) const {
    if (comm[0] != "PRIVMSG")
        std::cerr << PURPLE << "[" + user.getNickname() + "]: " << RESET << str << "\n";
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

void    Server::_ft_correct(std::vector<std::string> *str) {
    std::vector<std::string>::iterator i = str->begin();
    for ( ; i < str->end(); i++) {
        for (char comp = 2; comp <= 31; comp++) {
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
    _client_mess(user.getMessages(), user, comm);

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
	return (0);
}

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
    freeaddrinfo(servinfo);
    
    if ( listen(this->socket_fd, SOMAXCONN) == -1 )
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
			close(_users[i]->getSockfd());
			delete _users[i];
			_users.erase(_users.begin() + i);
			_usersFD.erase(_usersFD.begin() + i);
			--i;
		}
	}
}

int Server::start(void)
{
	_initializationServ();

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
        struct sockaddr_in their_addr;
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
            std::string name(this->_name_server);
            this->_users.push_back(new User(clientSocket, host, name) );
        }
        /* 
        ************************************************************
        ** processMessages
        ************************************************************
        */

        int	pret = poll(_usersFD.data(), _usersFD.size(), TIMEOUT);
        std::vector<int>	toErase;
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

        deleteBrokenConnections();
    }
    
   return (0);
}

bool Server::nickIsExist(const std::string nick) {
	size_t	usersCount = _users.size();
	for (size_t i = 0; i < usersCount; i++)
	{
		if (_users[i]->getNickname() == nick)
			return (true);
	}
	return (false);
}