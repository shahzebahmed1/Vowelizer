/*
* File Name: client.c
* Completed by: Shahzeb Ahmed
* Submission Date: November 15, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

int main(){

  char *ip = "127.0.0.1";
  int port = 8080;

  int sockfd;
  int tcp_sock, n;
  struct sockaddr_in addr,tcp_addr;
  char buffer[1024];
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  bzero(buffer, 1024);
  strcpy(buffer, "Client UDP Sucess");
  sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, sizeof(addr));



    // TCP socket setup
    tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0) {
        perror("\nTCP Socket error\n");
        exit(1);
    }

    memset(&tcp_addr, '\0', sizeof(tcp_addr));
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_port = htons(port);
    tcp_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(tcp_sock, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr)) < 0) {
        perror("[-]TCP Connect error");
        exit(1);
    }


while (1) {
    // Receive and print the menu from the server
    bzero(buffer, 1024);
    recv(tcp_sock, buffer, sizeof(buffer), 0); 
    printf("%s\n", buffer);

    printf("Enter choice: ");
    fgets(buffer, 1024, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
    send(tcp_sock, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "5") == 0) {
            printf("Exiting.\n");
            break;
        }

        if (strcmp(buffer, "1") == 0) {
            printf("\nEnter text to split: ");
            fgets(buffer, 1024, stdin);
            buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
            send(tcp_sock, buffer, strlen(buffer), 0);

            // Receive and print consonants
            bzero(buffer, 1024);
            recv(tcp_sock, buffer, sizeof(buffer), 0);
            printf("Consonants: %s\n", buffer);

            // Send acknowledgment to server
            send(tcp_sock, "ACK", 3, 0);
            bzero(buffer, 1024);
            strcpy(buffer, "UDP Received from Client");
            sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, sizeof(addr));

            // Set up timeout
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);

            struct timeval tv;
            tv.tv_sec = 5;  // Timeout of 5 seconds
            tv.tv_usec = 0;

            int activity = select(sockfd + 1, &readfds, NULL, NULL, &tv);

            if (activity <= 0) {
                printf("Timed Out...Exiting Gracefully");
                break;
            }

            // Now perform recvfrom as activity is detected
            bzero(buffer, 1024);
            addr_size = sizeof(addr);
            recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size);
            printf("Vowels: %s\n", buffer);
        }

        if (strcmp(buffer, "2") == 0) { 
            char consonants[1024];
            char vowels[1024];

            // Prompt for consonants string
            printf("Enter consonants: ");
            fgets(consonants, 1024, stdin);
            consonants[strcspn(consonants, "\n")] = 0; // Remove newline character

            // Send consonants string over TCP
            send(tcp_sock, consonants, strlen(consonants), 0);

            // Prompt for vowels string
            printf("Enter vowels: ");
            fgets(vowels, 1024, stdin);
            vowels[strcspn(vowels, "\n")] = 0; // Remove newline character

            // Send vowels string over UDP
            sendto(sockfd, vowels, 1024, 0, (struct sockaddr*)&addr, sizeof(addr));

            // Receive and print consonants
            bzero(buffer, 1024);
            recv(tcp_sock, buffer, sizeof(buffer), 0);
            printf("Merged: %s\n", buffer);
        }
        if (strcmp(buffer, "3") == 0) {
            printf("Enter text to split: ");
            fgets(buffer, 1024, stdin);
            buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
            send(tcp_sock, buffer, strlen(buffer), 0);

            // Receive and print consonants
            bzero(buffer, 1024);
            recv(tcp_sock, buffer, sizeof(buffer), 0);
            printf("Consonants: %s\n", buffer);

            // Send acknowledgment to server
            send(tcp_sock, "ACK", 3, 0);
            bzero(buffer, 1024);
            strcpy(buffer, "UDP Received from Client");
            sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, sizeof(addr));

            // Set up timeout
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);

            struct timeval tv;
            tv.tv_sec = 5;  // Timeout of 5 seconds
            tv.tv_usec = 0;

            int activity = select(sockfd + 1, &readfds, NULL, NULL, &tv);

            if (activity <= 0) {
                printf("Timed Out...Exiting Gracefully");
                break;
            }

            // Now perform recvfrom as activity is detected
            bzero(buffer, 1024);
            addr_size = sizeof(addr);
            recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size);
            printf("Vowels: %s\n", buffer);
        }

        if (strcmp(buffer, "4") == 0) { 
            char consonants[1024];
            char vowels[1024];

            // Prompt for consonants string
            printf("Enter consonants: ");
            fgets(consonants, 1024, stdin);
            consonants[strcspn(consonants, "\n")] = 0; // Remove newline character

            // Send consonants string over TCP
            send(tcp_sock, consonants, strlen(consonants), 0);

            // Prompt for vowels string
            printf("Enter vowels: ");
            fgets(vowels, 1024, stdin);
            vowels[strcspn(vowels, "\n")] = 0; // Remove newline character

            // Send vowels string over UDP
            sendto(sockfd, vowels, 1024, 0, (struct sockaddr*)&addr, sizeof(addr));
            printf("Vowels sent: %s\n", vowels);

            // Receive and print consonants
            bzero(buffer, 1024);
            recv(tcp_sock, buffer, sizeof(buffer), 0);
            printf("Merged: %s\n", buffer);
        }
    }
  close(tcp_sock);
  close(sockfd);
  printf("Program Closed");
  return 0;
}
