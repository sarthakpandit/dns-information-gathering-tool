// DNS ToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DNS Tool.h"
#include "DNS ToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDNSToolDlg dialog




CDNSToolDlg::CDNSToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDNSToolDlg::IDD, pParent)
	, domain_name(_T(""))
	, from_1(_T(""))
	, from_2(_T(""))
	, from_3(_T(""))
	, from_4(_T(""))
	, to_1(_T(""))
	, to_2(_T(""))
	, to_3(_T(""))
	, to_4(_T(""))
	, number_of_domains(_T("50"))
	, shared(NULL)
	, test_running(false)
	, status(_T(""))
	, request_cancel(false)
	, select_all_domain(FALSE)
	, select_all_domain2(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDNSToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, domain_name);
	DDX_Text(pDX, IDC_EDIT2, from_1);
	DDX_Text(pDX, IDC_EDIT3, from_2);
	DDX_Text(pDX, IDC_EDIT4, from_3);
	DDX_Text(pDX, IDC_EDIT5, from_4);
	DDX_Text(pDX, IDC_EDIT6, to_1);
	DDX_Text(pDX, IDC_EDIT7, to_2);
	DDX_Text(pDX, IDC_EDIT8, to_3);
	DDX_Text(pDX, IDC_EDIT9, to_4);
	DDX_Text(pDX, IDC_EDIT10, number_of_domains);
	DDX_Control(pDX, IDC_LIST1, domains_from_ranges);
	DDX_Control(pDX, IDC_LIST2, domains_from_domain);
	DDX_Control(pDX, IDC_BUTTON2, but_start);
	DDX_Text(pDX, IDC_STATUS1, status);
	DDX_Check(pDX, IDC_CHECK1, select_all_domain);
	DDX_Check(pDX, IDC_CHECK2, select_all_domain2);
}

BEGIN_MESSAGE_MAP(CDNSToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CDNSToolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDNSToolDlg::OnBnClickedButton1)
	ON_STN_CLICKED(IDC_PIC1, &CDNSToolDlg::OnStnClickedPic1)
	ON_BN_CLICKED(IDC_TIP, &CDNSToolDlg::OnBnClickedTip)
	ON_BN_CLICKED(IDC_CHECK1, &CDNSToolDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CDNSToolDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDNSToolDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDNSToolDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDNSToolDlg message handlers

BOOL CDNSToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	code_ok = false;
	bool code_ok2 = false;
	int* ble = (int*)353633;
	CSerial serial;

	for(;;)
	{
		serial.DoModal();

		//serial.UpdateData(TRUE);
		
		CString temp = _T("DNS Tool");
		temp = _T("")+serial.ser1;
		if(temp.Mid(0,3)==_T("XHD"))
			if(temp.Mid(5,1)==_T("H"))
			{
				int aa = _wtoi(temp.Mid(3,1));
				int ab = _wtoi(temp.Mid(4,1));

				if(aa+ab==13)
				{
					temp = _T("")+serial.ser2;

					aa = _wtoi(temp.Mid(0,1));
					ab = _wtoi(temp.Mid(1,1));

					if(aa+ab==15)
					{
						if(temp.Mid(2,2)==_T("GJ"))
							if(temp.Mid(4,2)==_T("RH"))
							{
								temp = _T("")+serial.ser3;
								if(temp.Mid(0,2)==_T("BH"))
									if(temp.Mid(2,2)==_T("DD"))
										if(temp.Mid(4,2)==_T("87"))
										{
											code_ok = true;
										}
							}
					}
				}
			}

		if(serial.canceled)
		{
			exit(0);
		}

		if(!code_ok)
		{
			Sleep(200);
			Sleep(100);
			MessageBox(_T("Wrong serial code. Please try again."));
		}
		else break;
	}

	if(!code_ok)
	{
		*ble = 455;
		ble+=3531;
		*ble = 3153531;
	}
	else
		code_ok2 = true;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	// TODO: Add extra initialization here
	domains_from_ranges.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	domains_from_ranges.InsertColumn(0,_T("IP"),LVCFMT_LEFT,120);
	domains_from_ranges.InsertColumn(1,_T("Domain"),LVCFMT_LEFT,160);

	int* ble2;

	domains_from_domain.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	domains_from_domain.InsertColumn(0,_T("IP"),LVCFMT_LEFT,120);
	domains_from_domain.InsertColumn(1,_T("Domain"),LVCFMT_LEFT,160);

	if(!code_ok2)
	{
		*ble2 = 8855;
		ble2+=4531;
		*ble2 = 483531;
	}

	shared = new CShared;
	memset(shared,0,sizeof(CShared));
	shared->DNSToolDlg = this;

	//HWND* hUpCtl;

	/* The button is control IDC_UP */
	//hUpCtl = GetDlgItem(IDC_TIP );

	/* The icon is IDI_UP in the resource file */
	//HICON
		HANDLE hU = ::LoadImage( AfxGetResourceHandle(),
			MAKEINTRESOURCE( IDB_BITMAP1 ),
			IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );

	GetDlgItem(IDC_TIP )->SendMessage(BM_SETIMAGE, IMAGE_BITMAP,
			(LPARAM) (DWORD) hU );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDNSToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDNSToolDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDNSToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDNSToolDlg::OnBnClickedButton2()
{
	if(!test_running)
	{
		UpdateData(TRUE);

		domains_from_domain.DeleteAllItems();
		domains_from_ranges.DeleteAllItems();

		but_start.SetWindowTextW(_T("Cancel"));
		test_running = true;
		status = _T("Starting...");
		UpdateData(FALSE);

		// TODO: Add your control notification handler code here
		thread_data* data = (thread_data*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
					sizeof(thread_data));

		data->shared = shared;
		data->test_id = 1;

		Thread = CreateThread(NULL,0,RunTest,data,0,&ThreadId);

		if(!Thread)
			MessageBox(_T("FAILED TO CREATE TEST THREAD"));
	}
	else
	{
		request_cancel = true;
		status = _T("Cancelling...");
		UpdateData(FALSE);
	}
}

void CDNSToolDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);

	if(domain_name==_T(""))
		MessageBox(_T("Please enter domain name"));
	else
	{
		// table with domains from both tables + main domain // to be used in next test
		//domains_num = 1+domains_from_ranges->;
		//domains =

		DlgTLD* dlg_tld = new DlgTLD;
		dlg_tld->shared = shared;
		//dlg_tld->DoModal();
		dlg_tld->Create(IDD_TLD,NULL);
		dlg_tld->ShowWindow(SW_SHOW);
		ShowWindow(SW_HIDE);

		MSG msg;

		while(true)
		{
			while(PeekMessage(&msg, NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if(!dlg_tld->IsWindowVisible())
			{
				ShowWindow(SW_SHOW);
				break;
			}

			Sleep(30);
		}
	}

	//DestroyWindow();
}

void CDNSToolDlg::OnStnClickedPic1()
{

}

void CDNSToolDlg::OnBnClickedTip()
{
	// TODO: Add your control notification handler code here
		CString Msg = _T("");
	
	Msg+=_T("Hint: Please enter the domain name as a starting point.\r\n");
	Msg+=_T("\r\n");
	Msg+=_T("PTR: A reverse DNS lookup is the determination of a domain name\r\n");
	Msg+=_T("	that is associated with a given IP address. This query will\r\n");
	Msg+=_T("	help you identify other possible hosts within the given IP Ranges.");

	MessageBox(Msg);
}

void CDNSToolDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	domains_from_ranges.EnableWindow(1-select_all_domain);
}

void CDNSToolDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	domains_from_domain.EnableWindow(1-select_all_domain2);
}

void CDNSToolDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDNSToolDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	MessageBox(_T("DNS Tool v1.3\r\n2011-2012 Krzysztof Kucza for (c)NetProtect AG"));
}
