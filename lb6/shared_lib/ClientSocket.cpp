#include "pch.h"
#include "ClientSocket.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

char* server = "127.0.0.1";

ClientSocket::result ClientSocket::SetupConnection()
{
	ClientSocket::result res;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	WSADATA wsaData;
	res.connectSocket = INVALID_SOCKET;

	// Initialize Winsock
	res.resultCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res.resultCode != 0)
	{
		res.message = "Failed to load Winsock library";
		return res;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	// Resolve the server address and port
	res.resultCode = getaddrinfo(server, DEFAULT_PORT, &hints, &result);
	if (res.resultCode != 0) {
		res.message = "getaddrinfo failed with error";
		WSACleanup();
		return res;
	}
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		res.connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (res.connectSocket == INVALID_SOCKET) {
			res.message = "socket failed with error";
			res.resultCode = WSAGetLastError();
			WSACleanup();
			return res;
		}
		// Connect to server.
		res.resultCode = connect(res.connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (res.resultCode == SOCKET_ERROR) {
			closesocket(res.connectSocket);
			res.connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (res.connectSocket == INVALID_SOCKET) {
		res.message = "Unable to connect to server";
		res.resultCode = -1;
		WSACleanup();
		return res;
	}
	return res;
}

void ClientSocket::CloseConnection(SOCKET connectSocket)
{
	closesocket(connectSocket);
	WSACleanup();
}

ClientSocket::result ClientSocket::DoDataTransfer(SOCKET connectSocket, std::vector<POINT> data)
{
	ClientSocket::result res;
	res.connectSocket = connectSocket;
	int resCode;
	// Send data
	for (int i = 0;i < data.size();i++)
	{
		std::string singlePoint = std::to_string(data[i].x) + "|" + std::to_string(data[i].y) + "#";
		const char* sendbuf = singlePoint.c_str();
		resCode = send(res.connectSocket, sendbuf, (int)strlen(sendbuf), 0);
		if (resCode == SOCKET_ERROR) {
			res.message = "send failed with error";
			res.resultCode = WSAGetLastError();
			closesocket(res.connectSocket);
			WSACleanup();
			return res;
		}
	}
	resCode = shutdown(res.connectSocket, SD_SEND);
	if (resCode == SOCKET_ERROR) {
		res.message = "shutdown failed with error";
		res.resultCode = WSAGetLastError();
		closesocket(res.connectSocket);
		WSACleanup();
		return res;
	}
	res.resultCode = 0;
	return res;
}