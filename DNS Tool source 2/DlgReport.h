#pragma once

class CDlgReport;

#include "shared.h"

// CDlgReport dialog

class CDlgReport : public CDialog
{
	DECLARE_DYNAMIC(CDlgReport)

public:
	CDlgReport(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgReport();

// Dialog Data
	enum { IDD = IDD_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	BOOL OnInitDialog();
	CShared* shared;
	DWORD ThreadId;
	HANDLE Thread;
	afx_msg void OnBnClickedButton4();
	BOOL rep_txt;
	BOOL rep_html;
	afx_msg void OnBnClickedButton2();
};
