
// Created by 1lusca - Lucas Schneider
// schneider.lusca@gmail.com

// Implementação de uma rede p2p
// Trabalho do Grau A - Redes de Computadores I
// Universidade do Vale do Rio dos Sinos
// https://github.com/1lusca/p2p-network

// April, 2022

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

void sender();
void receiver(int server_fd);
void *receiver_thread(void *server_fd);

int main(int argc, char const *argv[]) {

    int server_fd, new_socket;
    struct sockaddr_in address;
    int k = 0;

    // Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(6000);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Thread para recever os pacotes
    pthread_t tid;
    pthread_create(&tid, NULL, &receiver_thread, &server_fd);

    printf("\n***** Waiting for packages *****\n");
    printf("\n***** Menu *****\n");
    printf("Press 1 - Send command\n");
    printf("Press -1 - Exit\n");

    int ch;
    do {
        printf("\nSelect option: ");
        scanf("%d", &ch);
        switch (ch) {
        case 1:
            sender();
            break;
        case -1:
            printf("\nExiting\n");
            break;
        default:
            printf("\nError, option doesnt exit\n");
        }
    } while (ch);

    close(server_fd);

    return 0;
}

// Sender
void sender() {

    // Get the ip's
    char ips[10][15] = {""};
    char receiver_ip[15];
    int add = 1;
    int i = 0;
    while ((add == 1) && (i <= 9)) {
        printf("Press 1 for adding IP, press 2 to send: ");
        scanf("%d", &add);
        switch (add) {
            case 1:
                printf("Receiver IP: ");
                scanf("%s", receiver_ip);
                strcpy(ips[i], receiver_ip);
                i++;
                break;
            case 2:
                break;
            default:
                break;
        }
    }

    // Get the command
    char buffer[2000] = {0};
    char hello[1024] = {0};
    char dummy;
    printf("Command: ");
    scanf("%c", &dummy);
    scanf("%[^\n]s", hello);
    sprintf(buffer, hello);

    // Sends the data to the ip's
    for (int j = 0; j < i; j++) {

        // Socket
        int sock = 0;
        struct sockaddr_in serv_addr;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            printf("\n Socket creation error \n");
            return;
        }

        // Adress
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ips[j]);
        serv_addr.sin_port = htons(6000);

        // Connection
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            return;
        }

        // Sends the data
        send(sock, buffer, sizeof(buffer), 0);
        printf("Command sent\n");
        close(sock);

    }

}

// Calling receiver every 2 seconds
void *receiver_thread(void *server_fd) {
    int s_fd = *((int *)server_fd);
    while (1) {
        sleep(2);
        receiver(s_fd);
    }
}

// Receives the data
void receiver(int server_fd) {

    struct sockaddr_in address;
    char buffer[2000] = {0};
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;

    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    int k = 0;
    while (1) {
        k++;
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &ready_sockets)) {

                if (i == server_fd) {
                    int client_socket;

                    if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(client_socket, &current_sockets);
                } else {

                    // Read data
                    recv(i, buffer, sizeof(buffer), 0);
                    printf("\nCommand: %s\n", buffer);

                    // Execute command
                    system(buffer);

                    FD_CLR(i, &current_sockets);
                }
            }
        }

        if (k == (FD_SETSIZE * 2))
            break;
    }
}
