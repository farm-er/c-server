#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>



typedef struct request {

}request;

typedef struct response {

}response;


typedef int HandleFunc (request r ,response *w);



void ListenAndServer (int port, int maxcons, int buffersize) {

    int fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;

    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (bind(fileDescriptor, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        close(fileDescriptor);
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if ( listen(fileDescriptor, maxcons) != 0 ) {
        close(fileDescriptor);
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int newFD = accept( fileDescriptor, (struct sockaddr *)&client_addr, &client_addr_len);
        if (newFD < 0) {
            perror("accept");
            close(newFD);
            exit(EXIT_FAILURE);
        }

        /* receive request */

        char buffer[buffersize];

        int bytesReceived = recv(newFD, buffer, buffersize, 0);

        if ( bytesReceived < 0 ) {
            perror("recv");
            close(newFD);
            return;
        }
        if ( bytesReceived == 0 ) {
            printf("user closed connecion");
            close(newFD);
            return;
        }

        printf("\n%s\n", buffer);

        char *bodyStart = strstr(buffer, "\r\n\r\n");

        printf("\n%s\n", bodyStart);



        char *header = strtok(buffer, "\r\n");

        while (header != NULL) {
            printf("%s\n", header);
            header = strtok(NULL, "\r\n\r\n"); 
        }




        /* parse request and extract: METHOD, ROUTE, HEADERS, BODY */
        /* pass the request data and a response pointer to a function based on the route */
        /* let the user build the response based on the function */
        /* take the response and send it back */

        close(newFD);
    }

    close(fileDescriptor);
}




int main() {

    ListenAndServer(1235, 10, 1024);


    return 0;
}