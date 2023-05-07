
// RunnerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Runner.h"
#include "RunnerDlg.h"
#include "afxdialogex.h"
#include "mylib.h" 
#include <thread>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRunnerDlg dialog



CRunnerDlg::CRunnerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RUNNER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRunnerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRunnerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CRunnerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRunnerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void CRunnerDlg::ReadSettingsFromRegistry()
{
	int totalTrackingTime = ReadTotalTrackingTime();
	CString trackingTimeStr(std::to_string(totalTrackingTime).c_str());
	SetDlgItemText(TRACKING_TIME_INPUT, trackingTimeStr);

	int trackingResolution = ReadTrackingResolution();
	CString trackingResilutionStr(std::to_string(trackingResolution).c_str());
	SetDlgItemText(TRACKING_RESOLUTION_INPUT, trackingResilutionStr);
}

BOOL CRunnerDlg::SaveSettingsToRegistry()
{
	CString trackingTimeStr;
	GetDlgItemText(TRACKING_TIME_INPUT, trackingTimeStr);
	CString trackingResolutionStr;
	GetDlgItemText(TRACKING_RESOLUTION_INPUT, trackingResolutionStr);
 	int trackingTime = _ttoi(trackingTimeStr);
	int trackingResilution = _ttoi(trackingResolutionStr);

	if (trackingTime == 0 || trackingResilution == 0)
	{
		AfxMessageBox(_T("Wrong data format (must be integer)"), MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(APP_NAME, TRACK_TIME_REGISTRY_KEY, trackingTime);
	pApp->WriteProfileInt(APP_NAME, MEASUREMENT_RESOLUTION_REGISTRY_KEY, trackingResilution);

	return TRUE;
}

// CRunnerDlg message handlers
BOOL CRunnerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ReadSettingsFromRegistry();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRunnerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRunnerDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRunnerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRunnerDlg::SetPoints(std::vector<POINT> points) 
{
	this->points = points;
}

UINT CRunnerDlg::TrackMouse(LPVOID param)
{
	THREADSTRUCT* ts = (THREADSTRUCT*)param;

	CWnd* fieldOK = ts->dialog->GetDlgItem(IDOK);
	fieldOK->EnableWindow(FALSE);
	int trackingTime = ts->dialog->ReadTotalTrackingTime();

	int timeInSec = trackingTime / 1000;//convert to seconds
	CString timeStr(std::to_string(timeInSec).c_str());
	CString inProgressText(_T("Capturing mouse track for ") + timeStr + _T(" sec..."));
	ts->dialog->SetDlgItemTextW(STATUS_LABEL, inProgressText);

	set_measurement_resolution(ts->dialog->ReadTrackingResolution());
	set_track_time(trackingTime);
	ts->dialog->SetPoints(mouse_track());

	CString readyText("Ready. You can start capture again");
	ts->dialog->SetDlgItemTextW(STATUS_LABEL, readyText);
	fieldOK->EnableWindow(TRUE);

	return TRUE;
}

void CRunnerDlg::OnBnClickedOk()
{
	//saving current settings
	if (!SaveSettingsToRegistry()) 
	{
		return;
	}

	THREADSTRUCT* param = new THREADSTRUCT;
	param->dialog = this;
	//running in a separate thread in order not to block the UI
	AfxBeginThread(TrackMouse, param);
}


int CRunnerDlg::ReadTotalTrackingTime()
{
	CWinApp* pApp = AfxGetApp();
	return pApp->GetProfileInt(APP_NAME, TRACK_TIME_REGISTRY_KEY, DEFAULT_TOTAL_TRACKING_TIME_MS);
}

int CRunnerDlg::ReadTrackingResolution()
{
	CWinApp* pApp = AfxGetApp();
	return pApp->GetProfileInt(APP_NAME, MEASUREMENT_RESOLUTION_REGISTRY_KEY, DEFAULT_TRACKING_RESOLUTIONL_MS);
}


void CRunnerDlg::OnBnClickedCancel()
{
	SaveSettingsToRegistry();
	CDialogEx::OnCancel();
}
