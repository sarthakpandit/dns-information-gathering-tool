#pragma once

class DlgTLD;

#include "afxcmn.h"
#include "curl_tools.h"
#include "Shared.h"
#include "DNS ToolDlg.h"
#include "DlgWeb.h"
#include "afxwin.h"

// DlgTLD dialog

class DlgTLD : public CDialog
{
	DECLARE_DYNAMIC(DlgTLD)

public:
	DlgTLD(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgTLD();

// Dialog Data
	enum { IDD = IDD_TLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl table;

	BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	thread_status* this_struct;
	CURL* curl;
	BOOL d_generic;
	BOOL d_europe;
	BOOL d_asia;
	BOOL d_americas;
	BOOL d_oceania;
	BOOL d_africa;
	CShared* shared;
	DWORD ThreadId;
	HANDLE Thread;
	afx_msg void OnBnClickedButton3();
	BOOL select_all;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedCheck7();
	CButton but_start;
	bool test_running;
	CButton but_next;
	bool request_cancel;
	CString status;
	afx_msg void OnBnClickedTip();
	void LoadPrev(void);
};
