/*
* File Name: server.c
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
#include <ctype.h>

int shutdown_server = 0;


void handle_client(int client_sock, int sockfd, struct sockaddr_in client_addr) {
    char buffer[1024];
    socklen_t addr_size;
    int choice;

    while (1) { // Start of loop
        if (shutdown_server) {
        printf("Shutting down the server.\n");
        break; // Exit the loop
    }
        bzero(buffer, 1024);
        strcpy(buffer, "\n\nWELCOME TO VOWELIZER!\n\n1. Split text (basic encoding)\n2. Merge text (basic encoding)\n"
                       "3. Split text (advanced encoding)\n4. Merge text (advanced encoding)\n5. Quit\n");
        send(client_sock, buffer, strlen(buffer), 0);
        printf("Waiting for clients choice");
        recv(client_sock, buffer, sizeof(buffer), 0);
        choice = atoi(buffer);

        switch (choice) {
            case 1:
            bzero(buffer, 1024);
            recv(client_sock, buffer, sizeof(buffer), 0);  // Receive string from client

            char consonants[1024] = {0};
            char vowels[1024] = {0};

            // Logic to split buffer into consonants and vowels
            int j = 0, k = 0; // Declare j and k outside the loop
            for (int i = 0; buffer[i] != '\0'; i++) {
              if (strchr("aeiouAEIOU", buffer[i]) != NULL) {
                vowels[j++] = buffer[i];  // Add vowel to vowels array
                consonants[k++] = ' ';    // Add space in consonants array
                } else {
                  consonants[k++] = buffer[i]; // Add consonant/non-letter to consonants array
                  vowels[j++] = ' ';           // Add space in vowels array
                  }
            }
            vowels[j] = '\0';    // Null-terminate the vowels array
            consonants[k] = '\0'; // Null-terminate the consonants array

            // Send consonants via TCP
            send(client_sock, consonants, strlen(consonants), 0);

            // Wait for acknowledgment from client
            bzero(buffer, 1024);
            int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
            if (bytes_received > 0) {
              printf("Acknowledgment received from client.\n");
              bzero(buffer, 1024);
              addr_size = sizeof(client_addr);
              recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, &addr_size);
              printf("%s\n", buffer);
              
              bzero(buffer, 1024);
              strcpy(buffer, vowels);
              sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
              printf("Text sent to Client after Vowelizer: %s\n", vowels);
                    
              } else {
                  printf("[-]No acknowledgment received or error occurred.\n");
              }
            break;

            case 2:
            char buffer1[1024], buffer2[1024];

            // Clear the buffers
            bzero(buffer1, 1024);
            bzero(buffer2, 1024);

            // Receive the first message (consonants and spaces)
            recv(client_sock, buffer1, sizeof(buffer1), 0);  
            printf("Received First Message: %s\n", buffer1);

            // Receive the second message (vowels and spaces)
            addr_size = sizeof(client_addr);
            recvfrom(sockfd, buffer2, 1024, 0, (struct sockaddr*)&client_addr, &addr_size);
            printf("Received Second Message: %s\n", buffer2);

            // Combine the two messages
            for (int i = 0; buffer1[i] != '\0' && buffer2[i] != '\0'; i++) {
                if (buffer1[i] == ' ') {
                    buffer1[i] = buffer2[i];
                }
            }

            // Send the combined message back to the client
            send(client_sock, buffer1, strlen(buffer1), 0);
              break;

            case 3:
            bzero(buffer, 1024);
            recv(client_sock, buffer, sizeof(buffer), 0);  // Receive string from client
            char vowels2[1024] = {0}; // Arrays for vowels and others
            char others[1024] = {0};
            int non_vowel_count = 0; // Count of non-vowel characters

            for(int i = 0; buffer[i] != '\0'; i++) {
            char c = buffer[i];
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            // Append count and vowel to the vowels string
            int len = strlen(vowels2);
            snprintf(vowels2 + len, sizeof(vowels2) - len, "%d%c", non_vowel_count, c);
            non_vowel_count = 0; // Reset count
            } else {
            // Append other characters to the others string
            int len = strlen(others);
            others[len] = c;
            others[len + 1] = '\0';
            non_vowel_count++; // Increment count
              }
            }

            // Send consonants via TCP
            send(client_sock, others, strlen(others), 0);

            // Wait for acknowledgment from client
            bzero(buffer, 1024);
            int bytes_received2 = recv(client_sock, buffer, sizeof(buffer), 0);
            if (bytes_received2 > 0) {
            printf("[+]Acknowledgment received from client.\n");
            bzero(buffer, 1024);
            addr_size = sizeof(client_addr);
            recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, &addr_size);
            printf("%s\n", buffer);

            bzero(buffer, 1024);
            strcpy(buffer, vowels2);
            sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
            printf("Text sent to Client after Vowelizer: %s\n", vowels2);
                    
            } else {
            printf("[-]No acknowledgment received or error occurred.\n");
            }
            break;

            case 4:
            bzero(buffer1, 1024);
            bzero(buffer2, 1024);
            char result[1024];

            // Receive the first message (consonants and spaces)
            recv(client_sock, buffer1, sizeof(buffer1), 0);  
            printf("Received First Message: %s\n", buffer1);

            // Receive the second message (vowels and spaces)
            addr_size = sizeof(client_addr);
            recvfrom(sockfd, buffer2, 1024, 0, (struct sockaddr*)&client_addr, &addr_size);
            printf("Received Second Message: %s\n", buffer2);

            int w = 0, q = 0, s = 0, skip;

            while (buffer2[q] != '\0') {
            // Convert the number to an integer
            skip = buffer2[q++] - '0';
            
            // Copy the characters from buffer1
            while (skip-- > 0 && buffer1[w] != '\0') {
            result[s++] = buffer1[w++];
            }

            // Insert the vowel from buffer2
            if (isalpha(buffer2[q])) {
            result[s++] = buffer2[q++];
            }
            }

            // Copy any remaining characters from buffer1
            while (buffer1[w] != '\0') {
            result[s++] = buffer1[w++];
            }

            result[s] = '\0'; // Null-terminate the result string

            // Send the combined message back to the client
            send(client_sock, result, strlen(result), 0);
            break;

            case 5:
              printf("Shutdown signal received. Closing sockets and exiting.\n");
              close(client_sock); // Close the current client socket
              shutdown_server = 1; // Set the global flag
              break; // Exit the function

            default:
              strcpy(buffer, "Invalid choice. Please try again.\n");
              send(client_sock, buffer, strlen(buffer), 0);
              break;
        }
    }
}

int main(){

  char *ip = "127.0.0.1"; // Server IP
  int port = 8080; // Server port

  int sockfd;
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  char buffer[1024];
  socklen_t addr_size;
  int n;

  // Setting up UDP socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0){
    perror("[-]socket error");
    exit(1);
  }

  // Configure server address for UDP
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  // Bind the UDP socket
  n = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0) {
    perror("[-]bind error");
    exit(1);
  }

  // Initial communication over UDP
  bzero(buffer, 1024);
  addr_size = sizeof(client_addr);
  recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, &addr_size);
  printf("Server %s\n", buffer);


  // Setting up TCP socket
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0) {
      perror("[-]Socket error");
      exit(1);
  }
  printf("[+]TCP server socket created.\n");

  // Configure server address for TCP
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  // Bind the TCP socket
  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0) {
      perror("[-]Bind error");
      exit(1);
  }
  printf("[+]Bind to the port number: %d\n", port);

  // Listening for TCP clients
  listen(server_sock, 5);
  printf("Listening...\n");

  while (1) {
      // Accepting client connections
      addr_size = sizeof(client_addr);
      client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
      printf("[+]Client connected.\n");

      // Handling client requests
      handle_client(client_sock, sockfd, client_addr);

      // Closing the client socket
      close(client_sock);
      close(sockfd);
      break;
  }

  return 0;
}


