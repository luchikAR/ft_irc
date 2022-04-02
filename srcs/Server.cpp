

#include "../include/Server.hpp"

Server::Server(const char *port, const char *pass) {
    this->port = atoi(port);
	this->pass = atoi(pass);
    this->port_ch = port;
    
    // потому что worning - убрать в будущем
    socket_fd = 0;
    servinfo = 0;

    memset(&hints, 0, sizeof(hints));   // убедимся, что структура пуста
    hints.ai_family     = AF_UNSPEC;    // неважно, IPv4 или IPv6
    hints.ai_socktype   = SOCK_STREAM;  // TCP stream-sockets
    hints.ai_flags      = AI_PASSIVE;   // заполните мой IP-адрес за меня
}

Server::~Server() {
    // пока хз, может картинку вывести :)
    void();
}

int Server::start(void)
{
    int status = getaddrinfo(NULL, this->port_ch, &hints, &servinfo);
    if (status != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        exit (1);
    }

    // servinfo теперь указывает на связанный список на одну или больше структуру <i>addrinfo</i>
    // … Делаем что-то, где используем структуру <i>addrinfo</i> ….
    
    this->socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (this->socket_fd == -1) {
        std::cerr << "error on server: socket" << std::endl;
        exit (1);
    }
    if ( bind(this->socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1 ) {
        close(this->socket_fd);
        std::cerr << "error on server: bind" << std::endl;
        exit (1);
    }
    freeaddrinfo(servinfo); // и освобождаем связанный список
    
    if ( listen(this->socket_fd, BACKLOG) == -1 ) {
        close(this->socket_fd);
        std::cerr << "error on server: listen" << std::endl;
        exit (1);
    }
    std::cout << ("server: waiting for connections…\n");

    struct sockaddr_storage their_addr; // только дял accept ???
    socklen_t addr_size = sizeof (their_addr);
    int clientSocket = accept(this->socket_fd, (struct sockaddr *)&their_addr, &addr_size);
    if (clientSocket == -1) {
        std::cerr << "error on server: accept" << std::endl;
        exit (1);
    }
    
    char buf[4096];
    
    while (true)
    {
        memset(buf, 0, 4096);
 
        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            break;
        }
 
        if (bytesReceived == 0)
        {
            std::cout << "Client disconnected " << std::endl;
            break;
        }
 
        std::cout << std::string(buf, 0, bytesReceived) << std::endl;
 
        // Echo message back to client
        send(clientSocket, buf, bytesReceived + 1, 0);
    }
    close(clientSocket);
    
   return (0);
}