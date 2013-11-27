#pragma once

class CArecsDlg;

#include "shared.h"

#include "afxcmn.h"
#include "afxwin.h"
#include "afxtempl.h"


// CArecsDlg dialog

class CArecsDlg : public CDialog
{
	DECLARE_DYNAMIC(CArecsDlg)

public:
	CArecsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CArecsDlg();

// Dialog Data
	enum { IDD = IDD_ARECS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL find1;
	BOOL find2;
	BOOL find3;
	CString pages;
	CString com_ns;
	CString com_chars;
	CString com_charset;
	CListCtrl table;
	CButton but_start;
	CButton but_next;
	BOOL OnInitDialog();
	CShared* shared;
	DWORD ThreadId;
	HANDLE Thread;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedTip();
	CString status;
	bool test_running;
	bool request_cancel;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CString SDictionary;
	CList<CString> dict;
	afx_msg void OnBnClickedButton1();
};
