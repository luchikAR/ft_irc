

#include "../include/Server.hpp"

Server::Server(const char *port, const char *pass) {
    this->port = atoi(port);
	this->pass = atoi(pass);
    this->port_ch = port;

    memset(&hints, 0, sizeof(hints));   // убедимся, что структура пуста
    hints.ai_family     = AF_UNSPEC;    // неважно, IPv4 или IPv6
    hints.ai_socktype   = SOCK_STREAM;  // TCP stream-sockets
    hints.ai_flags      = AI_PASSIVE;   // заполните мой IP-адрес за меня
}

Server::~Server() {
    // пока хз, может картинку вывести :)
    void();
}

void Server::_print_error(std::string str) {
    std::cerr << RED << "[SERVER]: " << str << RESET << std::endl;
}

void Server::_system_mess(std::string str) {
    std::cerr << CYAN << "[SERVER]: " << str << RESET << std::endl;
}

int Server::start(void)
{
    // servinfo указывает на связанный список на одну или больше структуру <i>addrinfo</i>
    int status = getaddrinfo(NULL, this->port_ch, &hints, &servinfo);
    if (status != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        exit (1);
    }
    
    this->socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (this->socket_fd == -1) {
        _print_error("error on server: socket");
        exit (1);
    }
    if ( bind(this->socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1 ) {
        close(this->socket_fd);
        _print_error("error on server: bind");
        exit (1);
    }
    freeaddrinfo(servinfo); // и освобождаем связанный список
    
    if ( listen(this->socket_fd, SOMAXCONN) == -1 ) // SOMAXCONN вроде 128, default value
    {
        close(this->socket_fd);
        _print_error("error on server: listen");
        exit (1);
    }
    _system_mess("server: waiting for connections…");

    struct sockaddr_storage their_addr; // только дял accept ???
    socklen_t addr_size = sizeof (their_addr);
    
    int clientSocket = accept(this->socket_fd, (struct sockaddr *)&their_addr, &addr_size);
    if (clientSocket == -1) {
        _print_error("error on server: accept");
        exit (1);
    }
    // ---------------------------------
    // bbtcpserver.cpp не понятно зачем нужная часть
    // --------------------------------

    char buf[4096];
    
    while (true)
    {
        memset(buf, 0, 4096);
 
        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            _print_error("Error in recv(). Quitting");
            break;
        }
 
        if (bytesReceived == 0)
        {
            _print_error("Client disconnected ");
            break;
        }
 
        std::cout << std::string(buf, 0, bytesReceived) << std::endl;
 
        // Echo message back to client
        send(clientSocket, buf, bytesReceived + 1, 0);
    }
    close(clientSocket);
    
   return (0);
}