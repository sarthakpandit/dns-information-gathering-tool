// DlgTLD.cpp : implementation file
//

#include "stdafx.h"
#include "DNS Tool.h"
#include "DlgTLD.h"


// DlgTLD dialog

IMPLEMENT_DYNAMIC(DlgTLD, CDialog)

DlgTLD::DlgTLD(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTLD::IDD, pParent)
	, this_struct(NULL)
	, curl(NULL)
	, d_generic(TRUE)
	, d_europe(TRUE)
	, d_asia(FALSE)
	, d_americas(FALSE)
	, d_oceania(FALSE)
	, d_africa(FALSE)
	, select_all(FALSE)
	, test_running(false)
	, request_cancel(false)
	, status(_T(""))
{

}

DlgTLD::~DlgTLD()
{
}

void DlgTLD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, table);
	DDX_Check(pDX, IDC_CHECK1, d_generic);
	DDX_Check(pDX, IDC_CHECK2, d_europe);
	DDX_Check(pDX, IDC_CHECK3, d_asia);
	DDX_Check(pDX, IDC_CHECK4, d_americas);
	DDX_Check(pDX, IDC_CHECK5, d_oceania);
	DDX_Check(pDX, IDC_CHECK6, d_africa);
	DDX_Check(pDX, IDC_CHECK7, select_all);
	DDX_Control(pDX, IDC_BUTTON2, but_start);
	DDX_Control(pDX, IDC_BUTTON1, but_next);
	DDX_Text(pDX, IDC_ST, status);
}


BEGIN_MESSAGE_MAP(DlgTLD, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &DlgTLD::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &DlgTLD::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &DlgTLD::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &DlgTLD::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK7, &DlgTLD::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_TIP, &DlgTLD::OnBnClickedTip)
END_MESSAGE_MAP()


// DlgTLD message handlers

BOOL DlgTLD::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD af = table.GetExtendedStyle();
	table.SetExtendedStyle(af|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	table.SetExtendedStyle(af|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	table.InsertColumn(0,_T("TLD"),LVCFMT_LEFT,120);
	table.InsertColumn(1,_T("Institution"),LVCFMT_LEFT,150);
	table.InsertColumn(2,_T("Holder"),LVCFMT_LEFT,150);
	table.InsertColumn(3,_T("Country"),LVCFMT_LEFT,90);

	shared->DlgTLDa = this;

	if(table.GetItemCount()==0)
		;//LoadPrev();

	HANDLE hU = ::LoadImage( AfxGetResourceHandle(),
			MAKEINTRESOURCE( IDB_BITMAP1 ),
			IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );

	GetDlgItem(IDC_TIP )->SendMessage(BM_SETIMAGE, IMAGE_BITMAP,
			(LPARAM) (DWORD) hU );

	return TRUE;
}
void DlgTLD::OnBnClickedButton2()
{
	if(!test_running)
	{
		UpdateData(TRUE);

		table.DeleteAllItems();
		LoadPrev();

		but_start.SetWindowTextW(_T("Cancel"));
		but_next.EnableWindow(FALSE);
		test_running = true;
		status = _T("Starting...");
		UpdateData(FALSE);

		thread_data* data = (thread_data*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
					sizeof(thread_data));

		data->shared = shared;
		data->test_id = 2;

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

void DlgTLD::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//curl_easy_cleanup(curl);
	//curl_global_cleanup();

	UpdateData(TRUE);

	CDlgWeb* dlg_web = new CDlgWeb;
	dlg_web->shared = shared;
	//dlg_tld->DoModal();
	dlg_web->Create(IDD_WEB,NULL);
	dlg_web->ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);

	MSG msg;

	while(true)
	{
		while(PeekMessage(&msg, NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(!dlg_web->IsWindowVisible())
		{
			ShowWindow(SW_SHOW);
			break;
		}

		Sleep(30);
	}
}

void DlgTLD::OnBnClickedButton3()
{/*
	// TODO: Add your control notification handler code here
	int ag = table.GetItemCount();
	LVITEMW* item;
	for(int a=0;a<ag;a++)
	{
		table.GetItem(item);
		item->state = 1;
		table.SetItem(item);
		//table.SetCheck(ag,TRUE);
	}

	table.UpdateData(TRUE);
	table.RedrawWindow();
	*/
}

void DlgTLD::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void DlgTLD::OnBnClickedCheck7()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	table.EnableWindow(1-select_all);
}

void DlgTLD::OnBnClickedTip()
{
	// TODO: Add your control notification handler code here
	CString Msg = _T("");
	
	Msg+=_T("In the domain name www.example.com, the top-level domain is .com (or .COM, as domain names are not case-sensitive).\r\n");
	Msg+=_T("More details under http://en.wikipedia.org/wiki/List_of_Internet_top-level_domains ");

	MessageBox(Msg);
}

void DlgTLD::LoadPrev(void)
{
	// shared->DNSToolDlg->domains_from_ranges;
	// shared->DNSToolDlg->domains_from_domain;

	table.InsertItem(table.GetItemCount(),shared->DNSToolDlg->domain_name);

	int total;
	CString domain;

	total = shared->DNSToolDlg->domains_from_ranges.GetItemCount();

	for(int bb=0;bb<total;bb++)
	{
		if(shared->DNSToolDlg->select_all_domain==FALSE)
			if(shared->DNSToolDlg->domains_from_ranges.GetCheck(bb)==0)
				continue;

		domain = shared->DNSToolDlg->domains_from_ranges.GetItemText(bb,1);
		table.InsertItem(table.GetItemCount(),domain);
	}

	total = shared->DNSToolDlg->domains_from_domain.GetItemCount();

	for(int bb=0;bb<total;bb++)
	{
		if(shared->DNSToolDlg->select_all_domain2==FALSE)
			if(shared->DNSToolDlg->domains_from_domain.GetCheck(bb)==0)
				continue;

		domain = shared->DNSToolDlg->domains_from_domain.GetItemText(bb,1);
		table.InsertItem(table.GetItemCount(),domain);
	}

}
