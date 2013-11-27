// ArecsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DNS Tool.h"
#include "ArecsDlg.h"


// CArecsDlg dialog

IMPLEMENT_DYNAMIC(CArecsDlg, CDialog)

CArecsDlg::CArecsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArecsDlg::IDD, pParent)
	, find1(FALSE)
	, find2(FALSE)
	, find3(FALSE)
	, pages(_T(""))
	, com_ns(_T(""))
	, com_chars(_T(""))
	, com_charset(_T(""))
	, status(_T(""))
	, test_running(false)
	, request_cancel(false)
	, SDictionary(_T(""))
{

}

CArecsDlg::~CArecsDlg()
{
}

void CArecsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, find1);
	DDX_Check(pDX, IDC_CHECK2, find2);
	DDX_Check(pDX, IDC_CHECK3, find3);
	DDX_Text(pDX, IDC_EDIT1, pages);
	DDX_CBString(pDX, IDC_COMBO3, com_ns);
	DDX_CBString(pDX, IDC_COMBO1, com_chars);
	DDX_CBString(pDX, IDC_COMBO2, com_charset);
	DDX_Control(pDX, IDC_LIST1, table);
	DDX_Control(pDX, IDC_BUTTON2, but_start);
	DDX_Control(pDX, IDC_BUTTON1, but_next);
	DDX_Text(pDX, IDC_STA, status);
}


BEGIN_MESSAGE_MAP(CArecsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &CArecsDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_TIP, &CArecsDlg::OnBnClickedTip)
	ON_BN_CLICKED(IDC_BUTTON2, &CArecsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CArecsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CArecsDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CArecsDlg message handlers

BOOL CArecsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD af = table.GetExtendedStyle();
	table.SetExtendedStyle(af|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	table.SetExtendedStyle(af|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	table.InsertColumn(0,_T("Subdomain"),LVCFMT_LEFT,150);
	table.InsertColumn(1,_T("IP"),LVCFMT_LEFT,100);

	shared->ArecsDlg = this;

		HANDLE hU = ::LoadImage( AfxGetResourceHandle(),
			MAKEINTRESOURCE( IDB_BITMAP1 ),
			IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );

	GetDlgItem(IDC_TIP )->SendMessage(BM_SETIMAGE, IMAGE_BITMAP,
			(LPARAM) (DWORD) hU );

	((CComboBox*)GetDlgItem(IDC_COMBO1 ))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO2 ))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO3 ))->SetCurSel(0);

	CComboBox* ns_sel = ((CComboBox*)GetDlgItem(IDC_COMBO3 ));

	bool start = false;
	CString cur,cur_ns;
	for(int i=0;i<shared->DlgNS->table.GetItemCount();i++)
	{
		cur = shared->DlgNS->table.GetItemText(i,0);
		if(start&&(cur!=_T("")))
			break;
		if((!start)&&(cur==shared->DNSToolDlg->domain_name))
			start = true;
		
		if(start)
			ns_sel->InsertString(ns_sel->GetCount(),shared->DlgNS->table.GetItemText(i,1));
	}

	pages = _T("5");

	UpdateData(FALSE);

	return TRUE;
}
void CArecsDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CArecsDlg::OnBnClickedTip()
{
	// TODO: Add your control notification handler code here
	CString Msg = _T("");
	  
	Msg+=_T("Subdomain: For example, mail.example.com and calendar.example.com\r\n");
	Msg+=_T("are subdomains of the example.com domain.");

	MessageBox(Msg);
}

void CArecsDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if(!test_running)
	{
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
		data->test_id = 6;

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

void CArecsDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CString filters = _T("Text files (*.txt)|*.txt|All Files (*.*)|*.*||");

	CFileDialog fileDlg(TRUE,_T("Text files (*.txt)"), _T("*.txt"),
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY,filters,this);

    if(fileDlg.DoModal()==IDOK)
    {
	    CString path = fileDlg.GetPathName();

		dict.RemoveAll();
   
		int i;
		FILE * pFile;
		char* result;
		char line[1024+1];
		pFile = fopen(StringToChar(path), "r");
		if(pFile!=NULL)
		{
			while(true)
			{
				result = fgets(line,1024,pFile);
				i=0;
				while((line[i]!='\0')&&(line[i]!='\r')&&(line[i]!='\n'))
					i++;

				line[i] = '\0';

				dict.AddTail(CharToString(line));

				if (feof(pFile))
					break;
			}
			fclose(pFile);
		}
    }

}

void CArecsDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CConf* dlg_cc = new CConf;
	dlg_cc->shared = shared;
	//dlg_tld->DoModal();
	dlg_cc->Create(IDD_CONFIG,NULL);
	dlg_cc->ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);

	MSG msg;

	while(true)
	{
		while(PeekMessage(&msg, NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(!dlg_cc->IsWindowVisible())
		{
			ShowWindow(SW_SHOW);
			break;
		}

		Sleep(30);
	}
}
