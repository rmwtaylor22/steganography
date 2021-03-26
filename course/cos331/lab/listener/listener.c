#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>


#define MAX_CLIENT_BACKLOG 128
#define MAX_BUFFER_SIZE 4096
#define IS_MULTIPROCESS 0

void send_response(int accept_desc, char * request){
    char response_buffer[MAX_BUFFER_SIZE];
    char request_msg[MAX_BUFFER_SIZE];
    char file_buff[MAX_BUFFER_SIZE];
    memset(response_buffer, 0 , MAX_BUFFER_SIZE);
    unsigned long i;
    int bytes_sent;

    char command[6], file[100], version[15], info[200], path[1024];
    char * buf[1074];


    for (i=0; i<strlen(request); i++){
        request_msg[i] = request[i];
    }
    request_msg[i] = '\n';

    // divides up the incoming HTTP request into its command, file, and version
    strcpy(info, request_msg);
    sscanf(info, "%s %s %s", command, file, version);
    printf("COMMAND: %s  FILE: %s  VERSION: %s\n", command, file, version);

    // concatenates the whole file path to the current, shorter file path
    strcpy(path,"/home/CS/users/rwhite/.linux");
    strcat(path,file);

    // opens the file
    FILE *fp;
    fp = fopen(path, "r");
    if (fp == NULL) {
        // file does not exist
        printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
    }

    /*
    char c;
    int u = 0;
    do
    {
        /* Read single character from file
        c = fgetc(fp);

        /* Print character read on console
        file_buff[u] = (c);
        u++;

    } while(c != EOF);

    fclose(fp);
    */

    // HEADERS:
    // get length of file
    fseek(fp, 0, SEEK_END); // seek to end of file
    long int sz = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET); // seek back to beginning of file
    printf("the size: %ld \n", sz);

    // get time and date
    time_t the_time;
    time(&the_time);
    printf("Current time = %s", ctime(&the_time));

    // get the file type
    char *string,*found;
    string = strdup(path);
    int count = 0;


    while( (found = strsep(&string,".")) != NULL && count <2){
        count ++;
    }
    char *fileType;

    if(strcmp(found, "html") != 0 || strcmp(found, "txt") != 0){
        fileType = "text/html";
        printf("The file type: %s\n", fileType);
    } else {
        if(strcmp(found, "txt") != 0){
            fileType = "text/plain";
        } else {
            if(strcmp(found, "css") != 0){
                fileType = "text/css";
            } else {
                if(strcmp(found, "js") != 0){
                    fileType = "application/javascript";
                } else {
                    if(strcmp(found, "pdf") != 0){
                        fileType = "application/pdf";
                    } else {
                        if(strcmp(found, "gif") != 0){
                            fileType = "image/gif";
                        } else {
                            if(strcmp(found, "png") != 0){
                                fileType = "image/png";
                            } else {
                                if(strcmp(found, "jpeg") != 0){
                                    fileType = "image/jpeg";
                                } else {
                                    printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
                                    fileType = "text/html";
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    sprintf ((char *) buf, "HTTP/1.1 200 OK\r\nDate: %sContent-Type: %s\r\nContent-Length: %ld\r\n\n%s", ctime(&the_time), fileType, sz, "Hello World!");

    bytes_sent = send(accept_desc, buf, strlen((const char *) buf), 0);
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

            // do I want this?
            break;

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

    return_value = getaddrinfo(hostVal, portNum, &hints, &address_resource);
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

    // where handle connection is triggered
    while(1){

        accept_desc = accept(socket_desc, (struct sockaddr*) &remote_addr, &remote_addr_s);
        #if IS_MULTIPROCESS == 1
            // fork clones the process
            // everything in the process gets duplicated to the second process
            int pid = fork();
            if (pid == 0) {
                // child process starts ...
                handle_connection(accept_desc);
                close(accept_desc);
                close(socket_desc);
                exit(0);
                // ... child process terminated
            }

            if (pid == -1) {
                printf("Error: %s (line: %d)\n", strerror(errno), __LINE__);
                return pid;
            }
            // the parent process
            close(accept_desc);

        #else
            handle_connection(accept_desc);
            close(accept_desc);
            break;
        #endif
    }

    close(socket_desc);


    return 0;
}
