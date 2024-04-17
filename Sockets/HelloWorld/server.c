#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    //port

    char buffer[512];
    if (argc < 2)
    {
        fprintf(stderr, "No port number");
        return -1;
    }

    int port_number = atoi(argv[1]);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        fprintf(stderr, "Failed to open socket");
        return -1;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    if (bind(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
    {
        fprintf(stderr, "Failed to bind");
        return -1;
    }

    listen(socket_fd, 1);
 
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);
    int new_socket_fd, read_size;
    while (1)
    {
        new_socket_fd = accept(socket_fd, (struct sockaddr*) &client_address, &client_length);
        if (new_socket_fd < 0)
        {
            fprintf(stderr, "Failed to accept connection");
            return -1;
        }

        memset(buffer, 0, sizeof(buffer));
        read_size = read(new_socket_fd, buffer, sizeof(buffer) - 1);
        if (read_size < 0)
        {
            fprintf(stderr, "Failed to read from socket");
            return -1;
        }

        printf("Received message from client: %s\n", buffer);
        close(new_socket_fd);
    }
    close(socket_fd);

    return 0;
}