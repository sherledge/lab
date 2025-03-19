#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int serversocket, clientsocket, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t clientlen;
    char message[50];

    // Create a socket
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Set up the server address structure
    bzero((char *)&serveraddr, sizeof(serveraddr));
    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address
    serveraddr.sin_port = htons(port);

    // Bind the server socket to the specified port
    if (bind(serversocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Error on binding");
        exit(1);
    }

    // Start listening for incoming connections
    listen(serversocket, 5);
    clientlen = sizeof(clientaddr);
    
    // Accept a connection from a client
    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddr, &clientlen);
    if (clientsocket < 0) {
        perror("Error on accept");
        exit(1);
    }

    printf("Connection established with client\n");

    // Receive the message from the client
    bzero(message, 50);
    int n = recv(clientsocket, message, sizeof(message), 0);
    if (n < 0) {
        perror("Error in receiving message");
    } else {
        printf("Message received from client: %s\n", message);
    }

    // Send a response back to the client
    const char *response = "YOUR MESSAGE RECEIVED.";
    send(clientsocket, response, strlen(response), 0);

    // Close the client and server sockets
    close(clientsocket);
    close(serversocket);

    return 0;
}

