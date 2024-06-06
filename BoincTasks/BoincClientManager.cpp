// BoincClientManager.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgLogging.h"
#include "BoincClientManager.h"
#include "Translation.h"
#include "Tlhelp32.h"

// original files in BOINC BOINCClientManager.cpp


// CBoincClientManager

CBoincClientManager::CBoincClientManager()
{

}

CBoincClientManager::~CBoincClientManager()
{
}

/*
bool CBoincClientManager::StartupBoincCore(int iLocalHostThreadId)
{
CLoggingMessage *psLog;
CString sProgramDir, sFullProgramDir, sDataDir;
CWnd	*pWndLogging;
char	cBuffer[256];

psLog = new CLoggingMessage;
psLog->m_pFrom = "Start BOINC client";

pWndLogging = theApp.m_pDlgLogging;

if (IsRunning())
{
psLog->m_pText = "The BOINC client was already running";
pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
delete psLog;
return true;
}

// Boinc client is not running

sProgramDir = theApp.GetBoincDirectory((char *) &"INSTALLDIR");
sDataDir = theApp.GetBoincDirectory((char *) &"DATADIR");
sFullProgramDir = sProgramDir + "boinc.exe --detach" ;
//	sFullProgramDir = sProgramDir + "stress.exe" ;

bool bValid;
bValid = true;

if (sDataDir.GetLength() == 0) bValid = false;
if (sProgramDir.GetLength() == 0) bValid = false;

_snprintf_s(&cBuffer[0],255,_TRUNCATE,"Program: %s, Data directory: %s",sFullProgramDir, sDataDir);

psLog->m_pText = cBuffer;
pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_DEBUG);

if (bValid)
{
PROCESS_INFORMATION pi;
STARTUPINFO         si;
BOOL                bProcessStarted;

memset(&pi, 0, sizeof(pi));
memset(&si, 0, sizeof(si));

si.cb = sizeof(si);
si.dwFlags = STARTF_USESHOWWINDOW;
si.wShowWindow = SW_HIDE;

bProcessStarted = CreateProcess(
NULL,
sFullProgramDir.GetBuffer(),
NULL,
NULL,
FALSE,
CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW,
NULL,
sDataDir,
&si,
&pi
);
theApp.m_bBoincClientStartedByBoincTasks = true;		// here not when TRUE
if (bProcessStarted == TRUE)
{
psLog->m_pText = "The BOINC client started OK";
pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
if (iLocalHostThreadId > 0)
{
for (int iCount = 0; iCount < 60; iCount++)
{
Sleep(1000);

if (IsRunning())
{
break;
}
}
Sleep(2000);	// the client needs some time to start
::PostThreadMessage(iLocalHostThreadId, UWM_MSG_THREAD_CONNECT_RPC, 0, 0);
}
}
}
else
{
_snprintf_s(&cBuffer[0],255,_TRUNCATE,"Unable to start: Invalid directories: Program: %s, Data directory: %s",sFullProgramDir, sDataDir);
psLog->m_pText = cBuffer;
pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
}

delete psLog;
return false;
}
*/

bool CBoincClientManager::StartupBoincCore(int iLocalHostThreadId)
{
	CLoggingMessage *psLog;
	CString sProgramDir, sFullProgramDir, sDataDir;
	CWnd	*pWndLogging;
	char	cBuffer[256];

	psLog = new CLoggingMessage;
	psLog->m_pFrom = "Start BOINC client";

	pWndLogging = theApp.m_pDlgLogging;
	
	if (IsRunning())
	{
		psLog->m_pText = "The BOINC client was already running";
		pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
		delete psLog;
		return true;
	}

	// Boinc client is not running

	sProgramDir = theApp.GetBoincDirectory((char *) &"INSTALLDIR");
	sDataDir = theApp.GetBoincDirectory((char *) &"DATADIR");
	sFullProgramDir = sProgramDir + "boinc.exe" ;

	bool bValid;
	bValid = true;

	if (sDataDir.GetLength() == 0) bValid = false;
	if (sProgramDir.GetLength() == 0) bValid = false;
	
	_snprintf_s(&cBuffer[0],255,_TRUNCATE,"Program: %s, Data directory: %s",sFullProgramDir, sDataDir);

	psLog->m_pText = cBuffer;
	pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_DEBUG);

	if (bValid)
	{
        PROCESS_INFORMATION pi;
        STARTUPINFO         si;
        BOOL                bProcessStarted;

        memset(&pi, 0, sizeof(pi));
        memset(&si, 0, sizeof(si));

        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

// | DETACHED_PROCESS works unpredictable
        bProcessStarted = CreateProcess(
            NULL,
            sFullProgramDir.GetBuffer(),
            NULL,
            NULL,
            FALSE,
			CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW,
            NULL,
            sDataDir,
            &si,
            &pi
        );
		theApp.m_bBoincClientStartedByBoincTasks = true;		// here not when TRUE
		if (bProcessStarted == TRUE)
		{
			psLog->m_pText = "The BOINC client started OK";
			pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
			if (iLocalHostThreadId > 0)
			{
				for (int iCount = 0; iCount < 60; iCount++)
				{
					Sleep(1000);

					if (IsRunning())
					{
						break;
					}
				}
				Sleep(2000);	// the client needs some time to start
				::PostThreadMessage(iLocalHostThreadId, UWM_MSG_THREAD_CONNECT_RPC, 0, 0);
			}
        }
	}
	else
	{
		_snprintf_s(&cBuffer[0],255,_TRUNCATE,"Unable to start: Invalid directories: Program: %s, Data directory: %s",sFullProgramDir, sDataDir);
		psLog->m_pText = cBuffer;
		pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
	}
	
	delete psLog;
	return false;
}

//int CBoincClientManager::IsBOINCConfiguredAsDaemon()
//{
//	bool bReturnValue = false;
//	if (is_daemon_installed()) bReturnValue = 1;
//	return bReturnValue;
//}


bool CBoincClientManager::IsRunning()
{
	bool	bRunning;

//	HANDLE h = CreateMutexA(NULL, true, RUN_BOINC_CLIENT_MUTEX);
//   DWORD err = GetLastError();
//  if ((h==0) || (err == ERROR_ALREADY_EXISTS))
//	{
//		bRunning = true;
//	}
//	if (h) CloseHandle(h);

	bRunning = CheckProcess("boinc.exe");

	return bRunning;
}

bool CBoincClientManager::Close(int iLocalHostThreadId, CLoggingMessage *psLog)
{
	bool	bResult;
	CString	sFormat;

	bResult = false;
	::PostThreadMessage(iLocalHostThreadId,UWM_MSG_THREAD_QUIT_CLIENT,0,0);	// close down the client
	psLog->m_pText = CLIENT_MANAGER_BOINC_SHUTTING_DOWN;
	theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
	DWORD dwError;
	bResult = KillClient(&dwError);
	if (!bResult) 
	{
		sFormat.Format("The BOINC client failed to shut down; %d", dwError);
		psLog->m_pText = sFormat.GetBuffer();
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);		
		return false;
	}
	else
	{
		psLog->m_pText = "The BOINC client has shut down";
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);	
	}
	if (IsRunning())
	{
		psLog->m_pText = gszTranslation[PosClientCouldntShutdown];	//"The BOINC client couldn't be shut down";xxxx
		theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);	
		AfxMessageBox(psLog->m_pText);
		return false;
	}
	::PostThreadMessage(iLocalHostThreadId, UWM_MSG_THREAD_CONNECT_LOST, 0, 0);
	return true;
}

bool CBoincClientManager::KillClient(DWORD *pExitCode)
{
	*pExitCode = 0;
	for (int iCount = 0; iCount < 30; iCount++)
	{
		Sleep(1000);

		if (!IsRunning())
		{
			return true;
		}
	}

	if (CheckProcess("boinc.exe", true)) return true;
	*pExitCode = GetLastError();
	return false;
}

bool CBoincClientManager::CheckProcess(CString sProcess, bool bKill)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	bool	bKilled;
	BOOL	bStatus;

	bKilled = false;
	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		CloseHandle( hProcessSnap );			// clean the snapshot object
		return false;
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		hProcess = OpenProcess( MAXIMUM_ALLOWED, FALSE, pe32.th32ProcessID );
		if (hProcess != NULL)
		{
			CloseHandle( hProcess );
			hProcess = NULL;
		}

		CString sName;
		sName = pe32.szExeFile;
		if (sName == sProcess) //"boinc.exe")
		{
			if (bKill)
			{
				if (pe32.th32ProcessID != 0)
				{
					hProcess = OpenProcess(PROCESS_TERMINATE , FALSE, pe32.th32ProcessID);
					if (hProcess != NULL)
					{
						bStatus = TerminateProcess(hProcess,0);
						if (bStatus) bKilled = true;
					}
					if (hProcess != NULL)
					{
						CloseHandle( hProcess );
						hProcess = NULL;
					}
				}
			}
			else
			{
				CloseHandle( hProcessSnap );
				return true;
			}
		}
	} while( Process32Next( hProcessSnap, &pe32 ) );
	CloseHandle( hProcessSnap );

	if (bKill)
	{
		return bKilled;
	}

	return false;
}

bool CBoincClientManager::CheckProcess(DWORD dwPID)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
//	bool	bKilled;
//	BOOL	bStatus;

//	bKilled = false;
	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		CloseHandle( hProcessSnap );			// clean the snapshot object
		return false;
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		hProcess = OpenProcess( MAXIMUM_ALLOWED, FALSE, pe32.th32ProcessID );
		if (hProcess != NULL)
		{
			CloseHandle( hProcess );
			hProcess = NULL;
		}

		if (pe32.th32ProcessID == dwPID)
		{
			CloseHandle( hProcessSnap );
			return true;
		}
	} while( Process32Next( hProcessSnap, &pe32 ) );
	CloseHandle( hProcessSnap );

	return false;
}