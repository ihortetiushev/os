
// ReceiverDlg.h : header file
//
#pragma once
#include <string>
#include <queue>
#include <mutex>
#include "Buffer.h"
#include "Buffer.cpp" // hack in order to avoid linkage error (Method 2) https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp

// CReceiverDlg dialog
class CReceiverDlg : public CDialogEx
{
	static UINT StartProducer(LPVOID param);
	static UINT StartConsumer(LPVOID param);

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
	std::mutex(mu); //Global variable or place within class
	std::condition_variable cv;
	bool dataPartIsReady = false;
	bool allDataIsReady = false;
	bool dataIsProcessed = false;
	Buffer<int> dataBuffer;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
};
