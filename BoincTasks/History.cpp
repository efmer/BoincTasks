// History.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ListViewEx.h"	
#include "TemplateRemoteHistoryView.h"
#include "threadrpc.h"
#include <direct.h>
#include "xml.h"
#include "DlgLogging.h"
#include "History.h"
#include "DlgSettingsHistory.h"
#include "DlgSettingsExpert.h"

CHistoryItem::CHistoryItem()
{
	m_pHistoryRunItem = NULL;

	m_pcComputer = NULL;
	m_pcProject = NULL;
	m_pcApplication = NULL;
	m_pcName = NULL;
	m_pcPlanClass = NULL;
	m_pcUse = NULL;
}

CHistoryItem::~CHistoryItem()
{
	if (m_pHistoryRunItem != NULL) delete m_pHistoryRunItem;

	if (m_pcComputer != NULL)	delete m_pcComputer;
	if (m_pcProject != NULL)	delete m_pcProject;
	if (m_pcApplication != NULL)delete m_pcApplication;
	if (m_pcName != NULL)		delete m_pcName;
	if (m_pcPlanClass != NULL)	delete m_pcPlanClass;
	if (m_pcUse != NULL)		delete m_pcUse;
		 
}

void CHistoryItem::AddComputer(char *pcComputer)
{
	Add(&m_pcComputer, pcComputer);
}

void CHistoryItem::AddProject(char *pcProject)
{
	Add(&m_pcProject, pcProject);
}

void CHistoryItem::AddApplication(char *pcApplication)
{
	Add(&m_pcApplication, pcApplication);
}

void CHistoryItem::AddName(char *pcName)
{
	Add(&m_pcName, pcName);
}

void CHistoryItem::AddPlanClass(char *pcPlanClass)
{
	Add(&m_pcPlanClass, pcPlanClass);
}

void CHistoryItem::AddUse(char *pcUse)
{
	Add(&m_pcUse, pcUse);
}

void CHistoryItem::Add(char **pcDest, char *pcSource)
{
	int iLen;

	if (*pcDest != NULL)
	{
		delete *pcDest;
		*pcDest = NULL;
	}

	if (pcSource == NULL) return;
	iLen = (int) strlen(pcSource) + 1;
	*pcDest = new char [iLen];
	strcpy_s(*pcDest, iLen, pcSource);
}

void CHistoryItem::CopyItem(CHistoryItem *pHistoryItemFrom)
{
	this->AddComputer(pHistoryItemFrom->m_pcComputer);
	this->AddProject(pHistoryItemFrom->m_pcProject);
	this->AddApplication(pHistoryItemFrom->m_pcApplication);
	this->m_iVersion = pHistoryItemFrom->m_iVersion;
	this->AddName(pHistoryItemFrom->m_pcName);
	this->AddPlanClass(pHistoryItemFrom->m_pcPlanClass);
	this->m_iElapsedTimeCpu = pHistoryItemFrom->m_iElapsedTimeCpu;
	this->m_iElapsedTimeGpu = pHistoryItemFrom->m_iElapsedTimeGpu;
	this->m_iState		  = pHistoryItemFrom->m_iState;
	this->m_iExitStatus	  = pHistoryItemFrom->m_iExitStatus;
	this->m_i64ReportedTime = pHistoryItemFrom->m_i64ReportedTime;
	this->m_i64CompletedTime= pHistoryItemFrom->m_i64CompletedTime;
	this->AddUse(pHistoryItemFrom->m_pcUse);
	this->m_iCheckpoints	= pHistoryItemFrom->m_iCheckpoints;
	this->m_iCheckpoint	= pHistoryItemFrom->m_iCheckpoint;
	this->m_dReceivedTime	= pHistoryItemFrom->m_dReceivedTime;
}

CHistoryCountItem::CHistoryCountItem()
{
	m_pcComputer = NULL;
	m_pcProject = NULL;
}

CHistoryCountItem::~CHistoryCountItem()
{
	if (m_pcComputer != NULL)	delete m_pcComputer;
	if (m_pcProject != NULL)	delete m_pcProject;	 
}

void CHistoryCountItem::AddComputer(char *pcComputer)
{
	Add(&m_pcComputer, pcComputer);
}

void CHistoryCountItem::AddProject(char *pcProject)
{
	Add(&m_pcProject, pcProject);
}


void CHistoryCountItem::Add(char **pcDest, char *pcSource)
{
	int iLen;

	if (*pcDest != NULL)
	{
		delete *pcDest;
		*pcDest = NULL;
	}

	if (pcSource == NULL) return;
	iLen = (int) strlen(pcSource) + 1;
	*pcDest = new char [iLen];
	strcpy_s(*pcDest, iLen, pcSource);
}

CHistoryProjectTasksItem::CHistoryProjectTasksItem()
{
	m_pcProject = NULL;

	for (int iCount = 0; iCount <= MAX_HISTORY_STATISTICS_LEN+1; iCount++)
	{
		m_iReceivedCpu[iCount] = 0;
		m_iReceivedGpu[iCount] = 0;
	}
	m_bDeleted = false;
}

CHistoryProjectTasksItem::~CHistoryProjectTasksItem()
{
	if (m_pcProject != NULL)	delete m_pcProject;

	m_pcProject = NULL;
	m_bDeleted = true;
}

void CHistoryProjectTasksItem::AddProject(char *pcProject)
{
	Add(&m_pcProject, pcProject);
}

void CHistoryProjectTasksItem::Add(char **pcDest, char *pcSource)
{
	int iLen;

	if (*pcDest != NULL)
	{
		delete *pcDest;
		*pcDest = NULL;
	}

	if (pcSource == NULL) return;
	iLen = (int) strlen(pcSource) + 1;
	*pcDest = new char [iLen];
	strcpy_s(*pcDest, iLen, pcSource);
}

CHistoryProjectDeadlineItem::CHistoryProjectDeadlineItem()
{
	m_pcProject = NULL;

	for (int iCount = 0; iCount <= MAX_HISTORY_STATISTICS_LEN+1; iCount++)
	{
		m_dReceivedCpu[iCount] = 0;
		m_dReceivedGpu[iCount] = 0;
	}
	m_iCpuItems = 0;
	m_iGpuItems = 0;

	m_bDeleted = false;
}

CHistoryProjectDeadlineItem::~CHistoryProjectDeadlineItem()
{
	if (m_pcProject != NULL)	delete m_pcProject;

	m_pcProject = NULL;
	m_bDeleted = true;
}

void CHistoryProjectDeadlineItem::AddProject(char *pcProject)
{
	Add(&m_pcProject, pcProject);
}

void CHistoryProjectDeadlineItem::Add(char **pcDest, char *pcSource)
{
	int iLen;

	if (*pcDest != NULL)
	{
		delete *pcDest;
		*pcDest = NULL;
	}

	if (pcSource == NULL) return;
	iLen = (int) strlen(pcSource) + 1;
	*pcDest = new char [iLen];
	strcpy_s(*pcDest, iLen, pcSource);
}

// CHistory

CHistory::CHistory()
{
	m_pWndLogging = theApp.m_pDlgLogging;
}

CHistory::~CHistory()
{

}


// CHistory member functions

void CHistory::CleanupLong(std::deque<CHistoryItem*> *plHistory)
{
	bool	bRemove;

	for (int iPos = 0; iPos < (int) plHistory->size(); iPos++)
	{
		if (plHistory->at(iPos)->m_iState >= STATE_UPLOADING) bRemove = false;
		else bRemove = true;
		if (plHistory->at(iPos)->m_i64ReportedTime == 0) bRemove = true;
		if (bRemove)
		{
			delete plHistory->at(iPos);
			plHistory->erase(plHistory->begin()+iPos);
		}
	}
}

bool CHistory::Write(std::deque<CHistoryItem*> *plHistory, char *pcComputerName, bool bLong)
{
//	XOSITION	pos;
	int			iPos;

	CString		sFormat, sFileName;
	CString		*psHistory;
	CHistoryItem *pHistoryItem;
	int			iNr;

	psHistory = new CString;

	sFormat.LoadString(IDS_PROGRAM_VERSION);
	*psHistory += sFormat;
	*psHistory += "\r\n";

	for (iPos = 0; iPos < (int) plHistory->size(); iPos++)
	{
		pHistoryItem = plHistory->at(iPos);
		*psHistory += pHistoryItem->m_pcComputer;
		break;										// only one is more than enough
	}
	*psHistory += "\r\n";
	*psHistory += HISTORY_ID_BEGIN;
	*psHistory += "\r\n";
	*psHistory += "Project\tApplication\tVersion Number\tName\tPlanClass\tElapsed Time Cpu\tElapsed Time Gpu\tState\tExitStatus\tReported time\tCompleted time\tUse\tReceived\tVMem\tMem\r\n";

	iNr = 0;
	for (iPos = 0; iPos < (int) plHistory->size(); iPos++)
	{
		iNr++;
		pHistoryItem = plHistory->at(iPos);
		sFormat.Format("%d",iNr); AddString(psHistory, &sFormat, true);
		AddString(psHistory, pHistoryItem->m_pcProject);
		AddString(psHistory, pHistoryItem->m_pcApplication);
		sFormat.Format("%d",pHistoryItem->m_iVersion); AddString(psHistory, &sFormat);
		AddString(psHistory, pHistoryItem->m_pcName);
		AddString(psHistory, pHistoryItem->m_pcPlanClass);
		sFormat.Format("%d",pHistoryItem->m_iElapsedTimeCpu); AddString(psHistory, &sFormat);
		sFormat.Format("%d",pHistoryItem->m_iElapsedTimeGpu); AddString(psHistory, &sFormat);
		sFormat.Format("%d",pHistoryItem->m_iState>>16); AddString(psHistory, &sFormat);
		sFormat.Format("%d",pHistoryItem->m_iExitStatus); AddString(psHistory, &sFormat);
		sFormat.Format("%d",pHistoryItem->m_i64ReportedTime); AddString(psHistory, &sFormat);
		sFormat.Format("%d",pHistoryItem->m_i64CompletedTime); AddString(psHistory, &sFormat);
		AddString(psHistory, pHistoryItem->m_pcUse);		// added V 0.56
		sFormat.Format("%f",pHistoryItem->m_dReceivedTime); AddString(psHistory, &sFormat);		// added V 0.94
		sFormat.Format("%f",pHistoryItem->m_dVirtualMemory); AddString(psHistory, &sFormat);	// added V 1.50
		sFormat.Format("%f",pHistoryItem->m_dMemory); AddString(psHistory, &sFormat);			// added V 1.50

		sFormat = "-1x\t";
//		AddString(psHistory, &sFormat);		// removed V 0.56
//		AddString(psHistory, &sFormat);		// removed v 0.94
//		AddString(psHistory, &sFormat);		// removed v 1.50
//		AddString(psHistory, &sFormat);		// removed v 1.50
		AddString(psHistory, &sFormat);
		AddString(psHistory, &sFormat);
		AddString(psHistory, &sFormat);
		AddString(psHistory, &sFormat);
		AddString(psHistory, &sFormat);
		*psHistory += "\r\n";
	}
	sFormat.Format("%d\r\n",iNr); AddString(psHistory, &sFormat, true);
	sFormat = HISTORY_ID_END;	AddString(psHistory, &sFormat, true);

	if (bLong) sFileName = HISTORY_FILENAME_LONG;
	else sFileName = HISTORY_FILENAME;
	WriteFile(&sFileName, psHistory, pcComputerName);

	delete psHistory;
	return true;
}

void CHistory::AddString(CString *psHistory, CString *psAdd, bool bFirst)
{
	if (!bFirst) *psHistory += "\t";
	*psHistory += *psAdd;
}

void CHistory::AddString(CString *psHistory, char *pcAdd, bool bFirst)
{
	if (!bFirst) *psHistory += "\t";
	*psHistory += pcAdd;
}

// format = filename_computerName

bool CHistory::WriteFile(CString *pFileName, CString *psFileText, char *pcComputerName)
{
	CString sDirectory, sFile, sFile1, sFile2, sFileName, sFileName1, sFileName2, sFileBackup, sFileExtention, sComputerName;
	char	cBuffer[128];
	tm		tmtime;
	CXml	xml("");

	sComputerName = pcComputerName;
	sComputerName.Replace(' ','_');
	sFile = *pFileName;
	sFileExtention =  "_" + sComputerName;
	sFileExtention+= ".cvs";

	sFileName = *pFileName + sFileExtention;
	sFileName1 = *pFileName + sFileExtention + "1";
	sFileName2 = *pFileName + sFileExtention + "2";

	sDirectory = xml.GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);
	sDirectory += HISTORY_FOLDER;
	_mkdir(sDirectory);
	sFile = sDirectory + sFileName;
	sFile1 = sDirectory + sFileName1;
	sFile2 = sDirectory + sFileName2;

// backup

	if (theApp.m_pDlgSettingsHistory->m_bBackupHistory)
	{
		sDirectory += HISTORY_FOLDER_BACKUP;
		_mkdir(sDirectory);

		time_t tSystemTime;
		_time64(&tSystemTime);
		localtime_s(&tmtime, &tSystemTime);
		strftime(cBuffer, 128, "%d-%m-%Y-%H-", &tmtime);

		sFileBackup = sDirectory;
		sFileBackup+= cBuffer;
		sFileBackup+= sFileName;
		try
		{
			CFile::Remove(sFileBackup);
		}
		catch(CFileException* pEx )
		{
		    pEx->Delete();
		}
		try
		{
			CFile::Rename(sFile2, sFileBackup);		// back2 -> backup folder
		}
		catch(CFileException* pEx )
		{
		    pEx->Delete();
		}
	}
	else
	{
		try
		{
			CFile::Remove(sFile2);
		}
		catch(CFileException* pEx )
		{
		    pEx->Delete();
		}
	}
// backup

	try
	{
		CFile::Rename(sFile1, sFile2);			// back1 -> back2
	}
	catch(CFileException* pEx )
	{
	    pEx->Delete();
	}
	try
	{
		CFile::Rename(sFile, sFile1);			// back0 -> back1
	}
	catch(CFileException* pEx )
	{
	    pEx->Delete();
	}

	CFileException e;
	if( !m_file.Open( sFile.GetBuffer(), CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		CString sTxt;
		sTxt.Format("Unable to write to history file: %s",sFile);
		CLoggingMessage sLog;
		sLog.m_pFrom = "CHistory::WriteFile"; sLog.m_pText = sTxt.GetBuffer(); m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
		return false;
	}

	try
	{
		m_file.Write(*psFileText,psFileText->GetLength());
		m_file.Close();
	}
	catch(CFileException* pEx )
	{
	    pEx->Delete();
	}

	return true;

}

bool CHistory::Read(char  *pcComputerName, std::deque<CHistoryItem*> *plHistory, bool bHistoryLong)
{
	CString sDirectory, sFile, sFile1, sFile2, sFileName, sFileName1, sFileName2, sFileExtention, sComputerName;
	CString *psFileText;
	CString sLogTxt;
	CLoggingMessage sLog;
	int		iStatus;

	CXml	xml("");

	psFileText = new CString;

	sComputerName = pcComputerName;
	sComputerName.Replace(' ','_');

	sFileExtention = sComputerName;
	sFileExtention+= ".cvs";

	if (bHistoryLong)
	{
		sFileName = HISTORY_FILENAME_LONG;
		sFileName1 = HISTORY_FILENAME_LONG;
		sFileName2 = HISTORY_FILENAME_LONG;
	}
	else
	{
		sFileName = HISTORY_FILENAME;
		sFileName1 = HISTORY_FILENAME;
		sFileName2 = HISTORY_FILENAME;
	}

	sFileName += "_" + sFileExtention;
	sFileName1 += "_" + sFileExtention + "1";
	sFileName2 += "_" + sFileExtention + "2";

	sDirectory = xml.GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);
	sDirectory += HISTORY_FOLDER;
//	_mkdir(sDirectory);
	sFile = sDirectory + sFileName;
	sFile1 = sDirectory + sFileName1;
	sFile2 = sDirectory + sFileName2;

	*psFileText = "";
	iStatus = ReadFileAndCHeck(pcComputerName, &sFile, psFileText, plHistory);
	if (iStatus != HISTORY_STATUS_OK)
	{
		if (iStatus != HISTORY_STATUS_FILE_NOT_FOUND)
		{
			sLogTxt.Format("Unable to read to history file: %s, copy 1",sFile);
			sLog.m_pFrom = "CHistory::ReadFile"; sLog.m_pText = sLogTxt.GetBuffer(); m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
		}
		*psFileText = "";
		iStatus = ReadFileAndCHeck(pcComputerName, &sFile1, psFileText, plHistory);
		if (iStatus != HISTORY_STATUS_OK)
		{
			if (iStatus != HISTORY_STATUS_FILE_NOT_FOUND)
			{
				sLogTxt.Format("Unable to read to history file: %s",sFile1);
				sLog.m_pFrom = "CHistory::ReadFile"; sLog.m_pText = sLogTxt.GetBuffer(); m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
			}
			*psFileText = "";
			iStatus = ReadFileAndCHeck(pcComputerName, &sFile2, psFileText, plHistory);
			if (iStatus != HISTORY_STATUS_OK)
			{
				if (iStatus != HISTORY_STATUS_FILE_NOT_FOUND)
				{
					sLogTxt.Format("Unable to read to history file: %s",sFile2);
					sLog.m_pFrom = "CHistory::ReadFile"; sLog.m_pText = sLogTxt.GetBuffer(); m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &sLog, LOGGING_NORMAL);
				}
				delete psFileText;
				return false;
			}
		}
	}
	delete psFileText;
	return true;
}

int CHistory::ReadFileAndCHeck(char *pcComputerName, CString *pFileName,  CString *psFileText, std::deque<CHistoryItem*> *plHistory)
{
	bool	sStatus = false;
	char	bBuffer	[256];
	int		iReadCount, iTotalReadCount;

	CFileException e;
	if( !m_file.Open(pFileName->GetBuffer(), CFile::modeRead, &e ) )
	{
		if (e.m_cause == CFileException::fileNotFound)	// not really an error
		{
			return HISTORY_STATUS_FILE_NOT_FOUND;
		}
		return HISTORY_STATUS_ERROR;
	}

	iTotalReadCount = 0;
	do
	{
		iReadCount = m_file.Read(bBuffer, 255);
		bBuffer[iReadCount] = 0;
		if (iReadCount > 0) *psFileText += &bBuffer[0];
		iTotalReadCount += iReadCount;
	}while (iReadCount > 0);
	
	psFileText->Replace("\n","");		// strip lf
	m_file.Close();
	return ProcessHistory(pcComputerName, psFileText, plHistory);
}

int CHistory::ProcessHistory(char *pcComputerName, CString *psFileText, std::deque<CHistoryItem*> *plHistory)
{
	int				iPosition;
	int				iPos = 0, iNrCheck = 1;
	double			dVersion;
	CString			sLineText, sComputer;
	CHistoryItem	*pHistoryItem;
	bool			bEnd;
	int				iStatus;

	if (!ProcessHistoryGetOneLine(&iPos, psFileText, &sLineText, 0)) return HISTORY_STATUS_ERROR;
	dVersion = atof(sLineText);
	if (!ProcessHistoryGetOneLine(&iPos, psFileText, &sLineText, 0)) return HISTORY_STATUS_ERROR;
	sComputer = sLineText;
	if (!ProcessHistoryGetOneLine(&iPos, psFileText, &sLineText, 0)) return HISTORY_STATUS_ERROR;
	if (sLineText != HISTORY_ID_BEGIN) return HISTORY_STATUS_ERROR;

	// ok sofar so good
	if (!ProcessHistoryGetOneLine(&iPos, psFileText, &sLineText, 0)) return HISTORY_STATUS_ERROR;		// info line only so skip

	iStatus = HISTORY_STATUS_OK;
	bEnd	= false;
	while (!bEnd)
	{
		if (!ProcessHistoryGetOneLine(&iPos, psFileText, &sLineText, 0))
		{
			iStatus = HISTORY_STATUS_ERROR;
			break;
		}
		if (!AddHistoryItem(iNrCheck, pcComputerName, &sLineText, plHistory, &bEnd))
		{
			iStatus = HISTORY_STATUS_ERROR;
			break;
		}
		iNrCheck++;
	}
	if (!ProcessHistoryGetOneLine(&iPos, psFileText, &sLineText, 0))
	if (sLineText != HISTORY_ID_END) iStatus = HISTORY_STATUS_ERROR;					// check if end marker is ok.

	if (iStatus != HISTORY_STATUS_OK)	// oeps remove everything
	{
		for (iPosition = 0; iPosition < (int) plHistory->size(); iPosition++)
		{
			pHistoryItem = plHistory->at(iPosition);
			delete pHistoryItem;
		}
		plHistory->clear();
	}

	return iStatus;
}

bool CHistory::ProcessHistoryGetOneLine(int *piPos, CString *psFileText, CString *psLineText, char cStop)
{
	int iMax;
	char cChar;
	*psLineText = "";
	iMax = 5000;

	if (psFileText->GetAt(*piPos) == '\r') (*piPos) +=1;
	if (psFileText->GetAt(*piPos) == '\t') (*piPos) +=1;

	while (iMax--)
	{
		cChar = psFileText->GetAt(*piPos);
		if (cChar == 0 || cChar == '\r' || cChar == cStop)
		{
			return true;
		}
		*psLineText += cChar;
		(*piPos) +=1;
	}
	return false;
}

bool CHistory::AddHistoryItem(int iNrCheck, char *pcComputerName, CString *psLineText, std::deque<CHistoryItem*> *plHistory, bool *pbEnd)
{
	int		iPos, iVersion, iElapsedTimeCpu, iElapsedTimeGpu, iState, iExitStatus;
	_int64	i64ReportedTime, i64CompletedTime;
	double	dReceivedTime, dVirtualMemory, dMemory;
//	bool	bGone;
	CString sItem, sProject, sApplication, sName, sPlanClass, sUse;
	CHistoryItem	*pHistoryItem;

	iPos = 0;
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;
	if (iNrCheck != atoi(sItem))
	{
		if (iNrCheck-1 != atoi(sItem)) return false;	// number repeats itself
		*pbEnd = true;
		return true;
	}
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sProject, '\t')) return false;
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sApplication, '\t')) return false;
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;
	iVersion = atoi(sItem);
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sName, '\t')) return false;
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sPlanClass, '\t')) return false;
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;
	iElapsedTimeCpu = atoi(sItem);
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;
	iElapsedTimeGpu = atoi(sItem);
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;
	iState = atoi(sItem);
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;
	iExitStatus = atoi(sItem);
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;
	i64ReportedTime = _atoi64(sItem);
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;
	i64CompletedTime = _atoi64(sItem);

	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sUse, '\t')) return false;	// added 0.56
	if (sUse == "-1x")
	{
		sUse = "?";
	}
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;	// added 0.94
	dReceivedTime = atof(sItem);
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;	// added 1.50
	dVirtualMemory = atof(sItem);
	if (!ProcessHistoryGetOneLine(&iPos, psLineText, &sItem, '\t')) return false;	// added 1.50
	dMemory = atof(sItem);

	pHistoryItem = new CHistoryItem;
	pHistoryItem->AddComputer(pcComputerName);
	pHistoryItem->AddProject(sProject.GetBuffer()); sProject.ReleaseBuffer();
	pHistoryItem->AddApplication(sApplication.GetBuffer()); sApplication.ReleaseBuffer();
	pHistoryItem->m_iVersion		= iVersion;
	pHistoryItem->AddName(sName.GetBuffer());sName.ReleaseBuffer();
	pHistoryItem->AddPlanClass(sPlanClass.GetBuffer()); sPlanClass.ReleaseBuffer();
	pHistoryItem->m_iElapsedTimeCpu = iElapsedTimeCpu;
	pHistoryItem->m_iElapsedTimeGpu	= iElapsedTimeGpu;
	pHistoryItem->m_iState			= iState<<16;
	pHistoryItem->m_iExitStatus		= iExitStatus;
	pHistoryItem->m_i64ReportedTime	= i64ReportedTime;
	pHistoryItem->m_i64CompletedTime= i64CompletedTime;
	pHistoryItem->AddUse(sUse.GetBuffer()); sUse.ReleaseBuffer();					// added 0.56

	pHistoryItem->m_iCheckpoints	= 0;					// V 0.70
	pHistoryItem->m_iCheckpoint		= 0;					// V 0.70
	pHistoryItem->m_dReceivedTime	= dReceivedTime;		// V 0.94
	pHistoryItem->m_dVirtualMemory	= dVirtualMemory;		// V 1.50
	pHistoryItem->m_dMemory			= dMemory;				// V 1.50

	plHistory->push_back(pHistoryItem);
	return true;
}
