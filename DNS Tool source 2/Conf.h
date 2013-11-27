#pragma once


class CConf;

#include "shared.h"
#include "afxwin.h"

// CConf dialog

class CConf : public CDialog
{
	DECLARE_DYNAMIC(CConf)

public:
	CConf(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConf();

// Dialog Data
	enum { IDD = IDD_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	
public:
	BOOL OnInitDialog();
	CShared* shared;
	DWORD ThreadId;
	HANDLE Thread;
	bool test_running;
	bool request_cancel;
	afx_msg void OnBnClickedButton4();
	BOOL select_all;
	BOOL select_1;
	BOOL select_2;
	
	BOOL select_3;
	BOOL select_4;
	BOOL select_5;
	BOOL select_6;
	BOOL select_7;
	BOOL select_8;
	CString res_1;
	CString res_2;
	CString res_3;
	CString res_4;
	CString res_5;
	CString res_6;
	CString res_7;
	CString res_8;
	CButton but_start;
	CButton but_next;
	CString status;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
