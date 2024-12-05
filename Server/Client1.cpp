#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>  // For InetPton
#pragma comment(lib, "ws2_32.lib")

#define PORT 8888
#define BUFLEN 1024

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return -1;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed! Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    struct sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert the IP address to binary using inet_pton()
    const char* server_ip = "127.0.0.1";
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) != 1) {
        std::cerr << "Invalid address!" << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return -1;
    }

    const char* message = "Hello, Server!";
    std::cout << "Sending message: " << message << std::endl;

    int send_len = sendto(client_socket, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (send_len == SOCKET_ERROR) {
        std::cerr << "Send failed! Error: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return -1;
    }

    // Receive the response from the server
    char buffer[BUFLEN];
    int server_len = sizeof(server_addr);
    int recv_len = recvfrom(client_socket, buffer, BUFLEN, 0, (struct sockaddr*)&server_addr, &server_len);
    if (recv_len == SOCKET_ERROR) {
        std::cerr << "Receive failed! Error: " << WSAGetLastError() << std::endl;
    }
    else {
        buffer[recv_len] = '\0';  // Null-terminate the received string
        std::cout << "Received from server: " << buffer << std::endl;
    }

    closesocket(client_socket);
    WSACleanup();
    int a;
    std::cin >> a;
    return 0;
}
