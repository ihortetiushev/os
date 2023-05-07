
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
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();

	void ReadSettingsFromRegistry();
	BOOL SaveSettingsToRegistry();
	void SetPoints(std::vector<POINT> points);
	int ReadTotalTrackingTime();
	int ReadTrackingResolution();
public:
	void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
