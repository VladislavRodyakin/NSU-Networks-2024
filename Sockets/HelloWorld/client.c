#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    // ip port
    if (argc < 3) {
        fprintf(stderr, "Invalid arguments\n");
        return -1;
    }

    char *message = "Hello, World!";
    int port_number = atoi(argv[2]);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        fprintf(stderr, "Failed to open socket");
        return -1;
    }

    struct sockaddr_in server_addres;
    memset(&server_addres, 0, sizeof(server_addres));
    server_addres.sin_family = AF_INET;
    server_addres.sin_port = htons(port_number);

    if (inet_pton(AF_INET, argv[1], &server_addres.sin_addr) <= 0)
    {
        fprintf(stderr, "Invalid address");
        close(socket_fd);
        return -1;
    }
        
    if (connect(socket_fd, (struct sockaddr*)&server_addres, sizeof(server_addres)) < 0)
    { 
        fprintf(stderr, "Failed to connect");
        close(socket_fd);
        return -1;
    }

    if (write(socket_fd, message, strlen(message)) < 0)
    {    
        fprintf(stderr, "Failed to write to socket");
        close(socket_fd);
        return -1;
    }

    close(socket_fd);
    return 0;
}