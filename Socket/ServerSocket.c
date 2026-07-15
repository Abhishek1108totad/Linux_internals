#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Thread function dedicated strictly to receiving incoming messages
void *receive_messages(void *socket_fd) {
    int client_socket = *(int *)socket_fd;
    char buffer[BUFFER_SIZE];
    int read_bytes;

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        // UNDER THE HOOD: This blocks inside the kernel until Laptop B sends data Transport Layer
        read_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);
        
        if (read_bytes <= 0) {
            printf("\n[SYSTEM] Client disconnected or connection lost.\n");
            close(client_socket);
            exit(0);
        }
        
        // Print the received message to the screen
        printf("\n[REMOTE]: %s", buffer);
        printf("[YOU]: "); // Keep the input prompt clean
        fflush(stdout);
    }
    return NULL;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char message[BUFFER_SIZE];
    pthread_t recv_thread;

    // 1. Create TCP Socket it is Network Layer
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind socket to all available interfaces on Port 8080 Network Layer
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listens on all network cards
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Start listening for incoming connections Transport Layer
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("[SYSTEM] Server active on port %d. Awaiting connection from Laptop B...\n", PORT);

    // 4. Accept connection from Client (Laptop B)
    if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("[SYSTEM] Connection established with Laptop B!\n");
    printf("Type your message and press Enter. Start chatting!\n\n");

    // 5. Spawn the background thread to handle receiving data
    pthread_create(&recv_thread, NULL, receive_messages, (void *)&client_socket);

    // 6. Main loop handles sending data from the keyboard
    while (1) {
        printf("[YOU]: ");
        fgets(message, BUFFER_SIZE, stdin);
        
        // Send the message across the router via the socket Transport Layer
        send(client_socket, message, strlen(message), 0);
    }

    close(server_fd);
    return 0;
}