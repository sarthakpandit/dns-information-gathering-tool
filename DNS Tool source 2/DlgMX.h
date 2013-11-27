#pragma once
#include "afxcmn.h"

class CDlgMX;

#include "shared.h"

// CDlgMX dialog

class CDlgMX : public CDialog
{
	DECLARE_DYNAMIC(CDlgMX)

public:
	CDlgMX(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMX();

// Dialog Data
	enum { IDD = IDD_MX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl table;
	CString status;
	BOOL OnInitDialog();
	afx_msg void OnBnClickedButton4();
	CShared* shared;
	afx_msg void OnBnClickedButton2();
	DWORD ThreadId;
	HANDLE Thread;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedTip();
};
