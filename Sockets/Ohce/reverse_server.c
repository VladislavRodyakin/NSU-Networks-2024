#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3100
#define SIZE 512

//reversing on the same address
void reverse(char* input_text, int len) 
{
    /*
    a=a^b;
    b=a^b;
    a=b^a;
    */
    if (len < 2 || input_text == NULL) {
        return;
    }
    char tmp;
    for (int i = 0; i<len/2; i++){
        tmp = input_text[i];
        input_text[i] = input_text[len-i-1];
        input_text[len-i-1] = tmp;
    }
    
}

int main() 
{
    int server_fd;
    int client_fd;
    struct sockaddr_in server_address;
    int addrlen = sizeof(server_address);
    char buffer[SIZE] = {0};
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) 
    {
        fprintf(stderr, "Server socket creation failed");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        fprintf(stderr, "Failed to bind");
        return -1;
    }

    if (listen(server_fd, 1) < 0){
        fprintf(stderr, "Listen failed");
        return -1;
    }

    client_fd = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t*)&addrlen);
    if (client_fd  < 0){
        fprintf(stderr, "Failed to accept connection");
        close(server_fd);
        return -1;
    }

    printf("Connection accepted\n");

    char* message = malloc(SIZE);
    if (message == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        close(client_fd);
        close(server_fd);
        return -1;
    }
    int msg_chunk = 0;
    int read_size, fin_size = read(client_fd, message + SIZE * msg_chunk, SIZE);
    while(read_size == SIZE){
        message = realloc(message, SIZE * (++msg_chunk));        
        read_size = read(client_fd, message+(SIZE*(msg_chunk-1)), SIZE);
        fin_size += read_size;
    }
 
    char *start, *end;
    for (start = message; start < message + fin_size; start = end + 1) {
        end = strchr(start, '\n');
        if (end == NULL)
            end = message + fin_size;
        reverse(start, end - start);
    }


    if (send(client_fd, message, fin_size, 0) < 0){
        fprintf(stderr, "Failed to send back");
        free(message);
        close(client_fd);
        close(server_fd);
        return -1;
    }

    free(message);
    close(client_fd);
    close(server_fd);
    return 0;
}