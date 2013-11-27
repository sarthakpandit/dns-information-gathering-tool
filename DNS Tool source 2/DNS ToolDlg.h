// DNS ToolDlg.h : header file
//

#pragma once

class CDNSToolDlg;

#include "afxcmn.h"
#include "Shared.h"
#include "DlgTLD.h"
#include "Serial.h"
#include "afxwin.h"

// CDNSToolDlg dialog
class CDNSToolDlg : public CDialog
{
// Construction
public:
	CDNSToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DNSTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString domain_name;
	CString from_1;
	CString from_2;
	CString from_3;
	CString from_4;
	CString to_1;
	CString to_2;
	CString to_3;
	CString to_4;
	CString number_of_domains;
	CListCtrl domains_from_ranges;
	CListCtrl domains_from_domain;
	CShared* shared;
	afx_msg void OnBnClickedButton2();
	DWORD ThreadId;
	HANDLE Thread;
	afx_msg void OnBnClickedButton1();
	bool test_running;
	bool code_ok;
	CButton but_start;
	CString status;
	bool request_cancel;
	afx_msg void OnStnClickedPic1();
	afx_msg void OnBnClickedTip();
	BOOL select_all_domain;
	BOOL select_all_domain2;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	int domains_num;
	CString* domains;
};
