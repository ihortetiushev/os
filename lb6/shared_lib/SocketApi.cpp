#include "pch.h"
#include "SocketApi.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

SocketApi::result SocketApi::DoDataTransfer()
{
	SocketApi::result res;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	WSADATA wsaData;
	SOCKET connectSocket = INVALID_SOCKET;

	char* server = "127.0.0.1";
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
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET) {
			res.message = "socket failed with error";
			res.resultCode = WSAGetLastError();
			WSACleanup();
			return res;
		}
		// Connect to server.
		res.resultCode = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (res.resultCode == SOCKET_ERROR) {
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (connectSocket == INVALID_SOCKET) {
		res.message = "Unable to connect to server";
		res.resultCode = -1;
		WSACleanup();
		return res;
	}

	WSACleanup();
	res.resultCode = 0;
	return res;
}