#pragma once

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma comment (lib, "msvcprt.lib")


class ClientSocket
{

public:
	struct result
	{
		int resultCode;
		std::string message;
		SOCKET connectSocket;
	};
	__declspec(dllexport) result SetupConnection();
	__declspec(dllexport) void CloseConnection(SOCKET connectSocket);
	__declspec(dllexport) result DoDataTransfer(SOCKET connectSocket, std::vector<POINT> data);

};