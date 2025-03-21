#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SIZE 1024

void send_file(FILE *fp, int sockfd) {
    char data[SIZE];
    size_t n;

    while ((n = fread(data, 1, SIZE, fp)) > 0) {
        if (send(sockfd, data, n, 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
    }
}

int main() {
    const char *ip = "127.0.0.1";
    const int port = 8080;
    
    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    const char *filename = "hello.txt";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);  
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Connected to Server.\n");

    fp = fopen(filename, "rb");  
    if (fp == NULL) {
        perror("[-]Error in reading file.");
        exit(1);
    }

    send_file(fp, sockfd);
    printf("[+]File data sent successfully.\n");

    printf("[+]Closing the connection.\n");
    close(sockfd);

    return 0;
}
