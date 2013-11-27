// DlgReport.cpp : implementation file
//

#include "stdafx.h"
#include "DNS Tool.h"
#include "DlgReport.h"


// CDlgReport dialog

IMPLEMENT_DYNAMIC(CDlgReport, CDialog)

CDlgReport::CDlgReport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReport::IDD, pParent)
	, rep_txt(FALSE)
	, rep_html(FALSE)
{

}

CDlgReport::~CDlgReport()
{
}

void CDlgReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, rep_txt);
	DDX_Check(pDX, IDC_CHECK2, rep_html);
}


BEGIN_MESSAGE_MAP(CDlgReport, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgReport::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgReport::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgReport::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDlgReport message handlers

void CDlgReport::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//shared->
	exit(0);
}


BOOL CDlgReport::OnInitDialog()
{
	CDialog::OnInitDialog();

	shared->DlgReport = this;

	return TRUE;
}

void CDlgReport::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgReport::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString report = _T("");

	CDNSToolDlg* Dlg1 = shared->DNSToolDlg;
	DlgTLD* Dlg2 = shared->DlgTLDa;
	CDlgWeb* Dlg3 = shared->DlgWeb;
	CDlgMX* Dlg4 = shared->DlgMX;
	CDlgNS* Dlg5 = shared->DlgNS;
	CArecsDlg* Dlg6 = shared->ArecsDlg;
	CConf* Dlg7 = shared->Conf;

	if(rep_txt)
	{
		CString filters = _T("Text files (*.txt)|*.txt|All Files (*.*)|*.*||");

		CFileDialog fileDlg(FALSE,_T("Text files (*.txt)"), _T("*.txt"),
			OFN_OVERWRITEPROMPT| OFN_HIDEREADONLY,filters,this);

		if(fileDlg.DoModal()==IDOK)
		{
			CString path = fileDlg.GetPathName();

			// put data into string
			report+= _T("---------------------\nDNS Tool text report.\n---------------------\n");
			report+= _T("1. Start\n");
			report+= _T("2. TLD\n");
			report+= _T("3. WEB\n");
			report+= _T("4. MX\n");
			report+= _T("5. NS\n");
			report+= _T("6. A-Records\n");
			report+= _T("7. DNS Config\n");
			report+= _T("\n\n");

			report+= _T("-----------------\n1. Start\n-----------------\n\n");
			report+= _T("Main domain: ")+Dlg1->domain_name;
			report+= _T("\n\nIP Ranges\nFrom: ")+Dlg1->from_1+_T(".")+Dlg1->from_2+_T(".")+Dlg1->from_3+_T(".")+Dlg1->from_4
					+_T("\nTo: ")+Dlg1->to_1+_T(".")+Dlg1->to_2+_T(".")+Dlg1->to_3+_T(".")+Dlg1->to_4;
			report+= _T("\nDomains: \n");

			for(int i=0;i<Dlg1->domains_from_ranges.GetItemCount();i++)
			{
				report+= Dlg1->domains_from_ranges.GetItemText(i,0)+_T(" ")+Dlg1->domains_from_ranges.GetItemText(i,1)+_T("\n");
			}

			report+= _T("\nDomains from shared hosting: \n");

			for(int i=0;i<Dlg1->domains_from_domain.GetItemCount();i++)
			{
				report+= Dlg1->domains_from_domain.GetItemText(i,0)+_T(" ")+Dlg1->domains_from_domain.GetItemText(i,1)+_T("\n");
			}

			report+= _T("\n-----------------\n2. TLD\n-----------------\n\n");
			report+= _T("\nTLD | Institution | Holder | Country\n");
	
			for(int i=0;i<Dlg2->table.GetItemCount();i++)
			{
				report+= Dlg2->table.GetItemText(i,0)+_T(" | ")+Dlg2->table.GetItemText(i,1)+_T(" | ")+Dlg2->table.GetItemText(i,2)+_T(" | ")+Dlg2->table.GetItemText(i,3)+_T("\n");
			}

			report+= _T("\n-----------------\n3. WEB\n-----------------\n\n");
			report+= _T("\nTitle | Header | IP\n");
	
			for(int i=0;i<Dlg3->table.GetItemCount();i++)
			{
				report+= Dlg3->table.GetItemText(i,0)+_T(" | ")+Dlg3->table.GetItemText(i,1)+_T(" | ")+Dlg3->table.GetItemText(i,2)+_T("\n");
			}

			report+= _T("\n-----------------\n4. MX\n-----------------\n\n");
			report+= _T("\nDomain | Mail server(s) | IP(s)\n");

			CString dom1 = _T("");
			CString dom2 = _T("");
			bool nul = false;
	
			for(int i=0;i<Dlg4->table.GetItemCount();i++)
			{
				if(Dlg4->table.GetItemText(i,0)!=_T(""))
				{
					dom1 = Dlg4->table.GetItemText(i,0);
					dom2 = dom1;
				}
				else
				{
					int le = dom2.GetLength();
					dom2 = _T("");
					for(int b=0;b<le;b++)
						dom2+=_T(" ");
				}
				report+= dom2+_T(" | ")+Dlg4->table.GetItemText(i,1)+_T(" | ")+Dlg4->table.GetItemText(i,2)+_T("\n");
			}

			report+= _T("\n-----------------\n5. NS\n-----------------\n\n");
			report+= _T("\nDomain | Name server(s) | IP(s) | AXFR\n");

			dom1 = _T("");
			dom2 = _T("");
	
			for(int i=0;i<Dlg5->table.GetItemCount();i++)
			{
				if(Dlg5->table.GetItemText(i,0)!=_T(""))
				{
					dom1 = Dlg5->table.GetItemText(i,0);
					dom2 = dom1;
				}
				else
				{
					int le = dom2.GetLength();
					dom2 = _T("");
					for(int b=0;b<le;b++)
						dom2+=_T(" ");
				}
				report+= dom2+_T(" | ")+Dlg5->table.GetItemText(i,1)+_T(" | ")+Dlg5->table.GetItemText(i,2)+_T(" | ")+Dlg5->table.GetItemText(i,3)+_T("\n");
			}

			report+= _T("\n-----------------\n6. A-Records\n-----------------\n\n");
			report+= _T("Find subdomains by search engine: ");
			if(Dlg6->find1==TRUE)
				report+=_T("YES\n");
			else
				report+=_T("NO\n");
			report+= _T("Find subdomains via Dictionary query: ");
			if(Dlg6->find2==TRUE)
				report+=_T("YES\n");
			else
				report+=_T("NO\n");
			report+= _T("Find subdomains via Brute Force: ");
			if(Dlg6->find3==TRUE)
				report+=_T("YES\n");
			else
				report+=_T("NO\n");
			
			report+= _T("\nResults:\nSubdomain | IP\n");
	
			for(int i=0;i<Dlg6->table.GetItemCount();i++)
			{
				report+= Dlg6->table.GetItemText(i,0)+_T(" | ")+Dlg6->table.GetItemText(i,1)+_T("\n");
			}

			report+= _T("\n-----------------\n7. DNS Config\n-----------------\n");

			report+=_T("\n1.DNS servers have A records: ")+Dlg7->res_1;
			report+=_T("\n2.No CNAMEs for domain: ")+Dlg7->res_2;
			report+=_T("\n3.No NSs with CNAMEs: ")+Dlg7->res_3;
			report+=_T("\n4.Number of nameservers: ")+Dlg7->res_4;
			report+=_T("\n5.Number of mailservers: ")+Dlg7->res_5;
			report+=_T("\n6.All NS IPs public: ")+Dlg7->res_6;
			report+=_T("\n7.Reverse DNS entries for MX records: ")+Dlg7->res_7;
			report+=_T("\n8.Reverse DNS entries for NS records: ")+Dlg7->res_8;

			report+= _T("\n-----------------\n End of report \n-----------------\n");

			//convert to cstr
			char* ta = StringToChar(report);

			// save
			FILE * pFile;
			int result;
			pFile = fopen(StringToChar(path), "w");
			if(pFile!=NULL)
			{
				result = fputs((const char*)ta,pFile);
				fclose(pFile);
			}

			MessageBox(_T("Text report succesfully saved."));
		}


	}

	if(rep_html)
	{
		CString filters = _T("Html files (*.html)|*.html|All Files (*.*)|*.*||");

		CFileDialog fileDlg(FALSE,_T("Html files (*.html)"), _T("*.html"),
			OFN_OVERWRITEPROMPT| OFN_HIDEREADONLY,filters,this);

		if(fileDlg.DoModal()==IDOK)
		{
			CString path = fileDlg.GetPathName();

			// put data into string
			report = _T("");
			report+= _T("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\"><html><head><title>DNS Tool html report</title>");
			report+=_T("<style type=\"text/css\">	  .table {	clear:both;	border: 1px solid #000000; ");
			report+=_T("margin-top: 5px; margin-bottom: 5px;	background: #cccccc; border-spacing: 1px; ");
			report+=_T("	font-size: 12px;	font-family:\"Verdana\";	} ");
			report+=_T(" .first_tr {	background: #99ccff;	font-weight: bold;	height:25px;	font-size: 13px; } ");
			report+=_T(" td.first_td {	border: 1px solid #6699ff;	padding-left: 5px;	padding-right: 5px; } ");
			report+=_T(" td.left {	text-align: left;	padding-left: 5px;	padding-right: 5px; } ");
			report+=_T(" .tr_1 { background: #ffffff; } .tr_2 {	background: #eeeeee; } ");
			report+=_T(" .main2 {	border: 1px solid #333333;	background: #ddeeff; font-size: 1.25em;	text-align: left;	padding-left: 20px; margin-top: 30px; } ");
			report+=_T("</style></head><body>");

			report+= _T("<a name=\"top\"></a><h1>DNS Tool html report.</h1>");
			report+= _T("<h3><a href=\"#1\">1. Start</a></h3>");
			report+= _T("<h3><a href=\"#2\">2. TLD</a></h3>");
			report+= _T("<h3><a href=\"#3\">3. WEB</a></h3>");
			report+= _T("<h3><a href=\"#4\">4. MX</a></h3>");
			report+= _T("<h3><a href=\"#5\">5. NS</a></h3>");
			report+= _T("<h3><a href=\"#6\">6. A-Records</a></h3>");
			report+= _T("<h3><a href=\"#7\">7. DNS Config</a></h3>");
			report+= _T("<br><br>");

			report+= _T("<a name=\"1\"></a><div class=\"main2\">1. Start</div><a href=\"#top\">top</a>");
			report+= _T("<p>Main domain: ")+Dlg1->domain_name;
			report+= _T("</p><p>IP Ranges<br>From: ")+Dlg1->from_1+_T(".")+Dlg1->from_2+_T(".")+Dlg1->from_3+_T(".")+Dlg1->from_4
					+_T("<br>To: ")+Dlg1->to_1+_T(".")+Dlg1->to_2+_T(".")+Dlg1->to_3+_T(".")+Dlg1->to_4;
			report+= _T("</p><p>Domains: </p><table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Domain</td><td class=\"first_td\">Ip</td></tr> ");

			for(int i=0;i<Dlg1->domains_from_ranges.GetItemCount();i++)
			{
				report+= _T("<tr class=\"tr_")+IntToStr(i%2+1)+_T("\"><td class=\"left\">")+Dlg1->domains_from_ranges.GetItemText(i,0)+_T("</td><td class=\"left\">")+Dlg1->domains_from_ranges.GetItemText(i,1)+_T("</td></tr>");
			}

			report+= _T("</table><p>Domains from shared hosting: </p><table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Domain</td><td class=\"first_td\">Ip</td></tr> ");

			for(int i=0;i<Dlg1->domains_from_domain.GetItemCount();i++)
			{
				report+= _T("<tr class=\"tr_")+IntToStr(i%2+1)+_T("\"><td class=\"left\">")+Dlg1->domains_from_domain.GetItemText(i,0)+_T("</td><td class=\"left\">")+Dlg1->domains_from_domain.GetItemText(i,1)+_T("</td></tr>");
			}

			report+= _T("</table><a name=\"2\"></a><div class=\"main2\">2. TLD</div><a href=\"#top\">top</a>");
			report+= _T("<table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">TLD</td><td class=\"first_td\">Institution</td><td class=\"first_td\">Holder</td><td class=\"first_td\">Country</td></tr> ");
	
			for(int i=0;i<Dlg2->table.GetItemCount();i++)
			{
				report+= _T("<tr class=\"tr_")+IntToStr(i%2+1)+_T("\"><td class=\"left\">")+Dlg2->table.GetItemText(i,0)+_T("</td><td class=\"left\">")+Dlg2->table.GetItemText(i,1)+_T("</td><td class=\"left\">")+Dlg2->table.GetItemText(i,2)+_T("</td><td class=\"left\">")+Dlg2->table.GetItemText(i,3)+_T("</td></tr>");
			}

			report+= _T("</table><a name=\"3\"></a><div class=\"main2\">3. WEB</div><a href=\"#top\">top</a>");
			report+= _T("<table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Title</td><td class=\"first_td\">Header</td><td class=\"first_td\">IP</td></tr> ");
	
			for(int i=0;i<Dlg3->table.GetItemCount();i++)
			{
				report+= _T("<tr class=\"tr_")+IntToStr(i%2+1)+_T("\"><td class=\"left\">")+Dlg3->table.GetItemText(i,0)+_T("</td><td class=\"left\">")+Dlg3->table.GetItemText(i,1)+_T("</td><td class=\"left\">")+Dlg3->table.GetItemText(i,2)+_T("</td></tr>");
			}

			report+= _T("</table><a name=\"4\"></a><div class=\"main2\">4. MX</div><a href=\"#top\">top</a>");
			report+= _T("<table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Domain</td><td class=\"first_td\">Mail server(s)</td><td class=\"first_td\">IP</td></tr> ");

			CString dom2 = _T("");
			for(int i=0;i<Dlg4->table.GetItemCount();i++)
			{
				dom2 = Dlg4->table.GetItemText(i,0);
				report+= _T("<tr class=\"tr_")+IntToStr(i%2+1)+_T("\"><td class=\"left\">")+dom2+_T("</td><td class=\"left\">")+Dlg4->table.GetItemText(i,1)+_T("</td><td class=\"left\">")+Dlg4->table.GetItemText(i,2)+_T("</td></tr>");
			}

			report+= _T("</table><a name=\"5\"></a><div class=\"main2\">5. NS</div><a href=\"#top\">top</a>");
			report+= _T("<table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Domain</td><td class=\"first_td\">Name server(s)</td><td class=\"first_td\">IP</td><td class=\"first_td\">AXFR</td></tr> ");

			dom2 = _T("");
	
			for(int i=0;i<Dlg5->table.GetItemCount();i++)
			{
				dom2 = Dlg5->table.GetItemText(i,0);
				report+= _T("<tr class=\"tr_")+IntToStr(i%2+1)+_T("\"><td class=\"left\">")+dom2+_T("</td><td class=\"left\">")+Dlg5->table.GetItemText(i,1)+_T("</td><td class=\"left\">")+Dlg5->table.GetItemText(i,2)+_T("</td><td class=\"left\">")+Dlg5->table.GetItemText(i,3)+_T("</td></tr>");
			}

			report+= _T("</table><a name=\"6\"></a><div class=\"main2\">6. A-Records</div><a href=\"#top\">top</a>");
			report+= _T("<p>Find subdomains by search engine: ");
			if(Dlg6->find1==TRUE)
				report+=_T("YES</p>");
			else
				report+=_T("NO</p>");
			report+= _T("<p>Find subdomains via Dictionary query: ");
			if(Dlg6->find2==TRUE)
				report+=_T("YES</p>");
			else
				report+=_T("NO</p>");
			report+= _T("<p>Find subdomains via Brute Force: ");
			if(Dlg6->find3==TRUE)
				report+=_T("YES</p>");
			else
				report+=_T("NO</p>");
			
			report+= _T("<p>Results:</p>");
			report+= _T("<table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Subdomain</td><td class=\"first_td\">IP</td></tr> ");
	
			for(int i=0;i<Dlg6->table.GetItemCount();i++)
			{
				report+= _T("<tr class=\"tr_")+IntToStr(i%2+1)+_T("\"><td class=\"left\">")+Dlg6->table.GetItemText(i,0)+_T("</td><td class=\"left\">")+Dlg6->table.GetItemText(i,1)+_T("</td></tr>");
			}

			report+= _T("</table><a name=\"7\"></a><div class=\"main2\">7. DNS Config</div><a href=\"#top\">top</a>");

			report+= _T("<table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Test</td><td class=\"first_td\">Result</td></tr> ");

			report+=_T("<tr class=\"tr_1\"><td class=\"left\">1.DNS servers have A records</td><td>")+Dlg7->res_1+_T("</td></tr>");
			report+=_T("<tr class=\"tr_2\"><td class=\"left\">2.No CNAMEs for domain</td><td>")+Dlg7->res_2+_T("</td></tr>");
			report+=_T("<tr class=\"tr_1\"><td class=\"left\">3.No NSs with CNAMEs</td><td>")+Dlg7->res_3+_T("</td></tr>");
			report+=_T("<tr class=\"tr_2\"><td class=\"left\">4.Number of nameservers</td><td>")+Dlg7->res_4+_T("</td></tr>");
			report+=_T("<tr class=\"tr_1\"><td class=\"left\">5.Number of mailservers</td><td>")+Dlg7->res_5+_T("</td></tr>");
			report+=_T("<tr class=\"tr_2\"><td class=\"left\">6.All NS IPs public</td><td>")+Dlg7->res_6+_T("</td></tr>");
			report+=_T("<tr class=\"tr_1\"><td class=\"left\">7.Reverse DNS entries for MX records</td><td>")+Dlg7->res_7+_T("</td></tr>");
			report+=_T("<tr class=\"tr_2\"><td class=\"left\">8.Reverse DNS entries for NS records</td><td>")+Dlg7->res_8+_T("</td></tr>");

			report+= _T("</table><a href=\"#top\">top</a><h2>End of report</h2></body>");

			//convert to cstr
			char* ta = StringToChar(report);

			// save
			FILE * pFile;
			int result;
			pFile = fopen(StringToChar(path), "w");
			if(pFile!=NULL)
			{
				result = fputs((const char*)ta,pFile);
				fclose(pFile);
			}

			MessageBox(_T("Html report succesfully saved."));
		}


	}
}
