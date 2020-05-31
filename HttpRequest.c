#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <share.h>


#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[]) {
   
    
    if (argc != 5) {
        printf( "\nUsage: HttpRequest <site> <port> <path> <method>\n");
        printf("Example: HttpRequest www.google.com 80 /doodles GET\n");
        printf("Method: GET POST HEAD PUT OPTIONS CONNECT TRACE DELETE\n");
        printf("Port: 80 - 443\n");
        printf("Path: /something.php OR --> / <-- for index");
        
        return -6;
        }
        
    
    
    WSADATA wsa;
    struct addrinfo hints;
    struct addrinfo* peer_address;
    hints.ai_socktype = SOCK_STREAM;
    memset(&hints, 0, sizeof(hints));
    if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0) {
        printf("Failed to initializa Winsock...\n");
        return -1;
    }
    printf("Initializing Winsock...\n");
    printf("Remote address info:\n");

    if (getaddrinfo(argv[1], argv[2], &hints, &peer_address) != 0) {
        printf("Failed to retrive server information");
        return -2;
    }
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
        address_buffer, sizeof(address_buffer),
        service_buffer, sizeof(service_buffer), NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);

    printf("Creating socket...\n");

    SOCKET socket_peer;

    socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
    if (socket_peer == INVALID_SOCKET) {
        printf("Failed to retrive the socket descriptor N. %d\n", WSAGetLastError());
        return -3;
    }
    printf("Connecting...\n");

    if (connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen) == SOCKET_ERROR) {
        printf("Failed to connect to %s Error N. %d", argv[1], WSAGetLastError());
        return -4;
    }
    freeaddrinfo(peer_address);
    printf("Connected.\n");
   
    
    char buffer[300];
    
    sprintf(buffer, "%s %s HTTP/1.1\nHost: %s\nConnection : Upgrade, HTTP2 - Settings:\nUpgrade : h2c\nHTTP2 - Settings :\nUser - Agent : whatever\n\n",argv[4],argv[3],argv[1]);
    printf("Your request: %s \n", buffer);
    int sending;
    sending = send(socket_peer,buffer, (int)strlen(buffer), 0);
    
    if (sending == SOCKET_ERROR) {
        printf("Failed to send data...\n");
        closesocket(socket_peer);
        WSACleanup();
        return -5;
    }


    
    int recived = 0;
    int recvbuflen = 9000;
    char recvBuffer[9000];
    
        recived = recv(socket_peer, recvBuffer, recvbuflen, 0);
        printf("Response: \n");
    printf("byte received (%d) %.*s \n",recived,recived,recvBuffer);
    WSACleanup();
    closesocket(socket_peer);


    return 0;
}