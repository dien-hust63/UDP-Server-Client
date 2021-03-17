// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "conio.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char * argv[]) {
	//Step 1: Initinate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Version is not support");
	}
	//Step 2: Contruct socket
	SOCKET server;
	server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//Step 3: Bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	if (bind(server, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
		printf("Error ! Can't bind this address");
		_getch();
		return 0;
	}
	printf("Server start !");
	//Step 4: Communicate with client 
	sockaddr_in clientAddr;
	char buff[BUFF_SIZE], clientIP[INET_ADDRSTRLEN];
	int ret, clientAddrLen = sizeof(clientAddr), clientPort;

	while (1) {
		//receive message
		ret = recvfrom(server, buff, BUFF_SIZE, 0, (sockaddr *)&clientAddr, &clientAddrLen);
		if (ret == SOCKET_ERROR) {
			printf("Error : %d", WSAGetLastError());
		}
		else {
			buff[ret] = 0;
			inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
			clientPort = ntohs(clientAddr.sin_port);
			printf("Receive from client %s:%d %s\n", clientIP, clientPort, buff);
			//echo to client
			ret = sendto(server, buff, sizeof(buff), 0, (sockaddr *)&clientAddr, sizeof(clientAddr));
			if (ret == SOCKET_ERROR) {
				printf("Error: %d", WSAGetLastError());
			}
		}
	}//end while
	//Step 5:Close socket
	closesocket(server);
	//Step 6 : Terminate Winsock
	WSACleanup();
	return 0;
}



