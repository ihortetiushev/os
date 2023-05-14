
// ReceiverDlg.h : header file
//
#pragma once
#include <string>
#include <queue>
#include <mutex>
#include "Buffer.h"
#include "Buffer.cpp" // hack in order to avoid linkage error (Method 2) https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp
#include "MouseTrack.h"


// CReceiverDlg dialog
class CReceiverDlg : public CDialogEx
{
	static UINT DrawMainTrack(LPVOID param);
	static UINT DrawBackground(LPVOID param);
	static UINT CaptureMouseTrack(LPVOID param);


	typedef struct THREADSTRUCT
	{
		CReceiverDlg* dialog;
	} THREADSTRUCT;

	// Construction
public:
	CReceiverDlg(CWnd* pParent = nullptr);	// standard constructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECEIVER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Implementation
protected:
	HICON m_hIcon;

	std::mutex(mu);
	std::condition_variable cv;
	bool dataPartIsReady = false;
	bool dataIsProcessed = false;
	Buffer<POINT> dataBuffer;
	MouseTrack mouseTrack;

	std::vector<POINT> mouseData;

	void sendDataToBackgroundWaitUntilProcessed(POINT point);
	void ReadSettingsFromRegistry();
	BOOL SaveSettingsToRegistry();
	int getEditControlIntValue(int controlId);

	// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
	afx_msg void OnBnClickedClear();
	afx_msg void OnClose();
};
