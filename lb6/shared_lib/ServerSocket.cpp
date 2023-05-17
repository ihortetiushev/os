#include "pch.h"
#include "ServerSocket.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

SOCKET serverSocket;
SOCKET listenSocket;

ServerSocket::readResult ServerSocket::ReceiveData()
{
	ServerSocket::readResult res;
	int iResult;
	// Accept a client socket
	serverSocket = accept(listenSocket, NULL, NULL);
	if (serverSocket == INVALID_SOCKET) {
		res.message = "accept failed with error";
	    res.resultCode = WSAGetLastError();
		closesocket(listenSocket);
		WSACleanup();
		return res;
	}

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	std::vector<POINT> received;
	do {

		iResult = recv(serverSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			std::string singlePoint(recvbuf);
			int idxX = singlePoint.find_first_of("|");
			int idxY = singlePoint.find_first_of("#");
			std::string xStr = singlePoint.substr(0, idxX);
			std::string yStr = singlePoint.substr(idxX + 1, idxY - idxX - 1);
			POINT current;
			current.x = std::stoi(xStr);
			current.y = std::stoi(yStr);
			received.push_back(current);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			res.message = "recv failed with error";
			res.resultCode = WSAGetLastError();
			closesocket(serverSocket);
			WSACleanup();
			return res;
		}

	} while (iResult > 0);

	if (received.size() > 0)
	{
		res.resultCode = 0;
		res.message = "";
		res.data = received;
	}
	return res;
}

ServerSocket::result ServerSocket::SetupConnection()
{
	ServerSocket::result res;

	WSADATA wsaData;
	int iResult;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		res.message = "WSAStartup failed with error";
		res.resultCode = iResult;
		return res;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		res.message = "getaddrinfo failed with error";
		res.resultCode = iResult;
		return res;
	}

	// Create a SOCKET for the server to listen for client connections.
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		res.message = "socket failed with error";
		res.resultCode = WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return res;
	}

	// Setup the TCP listening socket
	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		res.message = "bind failed with error";
		res.resultCode = WSAGetLastError();
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return res;
	}

	freeaddrinfo(result);

	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		res.message = "listen failed with error";
		res.resultCode = WSAGetLastError();
		closesocket(listenSocket);
		WSACleanup();
		return res;
	}
	res.resultCode = 0;
	return res;
}

void ServerSocket::CloseConnection()
{
	int iResult;
	closesocket(listenSocket);

	// shutdown the connection since we're done
	iResult = shutdown(serverSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		return;
	}

	// cleanup
	closesocket(serverSocket);
	WSACleanup();
}
