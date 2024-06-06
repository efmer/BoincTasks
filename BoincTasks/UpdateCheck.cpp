// UpdateCheck.cpp : implementation file
//

#include "stdafx.h"

//#include "Label.h"

#include "BoincTasks.h"
//#include "resource.h"

//#include <afx.h>
//#include <afxwin.h>
#include <afxinet.h>
//#include <shlobj.h>
//#include <stdio.h>

#include "UpdateCheck.h"

// CUpdateCheck

CUpdateCheck::CUpdateCheck()
{
}

CUpdateCheck::~CUpdateCheck()
{
}

bool CUpdateCheck::GetLatestVersion(char * pcServer, char *pcHeaderType, char *pcLocation, char *pcFile, CUpdateVersion *pUpdateVersion)		//www.efmer.eu, BOINC_Tasks, download/boinc/boinc_tasks/unified/update_info_tasks_32_64.html
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
	CString szHeaders = _T("Accept: text/xml, text/xml\r\n");

	DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_DONT_CACHE;
	DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
	//Proxy setting if we need them
	LPCTSTR pstrProxyName = NULL;
	LPCTSTR pstrProxyBypass = NULL;

	DWORD dwFlags = NULL;

	sFile = pcLocation;
	sFile+=pcFile;

	CString txt = _T("Check for update: ");
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
			// warning these are bytes we get not (wchar_t)
			int len = (int) pFile->GetLength();
			char buf[2000];
			int numread;
			if ((numread = pFile->Read(buf,sizeof(buf)-1)) > 0)
			{
				buf[numread] = '\0';

				CStringA sBuffer = buf;				// ascii   buffer
				CStringA sBufferBeta = buf;

				CStringA sTxt = "Update file ";
				sTxt = sTxt + sBuffer;

				int pos = sBuffer.Find(":W!:");		//seperator
				pUpdateVersion->m_sVersionHttp = sBuffer.Left(pos);
				sBuffer = sBuffer.Mid(pos+4);
				CString sLink = pcServer;
				sLink += "/";
				pos = sBuffer.Find(".exe");			//.exe = end of the file name
				pUpdateVersion->m_sVersionExe = sLink + pcLocation;					//_T("/download/boinc/boinc_tasks/unified/");
				CStringA sTemp = sBuffer.Left(pos+4);
				pUpdateVersion->m_sVersionExe += sTemp;

				int posNextLine = sBufferBeta.Find("\n");			//new line seperator
				if (posNextLine >= 0)
				{
					sBufferBeta = sBufferBeta.Mid(posNextLine+1);
					int posBeta = sBufferBeta.Find(":B!:");//seperator
					if (posBeta >= 0)
					{
						pUpdateVersion->m_sVersionBetaHttp = sBufferBeta.Left(posBeta);
						sBufferBeta = sBufferBeta.Mid(posBeta+4);
						CString sLinkBeta = pcServer;
						sLinkBeta += "/";
						pos = sBufferBeta.Find(".exe");			//.exe = end of the file name
						pUpdateVersion->m_sVersionBetaExe = sLink + pcLocation;			//_T("/download/boinc/boinc_tasks/unified/");
						CStringA sTemp = sBufferBeta.Left(pos+4);
						pUpdateVersion->m_sVersionBetaExe += sTemp;
					}
				}

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
//		TRACE(_T("File transfer failed!! - %s"),szErr); 

		pEx->Delete();
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
		return false;
	}

	return bReturn;
}