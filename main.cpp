
// c masandilov.ru
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdio.h> //fprintf()
#include <string.h> // memset()
#include <stdlib.h> //exit()

int main(int argc, char *argv[])
{
    int                 status;
    struct addrinfo     hints;
    struct addrinfo     *servinfo;  // указатель на результаты

    memset(&hints, 0, sizeof hints); // убедимся, что структура пуста
    hints.ai_family = AF_UNSPEC;     // неважно, IPv4 или IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream-sockets
    hints.ai_flags = AI_PASSIVE;     // заполните мой IP-адрес за меня
    if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    // servinfo теперь указывает на связанный список на одну или больше структуру <i>addrinfo</i>// … Делаем что-то, где используем структуру <i>addrinfo</i> ….

    fprintf(stderr, "servinfo->ai_family: %d\n", servinfo->ai_family);
    fprintf(stderr, "servinfo->ai_socktype: %d\n", servinfo->ai_socktype);
    fprintf(stderr, "servinfo->ai_protocol: %d\n", servinfo->ai_protocol);

    freeaddrinfo(servinfo); // и освобождаем связанный список

    
    
    return 0;
}