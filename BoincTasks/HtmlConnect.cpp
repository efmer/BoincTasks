// HtmlConnect.cpp : implementation file
//

#include "stdafx.h"
#include <Windows.h>
#include "BoincTasks.h"
#include <afxinet.h>
#include "HtmlConnect.h"


// CHtmlConnect

CHtmlConnect::CHtmlConnect()
{
//	m_sVersion.LoadString(IDS_PROGRAM_VERSION);
//	m_sVersion.Remove('.');
	m_sVersion = CLOUD_VERSION;
	m_dwLastTick = 0;

	SessionBegin();
}

CHtmlConnect::~CHtmlConnect()
{
	SessionEnd();
}

void CHtmlConnect::SetServer(CString *psServer, int iPort)
{
	m_sServer = *psServer;
	m_iPort = iPort;
}

void CHtmlConnect::SessionBegin()
{
	char *pcHeaderType	= "BoincTasks_Cloud";
	m_pSession = new CInternetSession(pcHeaderType, 1, INTERNET_OPEN_TYPE_PRECONFIG);


//	CInternetSession session(pcHeaderType, 1, INTERNET_OPEN_TYPE_PRECONFIG);//, NULL, NULL, INTERNET_FLAG_ASYNC);

//	m_pSession->SetOption(INTERNET_FLAG_DONT_CACHE, 1); crash
//	m_pSession->SetOption(INTERNET_OPTION_END_BROWSER_SESSION, 1); crash
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,1000* 5);
	m_pSession->SetOption(INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT,1000* 5);
	m_pSession->SetOption(INTERNET_OPTION_CONTROL_SEND_TIMEOUT,1000* 5);
	m_pSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,1000* 5);
	m_pSession->SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT,1000* 5);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_BACKOFF,500);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES,1);
//	m_pSession->EnableStatusCallback(FALSE);		//Enable or disable status callbacks
}

void CHtmlConnect::SessionEnd()
{
	if (m_pSession != NULL)
	{
		m_pSession->Close();
		delete m_pSession;
	}
}

// CHtmlConnect member functions

CString CHtmlConnect::PostDataJson(CString sPhp, CString *psData, bool bVersionFolder)
{
	CHttpConnection*	pServer = NULL;   
	CHttpFile* pFile = NULL;
	DWORD dwResponseLength, dwTotalRequestLength;

	bool	bReturn = false;
	CString	sFile, sVersion;
	CString sResponse;
	CString sId, sToken;
//	int		iPort;
//	sId = "ACDFVGFDSEDFTGREDFREKJKLJHYFSWSXVNLJHUG";
	sToken = "1234567890";

	pFile = NULL;
	//sVersion.LoadString(IDS_PROGRAM_VERSION);
	sVersion = CLOUD_VERSION;
	if (m_iPort != 443)
	{
		sFile += "cloud/";
	}
	if (bVersionFolder)
	{
		sFile += "version/" + m_sVersion + "/";
	}
	sFile+= "php/";
	sFile+= sPhp;

	int iTimeout = 0;
	while ((m_dwLastTick + 100) > GetTickCount())
	{
		if (iTimeout++ > 4) break;
		Sleep(50);
	}
	m_dwLastTick = GetTickCount();


//	CInternetSession	m_session(pcHeaderType, 1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE);


//	DWORD dwRet;
	try {		
			//Create the multi-part form data that goes before and after the actual file upload.
	
		CString sFileName;
		CString sHTTPBoundary = _T("FFF3F395A90B452BB8BEDC878DDBD152");       
		CString sRequestHeaders = MakeRequestHeadersJson(sHTTPBoundary);
		dwTotalRequestLength = psData->GetLength();

		pServer = m_pSession->GetHttpConnection(m_sServer,NULL,m_iPort);
		if (m_iPort == 443)
		{
			DWORD dwRequestFlags = INTERNET_FLAG_SECURE;// | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_NO_AUTO_REDIRECT;
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, sFile, NULL, 1, NULL, NULL, dwRequestFlags);
		}
		else
		{
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, sFile);
		}

		pFile->AddRequestHeaders(sRequestHeaders);
		pFile->SendRequestEx(dwTotalRequestLength, HSR_SYNC | HSR_INITIATE);


		//upload the file.
		pFile->Write(psData->GetBuffer(), psData->GetLength()); psData->ReleaseBuffer();
	
		pFile->EndRequest(HSR_SYNC);

		//get the response from the server.
		LPSTR szResponse;
		dwResponseLength = (DWORD) pFile->GetLength();
		while (0 != dwResponseLength )
		{
			szResponse = (LPSTR)malloc(dwResponseLength + 1);
			szResponse[dwResponseLength] = '\0';
			pFile->Read(szResponse, dwResponseLength);
			sResponse += szResponse;
			free(szResponse);
			dwResponseLength = (DWORD) pFile->GetLength();
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
//		session.Close();
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

		CString sError = EXEPTION_TEXT;
		sError += szErr;

		return sError;
//		return EXEPTION_TEXT;
	}

	return sResponse;
}

CString CHtmlConnect::PostData(CString sPhp, CString *psData, CString sId)
{
	CHttpConnection*	pServer = NULL;
	CHttpFile* pFile = NULL;
	DWORD dwResponseLength, dwTotalRequestLength;

	bool	bReturn = false;
	CString	sFile, sVersion;
	CString sResponse;
	CString sToken;
	//	int		iPort;
	//	sId = "ACDFVGFDSEDFTGREDFREKJKLJHYFSWSXVNLJHUG";
	sToken = "1234567890";

	pFile = NULL;
	sVersion.LoadString(IDS_PROGRAM_VERSION);
	if (m_iPort != 443)
	{
		sFile += "cloud/";
	}
	sFile += "version/" + m_sVersion + "/";

	sFile += "php/";
	sFile += sPhp;

	int iTimeout = 0;
	while ((m_dwLastTick + 100) > GetTickCount())
	{
		if (iTimeout++ > 4) break;
		Sleep(50);
	}
	m_dwLastTick = GetTickCount();


//	CInternetSession	m_session(pcHeaderType, 1, INTERNET_OPEN_TYPE_PRECONFIG);

//	DWORD dwRet;
	try {		
			//Create the multi-part form data that goes before and after the actual file upload.
	
		CString sFileName;
//		sData = "RGl0IGlzIGVlbiB0ZXN0";	// base 64
		sFileName = "data.bt";
		CString sHTTPBoundary = _T("FFF3F395A90B452BB8BEDC878DDBD152");       
		CString sPreFileData = MakePreFileData(sHTTPBoundary, sId, sToken, sFileName);	//file.GetFileName());
		CString sPostFileData = MakePostFileData(sHTTPBoundary);
		CString sRequestHeaders = MakeRequestHeaders(sHTTPBoundary);
		dwTotalRequestLength = sPreFileData.GetLength() + sPostFileData.GetLength() + psData->GetLength();

		pServer = m_pSession->GetHttpConnection(m_sServer,NULL,m_iPort);

		if (m_iPort == 443)
		{
			DWORD dwRequestFlags = INTERNET_FLAG_SECURE;// | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_NO_AUTO_REDIRECT;
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, sFile, NULL, 1, NULL, NULL, dwRequestFlags);
		}
		else
		{
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, sFile);
		}

		pFile->AddRequestHeaders(sRequestHeaders);
		pFile->SendRequestEx(dwTotalRequestLength, HSR_SYNC | HSR_INITIATE);

		//Write out the headers and the form variables
		pFile->Write((LPSTR)(LPCSTR)sPreFileData, sPreFileData.GetLength());

		//upload the file.
		pFile->Write(psData->GetBuffer(), psData->GetLength()); psData->ReleaseBuffer();
	
//		CString headers;
//		pFile->QueryInfo(HTTP_QUERY_STATUS_TEXT,headers);

//		pFile->QueryInfoStatusCode(dwRet);//Check wininet.h for info about the status codes

		//Finish the upload.
		pFile->Write((LPSTR)(LPCSTR)sPostFileData, sPostFileData.GetLength());
		pFile->EndRequest(HSR_SYNC);

		//get the response from the server.
		LPSTR szResponse;
		dwResponseLength = (DWORD) pFile->GetLength();
		while (0 != dwResponseLength )
		{
			szResponse = (LPSTR)malloc(dwResponseLength + 1);
			szResponse[dwResponseLength] = '\0';
			pFile->Read(szResponse, dwResponseLength);
			sResponse += szResponse;
			free(szResponse);
			dwResponseLength = (DWORD) pFile->GetLength();
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
//		session.Close();
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
//		session.Close(); 
		return EXEPTION_TEXT;
	}

	return sResponse;
}

CString CHtmlConnect::MakeRequestHeadersJson(CString& sBoundary)
{
	CString sFormat;
	CString sData;
	sFormat = "Content-Type: application/json; charset=utf-8; boundary=%s\r\n";
	sData.Format(sFormat, sBoundary);
	return sData;
}


CString CHtmlConnect::MakeRequestHeaders(CString& sBoundary)
{
	CString sFormat;
	CString sData;
	sFormat = "Content-Type: multipart/form-data; boundary=%s\r\n";
	sData.Format(sFormat, sBoundary);
	return sData;
}

CString CHtmlConnect::MakePreFileData(CString& strBoundary, CString sId, CString sToken, CString& strFileName)
{
	CString sFormat;
	CString sData;

	sFormat = "--%s";
	sFormat += "\r\n";
	sFormat += "Content-Disposition: form-data; name=\"id\"";
	sFormat += "\r\n\r\n";
	sFormat += "%s";
	sFormat += "\r\n";

	sFormat += "--%s";
	sFormat += "\r\n";
	sFormat += "Content-Disposition: form-data; name=\"token\"";
	sFormat += "\r\n\r\n";
	sFormat += "%s";
	sFormat += "\r\n";

	sFormat += "--%s";
	sFormat += "\r\n";
	sFormat += "Content-Disposition: form-data; name=\"filename\"; filename=\"%s\"";
	sFormat += "\r\n";
	sFormat += "Content-Type: application/octet-stream";
	sFormat += "\r\n";
	sFormat += "Content-Transfer-Encoding: base64";
	sFormat += "\r\n\r\n";

	sData.Format(sFormat, strBoundary, sId, strBoundary, sToken, strBoundary, strFileName);

	return sData;
 }

CString CHtmlConnect::MakePostFileData(CString& sBoundary)
{
	CString sFormat;
	CString sData;

	sFormat = "\r\n";
	sFormat += "--%s";
	sFormat += "\r\n";
	sFormat += "Content-Disposition: form-data; name=\"submitted\"";
	sFormat += "\r\n\r\n";
	sFormat += "";
	sFormat += "\r\n";
	sFormat += "--%s--";
	sFormat += "\r\n";

	sData.Format(sFormat, sBoundary, sBoundary);

	return sData;
}