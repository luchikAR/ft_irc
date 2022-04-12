

#include "../include/Server.hpp"

bool	work = true; // для сигналов

void	Server::_sigHandler(int signum) {
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
    _g_cmd_name[0] = "NICK";
    _g_cmd_name[1] = "PASS";
    _g_cmd_name[2] = "USER";
    _g_cmd_name[3] = "NOTICE";
    _g_cmd_name[4] = "JOIN";
    _g_cmd_name[5] = "KICK";
    _g_cmd_name[6] = "PRIVMSG";
}

Server::~Server() {
    // Очистить _users массив 
    // картинку вывести :)
    void();
}

void Server::_print_error(std::string str) {
    std::cerr << CYAN << "[SERVER]: " << RED << str << RESET << "\r\n";
}

void Server::_system_mess(std::string str) {
    std::cerr << CYAN << "[SERVER]: " << GREEN << str << RESET << "\r\n";
}

int Server::start(void)
{
/*
*************************************************************
** Технически стартуем сервер: Создаём, биндим и слушаем сокет
*************************************************************
*/

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
/*
*****************************************************************
** O_NONBLOCK
** (если возможно, то файл открывается в режиме non-blocking. 
** Ни open, ни другие последующие операции над возвращаемым описателем файла не заставляют вызывающий процесс ждать.
** Этот режим не оказывает никакого действия на не-FIFO файлы.);
*****************************************************************
*/
	fcntl(this->socket_fd, F_SETFL, O_NONBLOCK);
    _system_mess("server: waiting for connections…");

/* 
************************************************************
** Начинаем работать с сообщением от пользователей
************************************************************
*/

	signal(SIGINT, sigHandler);
    char buf[4096];

    while (work)
    {

/* 
************************************************************
** grabConnection
************************************************************
*/
        struct sockaddr_storage their_addr; // только для accept
        socklen_t addr_size = sizeof (their_addr);
        int clientSocket = accept(this->socket_fd, (struct sockaddr *)&their_addr, &addr_size);
        if (clientSocket >= 0)
        {
            char	host[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(sockaddr.sin_addr), host, INET_ADDRSTRLEN);
            struct pollfd	pfd;
            pfd.fd = clientSocket;
            pfd.events = POLLIN;
            pfd.revents = 0;
            this->_usersFD.push_back(pfd);
            this->_users.push_back(new User(clientSocket, host, name));
        }
/* 
************************************************************
** processMessages
************************************************************
*/

// Убрать _usersFD.data() так как это C++ 11
        int	pret = poll(_usersFD.data(), _usersFD.size(), timeout);
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
                    else if (hadleMessages(*(_users[i])) == DISCONNECT)
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
        memset(buf, 0, 4096);
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1) {
            _print_error("Error in recv(). Quitting");
            break;
        }
        if (bytesReceived == 0) {
            _print_error("Client disconnected ");
            break;
        }
        // Server.registered(buf, User);
        // должен быть создан юзер
        // в его поля занашу три комады
        // - правильный пароль и не занятый ник
 
        _system_mess(std::string(buf, 0, bytesReceived));
        
        send(clientSocket, buf, bytesReceived + 1, 0);
    }
    close(clientSocket);
    
   return (0);
}