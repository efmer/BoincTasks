// ThreadGarbageCollector.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ThreadGarbageCollector.h"


// CThreadGarbageCollector

IMPLEMENT_DYNCREATE(CThreadGarbageCollector, CWinThread)

CThreadGarbageCollector::CThreadGarbageCollector()
{

}

CThreadGarbageCollector::~CThreadGarbageCollector()
{
}

BOOL CThreadGarbageCollector::InitInstance()
{
	HWND	hWnd;
	MSG		msg;
	CThreadGarbageCollectorData *pThreadGarbageCollectorData;

	hWnd = NULL;
	bool	bExit = false;
	SetThreadPriority(THREAD_PRIORITY_IDLE);

	while(!bExit)
	{
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT)
			{
				hWnd = (HWND) msg.wParam;
				bExit = true;
			}
			if (msg.message == UWM_MSG_THREAD_GARBAGE_COLLECTOR)
			{
				hWnd = (HWND) msg.wParam;

				pThreadGarbageCollectorData = (CThreadGarbageCollectorData *) msg.lParam;
				DeleteFiles(pThreadGarbageCollectorData);
				bExit = true;
			}

		}
		Sleep(100);
	}

	::PostMessage(hWnd, UWM_MSG_THREAD_GARBAGE_COLLECTOR_READY, this->m_nThreadID, 0);

	return FALSE;
}

int CThreadGarbageCollector::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

void CThreadGarbageCollector::DeleteFiles(CThreadGarbageCollectorData *pThreadGarbageCollectorData)
{
	CFileFind	finder;
	CString		sFolder, sFileFind, sFileName, sFormat;
	time_t		tRemoveTime;
	CTime		tFile;
	MSG			msg;
	bool		bStatus;
	int			iFilesFound, iFilesDeleted;

	_time64(&tRemoveTime);
	tRemoveTime -= pThreadGarbageCollectorData->m_iTimeToRemove;

	sFolder	 = pThreadGarbageCollectorData->m_sFolder;
	sFileFind = sFolder + pThreadGarbageCollectorData->m_sFind;

	sFormat+= "Begin report garbage collector\r\n";
	sFormat+= GetTimeString();
	sFormat+= "deleteFiles in folders:";
	sFormat+= sFileFind;
	sFormat+= "\r\n";
	pThreadGarbageCollectorData->m_sLog += sFormat;

	iFilesFound = 0;
	iFilesDeleted = 0;
	BOOL bWorking = finder.FindFile(sFileFind);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory())
		{
			iFilesFound++;
			sFileName = finder.GetFileName();
			sFileName = sFolder + sFileName;
			CFile cfile;
			if (cfile.Open(sFileName, CFile::modeRead | CFile::shareDenyNone))
			{
				CFileStatus status;
				if(cfile.GetStatus(status))    // virtual member function
				{
					if ((status.m_attribute&CFile::readOnly) == CFile::readOnly)
					{
						cfile.Close();
						sFormat = GetTimeString();
						sFormat+="File is read only: ";
						sFormat+= sFileName;
						sFormat+= "\r\n";
						pThreadGarbageCollectorData->m_sLog += sFormat;
						pThreadGarbageCollectorData->m_bReadOnly = true;
					}
					else
					{
						tFile = status.m_mtime;
					    time_t tTime = tFile.GetTime();
						cfile.Close();

						if (tTime < tRemoveTime)
						{
							bStatus = true;
							try	{CFile::Remove(sFileName);} catch(CFileException* pEx ) {pEx->Delete();bStatus=false;}
							sFormat = GetTimeString();
							if (bStatus)
							{
								sFormat+= "Delete:";
								iFilesDeleted++;
							}
							else sFormat+="Failed to delete: ";
							sFormat+= sFileName;
							sFormat+= "\r\n";
							pThreadGarbageCollectorData->m_sLog += sFormat;
						}
					}
				}
				else cfile.Close();
			}
		}
		Sleep(100);
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT)
			{
				sFormat = GetTimeString();
				sFormat+= "Aborting....\r\n";
				pThreadGarbageCollectorData->m_sLog += sFormat;
				break;
			}
		}
	}
	sFormat.Format("Files found: %d, Files deleted: %d\r\n", iFilesFound, iFilesDeleted);
	sFormat = GetTimeString() + sFormat;
	pThreadGarbageCollectorData->m_sLog += sFormat;
	pThreadGarbageCollectorData->m_sLog += GetTimeString();
	pThreadGarbageCollectorData->m_sLog += "End garbage collector\r\n";

}

CString CThreadGarbageCollector::GetTimeString()
{
	CString sTime;
	SYSTEMTIME oT;
	::GetLocalTime(&oT);
	sTime.Format("%02d/%02d/%04d, %02d:%02d:%02d -- ",oT.wMonth,oT.wDay,oT.wYear,oT.wHour,oT.wMinute,oT.wSecond); 
	return sTime;
}


BEGIN_MESSAGE_MAP(CThreadGarbageCollector, CWinThread)
END_MESSAGE_MAP()


// CThreadGarbageCollector message handlers
