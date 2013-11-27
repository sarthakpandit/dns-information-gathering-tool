
#include "Shared.h"

CString PTRRev(CString ip)
{
	CString a,b,c,d;
	int pos1 = 0;
	int pos2 = 0;

	char* ipc = StringToChar(ip);
	pos2 = FindStr(ipc,".",0);

	a = CharToString(MidStr(ipc,0,pos2));

	pos1 = pos2+1;
	pos2 = FindStr(ipc,".",pos1);

	b = CharToString(MidStr(ipc,pos1,pos2));

	pos1 = pos2+1;
	pos2 = FindStr(ipc,".",pos1);

	c = CharToString(MidStr(ipc,pos1,pos2));


	d = CharToString(MidStr(ipc,pos2+1,strlen(ipc)));

	CString result;
	result = d+_T(".")+c+_T(".")+b+_T(".")+a+_T(".in-addr.arpa");

	return result;

		//d c b a arpa
}

void TestPTR(CDNSToolDlg*	Dlg1)
{
	PDNS_RECORD pdns;
		DNS_FREE_TYPE freetype = DnsFreeRecordList;

		Dlg1->status = _T("Getting domains from PTR...");
		Dlg1->UpdateData(FALSE);

		CString ip, temp, ip2;
		int a,a_end,b,b_end,c,c_end,d,d_end;
		a=_wtoi(Dlg1->from_1);
		a_end=_wtoi(Dlg1->to_1);
		b=_wtoi(Dlg1->from_2);
		b_end=_wtoi(Dlg1->to_2);
		c=_wtoi(Dlg1->from_3);
		c_end=_wtoi(Dlg1->to_3);
		d=_wtoi(Dlg1->from_4);
		d_end=_wtoi(Dlg1->to_4);
		unsigned short trans_id = 1;
		char hi, low;
		int cursor;
		char* cur;
		while(true)
		{
			if(Dlg1->request_cancel)
			{
				Dlg1->status = _T("Canceled.");
				Dlg1->UpdateData(FALSE);
				break;
			}

			temp.Format(_T("%d"),d);
			ip = temp+_T(".");
			temp.Format(_T("%d"),c);
			ip+= temp+_T(".");
			temp.Format(_T("%d"),b);
			ip+= temp+_T(".");
			temp.Format(_T("%d"),a);
			ip+= temp+_T(".");
			ip+=_T("in-addr.arpa");

			temp.Format(_T("%d"),a);
			ip2 = temp+_T(".");
			temp.Format(_T("%d"),b);
			ip2+= temp+_T(".");
			temp.Format(_T("%d"),c);
			ip2+= temp+_T(".");
			temp.Format(_T("%d"),d);
			ip2+= temp;

			int res = DnsQuery(ip,  DNS_TYPE_PTR,DNS_QUERY_STANDARD, NULL, &pdns, 0);
			
			if(res==ERROR_SUCCESS)
			{
				CString result = pdns->Data.PTR.pNameHost;
				int itam = Dlg1->domains_from_ranges.InsertItem(Dlg1->domains_from_ranges.GetItemCount(),ip2);
				Dlg1->domains_from_ranges.SetItemText(itam,1,result);
			}

			if((a==a_end)&&(b==b_end)&&(c==c_end)&&(d==d_end))
				break;
			d++;
			if(d==256)
			{
				c++;
				d = 0;
			}
			if(c==256)
			{
				b++;
				c = 0;
			}
			if(b==256)
			{
				a++;
				b = 0;
			}
			if(a==256)
			{
				break;
			}

			 trans_id++;
		}
		//Dlg1->UpdateData(FALSE);
		if(pdns)
		{
			DnsRecordListFree(pdns, freetype);
			pdns = NULL;
		}

		// from domain shared hosting

	if(!(Dlg1->request_cancel))
	{

		Dlg1->status = _T("Getting domains from shared hosting...");
		Dlg1->UpdateData(FALSE);

		curl_global_init(CURL_GLOBAL_ALL);
		CURL* curl = curl_easy_init();

		thread_status* this_struct = new thread_status;

		this_struct->bufferlen = 0;
		this_struct->initialized = false;
		this_struct->data = NULL;
		this_struct->writepos = 0;

		curl_easy_setopt(curl,CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.2.8) Gecko/20100722 Firefox/3.6.8");

		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, writefunction);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA, (void *) &this_struct);

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 13);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15);

		NewCookie(curl);

		CURLcode result2;

		CString url = _T("http://serversniff.net/hip-");
		url+=Dlg1->domain_name;
		SetCurlUrl(curl,url);
		CleanData(this_struct);

		result2 = curl_easy_perform(curl);

		int pos1 = 0, pos2 = 0;
		int tab1 = 0, tab2 = 0;
		int i = 0;
		CString dom;

		if(this_struct->data)
		{
			tab1 = FindStr(this_struct->data,"<p>",0);
			tab1 = FindStr(this_struct->data,"<table>",tab1);
			tab2 = FindStr(this_struct->data,"</table>",tab1);
			i = 0;
			pos1 = 0;
			pos2 = 0;

			while(true)
			{
				if(Dlg1->request_cancel)
				{
					Dlg1->status = _T("Canceled.");
					Dlg1->UpdateData(FALSE);
					break;
				}

				if(i>_wtoi(Dlg1->number_of_domains))
					break;
				pos1 = FindStr(this_struct->data,"<tr>",pos2);
				if(pos1==-1)
					break;
				
				if(i>0)
				{
					pos1 = FindStr(this_struct->data,"<b>",pos1);
					pos2 = FindStr(this_struct->data,"</b>",pos1);
					dom = CharToString(MidStr(this_struct->data,pos1+1+3,pos2-1));
					int itam = Dlg1->domains_from_domain.InsertItem(Dlg1->domains_from_domain.GetItemCount(),_T("..."));
					if(dom.Mid(0,4)==_T("www."))
						dom = dom.Mid(4,999);
					Dlg1->domains_from_domain.SetItemText(itam,1,dom);
				}

				pos2 = FindStr(this_struct->data,"</tr>",pos1);
				i++;
			}
		}

		delete[] this_struct->data;
		curl_easy_cleanup(curl);
		curl_global_cleanup();

	}

	CString host, ipa, ipb;
	char* ac;
	unsigned char baw;
	int a2 = Dlg1->domains_from_domain.GetItemCount();
	int res;

	pdns = NULL;

	if(!(Dlg1->request_cancel))
	{
		for(int xx=0;xx<a2;xx++)
		{
			
			if(Dlg1->request_cancel)
			{
				Dlg1->status = _T("Canceled.");
				Dlg1->UpdateData(FALSE);
				break;
			}

			host = Dlg1->domains_from_domain.GetItemText(xx,1);

			res = DnsQuery(host,  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);
			
			if(res==ERROR_SUCCESS)
			{
				if(pdns->wType==DNS_TYPE_A)
				{
					ac = (char*)(&(pdns->Data.A.IpAddress));
					ipa = _T("");
					for(int hq=0;hq<4;hq++)
					{
						baw = ac[hq];//+128;
						ipb.Format(_T("%d"),baw);
						ipa+=ipb;
						if(hq<3)
							ipa+=_T(".");
					}
					Dlg1->domains_from_domain.SetItemText(xx,0,ipa);
				}
			}
			else
			{
				Dlg1->domains_from_domain.SetItemText(xx,0,_T("-"));
			}

			if(pdns)
			{
				DnsRecordListFree(pdns, freetype);
				pdns = NULL;
			}
		}
	}

	if(!Dlg1->request_cancel)
	{
		Dlg1->status = _T("Finished.");
		Dlg1->UpdateData(FALSE);
	}

	Dlg1->but_start.SetWindowTextW(_T("Restart"));
	Dlg1->test_running = false;
	Dlg1->request_cancel = false;
}

void TestTLD(CDNSToolDlg* Dlg1,DlgTLD* Dlg2)
{
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();

	thread_status* this_struct = new thread_status;

	this_struct->bufferlen = 0;
	this_struct->initialized = false;
	this_struct->data = NULL;
	this_struct->writepos = 0;

	curl_easy_setopt(curl,CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.2.8) Gecko/20100722 Firefox/3.6.8");

	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, writefunction);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA, (void *) &this_struct);

	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 13);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15);

	NewCookie(curl);

	// a

	CString aDOMAIN;
	char* dom_tem;// = StringToChar(Dlg1->domain_name);
	int posA;// = FindStr(dom_tem,".",0);
	/*
	if(posA==-1)
		aDOMAIN = CharToString(dom_tem);
	else
		aDOMAIN = CharToString(MidStr(dom_tem,0,posA));
*/
	CURLcode result2;

	int pos1=0,pos2=0;

	CString url;


	int b_domains_num = 0;
	CString b_domains[1000];
	CString b_domain;

	b_domains[0] = Dlg1->domain_name;
	b_domains_num++;

	{
		int total;
		CString domain;

		total = Dlg1->domains_from_ranges.GetItemCount();

		for(int bb=0;bb<total;bb++)
		{
			if(Dlg1->select_all_domain==FALSE)
				if(Dlg1->domains_from_ranges.GetCheck(bb)==0)
					continue;

			b_domain = Dlg1->domains_from_ranges.GetItemText(bb,1);
			//table.InsertItem(table.GetItemCount(),domain);
			b_domains[b_domains_num++] = b_domain;
		}

		total = Dlg1->domains_from_domain.GetItemCount();

		for(int bb=0;bb<total;bb++)
		{
			if(Dlg1->select_all_domain2==FALSE)
				if(Dlg1->domains_from_domain.GetCheck(bb)==0)
					continue;

			b_domain = Dlg1->domains_from_domain.GetItemText(bb,1);
			//table.InsertItem(table.GetItemCount(),domain);
			b_domains[b_domains_num++] = b_domain;
		}

	}
	
	char* posta;
	CString ag3;

	if((Dlg2->d_generic)||(Dlg2->d_asia)||(Dlg2->d_oceania)||(Dlg2->d_europe)||(Dlg2->d_americas)||(Dlg2->d_africa))
	{
		for(int nn=0;nn<b_domains_num;nn++)
		{
			if(Dlg2->request_cancel)
			{
				Dlg2->status = _T("Canceled.");
				Dlg2->UpdateData(FALSE);
				break;
			}

			dom_tem = StringToChar(b_domains[nn]);
			posA = FindStr(dom_tem,".",0);
			if(posA==-1)
				aDOMAIN = CharToString(dom_tem);
			else
				aDOMAIN = CharToString(MidStr(dom_tem,0,posA));

			url = _T("http://www.eurodns.com/domain-name-search");
			SetCurlUrl(curl,url);
			CleanData(this_struct);
			Dlg2->status = _T("Getting domains...[1]");
			Dlg2->UpdateData(FALSE);
			result2 = curl_easy_perform(curl);
			Sleep(500);

			CString postf;
			postf = _T("data%5BDomainSearch%5D%5BregGen%5D=0&data%5BDomainSearch%5D%5BregGen%5D=1&data%5BDomainSearch%5D%5BregAsia%5D=0&data%5BDomainSearch%5D%5BregAeu%5D=0&data%5BDomainSearch%5D%5BregEu%5D=0&data%5BDomainSearch%5D%5BregEu%5D=1&data%5BDomainSearch%5D%5BregAmericas%5D=0&data%5BDomainSearch%5D%5BregAfrica%5D=0&data%5BDomainSearch%5D%5BregPrereg%5D=1&data%5BDomainSearch%5D%5Bdomain%5D=");
			// start
			postf = _T("data%5BDomainSearch%5D%5BregGen%5D=0");
			if(Dlg2->d_generic)
				postf+=_T("&data%5BDomainSearch%5D%5BregGen%5D=1");
			postf+= _T("&data%5BDomainSearch%5D%5BregAsia%5D=0");
			if(Dlg2->d_asia)
				postf+=_T("&data%5BDomainSearch%5D%5BregAsia%5D=1");
			postf+= _T("&data%5BDomainSearch%5D%5BregAeu%5D=0");
			if(Dlg2->d_oceania)
				postf+=_T("&data%5BDomainSearch%5D%5BregAeu%5D=1");
			postf+= _T("&data%5BDomainSearch%5D%5BregEu%5D=0");
			if(Dlg2->d_europe)
				postf+=_T("&data%5BDomainSearch%5D%5BregEu%5D=1");
			postf+= _T("&data%5BDomainSearch%5D%5BregAmericas%5D=0");
			if(Dlg2->d_americas)
				postf+=_T("&data%5BDomainSearch%5D%5BregAmericas%5D=1");
			postf+= _T("&data%5BDomainSearch%5D%5BregAfrica%5D=0");
			if(Dlg2->d_africa)
				postf+=_T("&data%5BDomainSearch%5D%5BregAfrica%5D=1");
			postf+=_T("&data%5BDomainSearch%5D%5BregPrereg%5D=1&data%5BDomainSearch%5D%5Bdomain%5D=");
			postf+= aDOMAIN; // KEYWORD / DOMAIN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			postf+= _T("&data%5BDomainSearch%5D%5Bmode%5D=search&data%5BDomainSearch%5D%5Blayout%5D=");
			posta = StringToChar(postf);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, posta);

			int ag4 = postf.GetLength();
			
			ag3.Format(_T("%d"),ag4);
			char* con_len = StringToChar(_T("Content-Length: ")+ag3);
			struct curl_slist *headers=NULL;
			headers = curl_slist_append(headers, "Host: www.eurodns.com");
			headers = curl_slist_append(headers, "Connection: keep-alive");
			headers = curl_slist_append(headers, con_len); // 454+len_of_domain
			headers = curl_slist_append(headers, "Origin: http://www.eurodns.com");
			headers = curl_slist_append(headers, "X-Prototype-Version: 1.6.1");
			headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
			headers = curl_slist_append(headers, "X-Update: searchResultsDiv");
			headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.2.8) Gecko/20100722 Firefox/3.6.8");
			headers = curl_slist_append(headers, "Content-type: application/x-www-form-urlencoded; charset=UTF-8");
			headers = curl_slist_append(headers, "Accept: text/javascript, text/html, application/xml, text/xml, */*");
			headers = curl_slist_append(headers, "Referer: http://www.eurodns.com/domain-name-search/");
			headers = curl_slist_append(headers, "Accept-Encoding: gzip,deflate,sdch");
			headers = curl_slist_append(headers, "Accept-Language: pl-PL,pl;q=0.8,en-US;q=0.6,en;q=0.4");
			headers = curl_slist_append(headers, "Accept-Charset: ISO-8859-2,utf-8;q=0.7,*;q=0.3");
			//headers = curl_slist_append(headers, "Cookie: EuroDNS_V2=lrd956nmtl4rm8h35f15aep153; __utma=1.1064467309.1322370280.1322532452.1322538657.6; __utmb=1.1.10.1322538657; __utmc=1; __utmz=1.1322499920.4.2.utmcsr=google|utmccn=(organic)|utmcmd=organic|utmctr=eurodns");
			headers = curl_slist_append(headers, "");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

			url = _T("http://www.eurodns.com/tlds/searchResults");
			SetCurlUrl(curl,url);
			CleanData(this_struct);
			Dlg2->status = _T("Getting domains...[2]");
			Dlg2->UpdateData(FALSE);
			result2 = curl_easy_perform(curl);
			curl_easy_setopt(curl, CURLOPT_POST, 0);
			curl_slist_free_all(headers);

			CString domai, statu;

			if(/*(result2==CURLE_OK)&&(*/this_struct->data!=NULL)//)
			{
				Sleep(1000);

				headers = NULL;
				headers = curl_slist_append(headers, "Host: www.eurodns.com");
				headers = curl_slist_append(headers, "Connection: keep-alive");
				headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.2.8) Gecko/20100722 Firefox/3.6.8");
				headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
				headers = curl_slist_append(headers, "Referer: http://www.eurodns.com/domain-name-search/");
				headers = curl_slist_append(headers, "Accept-Encoding: gzip,deflate,sdch");
				headers = curl_slist_append(headers, "Accept-Language: pl-PL,pl;q=0.8,en-US;q=0.6,en;q=0.4");
				headers = curl_slist_append(headers, "Accept-Charset: ISO-8859-2,utf-8;q=0.7,*;q=0.3");
				headers = curl_slist_append(headers, "");
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
				url = _T("http://www.eurodns.com/tlds/searchFrame/");
				url+=aDOMAIN; //keyword!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				url+=_T("/");
				if(Dlg2->d_generic)
					url+=_T("GEN,");
				if(Dlg2->d_asia)
					url+=_T("ASIA,");
				if(Dlg2->d_oceania)
					url+=_T("AEU,");
				if(Dlg2->d_europe)
					url+=_T("EU,");
				if(Dlg2->d_americas)
					url+=_T("AMERICAS,");
				if(Dlg2->d_africa)
					url+=_T("AFRICA,");
				
				url+=_T("PREREG/");

				//curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING,"gzip,deflate,sdch");
				SetCurlUrl(curl,url);
				CleanData(this_struct);
				Dlg2->status = _T("Getting domains...[3]");
				Dlg2->UpdateData(FALSE);
				result2 = curl_easy_perform(curl);
				curl_slist_free_all(headers);
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, NULL);

				if(/*(result2==CURLE_OK)&&(*/this_struct->data!=NULL)//))
				{
					//MessageBox(_T("OK"));

					pos2 = 0;
					while(true)
					{
						pos1 = FindStr(this_struct->data,"\n",pos2);
						if(pos1==-1)
							break;
						pos1++;
						pos2 = FindStr(this_struct->data," ",pos1);
						if(pos2==-1)
							break;
						domai = CharToString(MidStr(this_struct->data,pos1,pos2));
						pos1 = pos2+1;
						pos2 = FindStr(this_struct->data,"<",pos1);
						statu = CharToString(MidStr(this_struct->data,pos1,pos2));

						if(statu==_T("taken"))
						{
							Dlg2->table.InsertItem(Dlg2->table.GetItemCount(),domai);
						}
					}
				}
				else
					Dlg2->MessageBox(_T("Error: 101"));
			}
		}

	}

	// WHOIS

	CString domaina, org, holder, country;

	int ag = Dlg2->table.GetItemCount();

	//CURLcode result2;

	pos1 = 0;
	pos2=0;
	int pos3=0, pos4=0, pos5= 0, pos6 = 0, pos7  = -1, pos2andhalf=0, pos8 = 0, pos9=0, pos10, pos11 = 0;

	Dlg2->status = _T("Getting whois...");
	Dlg2->UpdateData(FALSE);

	char* tehe = NULL;
	CString ragea;
	int aegaeg;

	for(int a=0;a<ag;a++)
	{
		Sleep(250);

		domaina = Dlg2->table.GetItemText(a,0);
/*
		NewCookie(curl);

		aegaeg = rand();
		ragea.Format(_T("%d"),aegaeg);
		tehe = StringToChar(_T("User-Agent: Mozilla/5.0 (Windows NT 5.1; en; rv:2.0) Gecko/20100101 Firefox/5.0 Opera 12.60 ")+ragea);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, tehe);
*/
		//url = _T("http://www.iana.org/cgi-bin/whois?q=");
		//url = _T("http://www.whois.net/whois/");
		//url = _T("http://who.is/whois/");
		url = _T("http://tools.vlddb.org/api/whois/");
		url+=domaina;
		//url+=_T("/");
		SetCurlUrl(curl,url);
		CleanData(this_struct);
		result2 = curl_easy_perform(curl);

		if(this_struct->data)
		{
			//pos3 = FindStr(this_struct->data,"WHOIS information for",0);
			pos3 = 0;

			pos1 = FindStr(this_struct->data,"Registrant:",pos3);
			pos4 = FindStr(this_struct->data,"MarkMonitor",pos3);

			pos5 = FindStr(this_struct->data,"PSI-USA",pos3);

			pos6 = FindStr(this_struct->data,"Registrant Name:",pos3);

			//pos7 = FindStr(this_struct->data,"You have submitted too many queries.",pos3);
			pos7 = -1;
			//pos5 = -1;

			pos8 = FindStr(this_struct->data,"NOT DISCLOSED!",pos3);

			pos9 = FindStr(this_struct->data,"Holder of domain name:",pos3);

			pos10 = FindStr(this_struct->data,"owner-fname:",pos3);

			pos11 = FindStr(this_struct->data,"Registrant:<br>",pos3);

			if((pos1!=-1)&&(pos4!=-1))
			{
				pos1 = FindStr(this_struct->data," ",pos1);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					holder = CharToString(MidStr(this_struct->data,pos1+8,pos2));
				else holder = _T("?");

				pos1 = FindStr(this_struct->data," ",pos2);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					org = CharToString(MidStr(this_struct->data,pos1+8,pos2));
				else org = _T("?");

				pos1 = FindStr(this_struct->data," ",pos2);
				pos2 = FindStr(this_struct->data,"<br>",pos1);

				pos1 = FindStr(this_struct->data," ",pos2);
				pos2 = FindStr(this_struct->data,"<br>",pos1);

				pos1 = FindStr(this_struct->data," ",pos2);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					country = CharToString(MidStr(this_struct->data,pos1+8,pos2));
				else country = _T("?");
			}
			else if(pos5!=-1)
			{
				pos1 = FindStr(this_struct->data,"[owner-c] fname:",pos3);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					holder = CharToString(MidStr(this_struct->data,pos1+29,pos2));
				else holder = _T("?");

				pos1 = FindStr(this_struct->data,"[owner-c] lname:",pos1);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					holder+=_T(" ")+CharToString(MidStr(this_struct->data,pos1+29,pos2));

				pos1 = FindStr(this_struct->data,"[owner-c] org:",pos1);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					org = CharToString(MidStr(this_struct->data,pos1+29,pos2));
				else org = _T("?");

				pos1 = FindStr(this_struct->data,"[owner-c] country:",pos1);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					country = CharToString(MidStr(this_struct->data,pos1+29,pos2));
				else country = _T("?");
			}
			else if(pos10!=-1)
			{
				pos1 = FindStr(this_struct->data,"owner-fname:",pos3);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					holder = CharToString(MidStr(this_struct->data,pos1+29,pos2));
				else holder = _T("?");

				pos1 = FindStr(this_struct->data,"owner-lname:",pos1);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					holder+=_T(" ")+CharToString(MidStr(this_struct->data,pos1+29,pos2));

				pos1 = FindStr(this_struct->data,"owner-org:",pos1);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					org = CharToString(MidStr(this_struct->data,pos1+29,pos2));
				else org = _T("-");

				pos1 = FindStr(this_struct->data,"owner-country:",pos1);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1)&&(pos2!=-1))
					country = CharToString(MidStr(this_struct->data,pos1+29,pos2));
				else country = _T("?");
			}
			else if(pos6!=-1)
			{
				int a1 = 16, a2 = 24, a3 = 19;
				if(FindStr(this_struct->data,"Registrant Name:                             ",0)!=-1)
					a1 = a2 = a3 = 45;

				pos1 = FindStr(this_struct->data,"Registrant Name:",pos3);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				pos2andhalf = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1))
				{
					if(pos2==-1)
					{
						holder = CharToString(MidStr(this_struct->data,pos1+a1,pos2andhalf));
						pos2 = pos2andhalf;
					}
					else if(pos2andhalf==-1)
						holder = CharToString(MidStr(this_struct->data,pos1+a1,pos2));
					else
					{
						holder = CharToString(MidStr(this_struct->data,pos1+a1,pos2<pos2andhalf?pos2:pos2andhalf));
						pos2 = pos2<pos2andhalf?pos2:pos2andhalf;
					}
				}
				else holder = _T("?");

				pos1 = FindStr(this_struct->data,"Registrant Organization:",pos2);
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				pos2andhalf = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1))
				{
					if(pos2==-1)
					{
						org = CharToString(MidStr(this_struct->data,pos1+a2,pos2andhalf));
						pos2 = pos2andhalf;
					}
					else if(pos2andhalf==-1)
						org = CharToString(MidStr(this_struct->data,pos1+a2,pos2));
					else
					{
						org = CharToString(MidStr(this_struct->data,pos1+a2,pos2<pos2andhalf?pos2:pos2andhalf));
						pos2 = pos2<pos2andhalf?pos2:pos2andhalf;
					}
				}
				else org = _T("?");

				int ggggg = 0;

				pos1 = FindStr(this_struct->data,"Registrant Country:",pos2);
				if(pos1==-1)
				{
					pos1 = FindStr(this_struct->data,"Registrant Country/Economy:",pos2);
					ggggg = 8;
				}
				pos2 = FindStr(this_struct->data,"<br>",pos1);
				pos2andhalf = FindStr(this_struct->data,"<br>",pos1);
				if((pos1!=-1))
				{
					if(pos2==-1)
						country = CharToString(MidStr(this_struct->data,pos1+a3+ggggg,pos2andhalf));
					else if(pos2andhalf==-1)
						country = CharToString(MidStr(this_struct->data,pos1+a3+ggggg,pos2));
					else
						country = CharToString(MidStr(this_struct->data,pos1+a3+ggggg,pos2<pos2andhalf?pos2:pos2andhalf));
				}
				else country = _T("?");
			}
			else if(pos9!=-1)
			{
				int poss = FindStr(this_struct->data,"<br> <br>",pos9);
				int nnum = 0;
				pos1 = pos9;
				while(true)
				{
					pos1 = FindStr(this_struct->data,"<br>",pos1+1);

					if(pos1>=poss)
						break;

					nnum++;
				}

				//6 == company
				//5 == invidual

				pos1 = FindStr(this_struct->data,"<br>",pos9);
				pos2 = FindStr(this_struct->data,"<br>",pos1+1);
				if((pos1!=-1)&&(pos2!=-1))
					holder = CharToString(MidStr(this_struct->data,pos1+4,pos2));
				else holder = _T("?");

				if(nnum==6)
				{
					pos1 = FindStr(this_struct->data,"<br>",pos2);
					pos2 = FindStr(this_struct->data,"<br>",pos1+1);
					if((pos1!=-1)&&(pos2!=-1))
						org = CharToString(MidStr(this_struct->data,pos1+4,pos2));
					else org = _T("?");
				}
				else org = _T("-");

				//org.Format(_T("%d"),nnum);

				pos1 = FindStr(this_struct->data,"<br>",pos2);
				pos2 = FindStr(this_struct->data,"<br>",pos1+1);

				pos1 = FindStr(this_struct->data,"<br>",pos2);
				pos2 = FindStr(this_struct->data,"<br>",pos1+1);

				pos1 = FindStr(this_struct->data,"<br>",pos2);
				pos2 = FindStr(this_struct->data,"<br>",pos1+1);
				if((pos1!=-1)&&(pos2!=-1))
					country = CharToString(MidStr(this_struct->data,pos1+4,pos2));
				else country = _T("?");
			}
			else if(pos7!=-1)
			{
				org = _T("ERR");
				holder = _T("ERR");
				country = _T("ERR");
			}
			else if(pos8!=-1)
			{
				org = _T("[HIDDEN]");
				holder = _T("[HIDDEN]");
				country = _T("[HIDDEN]");
			}
			else if(pos11!=-1)
			{
				pos1 = FindStr(this_struct->data,"Registrant:<br>",0);
				pos2 = FindStr(this_struct->data,"<br>",pos1+17);
				if((pos1!=-1)&&(pos2!=-1))
				{
					char* agg = MidStr(this_struct->data,pos1+15,pos2);
					
					int be;
					for(be =0;be<pos2-pos1;be++)
						if(agg[be]!=' ')
							break;

					agg+=be;

					holder = CharToString(agg);
				}
				else holder = _T("?");

				country = _T("?");
				org = _T("?");
			}
			else
			{
				org = _T("?");
				holder = _T("?");
				country = _T("?");
			}
/*
			// ORGANIZATION
			pos1 = FindStr(this_struct->data,"Registrant Organization:",pos3);
			
			if(pos1==-1)
				org = _T("?");
			else
			{
				pos2 = FindStr(this_struct->data,"\r",pos1);
				org = CharToString(MidStr(this_struct->data,pos1+24,pos2));
				org.Replace(_T("&nbsp;"),_T(""));
			}

			// NAME
			pos1 = FindStr(this_struct->data,"Registrant Name:",pos3);
			
			if(pos1==-1)
				holder = _T("?");
			else
			{
				pos2 = FindStr(this_struct->data,"\r",pos1);
				holder = CharToString(MidStr(this_struct->data,pos1+16,pos2));
			}

			// COUNTRY
			pos1 = FindStr(this_struct->data,"Registrant Country:",pos3);
			
			if(pos1==-1)
				country = _T("?");
			else
			{
				pos2 = FindStr(this_struct->data,"\r",pos1);
				country = CharToString(MidStr(this_struct->data,pos1+19,pos2));
			}
*/
			/* // COUNTRY (OLD)
			pos1 = FindStr(this_struct->data,"address:      ",pos3);

			pos5 = pos1;
			while(true)
			{
				pos4 = FindStr(this_struct->data,"\n",pos5);
				pos5 = FindStr(this_struct->data,"address:      ",pos5+1);
				if(pos5==pos4+1)
					pos1 = pos5;
				else break;
			}

			pos2 = FindStr(this_struct->data,"\n",pos1);
			if(pos1==-1)
				country = _T("[ERROR]");
			else
				country = CharToString(MidStr(this_struct->data,pos1+14,pos2));
			*/
		}
		else
		{
			org = _T("[ERROR]");
			holder = _T("[ERROR]");
			country = _T("[ERROR]");
		}

		Dlg2->table.SetItemText(a,1,org);
		Dlg2->table.SetItemText(a,2,holder);
		Dlg2->table.SetItemText(a,3,country);
		Sleep(200);
		if(Dlg2->request_cancel)
		{
			Dlg2->status = _T("Canceled.");
			Dlg2->UpdateData(FALSE);
			break;
		}
	}

	if(!Dlg2->request_cancel)
	{
		Dlg2->status = _T("Finished.");
		Dlg2->UpdateData(FALSE);
	}

	delete[] this_struct->data;
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	Dlg2->but_start.SetWindowTextW(_T("Restart"));
	Dlg2->but_next.EnableWindow(TRUE);
	Dlg2->test_running = false;
	Dlg2->request_cancel = false;
}

void TestWeb(CDNSToolDlg* Dlg1,DlgTLD* Dlg2, CDlgWeb* Dlg3)
{
	curl_global_init(CURL_GLOBAL_ALL);
		CURL* curl = curl_easy_init();

		thread_status* this_struct = new thread_status;

		this_struct->bufferlen = 0;
		this_struct->initialized = false;
		this_struct->data = NULL;
		this_struct->writepos = 0;

		curl_easy_setopt(curl,CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.2.8) Gecko/20100722 Firefox/3.6.8");

		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, writefunction);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA, (void *) &this_struct);

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT,6);
		curl_easy_setopt(curl, CURLOPT_HEADER,1);

		//
		int total = Dlg2->table.GetItemCount();

		CString url, domain, title, header, tem;
		char* ip;
		CURLcode result;
		int cc=0;

		for(int bb=0;bb<total;bb++)
		{
			
			if(Dlg3->request_cancel)
			{
				Dlg3->status = _T("Canceled.");
				Dlg3->UpdateData(FALSE);
				break;
			}

			if(Dlg2->select_all==FALSE)
				if(Dlg2->table.GetCheck(bb)==0)
					continue;

			for(int b2=0;b2<2;b2++)
			{
				url = _T("http://");
				if(b2==1)
					url = _T("https://");
				domain = Dlg2->table.GetItemText(bb,0);
				url+=domain;

				SetCurlUrl(curl,url);
				CleanData(this_struct);
				result = curl_easy_perform(curl);
				
				title = _T("-");
				//int nIndex = Dlg3->table.InsertItem(Dlg3->table.GetItemCount(),title);
				if(/*(result == CURLE_OK)*/(this_struct->data))
				{
					int nIndex = Dlg3->table.InsertItem(Dlg3->table.GetItemCount(),title);
					curl_easy_getinfo(curl,CURLINFO_PRIMARY_IP,&ip);
					//
					Dlg3->table.SetItemText(nIndex,2,CharToString(ip));
					Dlg3->domains[cc++] = domain;

					int pos = -1, pos_end = -1;
					if(this_struct->data)
					{
						pos = FindStr(this_struct->data,"<title>",0);
						pos_end = FindStr(this_struct->data,"</title>",pos);
						if((pos!=-1)&&(pos_end!=-1))
						{
							title = CharToString(MidStr(this_struct->data,pos+7,pos_end));
							Dlg3->table.SetItemText(nIndex,0,title);
						}

						pos = 0;
						pos_end = FindStr(this_struct->data,"\r",pos);
						if((pos!=-1)&&(pos_end!=-1))
						{
							header = CharToString(MidStr(this_struct->data,pos+0,pos_end));
							Dlg3->table.SetItemText(nIndex,1,header);
						}
					}
					
					break;
				}
			}

			tem.Format(_T("%d"),bb+1);
			Dlg3->status = _T("Checked ")+tem;
			tem.Format(_T("%d"),total);
			Dlg3->status+= _T("/")+tem;
			Dlg3->UpdateData(FALSE);
		}

		if(!Dlg3->request_cancel)
		{
			Dlg3->status = _T("Finished.");
			Dlg3->UpdateData(FALSE);
		}

		delete[] this_struct->data;
		curl_easy_cleanup(curl);
		curl_global_cleanup();

		Dlg3->but_start.SetWindowTextW(_T("Restart"));
		Dlg3->but_next.EnableWindow(TRUE);
		Dlg3->test_running = false;
		Dlg3->request_cancel = false;
}

void TestArecs(CDNSToolDlg* Dlg1,DlgTLD* Dlg2, CDlgWeb* Dlg3, CDlgMX* Dlg4,CDlgNS* Dlg5,CArecsDlg* Dlg6)
{
		curl_global_init(CURL_GLOBAL_ALL);
		CURL* curl = curl_easy_init();

		thread_status* this_struct = new thread_status;

		this_struct->bufferlen = 0;
		this_struct->initialized = false;
		this_struct->data = NULL;
		this_struct->writepos = 0;

		curl_easy_setopt(curl,CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.2.8) Gecko/20100722 Firefox/3.6.8");

		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, writefunction);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA, (void *) &this_struct);

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT,10);
		curl_easy_setopt(curl, CURLOPT_HEADER,1);

		CString url, subdomain;

		CURLcode result;

		PDNS_RECORD pdns;
		DNS_FREE_TYPE freetype = DnsFreeRecordList;
		int res;

		if(Dlg6->find1&&(!Dlg6->request_cancel))
		{
			Dlg6->status = _T("[1] Finding domains by search engine...");
			Dlg6->UpdateData(FALSE);

			url = _T("http://searchdns.netcraft.com/?restriction=site+ends+with&host=.");
			url+=Dlg1->domain_name;
			url+=_T("&lookup=wait..&position=limited");

			int page = 1;
			int table_s = 0;
			int table_end = 0;
			int pos1 = 0, pos2 = 0, pos3;
			bool is_first = true;

			while(true) // pages
			{
				if(Dlg6->request_cancel)
					break;

				SetCurlUrl(curl,url);
				CleanData(this_struct);
				result = curl_easy_perform(curl);

				table_s = FindStr(this_struct->data,"class=\"TBtable\"",0);
				table_end = FindStr(this_struct->data,"</table>",table_s);
				pos1 = pos2 = table_s;

				while(true) // items
				{
					pos1 = FindStr(this_struct->data,"<td align=\"left\">\n",pos2);
					if((pos1==-1)||(pos1>table_end))
						break;
					pos1 = FindStr(this_struct->data,"\">",pos1+17);
					pos2 = FindStr(this_struct->data,"<",pos1);

					subdomain = CharToString(MidStr(this_struct->data,pos1+2,pos2))+_T(".")+Dlg1->domain_name;
					
					if(!is_first)
						Dlg6->table.InsertItem(Dlg6->table.GetItemCount(),subdomain);
					is_first = false;
				}

				if(this_struct->data==NULL)
					break;
				
				if(page==_wtoi(Dlg6->pages))
					break;
				
				pos3 = FindStr(this_struct->data,"</div>",table_end);
				pos1 = FindStr(this_struct->data,"href=\"",table_end);
				if((pos1==-1)||(pos1>pos3)) // if there are no more pages
					break;
				pos2 = FindStr(this_struct->data,"\">",pos1+6);
				url = _T("http://searchdns.netcraft.com")+CharToString(MidStr(this_struct->data,pos1+6,pos2));
				url.Replace(_T("*"),_T(""));

				page++;
			}

			// ROBTEX

			Dlg6->status = _T("[1] Finding domains by search engine[2]...");
			Dlg6->UpdateData(FALSE);

			url = _T("http://www.robtex.com/dns/");
			url+=Dlg1->domain_name;
			url+=_T(".html");

			/*int */table_s = 0;
			/*int */table_end = 0;
			/*int */pos1 = 0, pos2 = 0, pos3;
			//bool is_first = true;

			SetCurlUrl(curl,url);
			CleanData(this_struct);
			result = curl_easy_perform(curl);

			table_s = FindStr(this_struct->data,"<span id=\"sharedsub\">",0);
			table_end = FindStr(this_struct->data,"</div>",table_s);
			pos1 = pos2 = table_s;

			while(true) // items
			{
				pos1 = FindStr(this_struct->data,"<li>",pos2);
				if((pos1==-1)||(pos1>table_end))
					break;
				pos1 = FindStr(this_struct->data,"\">",pos1+17);
				pos2 = FindStr(this_struct->data,"<",pos1);

				subdomain = CharToString(MidStr(this_struct->data,pos1+2,pos2));//+_T(".")+Dlg1->domain_name;
				
				//if(!is_first)
					Dlg6->table.InsertItem(Dlg6->table.GetItemCount(),subdomain);
				//is_first = false;
			}

			// IP's

			CString host, ipa, ipb;
			char* ac;
			unsigned char baw;
			int a2 = Dlg6->table.GetItemCount();
			for(int xx=0;xx<a2;xx++)
			{
				host = Dlg6->table.GetItemText(xx,0);

				res = DnsQuery(host,  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);
				
				if(res==ERROR_SUCCESS)
				{
					ac = (char*)(&(pdns->Data.A.IpAddress));
					ipa = _T("");
					for(int hq=0;hq<4;hq++)
					{
						baw = ac[hq];//+128;
						ipb.Format(_T("%d"),baw);
						ipa+=ipb;
						if(hq<3)
							ipa+=_T(".");
					}
					Dlg6->table.SetItemText(xx,1,ipa);
				}
				else
					Dlg6->table.SetItemText(xx,1,_T("-"));

				if(pdns)
				{
					DnsRecordListFree(pdns, freetype);
					pdns = NULL;
				}
			}
		}

		if(Dlg6->find2&&(!Dlg6->request_cancel))
		{
			Dlg6->status = _T("[2] Finding domains via dictionary...");
			Dlg6->UpdateData(FALSE);

			CComboBox* com = (CComboBox*)Dlg6->GetDlgItem(IDC_COMBO3);
			
			IP4_ARRAY* ia = NULL;
			
			res = DnsQuery(Dlg6->com_ns,  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);
			if(res==ERROR_SUCCESS)
			{
				ia = new IP4_ARRAY;
				ia->AddrCount = 1;
				ia->AddrArray[0] = pdns->Data.A.IpAddress;
			}

			POSITION lpos = Dlg6->dict.GetHeadPosition();
			CString domain;
			int xx;
			CString ipa, ipb;
			char* ac;
			unsigned char baw;

			for(int a=0;a<Dlg6->dict.GetCount();a++)
			{
				if(Dlg6->request_cancel)
					break;

				domain = Dlg6->dict.GetNext(lpos);
				if((domain==_T("\r\n"))||(domain==_T("")))
					continue;

				domain+=_T(".")+Dlg1->domain_name;

				res = DnsQuery(domain,  DNS_TYPE_A,DNS_QUERY_STANDARD, ia, &pdns, 0);
				
				if(res==ERROR_SUCCESS)
				{
					while(true)
					{
						if((pdns->wType==DNS_TYPE_A)&&(pdns->pName==domain))
						{
							ac = (char*)(&(pdns->Data.A.IpAddress));
							ipa = _T("");
							for(int hq=0;hq<4;hq++)
							{
								baw = ac[hq];//+128;
								ipb.Format(_T("%d"),baw);
								ipa+=ipb;
								if(hq<3)
									ipa+=_T(".");
							}
							xx = Dlg6->table.InsertItem(Dlg6->table.GetItemCount(),domain);
							Dlg6->table.SetItemText(xx,1,ipa);
						}

						pdns = pdns->pNext;

						if(pdns==NULL)
							break;
					}
					
				}

				if(pdns)
				{
					DnsRecordListFree(pdns, freetype);
					pdns = NULL;
				}
			}
		}

		if(Dlg6->find3&&(!Dlg6->request_cancel))
		{
			Dlg6->status = _T("[3] Finding domains via brute force...");
			Dlg6->UpdateData(FALSE);

			CComboBox* com = (CComboBox*)Dlg6->GetDlgItem(IDC_COMBO3);

			IP4_ARRAY* ia = NULL;
			
			res = DnsQuery(Dlg6->com_ns,  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);
			if(res==ERROR_SUCCESS)
			{
				ia = new IP4_ARRAY;
				ia->AddrCount = 1;
				ia->AddrArray[0] = pdns->Data.A.IpAddress;
			}
			

			CString dat = _T("abcdefghijklmnopqrstuvwxyz-0123456789");

			int data_chars = (Dlg6->com_charset==_T("Alphabetical"))?27:37;
			int max_chars = _wtoi(Dlg6->com_chars);
			int cur_chars = 1;

			CString domain;
			int xx;
			CString ipa, ipb;
			char* ac;
			unsigned char baw;

			unsigned char cur_domain[8];
			memset(cur_domain,0,8);

			int total_real = 0;

			for(int c=1;c<=max_chars;c++)
			{
				int total = data_chars;
				for(int a=1;a<c;a++)
					total*=data_chars;

				total_real+=total;
			}
			// a co z Brakiem ?
			// lepiej zakanczyc petle RECZNIE

			//for(int i=0;i<total;i++)
			CString temp, temp2;
			int curs = 0;
			unsigned short display = 0;
			int percemt;
			while(true)
			{
				if(Dlg6->request_cancel)
					break;

				domain = _T("");
				for(int b=cur_chars-1;b>=0;b--)
					domain+=dat.Mid(cur_domain[b],1);

				domain+=_T(".")+Dlg1->domain_name;

				res = DnsQuery(domain,  DNS_TYPE_A,DNS_QUERY_STANDARD, ia, &pdns, 0);
				
				if(res==ERROR_SUCCESS)
				{
					while(true)
					{
						if((pdns->wType==DNS_TYPE_A)&&(pdns->pName==domain))
						{
							ac = (char*)(&(pdns->Data.A.IpAddress));
							ipa = _T("");
							for(int hq=0;hq<4;hq++)
							{
								baw = ac[hq];//+128;
								ipb.Format(_T("%d"),baw);
								ipa+=ipb;
								if(hq<3)
									ipa+=_T(".");
							}
							xx = Dlg6->table.InsertItem(Dlg6->table.GetItemCount(),domain);
							Dlg6->table.SetItemText(xx,1,ipa);
						}

						pdns = pdns->pNext;

						if(pdns==NULL)
							break;
					}
					
				}

				if(pdns)
				{
					DnsRecordListFree(pdns, freetype);
					pdns = NULL;
				}

				//Dlg6->table.InsertItem(Dlg6->table.GetItemCount(),domain);
				//Dlg6->table.SetItemText(xx,1,ipa);
				
				curs++;
				display++;
				
				if(display==100)
				{
					display = 0;
					temp2 = _T("[3] Finding domains by brute force... [");
					percemt = int((float(curs)*100.0/(float(total_real*1.0))));
					temp.Format(_T("%d"),percemt);
					temp2+=temp+_T("%]");
					/*
					temp.Format(_T("%d"),curs);
					temp2+=temp+_T("/");
					temp.Format(_T("%d"),total_real);
					temp2+=temp+_T("]");
					*/

					Dlg6->status = temp2;
					Dlg6->UpdateData(FALSE);
				}
				
				cur_domain[0]++;
				for(int b=0;b<cur_chars;b++)
					if(cur_domain[b]==data_chars)
					{
						if(b==cur_chars-1)
						{
							cur_chars++;
							memset(cur_domain,0,8);
							break;
						}
						else
						{
							cur_domain[b+1]++;
							cur_domain[b] = 0;
						}
					}

				if(cur_chars>max_chars)
					break;
			}
		}

		if(!Dlg6->request_cancel)
		{
			Dlg6->status = _T("Finished.");
			Dlg6->UpdateData(FALSE);
		}
		else
		{
			Dlg6->status = _T("Canceled.");
			Dlg6->UpdateData(FALSE);
		}

		delete[] this_struct->data;
		curl_easy_cleanup(curl);
		curl_global_cleanup();

		Dlg6->but_start.SetWindowTextW(_T("Restart"));
		Dlg6->but_next.EnableWindow(TRUE);
		Dlg6->test_running = false;
		Dlg6->request_cancel = false;
}

void TestConfig(CDNSToolDlg* Dlg1,DlgTLD* Dlg2, CDlgWeb* Dlg3, CDlgMX* Dlg4,CDlgNS* Dlg5,CArecsDlg* Dlg6,CConf* Dlg7)
{
	PDNS_RECORD pdns;
	DNS_FREE_TYPE freetype = DnsFreeRecordList;
	int res;
	CString domain = Dlg1->domain_name;

	CString ns[100];
	CString ns_ip[100];
	int ns_r = 0;
	bool cname = false;
	CString host, ipa, ipb;
	char* ac;
	unsigned char baw;

	if(true)
	{
		res = DnsQuery(domain,  DNS_TYPE_NS,DNS_QUERY_STANDARD, NULL, &pdns, 0);

		if(res==ERROR_SUCCESS)
		{
			while(true)
			{
				if((pdns->wType==DNS_TYPE_NS))
				{
					ns[ns_r++] = pdns->Data.NS.pNameHost;
				}
				else if((pdns->wType==DNS_TYPE_CNAME))
					cname = true;

				pdns = pdns->pNext;

				if(pdns==NULL)
					break;
			}
		}

		if(pdns)
		{
			DnsRecordListFree(pdns, freetype);
			pdns = NULL;
		}

		for(int i=0;i<ns_r;i++)
		{
			res = DnsQuery(ns[i],  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);

			if(res==ERROR_SUCCESS)
				{
					ac = (char*)(&(pdns->Data.A.IpAddress));
					ipa = _T("");
					for(int hq=0;hq<4;hq++)
					{
						baw = ac[hq];//+128;
						ipb.Format(_T("%d"),baw);
						ipa+=ipb;
						if(hq<3)
							ipa+=_T(".");
					}
					ns_ip[i] = ipa;
				}
		}

	}

	if(((Dlg7->select_1)||(Dlg7->select_all))&&(!Dlg7->request_cancel)) // select all!!!!!!!!!!!!!!!!!!!!!!!!!!!1
	{
		Dlg7->status = _T("Check [1] in progress...");
		Dlg7->UpdateData(FALSE);

		if(true)
		{

			bool re = true;

			for(int i=0;i<ns_r;i++)
			{
				res = DnsQuery(ns[i],  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);
				if((res==ERROR_SUCCESS)&&(pdns->wType==DNS_TYPE_A))
					;
				else
				{
					re = false;

					if(pdns)
					{
						DnsRecordListFree(pdns, freetype);
						pdns = NULL;
					}

					break;
				}

				if(pdns)
				{
					DnsRecordListFree(pdns, freetype);
					pdns = NULL;
				}
			}

			if(re)
				Dlg7->res_1 = _T("YES");
			else
				Dlg7->res_1 = _T("NO");
		}
		else
		{
			Dlg7->res_1 = _T("[ERR!]");
		}

		if(pdns)
		{
			DnsRecordListFree(pdns, freetype);
			pdns = NULL;
		}

		Dlg7->UpdateData(FALSE);
	}

	if(((Dlg7->select_2)||(Dlg7->select_all))&&(!Dlg7->request_cancel))
	{
		Dlg7->status = _T("Check [2] in progress...");
		Dlg7->UpdateData(FALSE);

		if((ns_r>0)&&cname)
			Dlg7->res_2 = _T("NO");
		else
			Dlg7->res_2 = _T("YES");

		Dlg7->UpdateData(FALSE);
	}

	if(((Dlg7->select_3)||(Dlg7->select_all))&&(!Dlg7->request_cancel))
	{
		Dlg7->status = _T("Check [3] in progress...");
		Dlg7->UpdateData(FALSE);

		bool are_cnames = false;

		for(int i=0;i<ns_r;i++)
		{
			res = DnsQuery(ns[i],  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);
			if(res==ERROR_SUCCESS)
			{
				while(true)
				{
					if((pdns->wType==DNS_TYPE_CNAME))
					{
						are_cnames = true;
					}

					pdns = pdns->pNext;

					if(pdns==NULL)
						break;
				}
			}

			if(pdns)
			{
				DnsRecordListFree(pdns, freetype);
				pdns = NULL;
			}
		}

		
		if(!are_cnames)
			Dlg7->res_3 = _T("YES");
		else
			Dlg7->res_3 = _T("NO");

		Dlg7->UpdateData(FALSE);
	}

	if(((Dlg7->select_4)||(Dlg7->select_all))&&(!Dlg7->request_cancel))
	{
		Dlg7->status = _T("Check [4] in progress...");
		Dlg7->UpdateData(FALSE);

		Dlg7->res_4.Format(_T("%d"),ns_r);

		Dlg7->UpdateData(FALSE);
	}

	if(((Dlg7->select_5)||(Dlg7->select_all))&&(!Dlg7->request_cancel))
	{
		Dlg7->status = _T("Check [5] in progress...");
		Dlg7->UpdateData(FALSE);

		int mx_r = 0;

		res = DnsQuery(domain,  DNS_TYPE_MX,DNS_QUERY_STANDARD, NULL, &pdns, 0);
		if(res==ERROR_SUCCESS)
		{
			while(true)
			{
				if((pdns->wType==DNS_TYPE_MX))
				{
					mx_r++;
				}

				pdns = pdns->pNext;

				if(pdns==NULL)
					break;
			}
		}

		if(pdns)
		{
			DnsRecordListFree(pdns, freetype);
			pdns = NULL;
		}

		Dlg7->res_5.Format(_T("%d"),mx_r);

		Dlg7->UpdateData(FALSE);
	}

	if(((Dlg7->select_6)||(Dlg7->select_all))&&(!Dlg7->request_cancel))
	{
		Dlg7->status = _T("Check [6] in progress...");
		Dlg7->UpdateData(FALSE);

		bool public_ns = true;
		CString curr;

		for(int i=0;i<ns_r;i++)
		{
			curr = ns_ip[i];
			if(curr.Mid(0,3)==_T("10."))
				public_ns = false;
			if(curr.Mid(0,8)==_T("192.168."))
				public_ns = false;
			if(curr.Mid(0,4)==_T("172."))
			{
				int pos = FindStr(StringToChar(curr),".",0);
				int pos2 = FindStr(StringToChar(curr),".",pos+1);
				
				int sub = _wtoi(CharToString(MidStr(StringToChar(curr),pos+1,pos2)));
				if((sub>=16)&&(sub<=31))
					public_ns = false;
			}

			if(!public_ns)
				break;
		}

		if(public_ns)
			Dlg7->res_6 = _T("YES");
		else
			Dlg7->res_6 = _T("NO");

		Dlg7->UpdateData(FALSE);
	}

	if(((Dlg7->select_7)||(Dlg7->select_all))&&(!Dlg7->request_cancel))
	{
		Dlg7->status = _T("Check [7] in progress...");
		Dlg7->UpdateData(FALSE);

			CString mx[100];
			CString mx_ip[100];
			int mx_r = 0;
			//cname = false;
			//CString host, ipa, ipb;
			//char* ac;
			//unsigned char baw;

			if(true)
			{
				res = DnsQuery(domain,  DNS_TYPE_MX,DNS_QUERY_STANDARD, NULL, &pdns, 0);

				if(res==ERROR_SUCCESS)
				{
					while(true)
					{
						if((pdns->wType==DNS_TYPE_MX))
						{
							mx[mx_r++] = pdns->Data.MX.pNameExchange;
						}
						//else if((pdns->wType==DNS_TYPE_CNAME))
							//cname = true;

						pdns = pdns->pNext;

						if(pdns==NULL)
							break;
					}
				}

				if(pdns)
				{
					DnsRecordListFree(pdns, freetype);
					pdns = NULL;
				}

				for(int i=0;i<mx_r;i++)
				{
					res = DnsQuery(mx[i],  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);

					if(res==ERROR_SUCCESS)
						{
							ac = (char*)(&(pdns->Data.A.IpAddress));
							ipa = _T("");
							for(int hq=0;hq<4;hq++)
							{
								baw = ac[hq];//+128;
								ipb.Format(_T("%d"),baw);
								ipa+=ipb;
								if(hq<3)
									ipa+=_T(".");
							}
							mx_ip[i] = ipa;
						}
				}

			}


bool reverse = true;
		bool temp;

		for(int i=0;i<mx_r;i++)
		{

			res = DnsQuery(PTRRev(mx_ip[i]),  DNS_TYPE_PTR,DNS_QUERY_STANDARD, NULL, &pdns, 0);
			if(res==ERROR_SUCCESS)
			{
				temp = false;
				while(true)
				{
					if((pdns->wType==DNS_TYPE_PTR))
					{
						temp = true;
					}

					pdns = pdns->pNext;

					if(pdns==NULL)
						break;
				}

				if(!temp)
					reverse = false;
			}
			else reverse = false;

			if(pdns)
			{
				DnsRecordListFree(pdns, freetype);
				pdns = NULL;
			}
		}

		if(reverse)
			Dlg7->res_7 = _T("YES");
		else
			Dlg7->res_7 = _T("NO");

		Dlg7->UpdateData(FALSE);
	}

	if(((Dlg7->select_8)||(Dlg7->select_all))&&(!Dlg7->request_cancel))
	{
		Dlg7->status = _T("Check [8] in progress...");
		Dlg7->UpdateData(FALSE);

		bool reverse = true;
		bool temp;

		for(int i=0;i<ns_r;i++)
		{

			res = DnsQuery(PTRRev(ns_ip[i]),  DNS_TYPE_PTR,DNS_QUERY_STANDARD, NULL, &pdns, 0);
			if(res==ERROR_SUCCESS)
			{
				temp = false;
				while(true)
				{
					if((pdns->wType==DNS_TYPE_PTR))
					{
						temp = true;
					}

					pdns = pdns->pNext;

					if(pdns==NULL)
						break;
				}

				if(!temp)
					reverse = false;
			}
			else reverse = false;

			if(pdns)
			{
				DnsRecordListFree(pdns, freetype);
				pdns = NULL;
			}
		}

		if(reverse)
			Dlg7->res_8 = _T("YES");
		else
			Dlg7->res_8 = _T("NO");

		Dlg7->UpdateData(FALSE);
	}

	if(!Dlg7->request_cancel)
	{
		Dlg7->status = _T("Finished.");
		Dlg7->UpdateData(FALSE);
	}
	else
	{
		Dlg7->status = _T("Canceled.");
		Dlg7->UpdateData(FALSE);
	}

	Dlg7->but_start.SetWindowTextW(_T("Restart"));
	Dlg7->but_next.EnableWindow(TRUE);
	Dlg7->test_running = false;
	Dlg7->request_cancel = false;
}

CString ReversePtr(CString a, CString b, CString c, CString d)
{
	CString result = _T("err");

	return result;
}

DWORD WINAPI RunTest(LPVOID lpParam)
{
	thread_data* data = (thread_data*)lpParam;
	CShared* shared = data->shared;
	CDNSToolDlg*	Dlg1 = shared->DNSToolDlg;
	DlgTLD*			Dlg2 = shared->DlgTLDa;
	CDlgWeb*		Dlg3 = shared->DlgWeb;
	CDlgMX*			Dlg4 = shared->DlgMX;
	CDlgNS*			Dlg5 = shared->DlgNS;
	CArecsDlg*		Dlg6 = shared->ArecsDlg;
	CConf*			Dlg7 = shared->Conf;
	
	int test_type = data->test_id;
	//char buff_r[SOCK_BUFFER+1];
	//char buff_s[SOCK_BUFFER+1];
	CString msg;
/*
	AfxSocketInit();
	CAsyncSocket* sock_write = new CAsyncSocket;
	CAsyncSocket* sock_read = new CAsyncSocket;
	sock_write->Create(0,SOCK_DGRAM,FD_WRITE);
	sock_read->Create(9122,SOCK_DGRAM,FD_READ); // port 3835
*/
	if(test_type==1) /////////////////////////////////////////////////////////////////////////
	{
		TestPTR(Dlg1);
	}
	else if(test_type==2) /////////////////////////////////////////////////////////////////////////
	{
		TestTLD(Dlg1,Dlg2);
	}
	else if(test_type==3) /////////////////////////////////////////////////////////////////////////
	{
		TestWeb(Dlg1,Dlg2,Dlg3);
	}
	else if(test_type==4) /////////////////////////////////////////////////////////////////////////
	{
		PDNS_RECORD pdns;
		DNS_FREE_TYPE freetype = DnsFreeRecordList;

		CString ip;

		int total = Dlg2->table.GetItemCount();
		int res;
		int dd = 0;
		int index;
		int e;
		int f;

		for(int bb=-1;bb<total;bb++)
		{
			if(bb==-1)
				ip = Dlg1->domain_name;
			else
			{
				if(Dlg2->select_all==FALSE)
					if(Dlg2->table.GetCheck(bb)==0)
						continue;

				ip = Dlg2->table.GetItemText(bb,0);//Dlg2->domains[bb];
			}

			res = DnsQuery(ip,  DNS_TYPE_MX,DNS_QUERY_STANDARD, NULL, &pdns, 0);
				
			if(res==ERROR_SUCCESS)
			{
				e = Dlg4->table.GetItemCount();
				CString kill;
				dd = 0;
				f = 0;
				while(true)
				{
					if(pdns->wType==DNS_TYPE_MX)
					{
						if(dd==0)
							index = Dlg4->table.InsertItem(Dlg4->table.GetItemCount(),ip/*pdns->pName*/);
						else
							index = Dlg4->table.InsertItem(Dlg4->table.GetItemCount(),_T(""));
						
						Dlg4->table.SetItemText(index,1,pdns->Data.MX.pNameExchange);
					}
					/*
					if(pdns->Data.MX.wPreference>0)
					{
						if(dd==0)
						{
							index = Dlg4->table.InsertItem(Dlg4->table.GetItemCount(),pdns->pName);
							Dlg4->table.SetItemText(index,1,pdns->Data.MX.pNameExchange);
						}
						else
							index = Dlg4->table.InsertItem(Dlg4->table.GetItemCount(),_T(""));

					}
					else
						Dlg4->table.SetItemText(e+(f++),1,pdns->pName);
					*/
					pdns = pdns->pNext;

					dd++;

					if(pdns==NULL)
						break;
				}
			}

			if(pdns)
			{
				DnsRecordListFree(pdns, freetype);
				pdns = NULL;
			}
		}
		
		CString host, ipa, ipb;
		char* ac;
		unsigned char baw;
		int a2 = Dlg4->table.GetItemCount();
		for(int xx=0;xx<a2;xx++)
		{
			host = Dlg4->table.GetItemText(xx,1);

			res = DnsQuery(host,  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);
			
			if(res==ERROR_SUCCESS)
			{
				ac = (char*)(&(pdns->Data.A.IpAddress));
				ipa = _T("");
				for(int hq=0;hq<4;hq++)
				{
					baw = ac[hq];//+128;
					ipb.Format(_T("%d"),baw);
					ipa+=ipb;
					if(hq<3)
						ipa+=_T(".");
				}
				Dlg4->table.SetItemText(xx,2,ipa);
			}
			else
				Dlg4->table.SetItemText(xx,2,_T("-"));

			if(pdns)
			{
				DnsRecordListFree(pdns, freetype);
				pdns = NULL;
			}
		}
	}
	else if(test_type==5) // NS
	{
		PDNS_RECORD pdns;
		DNS_FREE_TYPE freetype = DnsFreeRecordList;

		CString ip;

		int total = Dlg3->table.GetItemCount();
		int res;
		int dd = 0;
		int index;
		int e;
		int f;

		for(int bb=-1;bb<total;bb++)
		{
			if(bb==-1)
				ip = Dlg1->domain_name;
			else
			{
				if(Dlg2->select_all==FALSE)
					if(Dlg2->table.GetCheck(bb)==0)
						continue;

				ip = Dlg2->table.GetItemText(bb,0);//Dlg2->domains[bb];
			}

			res = DnsQuery(ip,  DNS_TYPE_NS,DNS_QUERY_STANDARD, NULL, &pdns, 0);
				
			if(res==ERROR_SUCCESS)
			{
				e = Dlg5->table.GetItemCount();
				CString kill;
				dd = 0;
				f = 0;
				while(true)
				{
					if(pdns->wType==DNS_TYPE_NS)
					{
						if(dd==0)
							index = Dlg5->table.InsertItem(Dlg5->table.GetItemCount(),ip);
						else
							index = Dlg5->table.InsertItem(Dlg5->table.GetItemCount(),_T(""));

						Dlg5->table.SetItemText(index,1,pdns->Data.NS.pNameHost);
					}
					else
						Dlg5->table.SetItemText(e+(f++),1,pdns->pName);
					
					pdns = pdns->pNext;

					dd++;

					if(pdns==NULL)
						break;
				}
			}

			if(pdns)
			{
				DnsRecordListFree(pdns, freetype);
				pdns = NULL;
			}
		}

		// ip(s)
		
		CString host, ipa, ipb;
		char* ac;
		unsigned char baw;
		int a2 = Dlg5->table.GetItemCount();
		for(int xx=0;xx<a2;xx++)
		{
			host = Dlg5->table.GetItemText(xx,1);

			res = DnsQuery(host,  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);
			
			if(res==ERROR_SUCCESS)
			{
				ac = (char*)(&(pdns->Data.A.IpAddress));
				ipa = _T("");
				for(int hq=0;hq<4;hq++)
				{
					baw = ac[hq];//+128;
					ipb.Format(_T("%d"),baw);
					ipa+=ipb;
					if(hq<3)
						ipa+=_T(".");
				}
				Dlg5->table.SetItemText(xx,2,ipa);
			}
			else
				Dlg5->table.SetItemText(xx,2,_T("-"));

			if(pdns)
			{
				DnsRecordListFree(pdns, freetype);
				pdns = NULL;
			}
		}

		// axfr
		int he;
		for(int xx=0;xx<a2;xx++)
		{
			host = _T("");
			he = xx;
			while(host==_T(""))
			{
				host = Dlg5->table.GetItemText(he--,0);
				if(he==-1)
					break;
			}

			IP4_ARRAY* ia = NULL;
			
			res = DnsQuery(Dlg5->table.GetItemText(xx,1),  DNS_TYPE_A,DNS_QUERY_STANDARD, NULL, &pdns, 0);
			if(res==ERROR_SUCCESS)
			{
				ia = new IP4_ARRAY;
				ia->AddrCount = 1;
				ia->AddrArray[0] = pdns->Data.A.IpAddress;
			}

			res = DnsQuery(host,  DNS_TYPE_AXFR ,DNS_QUERY_USE_TCP_ONLY/*DNS_QUERY_STANDARD*/, ia, &pdns, 0);

			Dlg5->table.SetItemText(xx,3,_T("NO"));

			if((res==DNS_ERROR_BAD_PACKET)||(res==ERROR_SUCCESS))
			{
				if(pdns)
				{
					while(true)
					{
						if(pdns->wType==DNS_TYPE_SOA)
							Dlg5->table.SetItemText(xx,3,_T("YES!"));

						pdns = pdns->pNext;
						if(pdns==NULL)
							break;
					}
				}
				else
				{
					Dlg5->table.SetItemText(xx,3,_T("NO"));
				}
			}
			/*
			if(res==ERROR_SUCCESS)
				Dlg5->table.SetItemText(xx,3,_T("YES!"));
			else if(res==DNS_ERROR_RCODE_REFUSED)
				Dlg5->table.SetItemText(xx,3,_T("NO"));
			else
			{
				CString ag2e;
				ag2e.Format(_T("%d"),res);
				Dlg5->table.SetItemText(xx,3,ag2e);
			}*/

			if(pdns)
			{
				DnsRecordListFree(pdns, freetype);
				pdns = NULL;
			}
		}
	}
	else if(test_type==6)
	{
		TestArecs(Dlg1,Dlg2,Dlg3,Dlg4,Dlg5,Dlg6);
	}
	else if(test_type==7)
	{
		TestConfig(Dlg1,Dlg2,Dlg3,Dlg4,Dlg5,Dlg6,Dlg7);
	}

	return 0;
}