#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define MYHOST "localhost"
#define MYPORT "8080"


int main() {

    struct addrinfo hints;
    struct addrinfo * address_resource;
    int socket_desc;
    int return_value;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  //ip version 4
    hints.ai_socktype = SOCK_STREAM;  // We want a TCP connection
    hints.ai_flags = AI_PASSIVE;  // Listen on the socket

    return_value = getaddrinfo(MYHOST, MYPORT, &hints, &address_resource);
    if (return_value != 0){
        printf("Error: getaddrinfo failed");
        return 1;
    }

    socket_desc = socket(
            address_resource->ai_family,    // IPv4
            address_resource->ai_socktype,  // Streaming Protocol
            address_resource->ai_protocol   // TCP
            );

    return 0;
}
