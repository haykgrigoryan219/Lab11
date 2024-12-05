#include "UDPServer.h"
#include <iostream>
#include <cstring>

#define PORT2 8888  // Port2 that client2 will bind to (can be different from the server's PORT)

int main() {
    // Initialize the UDP socket for client2
    UDPServer client2_sock(PORT2);  // Bind to Port2

    // Create a sockaddr_in structure for client1's address
    struct sockaddr_in si_client1;
    char buf[BUFLEN];
    int slen = sizeof(si_client1);

    // Print message indicating client2 has started
    std::cout << "Client2 started on port " << PORT2 << std::endl;
    std::cout << "Waiting for messages from Client1..." << std::endl;

    // Loop to listen for messages from client1
    while (true) {
        // Receive a datagram (message) from client1
        int recv_len = client2_sock.RecvDatagram(buf, BUFLEN, (struct sockaddr*)&si_client1, &slen);

        // Check if the message was received successfully
        if (recv_len == SOCKET_ERROR) {
            std::cerr << "recvfrom() failed with error code " << WSAGetLastError() << std::endl;
            continue;
        }

        // If message length is 0, print a debug message
        if (recv_len == 0) {
            std::cerr << "Received message is empty (recv_len = 0)" << std::endl;
        }

        // Null-terminate the received message so we can print it
        buf[recv_len] = '\0';

        // Check the content of the buffer
        std::cout << "Received message from Client1: \"" << buf << "\"" << std::endl;

        // Send a response back to client1
        const char* response = "Message received by Client2!";
        int send_len = client2_sock.SendDatagram((char*)response, (unsigned int)strlen(response), (struct sockaddr*)&si_client1, slen);
        if (send_len == SOCKET_ERROR) {
            std::cerr << "Send failed with error code " << WSAGetLastError() << std::endl;
        }
        else {
            std::cout << "Response sent to Client1: " << response << std::endl;
        }
    }

    return 0;
}
