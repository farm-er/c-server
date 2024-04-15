#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "cJSON/cJSON.h"


#define PORT 1235
#define MAX_CONS 10
#define BUFFER_SIZE 1024


char *Encode() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", "john Doe");
    cJSON_AddBoolToObject(json, "alive", cJSON_True);
    cJSON_AddNumberToObject(json, "age", 25);

    char *js = cJSON_Print(json);
    cJSON_Delete(json);

    return js;
}




void HandleCon (int fileDescriptor) {

    char buffer[BUFFER_SIZE];

    int bytesReceived = recv(fileDescriptor, buffer, BUFFER_SIZE, 0);

    // check the request
    if ( bytesReceived < 0 ) {
        perror("recv");
        close(fileDescriptor);
        return;
    }else if ( bytesReceived == 0 ) {
        printf("user closed connecion");
        close(fileDescriptor);
        return;
    }

    char *header = "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/json\r\n"
                    "\r\n";

    printf("\nheader:\n%s\n", header);

    char *data = Encode();  

    printf("\ndata:\n%s\n", data);


    int size = strlen(data) + strlen(header) + 1;
    char *response = calloc(1, size);
    strcat(response, header);
    strcat(response, data);

    if ( send(fileDescriptor, response, size, 0) < 0 ) {
        perror("send");
    }
    
    printf("\nresponse:\n%s\n", response);

    printf("%d\n", fileDescriptor);
    free(response);

    close(fileDescriptor);
    
}


void SetSocket() {
    

    int fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

   
    printf("%d\n", fileDescriptor);
    if (fileDescriptor < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in addr;
    /*
    the general structure of socket addresses:
        struct sockaddr{
            __SOCKADDR_COMMON (sa_);	 Common data: address family and length.  
            char sa_data[14];		 Address data.  and it's format depends on the address family
        };
    for IPv4 addresses there's:
        struct sockaddr_in{
            __SOCKADDR_COMMON (sin_);
            in_port_t sin_port;			 Port number.  
            struct in_addr sin_addr;		 Internet address.  

            Pad to size of `struct sockaddr'.  
            unsigned char sin_zero[sizeof (struct sockaddr)
                    - __SOCKADDR_COMMON_SIZE
                    - sizeof (in_port_t)
                    - sizeof (struct in_addr)];
        };
    */

   // always AF_INET for IPv4
   addr.sin_family = AF_INET;
   addr.sin_port = htons(PORT); // converted to network byte order
   addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // converted to network byte order

    printf("Address family: %d\n", addr.sin_family);
    printf("Port number: %d\n", ntohs(addr.sin_port)); // Convert back to host byte order for printing
    printf("IPv4 address: %s\n", inet_ntoa(addr.sin_addr));

    if (bind(fileDescriptor, (struct sockaddr *)&addr, sizeof(addr)) != 0 ) {       
        close(fileDescriptor);
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if ( listen(fileDescriptor, MAX_CONS) != 0 ) {
        close(fileDescriptor);
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("new socket on: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    int newFD;
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        newFD = accept( fileDescriptor, (struct sockaddr *)&client_addr, &client_addr_len);
        if (newFD < 0) {
            perror("accept");
            close(newFD);
            exit(EXIT_FAILURE);
        }

        // handling connection
        printf("before handling connection\n");
        HandleCon(newFD);
        printf("finished while\n");
        close(newFD);
    }

    close(fileDescriptor);
}



int main() {

    SetSocket();

    return 0;
}