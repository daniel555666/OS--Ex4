/*
** server.c -- a stream socket server demo
*/
#include "Ex4.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>

#define PORT "3490" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold
pstack st = NULL;
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}
void *myThreadFun(int new_fd)
{
    printf("new client connect %d\n", new_fd);
    char buf[MAXDATASIZE];
    int numbytes;
    while (1)
    {
        if ((numbytes = recv(new_fd, buf, MAXDATASIZE - 1, 0)) == -1)
        {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        printf("GET: %s\n", buf);
        if (!strncmp(buf, "EXIT", 4))
        {
            break;
        }
        else if (!strncmp(buf, "PUSH ", 5))
        {
            PUSH(&st, &buf[5]);
            bzero(buf, MAXDATASIZE);
            strcpy(buf, "I PUSH");
        }
        else if (!strcmp(buf, "TOP"))
        {
            bzero(buf, MAXDATASIZE);
            if (IsEmpty(st))
            {
                strcpy(buf, "The stack is empty");
            }
            else
            {
                strcpy(buf, TOP(st));
            }
        }
        else if (!strcmp(buf, "POP"))
        {
            bzero(buf, MAXDATASIZE);
            if (IsEmpty(st))
            {
                strcpy(buf, "The stack is empty");
            }
            else
            {
                strcpy(buf, POP(&st));
            }
        }

        printf("SEND: %s\n", buf);
        if (send(new_fd, buf, strlen(buf), 0) == -1)
        {
            perror("send");
            exit(1);
        }
    }
    printf("Colse %d\n", new_fd);
    close(new_fd);
    return NULL;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1)
    { // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);
        pthread_t th;
        pthread_create(&th, NULL, myThreadFun, new_fd);

        // if (i == threadSize)
        // {
        //     for (int j = 0; j < threadSize; j++)
        //     {
        //         pthread_join(th[j], NULL);
        //         printf("%d\n", j);
        //     }
        //     i = 0;
        // }
    }
    return 0;
}