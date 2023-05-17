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

class ServerSocket
{

public:
	struct result
	{
		int resultCode;
		std::string message;
	};
	struct readResult
	{
		int resultCode;
		std::string message;
		std::vector<POINT> data;
	};
	__declspec(dllexport) result SetupConnection();
	__declspec(dllexport) readResult ReceiveData();
	__declspec(dllexport) void CloseConnection();
};