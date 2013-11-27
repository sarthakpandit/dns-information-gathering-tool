// DlgMX.cpp : implementation file
//

#include "stdafx.h"
#include "DNS Tool.h"
#include "DlgMX.h"


// CDlgMX dialog

IMPLEMENT_DYNAMIC(CDlgMX, CDialog)

CDlgMX::CDlgMX(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMX::IDD, pParent)
	, status(_T(""))
{

}

CDlgMX::~CDlgMX()
{
}

void CDlgMX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, table);
	DDX_Text(pDX, IDC_STA, status);
}


BEGIN_MESSAGE_MAP(CDlgMX, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgMX::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgMX::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMX::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_TIP, &CDlgMX::OnBnClickedTip)
END_MESSAGE_MAP()


// CDlgMX message handlers

BOOL CDlgMX::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD af = table.GetExtendedStyle();
	table.SetExtendedStyle(af|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	table.SetExtendedStyle(af|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	table.InsertColumn(0,_T("Domain"),LVCFMT_LEFT,140);
	table.InsertColumn(1,_T("Mail Server(s)"),LVCFMT_LEFT,150);
	table.InsertColumn(2,_T("IP(s)"),LVCFMT_LEFT,100);

	shared->DlgMX = this;

		HANDLE hU = ::LoadImage( AfxGetResourceHandle(),
			MAKEINTRESOURCE( IDB_BITMAP1 ),
			IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );

	GetDlgItem(IDC_TIP )->SendMessage(BM_SETIMAGE, IMAGE_BITMAP,
			(LPARAM) (DWORD) hU );

	return TRUE;
}
void CDlgMX::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgMX::OnBnClickedButton2()
{
	UpdateData(TRUE);

	thread_data* data = (thread_data*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                sizeof(thread_data));

	data->shared = shared;
	data->test_id = 4;

	Thread = CreateThread(NULL,0,RunTest,data,0,&ThreadId);

	if(!Thread)
		MessageBox(_T("FAILED TO CREATE TEST THREAD"));
}

void CDlgMX::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	// next
	UpdateData(TRUE);

	CDlgNS* dlg_ns = new CDlgNS;
	dlg_ns->shared = shared;
	//dlg_tld->DoModal();
	dlg_ns->Create(IDD_NS,NULL);
	dlg_ns->ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);

	MSG msg;

	while(true)
	{
		while(PeekMessage(&msg, NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(!dlg_ns->IsWindowVisible())
		{
			ShowWindow(SW_SHOW);
			break;
		}

		Sleep(30);
	}
}

void CDlgMX::OnBnClickedTip()
{
	// TODO: Add your control notification handler code here

	CString Msg = _T("");
	
	Msg+=_T("A mail exchanger record (MX record) is a type of resource record in the Domain Name System\r\n");
	Msg+=_T("that specifies a mail server responsible for accepting email messages on behalf of a recipient's\r\n");
	Msg+=_T("domain and a preference value used to prioritize mail delivery if multiple mail servers are available.");

	MessageBox(Msg);
}
