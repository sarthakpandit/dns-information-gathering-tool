#pragma once

class CDlgNS;

#include "shared.h"
#include "afxcmn.h"

// CDlgNS dialog

class CDlgNS : public CDialog
{
	DECLARE_DYNAMIC(CDlgNS)

public:
	CDlgNS(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNS();

// Dialog Data
	enum { IDD = IDD_NS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl table;
	BOOL OnInitDialog();
	CShared* shared;
	DWORD ThreadId;
	HANDLE Thread;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedTip();
	afx_msg void OnBnClickedButton1();
};
