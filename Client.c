#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char message[1024] = {0};
    char buffer[1024] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        return 1;
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed.\n");
        return 1;
    }

    printf("Connected to server.\n");

    while (1) {
        // Get input from user
        printf("Enter message (type 'exit' to quit): ");
        fgets(message, sizeof(message), stdin);

        // Send the message to the server
        send(client_socket, message, strlen(message), 0);

        // If the user types 'exit', break the loop and quit
        if (strcmp(message, "exit\n") == 0)
            break;

        // Receive response from the server
        int received_bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (received_bytes == SOCKET_ERROR || received_bytes == 0) {
            printf("Server disconnected.\n");
            break;
        }

        // Display the received message from the server
        printf("Server response: %s\n", buffer);

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
    }

    // Close the socket and cleanup
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
