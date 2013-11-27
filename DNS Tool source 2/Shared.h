#pragma once

class CShared;

#define SOCK_BUFFER 32768

#include "stdafx.h"
#include "DNS Tool.h"
#include "DNS ToolDlg.h"
#include "DlgTLD.h"
#include "DlgWeb.h"
#include "DlgMX.h"
#include "DlgNS.h"
#include "ArecsDlg.h"
#include "Conf.h"
#include "DlgReport.h"
#include "Windns.h"

class CShared
{
	public:
		CDNSToolDlg* DNSToolDlg;
		DlgTLD* DlgTLDa;
		CDlgWeb* DlgWeb;
		CDlgMX* DlgMX;
		CDlgNS* DlgNS;
		CArecsDlg* ArecsDlg;
		CConf* Conf;
		CDlgReport* DlgReport;
};

struct thread_data
{
	CShared* shared;
	int test_id;
};

DWORD WINAPI RunTest(LPVOID lpParam);

CString ReversePtr(CString a, CString b, CString c, CString d);