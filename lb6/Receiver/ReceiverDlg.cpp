
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
const int TOP_OFFSET = 150;
const float SCALE = 0.5;
const CString APP_NAME = _T("Receiver");
const CString RED_COLOR_SETTING_KEY = _T("RED_COLOR");
const CString GREEN_COLOR_SETTING_KEY = _T("GREEN_COLOR");
const CString BLUE_COLOR_SETTING_KEY = _T("BLUE_COLOR");
const CString BRUSH_SIZE_SETTING_LEY = _T("BRUSH_SIZE");

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
	ON_BN_CLICKED(BUTTON_DRAW, &CReceiverDlg::OnBnClickedDrawButton)
	ON_BN_CLICKED(BUTTON_CLEAR, &CReceiverDlg::OnBnClickedClear)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CReceiverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ReadSettingsFromRegistry();

	//make dialog full screen size
	CRect rcDesktop;
	rcDesktop.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	rcDesktop.right = rcDesktop.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	rcDesktop.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	rcDesktop.bottom = rcDesktop.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
	MoveWindow(rcDesktop, FALSE);

	THREADSTRUCT* param = new THREADSTRUCT;
	param->dialog = this;
	AfxBeginThread(SetupServerSocket, param);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CReceiverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CReceiverDlg::ReceiveData(LPVOID param)
{
	THREADSTRUCT* ts = (THREADSTRUCT*)param;
	ServerSocket::readResult res = ts->dialog->serverSocket.ReceiveData();
	if (res.resultCode == 0) 
	{
		CWnd* brawButton = ts->dialog->GetDlgItem(BUTTON_DRAW);
		brawButton->EnableWindow(TRUE);
		ts->dialog->mouseData = res.data;
		ts->dialog->StartDrawing();
	}

	//after data receiving get ready for getting new data again
	AfxBeginThread(ReceiveData, param);
	return TRUE;
}

UINT CReceiverDlg::SetupServerSocket(LPVOID param)
{
	THREADSTRUCT* ts = (THREADSTRUCT*)param;

	ServerSocket::result res = ts->dialog->serverSocket.SetupConnection();
	if (res.resultCode != 0) 
	{
		return FALSE;
	}

	AfxBeginThread(ReceiveData, param);

	return TRUE;
}

Point convertToPoint(POINT fromPoint) 
{
	Point gdiPoint;
	gdiPoint.X = (INT)(fromPoint.x * SCALE);
	gdiPoint.Y = (INT)(fromPoint.y * SCALE) + TOP_OFFSET; //in order not to overlap witht the controls
	return gdiPoint;
}

UINT CReceiverDlg::DrawBackground(LPVOID param)
{
	THREADSTRUCT* ts = (THREADSTRUCT*)param;
	CRect client;
	ts->dialog->GetClientRect(&client);
	CWnd* btn = ts->dialog->GetDlgItem(ID_BTN2);
	btn->EnableWindow(FALSE);
	CDC* dc = ts->dialog->GetDC();
	Graphics graphics(dc->m_hDC);
	int mainSize = ts->dialog->GetEditControlIntValue(EDIT_SIZE);
	int size = mainSize * 15; //15 times bigger that main brush size
	Pen semiTransPen(Color(5, 0, 0, 255), (REAL)size);
	while (true) 
	{
		std::unique_lock<std::mutex> lock(ts->dialog->mu);
		ts->dialog->cv.wait(lock, [ts] {return ts->dialog->dataPartIsReady;});//Wait for partial data to be ready
		if (ts->dialog->dataBuffer.getSize() == 0) {
			//data ready bust size is zero - we're done
			// reset all the variables
			ts->dialog->dataPartIsReady = false;
			ts->dialog->dataIsProcessed = false;
			lock.unlock();
			break;
		}

		std::vector<POINT> nextDataBulk = ts->dialog->dataBuffer.getAll();
		for (int i=0; i< nextDataBulk.size(); i++)
		{
			POINT point = nextDataBulk[i];
			Point gdiPoint = convertToPoint(point);
			Rect rect(gdiPoint.X, gdiPoint.Y, 10, 10);
			graphics.DrawArc(&semiTransPen, rect, 0, 360);
		}

		ts->dialog->dataIsProcessed = true;
		ts->dialog->dataPartIsReady = false;
		lock.unlock();
		ts->dialog->cv.notify_one();
	}
	ts->dialog->ReleaseDC(dc);
	btn->EnableWindow(TRUE);

	return TRUE;
}

void CReceiverDlg::SendDataToBackgroundWaitUntilProcessed(POINT current)
{
	std::unique_lock<std::mutex> lock(mu);
	dataBuffer.push(current);
	lock.unlock();
	dataPartIsReady = true;
	dataIsProcessed = false;
	cv.notify_one();
	{
		std::unique_lock<std::mutex> processedLock(mu);
		cv.wait(processedLock, [this] {return dataIsProcessed;});
	}
}

UINT CReceiverDlg::DrawMainTrack(LPVOID param)
{
	THREADSTRUCT* ts = (THREADSTRUCT*)param;
	CRect client;
	ts->dialog->GetClientRect(&client);
	CDC* dc = ts->dialog->GetDC();
	Graphics graphics(dc->m_hDC);
	int red = ts->dialog->GetEditControlIntValue(EDIT_RED);
	int green = ts->dialog->GetEditControlIntValue(EDIT_GREEN);
	int blue = ts->dialog->GetEditControlIntValue(EDIT_BLUE);
	int penSize = ts->dialog->GetEditControlIntValue(EDIT_SIZE);
	Pen pen(Color(red, green, blue), (REAL)penSize);

	POINT lastPoint;
	lastPoint.x = -1;
	for (int i = 0; i < ts->dialog->mouseData.size(); i++)
	{
		POINT current = ts->dialog->mouseData[i];
		ts->dialog->SendDataToBackgroundWaitUntilProcessed(current);
		if (lastPoint.x == -1)
		{
			lastPoint = current;
			continue;
		}
		POINT from = lastPoint;
		POINT to = current;
		graphics.DrawLine(&pen, convertToPoint(from), convertToPoint(to));
		lastPoint = to;
	}
	std::unique_lock<std::mutex> lock(ts->dialog->mu);
	ts->dialog->dataPartIsReady = true;
	lock.unlock();
	ts->dialog->cv.notify_one();

	ts->dialog->ReleaseDC(dc);
	return TRUE;
}

void CReceiverDlg::StartDrawing() 
{
	THREADSTRUCT* param = new THREADSTRUCT;
	param->dialog = this;
	AfxBeginThread(DrawMainTrack, param);
	AfxBeginThread(DrawBackground, param);
}

void CReceiverDlg::OnBnClickedDrawButton()
{
	StartDrawing();
}

void CReceiverDlg::OnBnClickedClear()
{
	//redraw dialog with clearing all the drawings if exits
	Invalidate();
	UpdateWindow();
}

void CReceiverDlg::ReadSettingsFromRegistry()
{
	CWinApp* pApp = AfxGetApp();
	int redColor = pApp->GetProfileInt(APP_NAME, RED_COLOR_SETTING_KEY, 0);
	CString redCorolStr(std::to_string(redColor).c_str());
	SetDlgItemText(EDIT_RED, redCorolStr);

	int greenColor = pApp->GetProfileInt(APP_NAME, GREEN_COLOR_SETTING_KEY, 0);
	CString greenColorStr(std::to_string(greenColor).c_str());
	SetDlgItemText(EDIT_GREEN, greenColorStr);

	int blueColor = pApp->GetProfileInt(APP_NAME, BLUE_COLOR_SETTING_KEY, 255);
	CString blueColorStr(std::to_string(blueColor).c_str());
	SetDlgItemText(EDIT_BLUE, blueColorStr);

	int brushSize = pApp->GetProfileInt(APP_NAME, BRUSH_SIZE_SETTING_LEY, 5);
	CString brushSizeStr(std::to_string(brushSize).c_str());
	SetDlgItemText(EDIT_SIZE, brushSizeStr);
}

int CReceiverDlg::GetEditControlIntValue(int controlId)
{
	CString strValue;
	GetDlgItemText(controlId, strValue);
	return _ttoi(strValue);
}

BOOL CReceiverDlg::SaveSettingsToRegistry()
{
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(APP_NAME, RED_COLOR_SETTING_KEY, GetEditControlIntValue(EDIT_RED));
	pApp->WriteProfileInt(APP_NAME, GREEN_COLOR_SETTING_KEY, GetEditControlIntValue(EDIT_GREEN));
	pApp->WriteProfileInt(APP_NAME, BLUE_COLOR_SETTING_KEY, GetEditControlIntValue(EDIT_BLUE));
	pApp->WriteProfileInt(APP_NAME, BRUSH_SIZE_SETTING_LEY, GetEditControlIntValue(EDIT_SIZE));
	return TRUE;
}

void CReceiverDlg::OnClose()
{
	CloseConnection();
	SaveSettingsToRegistry();
	CDialogEx::OnClose();
}

void CReceiverDlg::CloseConnection() 
{
	serverSocket.CloseConnection();
}
