
// Receiver.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include <sstream>

#include "resource.h"		// main symbols
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

// CReceiverApp:
// See Receiver.cpp for the implementation of this class
//

class CReceiverApp : public CWinApp
{
public:
	CReceiverApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	ULONG_PTR gdiplusToken;
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CReceiverApp theApp;
