
// RunnerDlg.h : header file
//

#pragma once
#include <vector> 

// CRunnerDlg dialog
class CRunnerDlg : public CDialogEx
{
	static UINT TrackMouse(LPVOID param);
	typedef struct THREADSTRUCT
	{
		CRunnerDlg* _this;
		//you can add here other parameters you might be interested on
	} THREADSTRUCT;
// Construction

public:
	CRunnerDlg(CWnd* pParent = nullptr);	// standard constructor
	void SetPoints(std::vector<POINT> points);

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
	DECLARE_MESSAGE_MAP()
public:
	void OnBnClickedOk();
};
