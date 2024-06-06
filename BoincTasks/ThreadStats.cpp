// ThreadStats.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSettingsExpert.h"
#include <afxinet.h>
#include <direct.h>
#include "Xml.h"
#include "ThreadStats.h"


// CThreadStats

IMPLEMENT_DYNCREATE(CThreadStats, CWinThread)

CThreadStats::CThreadStats()
{
}

CThreadStats::~CThreadStats()
{
}

BOOL CThreadStats::InitInstance()
{
	bool	bExit;
	MSG		msg;
	HWND	hWnd;

	bExit = false;

	while(!bExit)
	{
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT)
			{
				hWnd = (HWND) msg.wParam;
				bExit = true;
			}

			if (msg.message == UWM_MSG_THREAD_START_STATS)			// read from file
			{
				bool	bStatus;
				int		iStatus;

				std::string	*psCPID;
				CString *psStats, sCPID;
				psStats = new CString;
				hWnd = (HWND) msg.wParam;
				psCPID= (std::string *) msg.lParam; 
				sCPID = psCPID->c_str();

				if (NeedToUpdate(&sCPID))
				{
					bStatus = ReadStatsWeb(&sCPID, psStats);
					if (bStatus) iStatus = STATS_OK_WEB;
					else iStatus = STATS_ERROR;
				}
				else
				{
					bStatus = ReadStatsFile(&sCPID, psStats);
					if (bStatus) iStatus = STATS_OK_FILE;
					else iStatus = STATS_ERROR;
				}
				::PostMessage(hWnd,UWM_MSG_THREAD_READY_STATS, (WPARAM) psStats, (LPARAM) iStatus);
			}
			
/*
			if (msg.message == UWM_MSG_THREAD_START_STATS_FULL)		// read from www
			{
				bool	bStatus;
				int		iStatus;

				std::string *psCPID;
				CString	 *psStats, sCPID;
				psStats = new CString;
				hWnd = (HWND) msg.wParam;
				psCPID= (std::string *) msg.lParam; 
				sCPID = psCPID->c_str();
				bStatus = ReadStatsWeb(&sCPID, psStats);

				if (bStatus) iStatus = STATS_OK_WEB;
				else iStatus = STATS_ERROR;

				::PostMessage(hWnd,UWM_MSG_THREAD_READY_STATS, (WPARAM) psStats, (LPARAM) iStatus);
			}
*/

		}
		Sleep(10);
	}
	return FALSE;
}

int CThreadStats::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadStats, CWinThread)
END_MESSAGE_MAP()


bool CThreadStats::NeedToUpdate(CString *psCPID)
{
	time_t		tRemoveTime;
	CString		sDirectory, sFileName;
	CXml		xml("");
	CTime		tFile;

	_time64(&tRemoveTime);
	tRemoveTime += 60*60*6;		// 6 hours;

	sDirectory = xml.GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);
	sDirectory += STATS_FOLDER;
	sFileName = sDirectory + STATS_FILE + *psCPID + ".xml";
	
	CFile cfile;
	if (cfile.Open(sFileName, CFile::modeRead | CFile::shareDenyNone))
	{
		CFileStatus status;
		if(cfile.GetStatus(status))    // virtual member function
		{
			tFile = status.m_mtime;
		    time_t tTime = tFile.GetTime();
			cfile.Close();

			if (tTime < tRemoveTime)
			{
				return false;
			}
		}
	}
	return true;
}


bool CThreadStats::ReadStatsWeb(CString *psCPID, CString *psStats)
{
	bool	bStart, bStop;
	bool	bReturn = false;
	CString sVersion, sFile;
	CXml	xml("");
	CFile	file;

	return false; // disabled in v 1.72

	CInternetSession session("BT Stats");
	int ntimeOut = 5;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,1000* ntimeOut);
	session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,1000);
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES,2);

	//Enable or disable status callbacks
	session.EnableStatusCallback(TRUE);


	CStdioFile *filedat = NULL;
	CString sUrl;
	
	//cpid=E0FB5D3E93998F02431E30F278A06804"
	bStart = false;
	bStop = false;
//	sUrl = "http://boinc.netsoft-online.com/get_user.php?cpid=";
	sUrl = "http://boinc.netsoft-online.com/e107_plugins/boinc/get_user.php?html=&cpid=";
	sUrl+= psCPID->MakeUpper();
	try
	{
		filedat = (CStdioFile*) session.OpenURL(sUrl,1,INTERNET_FLAG_TRANSFER_ASCII,NULL,0);
	}
	catch(CInternetException *e)
	{
		filedat = NULL;
		e->Delete();
	}
	if (filedat)
	{
		CString sLine;
		//m_out += "\\r\\n";
		for (int i=0; i < 20000 && filedat->ReadString(sLine); i++)
		{
			if (sLine.Find("<user>") >= 0)
			{
				bStart = true;
			}
			if (bStart)
			{
				if (sLine.Find("</user>") >= 0)
				{
					bStart = false;
					bStop = true;
					sLine += "\r\n";
					*psStats += sLine;
					break;
				}
				sLine += "\r\n";
				*psStats += sLine;
			}
		}
		filedat->Close();
		delete filedat;
	}

	if (psStats->Find("<project>") < 0)
	{
		return false;
	}
	if (psStats->Find("</project>") < 0)
	{
		return false;
	}

	if (bStop)
	{
		CString sDirectory, sFile;
		sDirectory = xml.GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);
		sDirectory += STATS_FOLDER;
		_mkdir(sDirectory);

		sFile = sDirectory + STATS_FILE + *psCPID + ".xml";
	
		CFileException e;
		if( !file.Open( sFile.GetBuffer(), CFile::modeCreate | CFile::modeWrite, &e ) )
		{
			return false;
		}

		try
		{
			file.Write(*psStats,psStats->GetLength());
			file.Close();
		}
		catch(CFileException* pEx )
		{
		    pEx->Delete();
		}

		return true;
	}

	return false;
}

bool CThreadStats::ReadStatsFile(CString *psCPID, CString *psStats)
{
	CString sDirectory, sFile;
	CXml	xml("");
	CFile	file;
	int		iTotalReadCount, iReadCount;
	char	bBuffer	[256];

	sDirectory = xml.GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);
	sDirectory += STATS_FOLDER;
	sFile = sDirectory + STATS_FILE + *psCPID + ".xml";

	CFileException e;
	if( !file.Open(sFile.GetBuffer(), CFile::modeRead, &e ) )
	{
		if (e.m_cause == CFileException::fileNotFound)	// not really an error
		{
			return false;
		}
		return false;
	}

	iTotalReadCount = 0;
	do
	{
		iReadCount = file.Read(bBuffer, 255);
		bBuffer[iReadCount] = 0;
		if (iReadCount > 0) *psStats += &bBuffer[0];
		iTotalReadCount += iReadCount;
	}while (iReadCount > 0);

	file.Close();
	return true;
}