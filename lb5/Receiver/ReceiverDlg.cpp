
// ReceiverDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Receiver.h"
#include "ReceiverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReceiverDlg dialog



CReceiverDlg::CReceiverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RECEIVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReceiverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CReceiverDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BTN1, &CReceiverDlg::OnBnClickedBtn1)
	ON_BN_CLICKED(ID_BTN2, &CReceiverDlg::OnBnClickedBtn2)
END_MESSAGE_MAP()


// CReceiverDlg message handlers

BOOL CReceiverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CRect rcDesktop;
	rcDesktop.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	rcDesktop.right = rcDesktop.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	rcDesktop.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	rcDesktop.bottom = rcDesktop.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
	MoveWindow(rcDesktop, FALSE);
	//RECT rect;
	//GetWindowRect(&rect);

	//int width;
	//int height;

	// Code for calculating width and height

//	SetWindowPos(&wndTop, rect.left, rect.top, width, height, SWP_SHOWWINDOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CReceiverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
	
		//CPaintDC* pDC = new CPaintDC(this);
		//pDC->Rectangle(1, 1, 100, 100);
		/*CRect client;
		GetClientRect(&client);
		CDC* dc = GetDC();
		//dc->LineTo(100, 100);
		Graphics graphics(dc->m_hDC);
		Pen redPen(Color(255,0,0), 2.0);
		// Initialize the coordinates of the points that define the line.
		INT x1 = 0;
		INT y1 = 0;
		INT x2 = 100;
		INT y2 = 100;

		// Draw the line.
		graphics.DrawLine(&redPen, 10, 10, 100, 100);*/
	
		CDialogEx::OnPaint();
		//ReleaseDC(dc);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CReceiverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CReceiverDlg::OnBnClickedBtn1()
{
	THREADSTRUCT* param = new THREADSTRUCT;
	param->dialog = this;
	//running in a separate thread in order not to block the UI
	AfxBeginThread(DoSomethingInSeparateThread, param);

}

UINT CReceiverDlg::DoSomethingElseInSeparateThread(LPVOID param)
{
	THREADSTRUCT* ts = (THREADSTRUCT*)param;
	CRect client;
	ts->dialog->GetClientRect(&client);
	CWnd* btn = ts->dialog->GetDlgItem(ID_BTN2);
	btn->EnableWindow(FALSE);
	CDC* dc = ts->dialog->GetDC();
	Graphics graphics(dc->m_hDC);
	Pen redPen(Color(255, 0, 0), 5.0);

	Rect rect(400,25,5,5);
	REAL startAngle = 45.0F;
	REAL sweepAngle = 360;
	// Draw arc to screen.
	graphics.DrawArc(&redPen, rect, startAngle, sweepAngle);
	ts->dialog->SetDlgItemTextW(LABEL2, _T("100"));
	for (int i = 10; i > 0; i--)
	{
		CString timeStr(std::to_string(i).c_str());
		ts->dialog->SetDlgItemTextW(LABEL2, timeStr);
		Sleep(1000);
	}
	ts->dialog->SetDlgItemTextW(LABEL2, _T(""));

	Pen greenPen(Color(0, 255, 0), 5.0);
	graphics.DrawArc(&greenPen, rect, startAngle, sweepAngle);
	ts->dialog->ReleaseDC(dc);
	btn->EnableWindow(TRUE);

	return TRUE;
}

UINT CReceiverDlg::DoSomethingInSeparateThread(LPVOID param)
{
	THREADSTRUCT* ts = (THREADSTRUCT*)param;
	CRect client;
	ts->dialog->GetClientRect(&client);
	CWnd* btn = ts->dialog->GetDlgItem(ID_BTN1);
	btn->EnableWindow(FALSE);
	CDC* dc = ts->dialog->GetDC();
	Graphics graphics(dc->m_hDC);
	Pen redPen(Color(255, 0, 0), 5.0);

	Rect rect(180, 25, 5, 5);
	REAL startAngle = 45.0F;
	REAL sweepAngle = 360;
	// Draw arc to screen.
	graphics.DrawArc(&redPen, rect, startAngle, sweepAngle);

	ts->dialog->SetDlgItemTextW(LABEL1, _T("100"));
	for (int i = 10; i > 0; i--) 
	{
		CString timeStr(std::to_string(i).c_str());
		ts->dialog->SetDlgItemTextW(LABEL1, timeStr);
		Sleep(1000);
	}
	ts->dialog->SetDlgItemTextW(LABEL1, _T(""));

	Pen greenPen(Color(0, 255, 0), 5.0);
	graphics.DrawArc(&greenPen, rect, startAngle, sweepAngle);
	ts->dialog->ReleaseDC(dc);
	btn->EnableWindow(TRUE);
	return TRUE;
}

void CReceiverDlg::OnBnClickedBtn2()
{
	THREADSTRUCT* param = new THREADSTRUCT;
	param->dialog = this;
	//running in a separate thread in order not to block the UI
	AfxBeginThread(DoSomethingElseInSeparateThread, param);
}
