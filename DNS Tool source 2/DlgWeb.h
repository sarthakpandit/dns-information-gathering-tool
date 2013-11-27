#pragma once
#include "afxcmn.h"

class CDlgWeb;

#include "shared.h"
#include "afxwin.h"

// CDlgWeb dialog

class CDlgWeb : public CDialog
{
	DECLARE_DYNAMIC(CDlgWeb)

public:
	CDlgWeb(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWeb();

	BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_WEB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl table;
	BOOL select_all;
	CShared* shared;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheck1();
	DWORD ThreadId;
	HANDLE Thread;
	CString status;
	afx_msg void OnBnClickedButton1();
	CString* domains;
	afx_msg void OnBnClickedTip();
	bool test_running;
	bool request_cancel;
	CButton but_start;
	CButton but_next;
};
