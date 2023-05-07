
// Runner.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRunnerApp:
// See Runner.cpp for the implementation of this class
//

class CRunnerApp : public CWinApp
{
public:
	CRunnerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CRunnerApp theApp;
