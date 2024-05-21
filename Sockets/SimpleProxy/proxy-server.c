#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h>
#include <poll.h>


#define PORT 3000
#define MAX_CLIENTS 5
#define BUFFER_SIZE 512
#define DEST_PORT 4000


void handle_client(struct sockaddr_in* destination_address, char* buffer, size_t readed_bytes)
{
    printf("handling the client\n");
    int dest_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(connect(dest_socket, destination_address, sizeof(*destination_address)) < 0)
    {
        printf("cant connect to the server\n");
        return;
    }

    struct pollfd server_fd;
    memset(&server_fd, 0, sizeof(server_fd));
    server_fd.fd = dest_socket;
    server_fd.events = POLLOUT;
    int timeout = poll(&server_fd, 1, 1000);
    if(timeout == 0)
    {
        printf("Cant write to the server\n");
        return;
    }

    write(dest_socket, buffer, readed_bytes);
    close(dest_socket);
    printf("stop handling\n");
    return;    
}

int main()
{
    char buffer[BUFFER_SIZE];

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1)
    {
        perror("Server socket cant be created");
        exit(1);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(struct sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
   
    struct sockaddr_in destination_address;
    memset(&destination_address, 0, sizeof(struct sockaddr_in));
    destination_address.sin_family = AF_INET;
    destination_address.sin_addr.s_addr = INADDR_ANY;
    destination_address.sin_port = htons(DEST_PORT);

    
    if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1)
    {
        perror("Cant bind server socket");
        exit(1);
    }
    
    if(listen(server_socket, MAX_CLIENTS) == -1)
    {
        perror("Cant listen");
        exit(1);
    }

    struct pollfd server_fds[MAX_CLIENTS + 1];
    memset(server_fds, 0, sizeof(server_fds));
    server_fds[0].fd = server_socket;
    server_fds[0].events = POLLIN;
    for(nfds_t clientsSocketNumber = 1; clientsSocketNumber <= MAX_CLIENTS; clientsSocketNumber++)
    {
        server_fds[clientsSocketNumber].fd = -1;
        server_fds[clientsSocketNumber].events = POLLIN;
    }
    nfds_t clientCounter = 0;
    nfds_t currentSockets = 1;
    int serverIsFull = 0;
    while(1)
    {
        if(poll((struct pollfd *)&server_fds[serverIsFull], currentSockets, -1) == -1)
        {
            perror("Cant using poll");
            exit(1);
        }

        if((server_fds[0].revents & POLLIN) && (serverIsFull == 0)) 
        {
            int clientFd;
            if((clientFd = accept(server_socket, NULL, NULL)) == -1)
            {
                perror("Cant accept");
                exit(1);
            }
            printf("new client\n");
            clientCounter++;
            server_fds[clientCounter].fd = clientFd;
            currentSockets++;
            if(clientCounter == MAX_CLIENTS)
            {
                serverIsFull = 1;
            }
        }

        poll((struct pollfd *)&server_fds[serverIsFull], currentSockets, -1);   
        for(nfds_t clientSocketNumber = 1; clientSocketNumber <= clientCounter; clientSocketNumber++)
        {
            if(server_fds[clientSocketNumber].revents & POLLIN)
            {
                
                size_t readed_bytes = read(server_fds[clientSocketNumber].fd, buffer, BUFFER_SIZE);
                if(readed_bytes == 0)
                {
                    printf("delete client\n");
                    close(server_fds[clientSocketNumber].fd);
                    server_fds[clientSocketNumber].fd = server_fds[clientCounter].fd;
                    server_fds[clientCounter].fd = -1;
                    clientCounter--;
                    currentSockets--;
                    serverIsFull = 0;
                    continue;
                }
                handle_client(&destination_address, buffer, readed_bytes);
            }

        }
         
    }

}