#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define MYHOST "localhost"
#define MYPORT "8080"
#define MAX_CLIENT_BACKLOG 128
#define MAX_BUFFER_SIZE 4096

void send_response(int accept_desc, char * request){
    char response_buffer[MAX_BUFFER_SIZE];
    memset(response_buffer, 0 , MAX_BUFFER_SIZE);
    unsigned long i;
    int bytes_sent;

    for (i=0; i<strlen(request); i++){
        response_buffer[i] = (char) toupper(request[i]);
    }
    response_buffer[i] = '\n';

    bytes_sent = send(accept_desc, response_buffer, strlen(response_buffer), 0);
    if (bytes_sent == -1){
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
    }
}

void handle_connection(int accept_desc) {
    char c;
    int bytes_read;
    int cursor = 0;
    char request_buffer[MAX_BUFFER_SIZE];

    memset(request_buffer, 0, MAX_BUFFER_SIZE);

    while(1){
        bytes_read = recv(accept_desc, &c, 1, 0);
        if (bytes_read <= 0){
            if (bytes_read == -1){
                printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
            }
            break;
        }

        if (c == '\n') {
            // send

            send_response(accept_desc, request_buffer);

            if (strlen(request_buffer) == 0){
                return;
            }

            cursor = 0;
            memset(request_buffer, 0, MAX_BUFFER_SIZE);

        } else {
            if (cursor < MAX_BUFFER_SIZE){
                request_buffer[cursor] = c;
                cursor++;
            }
        }
    }
}

int main(argc, argv)
    int     argc;
    char    *argv[];{

    int socket_desc, accept_desc;
    int return_value;
    int enable = 1;

    // storing command line arguments
    const char * hostVal = argv[1];
    const char * portNum = argv[2];
    printf("DOCROOT: %s\n", hostVal);
    printf("PORT NUM: %s\n", portNum);

    struct addrinfo hints;
    struct addrinfo * address_resource;
    struct sockaddr_storage remote_addr;
    // remote address size
    socklen_t remote_addr_s = sizeof(remote_addr);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  //ip version 4
    hints.ai_socktype = SOCK_STREAM;  // We want a TCP connection
                        // SOCK_STREAM: ordered packets (e.g. TCP)
                        // SOCK_DGRAM: possibly unordered packets (e.g. UDP)
    hints.ai_flags = AI_PASSIVE;  // Listen on the socket


    return_value = getaddrinfo(MYHOST, MYPORT, &hints, &address_resource);
    //return_value = getaddrinfo(hostVal, portNum, &hints, &address_resource);
    if (return_value != 0){
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    // socket(): creates a new TCP/IP socket, no address or port is assigned yet
    socket_desc = socket(
            address_resource->ai_family,    // IPv4
            address_resource->ai_socktype,  // Streaming Protocol
            address_resource->ai_protocol   // TCP
            );

    if (socket_desc == -1) {
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return socket_desc;
    }

    // makes the address of the socket reuseable
    return_value = setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (return_value == -1){
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    // bind(): assign address and port to socket
    return_value = bind(socket_desc, address_resource->ai_addr, address_resource->ai_addrlen);
    if (return_value == -1){
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    freeaddrinfo(address_resource);

    return_value = listen(socket_desc, MAX_CLIENT_BACKLOG);
    if (return_value == -1){
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
        return return_value;
    }

    // code that is triggered when data comes in on the socket
    accept_desc = accept(socket_desc, (struct sockaddr*) &remote_addr, &remote_addr_s);
    handle_connection(accept_desc);
    close(accept_desc);
    close(socket_desc);


    return 0;
}
