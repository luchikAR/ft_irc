

#include "../include/Server.hpp"

Server::Server(const char *port, const char *pass) {
    this->port = atoi(port);
	this->pass = atoi(pass);
    
    // потому что worning
    status = 0;
    servinfo = 0;

    memset(&hints, 0, sizeof(hints)); // убедимся, что структура пуста
    hints.ai_family     = AF_UNSPEC;     // неважно, IPv4 или IPv6
    hints.ai_socktype   = SOCK_STREAM; // TCP stream-sockets
    hints.ai_flags      = AI_PASSIVE;     // заполните мой IP-адрес за меня
}

int Server::start(void)
{

    /* old version
    status = getaddrinfo("www.example.com", "http", &hints, &servinfo);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return (1);
    }
    // servinfo теперь указывает на связанный список на одну или больше структуру <i>addrinfo</i>// … Делаем что-то, где используем структуру <i>addrinfo</i> ….
    
    int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol); // создаем сокет

    bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);

    freeaddrinfo(servinfo); // и освобождаем связанный список
    */
   return (0);
}