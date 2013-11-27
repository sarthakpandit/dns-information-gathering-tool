// Conf.cpp : implementation file
//

#include "stdafx.h"
#include "DNS Tool.h"
#include "Conf.h"


// CConf dialog

IMPLEMENT_DYNAMIC(CConf, CDialog)

CConf::CConf(CWnd* pParent /*=NULL*/)
	: CDialog(CConf::IDD, pParent)
	, test_running(false)
	, request_cancel(false)
	, select_all(FALSE)
	, select_1(FALSE)
	, select_2(FALSE)
	, select_3(FALSE)
	, select_4(FALSE)
	, select_5(FALSE)
	, select_6(FALSE)
	, select_7(FALSE)
	, select_8(FALSE)
	, res_1(_T(""))
	, res_2(_T(""))
	, res_3(_T(""))
	, res_4(_T(""))
	, res_5(_T(""))
	, res_6(_T(""))
	, res_7(_T(""))
	, res_8(_T(""))
	, status(_T(""))
{

}

CConf::~CConf()
{
}

void CConf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK9, select_all);
	DDX_Check(pDX, IDC_CHECK1, select_1);
	DDX_Check(pDX, IDC_CHECK2, select_2);
	DDX_Check(pDX, IDC_CHECK3, select_3);
	DDX_Check(pDX, IDC_CHECK4, select_4);
	DDX_Check(pDX, IDC_CHECK5, select_5);
	DDX_Check(pDX, IDC_CHECK6, select_6);
	DDX_Check(pDX, IDC_CHECK8, select_7);
	DDX_Check(pDX, IDC_CHECK7, select_8);
	DDX_Text(pDX, IDC_STATIC1, res_1);
	DDX_Text(pDX, IDC_STATIC2, res_2);
	DDX_Text(pDX, IDC_STATIC3, res_3);
	DDX_Text(pDX, IDC_STATIC4, res_4);
	DDX_Text(pDX, IDC_STATIC5, res_5);
	DDX_Text(pDX, IDC_STATIC6, res_6);
	DDX_Text(pDX, IDC_STATIC7, res_7);
	DDX_Text(pDX, IDC_STATIC8, res_8);
	DDX_Control(pDX, IDC_BUTTON2, but_start);
	DDX_Control(pDX, IDC_BUTTON1, but_next);
	DDX_Text(pDX, IDC_STATIC23, status);
}


BEGIN_MESSAGE_MAP(CConf, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &CConf::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CConf::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CConf::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CConf message handlers

BOOL CConf::OnInitDialog()
{
	CDialog::OnInitDialog();

	shared->Conf = this;

	return TRUE;
}

void CConf::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CConf::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if(!test_running)
	{
		UpdateData(TRUE);

		but_start.SetWindowTextW(_T("Cancel"));
		but_next.EnableWindow(FALSE);
		test_running = true;
		status = _T("Starting...");
		UpdateData(FALSE);

		// start
		thread_data* data = (thread_data*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
					sizeof(thread_data));

		data->shared = shared;
		data->test_id = 7;

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

void CConf::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CDlgReport* dlg_rep = new CDlgReport;
	dlg_rep->shared = shared;
	//dlg_tld->DoModal();
	dlg_rep->Create(IDD_REPORT,NULL);
	dlg_rep->ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);

	MSG msg;

	while(true)
	{
		while(PeekMessage(&msg, NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(!dlg_rep->IsWindowVisible())
		{
			ShowWindow(SW_SHOW);
			break;
		}

		Sleep(30);
	}
}

HBRUSH CConf::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	if(nCtlColor==CTLCOLOR_STATIC)
	{
		CStatic* staticc = reinterpret_cast<CStatic*>(pWnd);
		CString str;
		staticc->GetWindowTextW(str);
		if(str==_T("YES"))
		{
			//if(reverse)
				pDC->SetTextColor(RGB(5, 200, 5));
			//else
				//pDC->SetTextColor(RGB(200, 5, 5));
		}
		else if(str==_T("NO"))
		{
			//if(!reverse)
				//pDC->SetTextColor(RGB(5, 200, 5));
			//else
				pDC->SetTextColor(RGB(200, 5, 5));
		}
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
