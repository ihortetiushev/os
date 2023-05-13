
// ReceiverDlg.h : header file
//
#include <string>
#pragma once


// CReceiverDlg dialog
class CReceiverDlg : public CDialogEx
{
	static UINT DoSomethingInSeparateThread(LPVOID param);
	static UINT DoSomethingElseInSeparateThread(LPVOID param);

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

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
};
