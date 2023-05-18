
// RunnerDlg.h : header file
//

#pragma once
#include <vector> 

const int DEFAULT_TOTAL_TRACKING_TIME_MS = 60000;
const int DEFAULT_TRACKING_RESOLUTIONL_MS = 20;
const LPCTSTR APP_NAME = L"MouseTracker";
const LPCTSTR TRACK_TIME_REGISTRY_KEY = L"TOTAL_TRACKING_TIME";
const LPCTSTR MEASUREMENT_RESOLUTION_REGISTRY_KEY = L"MEASUREMENT_RESOLUTION";

// CRunnerDlg dialog
class CRunnerDlg : public CDialogEx
{
	static UINT TrackMouse(LPVOID param);
	static LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);
// Construction

public:
	typedef struct THREADSTRUCT
	{
		CRunnerDlg* dialog;
		//you can add here other parameters you might be interested on
	} THREADSTRUCT;
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
	// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();

	void ReadSettingsFromRegistry();
	void SetGlobalKeyboardHook();
	BOOL SaveSettingsToRegistry();
	int ReadTotalTrackingTime();
	int ReadTrackingResolution();
	void StartCapture();
	void DoDataTransfer();
public:
	void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
