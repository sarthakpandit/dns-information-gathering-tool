// DlgNS.cpp : implementation file
//

#include "stdafx.h"
#include "DNS Tool.h"
#include "DlgNS.h"


// CDlgNS dialog

IMPLEMENT_DYNAMIC(CDlgNS, CDialog)

CDlgNS::CDlgNS(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNS::IDD, pParent)
{

}

CDlgNS::~CDlgNS()
{
}

void CDlgNS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, table);
}


BEGIN_MESSAGE_MAP(CDlgNS, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgNS::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgNS::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_TIP, &CDlgNS::OnBnClickedTip)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgNS::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgNS message handlers

void CDlgNS::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here

	// BACK
	OnOK();
}

void CDlgNS::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	//START
	UpdateData(TRUE);

	thread_data* data = (thread_data*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                sizeof(thread_data));

	data->shared = shared;
	data->test_id = 5;

	Thread = CreateThread(NULL,0,RunTest,data,0,&ThreadId);

	if(!Thread)
		MessageBox(_T("FAILED TO CREATE TEST THREAD"));
}

BOOL CDlgNS::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD af = table.GetExtendedStyle();
	table.SetExtendedStyle(af|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	table.SetExtendedStyle(af|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	table.InsertColumn(0,_T("Domain"),LVCFMT_LEFT,100);
	table.InsertColumn(1,_T("Domain Server(s)"),LVCFMT_LEFT,150);
	table.InsertColumn(2,_T("IP(s)"),LVCFMT_LEFT,100);
	table.InsertColumn(3,_T("AXFR"),LVCFMT_LEFT,40);

	shared->DlgNS = this;

		HANDLE hU = ::LoadImage( AfxGetResourceHandle(),
			MAKEINTRESOURCE( IDB_BITMAP1 ),
			IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );

	GetDlgItem(IDC_TIP )->SendMessage(BM_SETIMAGE, IMAGE_BITMAP,
			(LPARAM) (DWORD) hU );

	return TRUE;
}
void CDlgNS::OnBnClickedTip()
{
	// TODO: Add your control notification handler code here
	CString Msg = _T("");
	 
	Msg+=_T("AXFR: The zone transfer is one of the many mechanisms available for administrators to employ\r\n");
	Msg+=_T("for replicating the databases containing the DNS data across a set of DNS servers.");

	MessageBox(Msg);
}

void CDlgNS::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CArecsDlg* dlg_arecs = new CArecsDlg;
	dlg_arecs->shared = shared;
	//dlg_tld->DoModal();
	dlg_arecs->Create(IDD_ARECS,NULL);
	dlg_arecs->ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);

	MSG msg;

	while(true)
	{
		while(PeekMessage(&msg, NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(!dlg_arecs->IsWindowVisible())
		{
			ShowWindow(SW_SHOW);
			break;
		}

		Sleep(30);
	}
}
