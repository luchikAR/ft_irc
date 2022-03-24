
// c masandilov.ru
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdio.h> //fprintf()
#include <string.h> // memset()
#include <stdlib.h> //exit()

int main(int argc, char *argv[])
{
    // ------------------------------------------------
    // заполняем структуру addrinfo и связный список
    int                 status;     // проверка возвращаемого рез.
    struct addrinfo     hints;
    struct addrinfo     *servinfo;  // указатель на результаты

    memset(&hints, 0, sizeof hints); // убедимся, что структура пуста
    hints.ai_family = AF_UNSPEC;     // неважно, IPv4 или IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream-sockets
    hints.ai_flags = AI_PASSIVE;     // заполните мой IP-адрес за меня
    
    // getaddrinfo  (имя хоста или IP-адрес для соединения #указать argv[1]!#, 
    //              сервис который может быть номером порта («80») или названием сервиса («http»), 
    //              указатель на struct addrinfo, которую вы уже заполнили минимальной информацией,
    //              ) 
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

    
    
    return 0;
}