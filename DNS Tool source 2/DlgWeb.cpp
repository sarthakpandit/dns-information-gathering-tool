// DlgWeb.cpp : implementation file
//

#include "stdafx.h"
#include "DNS Tool.h"
#include "DlgWeb.h"


// CDlgWeb dialog

IMPLEMENT_DYNAMIC(CDlgWeb, CDialog)

CDlgWeb::CDlgWeb(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWeb::IDD, pParent)
	, select_all(FALSE)
	, status(_T(""))
	, test_running(false)
	, request_cancel(false)
{

}

CDlgWeb::~CDlgWeb()
{
}

void CDlgWeb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, table);
	DDX_Check(pDX, IDC_CHECK1, select_all);
	DDX_Text(pDX, IDC_STA, status);
	DDX_Control(pDX, IDC_BUTTON2, but_start);
	DDX_Control(pDX, IDC_BUTTON1, but_next);
}


BEGIN_MESSAGE_MAP(CDlgWeb, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgWeb::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgWeb::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgWeb::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgWeb::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_TIP, &CDlgWeb::OnBnClickedTip)
END_MESSAGE_MAP()


// CDlgWeb message handlers

BOOL CDlgWeb::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD af = table.GetExtendedStyle();
	table.SetExtendedStyle(af|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	table.SetExtendedStyle(af|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT/*|LVS_EX_CHECKBOXES*/);
	table.InsertColumn(0,_T("Title"),LVCFMT_LEFT,150);
	table.InsertColumn(1,_T("Header"),LVCFMT_LEFT,150);
	table.InsertColumn(2,_T("IP"),LVCFMT_LEFT,100);

	shared->DlgWeb = this;

	HANDLE hU = ::LoadImage( AfxGetResourceHandle(),
			MAKEINTRESOURCE( IDB_BITMAP1 ),
			IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );

	GetDlgItem(IDC_TIP )->SendMessage(BM_SETIMAGE, IMAGE_BITMAP,
			(LPARAM) (DWORD) hU );

	return TRUE;
}
void CDlgWeb::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgWeb::OnBnClickedButton2()
{
	if(!test_running)
	{
		domains = new CString[shared->DlgTLDa->table.GetItemCount()];
		UpdateData(TRUE);

		table.DeleteAllItems();

		but_start.SetWindowTextW(_T("Cancel"));
		but_next.EnableWindow(FALSE);
		test_running = true;
		status = _T("Starting...");
		UpdateData(FALSE);

		// start
		thread_data* data = (thread_data*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
					sizeof(thread_data));

		data->shared = shared;
		data->test_id = 3;

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

void CDlgWeb::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	table.EnableWindow(1-select_all);
}

void CDlgWeb::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	CDlgMX* dlg_mx = new CDlgMX;
	dlg_mx->shared = shared;
	//dlg_tld->DoModal();
	dlg_mx->Create(IDD_MX,NULL);
	dlg_mx->ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);

	MSG msg;

	while(true)
	{
		while(PeekMessage(&msg, NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(!dlg_mx->IsWindowVisible())
		{
			ShowWindow(SW_SHOW);
			break;
		}

		Sleep(30);
	}
}

void CDlgWeb::OnBnClickedTip()
{
	// TODO: Add your control notification handler code here
	CString Msg = _T("");
	 
	Msg+=_T("Based on the web site title, server header or the IP registrant\r\n");
	Msg+=_T("you can determine if the site belongs to the same organization.");

	MessageBox(Msg);
}
