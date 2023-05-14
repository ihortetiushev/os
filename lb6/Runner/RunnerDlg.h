
// RunnerDlg.h : header file
//

#pragma once
#include <vector> 
#include <sstream>
#include "MouseTrack.h"
#include "SocketApi.h"
#include <winsock2.h>

#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>



#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

const int DEFAULT_TOTAL_TRACKING_TIME_MS = 60000;
const int DEFAULT_TRACKING_RESOLUTIONL_MS = 20;
const LPCTSTR APP_NAME = L"MouseTracker";
const LPCTSTR TRACK_TIME_REGISTRY_KEY = L"TOTAL_TRACKING_TIME";
const LPCTSTR MEASUREMENT_RESOLUTION_REGISTRY_KEY = L"MEASUREMENT_RESOLUTION";
const int TRANSFER_DATA_KEY = 100;
const int CAPTURE_DATA_KEY = 200;
// CRunnerDlg dialog
class CRunnerDlg : public CDialogEx
{
	static UINT TrackMouse(LPVOID param);
	typedef struct THREADSTRUCT
	{
		CRunnerDlg* dialog;
		//you can add here other parameters you might be interested on
	} THREADSTRUCT;
// Construction

public:
	CRunnerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RUNNER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	std::vector<POINT> points;
	BOOL captureInProgress = FALSE;
	MouseTrack mouseTracker;
	SocketApi socketApi;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();

	void ReadSettingsFromRegistry();
	void RegisterHotKeys();
	void UnRegisterHotKeys();
	BOOL SaveSettingsToRegistry();
	int ReadTotalTrackingTime();
	int ReadTrackingResolution();
	void StartCapture();
	void DoDataTransfer();
public:
	void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
