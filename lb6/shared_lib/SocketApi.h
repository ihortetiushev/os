#pragma once

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class SocketApi
{

public:
	__declspec(dllexport) struct result
	{
		int resultCode;
		std::string message;
	};
	__declspec(dllexport) result DoDataTransfer();


};