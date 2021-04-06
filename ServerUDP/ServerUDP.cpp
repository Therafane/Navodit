// ServerUDP.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main(int argc, char* argv[])
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	char broadcast = '1';
	int broadOk = setsockopt(out, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	if (broadOk != 0)
	{
		cout << "Error in setting Broadcast option";
		closesocket(out);
		return;
	}

	sockaddr_in receiver, sender;

	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(2620);
	receiver.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(out, (sockaddr*)&receiver, sizeof(receiver)) < 0)
	{
		cout << "Error in BINDING" << WSAGetLastError();
		closesocket(out);
		return;
	}

	struct addrinfo* result = NULL;

	SOCKET connections[100];

	char buf[1];
	int senderLength = sizeof(sender);
	ZeroMemory(&sender, senderLength);

	recvfrom(out, (char*)buf, 1, 0, (sockaddr*)&sender, &senderLength);
	if (buf[0] == 'r') {
		sendto(out, (char*)buf, 1, 0, (sockaddr*)&sender, senderLength);
	}
	else {
		sendto(out, (char*)buf, 1, 0, (sockaddr*)&sender, senderLength);
		char ip[256];
		inet_ntop(AF_INET, &sender.sin_addr, ip, 256);
		cout << "FROM: " << ip;

		SOCKET se = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (bind(se, (sockaddr*)&receiver, sizeof(receiver)) == SOCKET_ERROR)
		{
			cout << "error bind";
		}

		if (listen(se, SOMAXCONN) != 0) {
			cout << "Error listen";
		}

		SOCKET newCon = accept(se, NULL, NULL);
		char mes[20] = "hello pyatuh";
		send(newCon, mes, 20, NULL);
		recv(newCon, mes, 20, NULL);
		cout << mes;
		return;
	}

	char serverIp[256];
	inet_ntop(AF_INET, &sender.sin_addr, serverIp, 256);

	// Close the socket
	closesocket(out);

	// Close down Winsock
	WSACleanup();
}