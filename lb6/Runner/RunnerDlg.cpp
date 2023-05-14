
// RunnerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Runner.h"
#include "RunnerDlg.h"
#include "afxdialogex.h"
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
	ON_MESSAGE(WM_HOTKEY, &CRunnerDlg::OnHotKey)
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

LRESULT CRunnerDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case CAPTURE_DATA_KEY:
		StartCapture();
		break;
	case TRANSFER_DATA_KEY:
		DoDataTransfer();
		break;
	}
	return 0;
}

void CRunnerDlg::RegisterHotKeys()
{
	RegisterHotKey(GetSafeHwnd(), TRANSFER_DATA_KEY, MOD_CONTROL, 'M');
	RegisterHotKey(GetSafeHwnd(), CAPTURE_DATA_KEY, MOD_ALT | MOD_CONTROL, 'M');
}

void CRunnerDlg::UnRegisterHotKeys()
{
	UnregisterHotKey(GetSafeHwnd(), TRANSFER_DATA_KEY);
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
	RegisterHotKeys();

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

	ts->dialog->mouseTracker.SetMeasurementResolution(ts->dialog->ReadTrackingResolution());
	ts->dialog->mouseTracker.SetTrackingTime(trackingTime);
	ts->dialog->points = ts->dialog->mouseTracker.DoMouseTrack();
	ts->dialog->captureInProgress = FALSE;

	CString readyText("Ready. Ctr + M to transfer data.");
	ts->dialog->SetDlgItemTextW(STATUS_LABEL, readyText);
	fieldOK->EnableWindow(TRUE);

	return TRUE;
}

static CString toCString(int value)
{
	CString str(std::to_string(value).c_str());
	return str;
}

static CString toCString(std::string value)
{
	CString str(value.c_str());
	return str;
}

void CRunnerDlg::DoDataTransfer()
{
	//for now only changing label on UI
	SetDlgItemText(STATUS_LABEL, _T("Transfering data..."));
	//implement actual data transfer

	SocketApi::result transferResult = socketApi.DoDataTransfer();
	if (transferResult.resultCode != 0)
	{
		AfxMessageBox(toCString(transferResult.message) + ". Error: " + toCString(transferResult.resultCode));
	}

	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	int res;
	WSADATA wsaData;
	SOCKET connectSocket = INVALID_SOCKET;

	char* server = "127.0.0.1";
	// Initialize Winsock
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0)
	{
		AfxMessageBox(_T("Failed to load Winsock library!. Error: ") + toCString(res));
		return;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	// Resolve the server address and port
	res = getaddrinfo(server, DEFAULT_PORT, &hints, &result);
	if (res != 0) {
		AfxMessageBox(_T("getaddrinfo failed with error: ") + toCString(res));
		WSACleanup();
		return;
	}
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET) {
			AfxMessageBox(_T("socket failed with error:") + toCString(WSAGetLastError()));
			WSACleanup();
			return;
		}
		// Connect to server.
		res = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (res == SOCKET_ERROR) {
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (connectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}

	// Send an initial buffer
	for (int i = 0;i < points.size();i++)
	{
		//std::string str(oss.str());
		std::string singlePoint = std::to_string(points[i].x) + "|" + std::to_string(points[i].y) + "#";
		//std::string singlePoint = std::to_string(10) + "|" + std::to_string(123456) + "#";
		const char* sendbuf = singlePoint.c_str();
		res = send(connectSocket, sendbuf, (int)strlen(sendbuf), 0);
		if (res == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket);
			WSACleanup();
			return;
		}
	}

	res = shutdown(connectSocket, SD_SEND);
	if (res == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return;
	}

	closesocket(connectSocket);
	WSACleanup();


	SetDlgItemTextW(STATUS_LABEL, _T("Transer is completed"));
	Sleep(3000);
	SetDlgItemTextW(STATUS_LABEL, _T("Waiting for user input"));
}

void CRunnerDlg::StartCapture()
{
	if (captureInProgress)
	{
		//already in progress
		//silently do nothing
		return;
	}
	//saving current settings
	if (!SaveSettingsToRegistry())
	{
		return;
	}
	captureInProgress = TRUE;
	THREADSTRUCT* param = new THREADSTRUCT;
	param->dialog = this;
	//running in a separate thread in order not to block the UI
	AfxBeginThread(TrackMouse, param);

}

void CRunnerDlg::OnBnClickedOk()
{
	StartCapture();
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
	UnRegisterHotKeys();
	CDialogEx::OnCancel();
}
