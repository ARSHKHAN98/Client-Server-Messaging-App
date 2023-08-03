#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[1024] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        return 1;
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Failed to bind the socket.\n");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Error in listening.\n");
        return 1;
    }

    printf("Server is listening for incoming connections...\n");

    // Accept an incoming connection
    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) == INVALID_SOCKET) {
        printf("Failed to accept the connection.\n");
        return 1;
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    while (1) {
        // Receive data from client
        int received_bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (received_bytes == SOCKET_ERROR || received_bytes == 0) {
            printf("Client disconnected.\n");
            break;
        }

        // Echo back the received data
        send(client_socket, buffer, received_bytes, 0);

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
    }

    // Close the sockets and cleanup
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
