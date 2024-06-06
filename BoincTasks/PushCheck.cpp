// UpdateCheck.cpp : implementation file
//

#include "stdafx.h"

//#include "Label.h"

#include "BoincTasks.h"
#include <afxinet.h>
#include "threadrpc.h"
#include "DlgNotices.h"
#include "CreateHtmlNotices.h"
#include "Xml.h"
#include "PushCheck.h"

// CUpdateCheck

CPushCheck::CPushCheck()
{
}

CPushCheck::~CPushCheck()
{
}

bool CPushCheck::ReadPushNotice(CString *psTxt)
{
	char *pcServer		= "www.efmer.eu";
	char *pcLocationBt	= "download/boinc/boinc_tasks/unified/";
	char *pcFileXml		= "push.html";
	char *pcPush		= "push";

	CStringA sATxt;

	if (ReadFile(pcServer, pcPush, pcLocationBt, pcFileXml, &sATxt))
	{
//		pDlg->ReadXmlPush(&sATxt);
		*psTxt = sATxt;
		return true;
	}
	psTxt = NULL;
	return false;
}




bool CPushCheck::ReadFile(char * pcServer, char *pcHeaderType, char *pcLocation, char *pcFile, CString *psXml)
{
	bool	bReturn = false;
	CString sVersion, sFile;

	//the verb we will be using for this connection
	//if NULL then GET is assumed
	LPCTSTR pstrVerb = _T("GET");
	//the address of the url in the request was obtained from
	LPCTSTR pstrReferer = NULL;
	LPCTSTR pstrAcceptTypes = NULL;
	//Http version we are using; NULL = HTTP/1.0
	LPCTSTR pstrVersion = NULL;
	CString szHeaders = _T("Accept: text/html, text/htm\r\n");

	DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_DONT_CACHE;
	DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
	//Proxy setting if we need them
	LPCTSTR pstrProxyName = NULL;
	LPCTSTR pstrProxyBypass = NULL;

	DWORD dwFlags = NULL;

	sFile = pcLocation;
	sFile+=pcFile;

	CString txt = _T("Check for messages: ");
	txt = txt + pcServer + _T(" file:") + sFile;
	
	CInternetSession	session(pcHeaderType, 1, INTERNET_OPEN_TYPE_PRECONFIG);


	int ntimeOut = 5;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,1000* ntimeOut);
	session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,1000);
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES,1);

	//Enable or disable status callbacks
	session.EnableStatusCallback(TRUE);

	CHttpConnection*	pServer = NULL;   
	CHttpFile* pFile = NULL;
	DWORD dwRet;

	try {		

		pServer = session.GetHttpConnection(pcServer, 80, pcHeaderType);
		pFile = pServer->OpenRequest(pstrVerb, sFile, pstrReferer, 1, &pstrAcceptTypes, pstrVersion, dwHttpRequestFlags);
		pFile->AddRequestHeaders(szHeaders);
		pFile->AddRequestHeaders(_T("User-Agent: GetWebFile/1.0\r\n"), HTTP_ADDREQ_FLAG_ADD_IF_NEW);
		pFile->SendRequest();
		pFile->QueryInfoStatusCode(dwRet);//Check wininet.h for info about the status codes

		if (dwRet == HTTP_STATUS_DENIED)
		{
	//		return false;
		}

		if (dwRet == HTTP_STATUS_PROXY_AUTH_REQ)
		{
			dwRet = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,  FLAGS_ERROR_UI_FLAGS_GENERATE_DATA |FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);
			pFile->SendRequest();
			pFile->QueryInfoStatusCode(dwRet);//Check wininet.h for info about the status codes
		}

		if(dwRet == HTTP_STATUS_OK)
		{
			// warning these are bytes (wchar_t)
			int len = (int) pFile->GetLength();
			char buf[16000];
			int numread;
			if ((numread = pFile->Read(buf,sizeof(buf)-1)) > 0)
			{
				buf[numread] = '\0';
				*psXml = buf;

				bReturn = true;
			}
		//	myfile.Close();

		}
		if(pFile)
		{
			pFile->Close();  
			delete pFile;
			pFile = NULL;
		}
		if(pServer)
		{
			pServer->Close();
			delete pServer;
			pServer = NULL;
		}
		session.Close();
//		pWebFileDlg->m_ctrlProgress.SetPos(0);
	}
	catch (CInternetException* pEx) 
	{
      // catch any exceptions from WinINet      
		TCHAR szErr[1024];
		szErr[0] = '\0';
        if(!pEx->GetErrorMessage(szErr, 1024)) strcpy_s(szErr,_T("Update unknown error"));
		TRACE(_T("File transfer failed!! - %s"),szErr); 

		pEx->Delete();
		if(pFile)
		{
			pFile->Close();  
			pFile = NULL;
			delete pFile;
		}
		if(pServer)
		{
			pServer->Close();
			pServer = NULL;
			delete pServer;
		}
		session.Close(); 
		return false;
	}

	return bReturn;
}