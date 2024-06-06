// Translation.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "MultiLanguage.h"
#include "Translation.h"

//#define GetGroupDialogSettingsViewGridHorizontal	"ViewGridHorizontal"
//#define PosGroupDialogSettingsViewGridHorizontal	PosGroupDialogSettings+19
//#define GetGroupDialogSettingsViewGridVertical		"ViewGridVertical"
//#define PosGroupDialogSettingsViewGridVertical		PosGroupDialogSettings+20


char *gszTranslation[TRANSLATIONS_NR+1];

UINT gCodePage;

// CTranslation

class CMultiLanguage;
CTranslation::CTranslation()
{
	m_bOpen = false;
	m_bWarning = false;
	m_bError = false;
	m_bOverride = false;

	m_lcid = 0;
	
	for (int iCount = 0; iCount < TRANSLATIONS_NR; iCount++)
	{
		gszTranslation[iCount] = NULL;
	}
}

CTranslation::~CTranslation()
{
	for (int iCount = 0; iCount < TRANSLATIONS_NR; iCount++)
	{
		if (gszTranslation[iCount] != NULL) delete gszTranslation[iCount];
	}	
}


// CTranslation member functions

bool CTranslation::Open(CString *psLog, bool bOverride)
{
	CString		sFormat;
	m_bOverride = bOverride;

	if (m_bOpen) return true;

	if (bOverride)	m_sFile = theApp.m_pMultiLanguage->GetLanguageFile("BoincTasks", "OVERRIDE", psLog);
	else m_sFile = theApp.m_pMultiLanguage->FindLangFile("BoincTasks",0, &m_lcid, psLog);

	if (m_sFile == "" && !bOverride)
	{
		// ok nothing 
		if (m_lcid != 1033)
		{
			sFormat.Format("Couldn't open language ID %d, trying 1033 instead\r\n", m_lcid);
			*psLog += sFormat;
			m_sFile = theApp.m_pMultiLanguage->FindLangFile("BoincTasks",1033, &m_lcid, psLog);
		}
	}

	if (m_sFile != "")
	{
		if (m_file.Open(m_sFile, CFile::modeRead))
		{
			m_sLog = "Open file:" + m_sFile + "\r\n";
			m_bOpen = true;
			return true;
		}
		Sleep(2000);	// try again in 3 seconds, the file may be locked
		if (m_file.Open(m_sFile, CFile::modeRead))
		{
			m_sLog = "(second try) Open file:" + m_sFile + "\r\n";
			m_bOpen = true;
			return true;
		}
	}
	
	*psLog += "Unable to open: ";
	*psLog += m_sFile;
	*psLog += "\r\r";

	return false;
}

bool CTranslation::Read(CString *psLog)
{
	BYTE	bBuffer[101];
	CString sBuffer, sFileTxt, sFormat;

	if (!m_bOpen) return false;

	int iRead;

	iRead = 1;

	while (iRead)
	{
		iRead = m_file.Read(bBuffer, 100);
		bBuffer[iRead] = 0;
		sBuffer = bBuffer;
		sBuffer.Replace("\t","");		// strip tab
		sBuffer.Replace("\r","");		// strip cr
		sBuffer.Replace("\n","");		// strip lf
//		sBuffer.MakeLower();
		m_sText += sBuffer;
	}

	m_sText.Replace("\\r","\r");	// add \r
	m_sText.Replace("\\n","\n");	// add \n
	m_sText.Replace("%09","\t");	// %09 = tab
	m_sText.Replace("%20"," ");		// %20 = space

	m_sTextLower = m_sText;


	CharLower(m_sTextLower.GetBuffer());m_sTextLower.ReleaseBuffer();
 // 	m_sTextLower.MakeLower();
	ProcessView();

	if (m_bError)
	{
		if (m_sFile != "")
		{
			sFileTxt = m_sFile;
			sFileTxt.Replace(".btlang", "_error.txt");
			sFormat.Format("Fatal error in language file: %s\r\nLook in %s for more details", m_sFile, sFileTxt);
			*psLog += sFormat;
			*psLog += "\r\n";
			Close();
			if (m_file.Open(sFileTxt, CFile::modeCreate|CFile::modeWrite))
			{
				m_file.Write(m_sLog,m_sLog.GetLength());
				m_file.Close();
			}
			AfxMessageBox(sFormat);		
			return false;
		}
		else
		{
			sFormat = "No language file *.btlang found, install BoincTasks again!";
			*psLog += sFormat;
			AfxMessageBox(sFormat);
			return false;
		}
	}

	return true;
}

bool CTranslation::ProcessView()
{
	int iPosBegin, iPosEnd;
	CString sGroup;

	sGroup = GroupCodePage;	GetGroup(sGroup, &iPosBegin,&iPosEnd);
	if (Get(sGroup, GetCodePage, iPosBegin, iPosEnd ,PosCodePage))					// temp storage
	{
		gCodePage = (UINT) atoi(gszTranslation[PosCodePage]);
	}
	else gCodePage = 0;

	delete gszTranslation[PosCodePage];
	gszTranslation[PosCodePage] = NULL;												// released;
	Get(sGroup, GetCharSet,		iPosBegin, iPosEnd ,PosCharSet);

	sGroup = GroupViewProjects;	GetGroup(sGroup, &iPosBegin,&iPosEnd);
	Get(sGroup, GetViewProjecsRowProject,		iPosBegin, iPosEnd ,PosViewProjecsRowProject);
	Get(sGroup, GetViewProjecsRowAccount,		iPosBegin, iPosEnd ,PosViewProjecsRowAccount);
	Get(sGroup, GetViewProjecsRowTeam,			iPosBegin, iPosEnd ,PosViewProjecsRowTeam);
	Get(sGroup, GetViewProjecsRowCredits,		iPosBegin, iPosEnd ,PosViewProjecsRowCredits);
	Get(sGroup, GetViewProjecsRowAvgCredits,	iPosBegin, iPosEnd ,PosViewProjecsRowAvgCredits);
	Get(sGroup, GetViewProjecsRowHostCredits,	iPosBegin, iPosEnd ,PosViewProjecsRowHostCredits);
	Get(sGroup, GetViewProjecsRowAvgHostCredits,iPosBegin,iPosEnd ,PosViewProjecsRowAvgHostCredits);
	Get(sGroup, GetViewProjecsRowShareP,		iPosBegin, iPosEnd ,PosViewProjecsRowShareP);
	Get(sGroup, GetViewProjecsRowTasks,			iPosBegin, iPosEnd ,PosViewProjecsRowTasks);
	Get(sGroup, GetViewProjecsRowTimeLeft,		iPosBegin, iPosEnd ,PosViewProjecsRowTimeLeft);
	Get(sGroup, GetViewProjecsRowStatus,		iPosBegin, iPosEnd ,PosViewProjecsRowStatus);
	Get(sGroup, GetViewProjecsRowComputer,		iPosBegin, iPosEnd ,PosViewProjecsRowComputer);
	Get(sGroup, GetViewProjecsRowVenue,			iPosBegin, iPosEnd ,PosViewProjecsRowVenue);
	Get(sGroup, GetViewProjecsRowTasksDay,		iPosBegin, iPosEnd ,PosViewProjecsRowTasksDay);
	Get(sGroup, GetViewProjecsRowTasksWeek,		iPosBegin, iPosEnd ,PosViewProjecsRowTasksWeek);
	Get(sGroup, GetViewProjecsRowProjectRank,	iPosBegin, iPosEnd ,PosViewProjecsRowProjectRank);
	Get(sGroup, GetViewProjecsRowTeamRank,		iPosBegin, iPosEnd ,PosViewProjecsRowTeamRank);

	Get(sGroup, GetViewProjecsStatusSuspendedByGui,	iPosBegin, iPosEnd ,PosViewProjecsStatusSuspendedByGui);
	Get(sGroup, GetViewProjecsStatusNoNewTasks,		iPosBegin, iPosEnd ,PosViewProjecsStatusNoNewTasks);
	Get(sGroup, GetViewProjecsStatusDefered,		iPosBegin, iPosEnd ,PosViewProjecsStatusDefered);
	Get(sGroup, GetViewProjecsStatusUpdating,		iPosBegin, iPosEnd ,PosViewProjecsStatusUpdating);
	Get(sGroup, GetViewProjecsStatusUpdatingAM,		iPosBegin, iPosEnd ,PosViewProjecsStatusUpdatingAM);
	Get(sGroup, GetViewProjecsStatusAttaching,		iPosBegin, iPosEnd ,PosViewProjecsStatusAttaching);
	Get(sGroup, GetViewProjecsStatusInitializing,	iPosBegin, iPosEnd ,PosViewProjecsStatusInitializing);
	Get(sGroup, GetViewProjecsStatusInProgress,		iPosBegin, iPosEnd ,PosViewProjecsStatusInProgress);
	Get(sGroup, GetViewProjecsStatusReport,			iPosBegin, iPosEnd ,PosViewProjecsStatusReport);
	Get(sGroup, GetViewProjecsStatusNeedWork,		iPosBegin, iPosEnd ,PosViewProjecsStatusNeedWork);
	Get(sGroup, GetViewProjecsStatusTrickle,		iPosBegin, iPosEnd ,PosViewProjecsStatusTrickle);
	Get(sGroup, GetViewProjecsStatusProjectReq,		iPosBegin, iPosEnd ,PosViewProjecsStatusProjectReq);

	sGroup = GroupViewTasks; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetViewTasksRowProject,			iPosBegin, iPosEnd ,PosViewTasksRowProject);
	Get(sGroup, GetViewTasksRowApplication,		iPosBegin, iPosEnd ,PosViewTasksRowApplication);
	Get(sGroup, GetViewTasksRowName,			iPosBegin, iPosEnd ,PosViewTasksRowName);
	Get(sGroup, GetViewTasksRowElapsed,			iPosBegin, iPosEnd ,PosViewTasksRowElapsed);
	Get(sGroup, GetViewTasksRowCpuP,			iPosBegin, iPosEnd ,PosViewTasksRowCpuP);
	Get(sGroup, GetViewTasksRowProgress,		iPosBegin, iPosEnd ,PosViewTasksRowProgress);
	Get(sGroup, GetViewTasksRowTimeLeft,		iPosBegin, iPosEnd ,PosViewTasksRowTimeLeft);
	Get(sGroup, GetViewTasksRowDeadline,		iPosBegin, iPosEnd ,PosViewTasksRowDeadline);
	Get(sGroup, GetViewTasksRowUse,				iPosBegin, iPosEnd ,PosViewTasksRowUse);
	Get(sGroup, GetViewTasksRowStatus,			iPosBegin, iPosEnd ,PosViewTasksRowStatus);
	Get(sGroup, GetViewTasksRowThrottle,		iPosBegin, iPosEnd ,PosViewTasksRowThrottle);
	Get(sGroup, GetViewTasksRowTemperature,		iPosBegin, iPosEnd ,PosViewTasksRowTemperature);
	Get(sGroup, GetViewTasksRowComputer,		iPosBegin, iPosEnd ,PosViewTasksRowComputer);
	Get(sGroup, GetViewTasksRowCheckpoint,		iPosBegin, iPosEnd ,PosViewTasksRowCheckpoint);
	Get(sGroup, GetViewTasksRowDebt,			iPosBegin, iPosEnd ,PosViewTasksRowDebt);
	Get(sGroup, GetViewTasksRowReceived,		iPosBegin, iPosEnd ,PosViewTasksRowReceived);
	Get(sGroup, GetViewTasksRowAccount,			iPosBegin, iPosEnd ,PosViewTasksRowAccount);
	Get(sGroup, GetViewTasksRowVirtual,			iPosBegin, iPosEnd ,PosViewTasksRowVirtual);
	Get(sGroup, GetViewTasksRowMemory,			iPosBegin, iPosEnd ,PosViewTasksRowMemory);
	Get(sGroup, GetViewTasksRowShare,			iPosBegin, iPosEnd, PosViewTasksRowShare);

	Get(sGroup, GetViewTasksStatusComputation,	iPosBegin, iPosEnd ,PosViewTasksStatusComputation);
	Get(sGroup, GetViewTasksStatusDownloading,	iPosBegin, iPosEnd ,PosViewTasksStatusDownloading);

	Get(sGroup, GetViewTasksStatusDownloadError,iPosBegin, iPosEnd ,PosViewTasksStatusDownloadError);

	Get(sGroup, GetViewTasksStatusUploading,	iPosBegin, iPosEnd ,PosViewTasksStatusUploading);
	Get(sGroup, GetViewTasksStatusReadyTR,		iPosBegin, iPosEnd ,PosViewTasksStatusReadyTR);
	Get(sGroup, GetViewTasksStatusReadyTS,		iPosBegin, iPosEnd ,PosViewTasksStatusReadyTS);
	Get(sGroup, GetViewTasksStatusWaitingTR,	iPosBegin, iPosEnd ,PosViewTasksStatusWaitingTR);
	Get(sGroup, GetViewTasksStatusRunning,		iPosBegin, iPosEnd ,PosViewTasksStatusRunning);
	Get(sGroup, GetViewTasksStatusRunningHP,	iPosBegin, iPosEnd ,PosViewTasksStatusRunningHP);
	Get(sGroup, GetViewTasksStatusRunningLP,	iPosBegin, iPosEnd ,PosViewTasksStatusRunningLP);
	Get(sGroup, GetViewTasksStatusAborted,		iPosBegin, iPosEnd ,PosViewTasksStatusAborted);
	Get(sGroup, GetViewTasksStatusNew,			iPosBegin, iPosEnd ,PosViewTasksStatusNew);
	Get(sGroup, GetViewTasksStatusSuspended,	iPosBegin, iPosEnd ,PosViewTasksStatusSuspended);
	Get(sGroup, GetViewTasksSuspendPrefix,		iPosBegin, iPosEnd ,PosViewTasksSuspendPrefix);
	Get(sGroup, GetViewTasksSuspendOnBatt,		iPosBegin, iPosEnd ,PosViewTasksSuspendOnBatt);
	Get(sGroup, GetViewTasksSuspendUserAct,		iPosBegin, iPosEnd ,PosViewTasksSuspendUserAct);
	Get(sGroup, GetViewTasksSuspendUserRq,		iPosBegin, iPosEnd ,PosViewTasksSuspendUserRq);
	Get(sGroup, GetViewTasksSuspendTimeOD,		iPosBegin, iPosEnd ,PosViewTasksSuspendTimeOD);
	Get(sGroup, GetViewTasksSuspendBench,		iPosBegin, iPosEnd ,PosViewTasksSuspendBench);

	Get(sGroup, GetViewTasksSuspendDisk,		iPosBegin, iPosEnd ,PosViewTasksSuspendDisk);
	Get(sGroup, GetViewTasksSuspendNoRI,		iPosBegin, iPosEnd ,PosViewTasksSuspendNoRI);
	Get(sGroup, GetViewTasksSuspendInitial,		iPosBegin, iPosEnd ,PosViewTasksSuspendInitial);
	Get(sGroup, GetViewTasksSuspendEx,			iPosBegin, iPosEnd ,PosViewTasksSuspendEx);
	Get(sGroup, GetViewTasksSuspendCpuUsage,	iPosBegin, iPosEnd ,PosViewTasksSuspendCpuUsage);
	Get(sGroup, GetViewTasksSuspendMemory,		iPosBegin, iPosEnd ,PosViewTasksSuspendMemory);

	Get(sGroup, GetViewTasksSuspendProject,		iPosBegin, iPosEnd ,PosViewTasksSuspendProject);
	Get(sGroup, GetViewTasksWarningDeadline,	iPosBegin, iPosEnd ,PosViewTasksWarningDeadline);
	Get(sGroup, GetViewTasksInitializing,		iPosBegin, iPosEnd ,PosViewTasksInitializing);
	Get(sGroup, GetViewTasksFilterTasks,		iPosBegin, iPosEnd ,PosViewTasksFilterTasks);

	Get(sGroup, GetViewTasksStatusAbortedProject,	iPosBegin, iPosEnd ,PosViewTasksStatusAbortedProject);
	Get(sGroup, GetViewTasksStatusAbortedUser,		iPosBegin, iPosEnd ,PosViewTasksStatusAbortedUser);

	sGroup = GroupViewTransfers; GetGroup(sGroup, &iPosBegin,&iPosEnd);
	Get(sGroup, GetViewTransfersRowProject,	iPosBegin, iPosEnd ,PosViewTransfersRowProject);
	Get(sGroup, GetViewTransfersRowFile,	iPosBegin, iPosEnd ,PosViewTransfersRowFile);
	Get(sGroup, GetViewTransfersRowProgress,iPosBegin, iPosEnd ,PosViewTransfersRowProgress);
	Get(sGroup, GetViewTransfersRowSize,	iPosBegin, iPosEnd ,PosViewTransfersRowSize);
	Get(sGroup, GetViewTransfersRowElapsed,	iPosBegin, iPosEnd ,PosViewTransfersRowElapsed);
	Get(sGroup, GetViewTransfersRowSpeed,	iPosBegin, iPosEnd ,PosViewTransfersRowSpeed);
	Get(sGroup, GetViewTransfersRowStatus,	iPosBegin, iPosEnd ,PosViewTransfersRowStatus);
	Get(sGroup, GetViewTransfersRowComputer,iPosBegin, iPosEnd ,PosViewTransfersRowComputer);

	Get(sGroup, GetViewTransfersStatusUploading,	iPosBegin, iPosEnd ,PosViewTransfersStatusUploading);
	Get(sGroup, GetViewTransfersStatusDownloading,	iPosBegin, iPosEnd ,PosViewTransfersStatusDownloading);
	Get(sGroup, GetViewTransfersStatusRetry,		iPosBegin, iPosEnd ,PosViewTransfersStatusRetry);
	Get(sGroup, GetViewTransfersStatusUploadP,		iPosBegin, iPosEnd ,PosViewTransfersStatusUploadP);
	Get(sGroup, GetViewTransfersStatusDownloadP,	iPosBegin, iPosEnd ,PosViewTransfersStatusDownloadP);
	Get(sGroup, GetViewTransfersStatusRetryCount,	iPosBegin, iPosEnd ,PosViewTransfersStatusRetryCount);
	Get(sGroup, GetViewTransfersStatusProjectBack,	iPosBegin, iPosEnd ,PosViewTransfersStatusProjectBack);

	sGroup = GroupViewMessages; GetGroup(sGroup, &iPosBegin,&iPosEnd);
	Get(sGroup, GetViewMessagesRowNr,			iPosBegin, iPosEnd ,PosMessagesRowNr);
	Get(sGroup, GetViewMessagesRowProject,		iPosBegin, iPosEnd ,PosViewMessagesRowProject);
	Get(sGroup, GetViewMessagesRowTime,			iPosBegin, iPosEnd ,PosViewMessagesRowTime);
	Get(sGroup, GetViewMessagesRowMessage,		iPosBegin, iPosEnd ,PosViewMessagesRowMessage);
	Get(sGroup, GetViewMessagesRowComputer,		iPosBegin, iPosEnd ,PosViewMessagesRowComputer);
	Get(sGroup, GetViewMessagesStatusSelectOne,	iPosBegin, iPosEnd ,PosViewMessagesStatusSelectOne);

	sGroup = GroupViewHistory; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetViewHistoryRowProject,			iPosBegin, iPosEnd ,PosViewHistoryRowProject);
	Get(sGroup, GetViewHistoryRowApplication,		iPosBegin, iPosEnd ,PosViewHistoryRowApplication);
	Get(sGroup, GetViewHistoryRowName,				iPosBegin, iPosEnd ,PosViewHistoryRowName);
	Get(sGroup, GetViewHistoryRowElapsed,			iPosBegin, iPosEnd ,PosViewHistoryRowElapsed);
	Get(sGroup, GetViewHistoryRowCompleted,			iPosBegin, iPosEnd ,PosViewHistoryRowCompleted);
	Get(sGroup, GetViewHistoryRowReported,			iPosBegin, iPosEnd ,PosViewHistoryRowReported);
	Get(sGroup, GetViewHistoryRowUse,				iPosBegin, iPosEnd ,PosViewHistoryRowUse);
	Get(sGroup, GetViewHistoryRowCpuP,				iPosBegin, iPosEnd ,PosViewHistoryRowCpuP);
	Get(sGroup, GetViewHistoryRowStatus,			iPosBegin, iPosEnd ,PosViewHistoryRowStatus);
	Get(sGroup, GetViewHistoryRowComputer,			iPosBegin, iPosEnd ,PosViewHistoryRowComputer);
	Get(sGroup, GetViewHistoryRowVirtual,			iPosBegin, iPosEnd ,PosViewHistoryRowVirtual);
	Get(sGroup, GetViewHistoryRowMemory,			iPosBegin, iPosEnd ,PosViewHistoryRowMemory);

	Get(sGroup, GetViewHistoryStatusReportedOk,		iPosBegin, iPosEnd ,PosViewHistoryStatusReportedOk);
	Get(sGroup, GetViewHistoryStatusReportedAb,		iPosBegin, iPosEnd ,PosViewHistoryStatusReportedAb);
	Get(sGroup, GetViewHistoryStatusReportedCompErr,iPosBegin, iPosEnd ,PosViewHistoryStatusReportedCompErr);
	Get(sGroup, GetViewHistoryStatusCompErr,		iPosBegin, iPosEnd ,PosViewHistoryStatusCompErr);

	Get(sGroup, GetViewHistoryStatusWaitingForData,	iPosBegin, iPosEnd ,PosViewHistoryStatusWaitingForData);
	Get(sGroup, GetViewHistoryStatusToEnable,		iPosBegin, iPosEnd ,PosViewHistoryStatusToEnable);
	Get(sGroup, GetViewHistoryStatusToEnableMenu,	iPosBegin, iPosEnd ,PosViewHistoryStatusToEnableMenu);
//	Get(sGroup, GetViewHistoryStatusNotSupported,	iPosBegin, iPosEnd ,PosViewHistoryStatusNotSupported);
	Get(sGroup, GetViewHistoryMessageTooLarge,		iPosBegin, iPosEnd ,PosViewHistoryMessageTooLarge);

	Get(sGroup, GetViewHistoryMessageMissed,		iPosBegin, iPosEnd ,PosViewHistoryMessageMissed);

	sGroup = GroupViewComputer; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetViewComputerTitle,				iPosBegin, iPosEnd ,PosViewComputerTitle);
	Get(sGroup, GetViewComputerEncrypted,			iPosBegin, iPosEnd, PosViewComputerEncrypted);
	Get(sGroup, GetViewComputerIp,					iPosBegin, iPosEnd ,PosViewComputerIp);
	Get(sGroup, GetViewComputerMAC,					iPosBegin, iPosEnd ,PosViewComputerMAC);
	Get(sGroup, GetViewComputerPort,				iPosBegin, iPosEnd ,PosViewComputerPort);
	Get(sGroup, GetViewComputerPassword,			iPosBegin, iPosEnd ,PosViewComputerPassword);
	Get(sGroup, GetViewComputerGroup,				iPosBegin, iPosEnd ,PosViewComputerGroup);
	Get(sGroup, GetViewComputerBoinc,				iPosBegin, iPosEnd ,PosViewComputerBoinc);
	Get(sGroup, GetViewComputerBoincPlatform,		iPosBegin, iPosEnd ,PosViewComputerBoincPlatform);
	Get(sGroup, GetViewComputerTThrottle,			iPosBegin, iPosEnd ,PosViewComputerTThrottle);
	Get(sGroup, GetViewComputerStatus,				iPosBegin, iPosEnd ,PosViewComputerStatus);
//	Get(sGroup, GetViewComputerDaysWork,			iPosBegin, iPosEnd ,PosViewComputerDaysWork);
//	Get(sGroup, GetViewComputerWantedTaks,			iPosBegin, iPosEnd ,PosViewComputerWantedTaks);

	Get(sGroup, GetViewComputerAddNew,				iPosBegin, iPosEnd ,PosViewComputerAddNew);
	Get(sGroup, GetViewComputerAddThis,				iPosBegin, iPosEnd ,PosViewComputerAddThis);
	Get(sGroup, GetViewComputerErrorEmptyId,		iPosBegin, iPosEnd ,PosViewComputerErrorEmptyId);
	Get(sGroup, GetViewComputerErrorIdenticalId,	iPosBegin, iPosEnd ,PosViewComputerErrorIdenticalId);
	Get(sGroup, GetViewComputerApplyChanges,		iPosBegin, iPosEnd ,PosViewComputerApplyChanges);

	sGroup = GroupBarViewSelect; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetBarViewSelectHistory,			iPosBegin, iPosEnd ,PosBarViewSelectHistory);
	Get(sGroup, GetBarViewSelectMessages,			iPosBegin, iPosEnd ,PosBarViewSelectMessages);
	Get(sGroup, GetBarViewSelectTransfers,			iPosBegin, iPosEnd ,PosBarViewSelectTransfers);
	Get(sGroup, GetBarViewSelectTasks,				iPosBegin, iPosEnd ,PosBarViewSelectTasks);
	Get(sGroup, GetBarViewSelectProjects,			iPosBegin, iPosEnd ,PosBarViewSelectProjects);
	Get(sGroup, GetBarViewSelectComputers,			iPosBegin, iPosEnd ,PosBarViewSelectComputers);
	Get(sGroup, GetBarViewSelectNotices,			iPosBegin, iPosEnd ,PosBarViewSelectNotices);

	sGroup = GroupBarComputers; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetBarComputersAllComputers,		iPosBegin, iPosEnd ,PosBarComputersAllComputers);	

	sGroup = GroupBarProjects; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetBarProjectsAllProjects,		iPosBegin, iPosEnd ,PosBarProjectsAllProjects);	

	sGroup = GroupMenuComputer; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetMenuComputer,					iPosBegin, iPosEnd ,PosMenuComputer);
	Get(sGroup, GetMenuComputerDetect,				iPosBegin, iPosEnd ,PosMenuComputerDetect);
	Get(sGroup, GetMenuComputerAdd,					iPosBegin, iPosEnd ,PosMenuComputerAdd);
	Get(sGroup, GetMenuComputerRemove,				iPosBegin, iPosEnd ,PosMenuComputerRemove);
//	Get(sGroup, GetMenuComputerConnectCombined,		iPosBegin, iPosEnd ,PosMenuComputerConnectCombined);
	Get(sGroup, GetMenuComputerConnect,				iPosBegin, iPosEnd ,PosMenuComputerConnect);

	sGroup = GroupMenuMain; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetGroupMenuFile,					iPosBegin, iPosEnd ,PosGroupMenuFile);
	Get(sGroup, GetGroupMenuFileStartBoinc,			iPosBegin, iPosEnd ,PosGroupMenuFileStartBoinc);
	Get(sGroup, GetGroupMenuFileStartBoincS,		iPosBegin, iPosEnd ,PosGroupMenuFileStartBoincS);
	Get(sGroup, GetGroupMenuFileStopBoinc,			iPosBegin, iPosEnd ,PosGroupMenuFileStopBoinc);
	Get(sGroup, GetGroupMenuFileStopBoincS,			iPosBegin, iPosEnd ,PosGroupMenuFileStopBoincS);
	Get(sGroup, GetGroupMenuFileReadColorConfig,	iPosBegin, iPosEnd ,PosGroupMenuFileReadColorConfig);
	Get(sGroup, GetGroupMenuFileReadColorConfigS,	iPosBegin, iPosEnd ,PosGroupMenuFileReadColorConfigS);
	Get(sGroup, GetGroupMenuFileWriteColorConfig,	iPosBegin, iPosEnd ,PosGroupMenuFileWriteColorConfig);
	Get(sGroup, GetGroupMenuFileWriteColorConfigS,	iPosBegin, iPosEnd ,PosGroupMenuFileWriteColorConfigS);
	Get(sGroup, GetGroupMenuFileExit,				iPosBegin, iPosEnd ,PosGroupMenuFileExit);
	Get(sGroup, GetGroupMenuFileExitS,				iPosBegin, iPosEnd ,PosGroupMenuFileExitS);

	Get(sGroup, GetGroupMenuView,					iPosBegin, iPosEnd ,PosGroupMenuView);
	Get(sGroup, GetGroupMenuViewToolGraphic,		iPosBegin, iPosEnd ,PosGroupMenuViewToolGraphic);
	Get(sGroup, GetGroupMenuViewToolGraphicS,		iPosBegin, iPosEnd ,PosGroupMenuViewToolGraphicS);
	Get(sGroup, GetGroupMenuViewToolOperations,		iPosBegin, iPosEnd ,PosGroupMenuViewToolOperations);
	Get(sGroup, GetGroupMenuViewToolOperationsS,	iPosBegin, iPosEnd ,PosGroupMenuViewToolOperationsS);
	Get(sGroup, GetGroupMenuViewViewSelection,		iPosBegin, iPosEnd ,PosGroupMenuViewViewSelection);
	Get(sGroup, GetGroupMenuViewViewSelectionS,		iPosBegin, iPosEnd ,PosGroupMenuViewViewSelectionS);
	Get(sGroup, GetGroupMenuViewComputer,			iPosBegin, iPosEnd ,PosGroupMenuViewComputer);
	Get(sGroup, GetGroupMenuViewComputerS,			iPosBegin, iPosEnd ,PosGroupMenuViewComputerS);
	Get(sGroup, GetGroupMenuViewProject,			iPosBegin, iPosEnd ,PosGroupMenuViewProject);
	Get(sGroup, GetGroupMenuViewProjectS,			iPosBegin, iPosEnd ,PosGroupMenuViewProjectS);

	Get(sGroup, GetGroupMenuViewAllow,				iPosBegin, iPosEnd ,PosGroupMenuViewAllow);
	Get(sGroup, GetGroupMenuViewAllowS,				iPosBegin, iPosEnd ,PosGroupMenuViewAllowS);
	Get(sGroup, GetGroupMenuViewStatus,				iPosBegin, iPosEnd ,PosGroupMenuViewStatus);
	Get(sGroup, GetGroupMenuViewStatusS,			iPosBegin, iPosEnd ,PosGroupMenuViewStatusS);

	Get(sGroup, GetGroupMenuShow,				iPosBegin, iPosEnd ,PosGroupMenuShow);
	Get(sGroup, GetGroupMenuShowComputers,		iPosBegin, iPosEnd ,PosGroupMenuShowComputers);
	Get(sGroup, GetGroupMenuShowComputersS,		iPosBegin, iPosEnd ,PosGroupMenuShowComputersS);
	Get(sGroup, GetGroupMenuShowProjects,		iPosBegin, iPosEnd ,PosGroupMenuShowProjects);
	Get(sGroup, GetGroupMenuShowProjectsS,		iPosBegin, iPosEnd ,PosGroupMenuShowProjectsS);
	Get(sGroup, GetGroupMenuShowTasks,			iPosBegin, iPosEnd ,PosGroupMenuShowTasks);
	Get(sGroup, GetGroupMenuShowTasksS,			iPosBegin, iPosEnd ,PosGroupMenuShowTasksS);
	Get(sGroup, GetGroupMenuShowTransfers,		iPosBegin, iPosEnd ,PosGroupMenuShowTransfers);
	Get(sGroup, GetGroupMenuShowTransfersS,		iPosBegin, iPosEnd ,PosGroupMenuShowTransfersS);
	Get(sGroup, GetGroupMenuShowMessages,		iPosBegin, iPosEnd ,PosGroupMenuShowMessages);
	Get(sGroup, GetGroupMenuShowMessagesS,		iPosBegin, iPosEnd ,PosGroupMenuShowMessagesS);
	Get(sGroup, GetGroupMenuShowHistory,		iPosBegin, iPosEnd ,PosGroupMenuShowHistory);
	Get(sGroup, GetGroupMenuShowHistoryS,		iPosBegin, iPosEnd ,PosGroupMenuShowHistoryS);
	Get(sGroup, GetGroupMenuShowHistoryLong,	iPosBegin, iPosEnd ,PosGroupMenuShowHistoryLong);
	Get(sGroup, GetGroupMenuShowHistoryLongS,	iPosBegin, iPosEnd ,PosGroupMenuShowHistoryLongS);
	Get(sGroup, GetGroupMenuShowNotices,		iPosBegin, iPosEnd ,PosGroupMenuShowNotices);
	Get(sGroup, GetGroupMenuShowNoticesS,		iPosBegin, iPosEnd ,PosGroupMenuShowNoticesS);
	Get(sGroup, GetGroupMenuShowStatistics,		iPosBegin, iPosEnd ,PosGroupMenuShowStatistics);
	Get(sGroup, GetGroupMenuShowStatisticsS,	iPosBegin, iPosEnd ,PosGroupMenuShowStatisticsS);
	Get(sGroup, GetGroupMenuShowStatisticsTasks,iPosBegin, iPosEnd ,PosGroupMenuShowStatisticsTasks);
	Get(sGroup, GetGroupMenuShowStatisticsTasksS,iPosBegin, iPosEnd ,PosGroupMenuShowStatisticsTasksS);

	Get(sGroup, GetGroupMenuShowTemperature,	iPosBegin, iPosEnd ,PosGroupMenuShowTemperature);
	Get(sGroup, GetGroupMenuShowTemperatureS,	iPosBegin, iPosEnd ,PosGroupMenuShowTemperatureS);
	Get(sGroup, GetGroupMenuShowGraphTransfer,	iPosBegin, iPosEnd ,PosGroupMenuShowGraphTransfer);
	Get(sGroup, GetGroupMenuShowGraphTransferS,	iPosBegin, iPosEnd ,PosGroupMenuShowGraphTransferS);
	Get(sGroup, GetGroupMenuShowGraphDeadline,	iPosBegin, iPosEnd ,PosGroupMenuShowGraphDeadline);
	Get(sGroup, GetGroupMenuShowGraphDeadlineS,	iPosBegin, iPosEnd ,PosGroupMenuShowGraphDeadlineS);

	Get(sGroup, GetGroupMenuProjects,			iPosBegin, iPosEnd ,PosGroupMenuProjects);
	Get(sGroup, GetGroupMenuProjectsAdd,		iPosBegin, iPosEnd ,PosGroupMenuProjectsAdd);
	Get(sGroup, GetGroupMenuProjectsAddAccountM,iPosBegin, iPosEnd ,PosGroupMenuProjectsAddAccountM);
	Get(sGroup, GetGroupMenuProjectsSynchronize,iPosBegin, iPosEnd ,PosGroupMenuProjectsSynchronize);

	Get(sGroup, GetGroupMenuProjectsSetDebt,	iPosBegin, iPosEnd ,PosGroupMenuProjectsSetDebt);

	Get(sGroup, GetGroupMenuExtra,				iPosBegin, iPosEnd ,PosGroupMenuExtra);
	Get(sGroup, GetGroupMenuExtraSetting,		iPosBegin, iPosEnd ,PosGroupMenuExtraSetting);
	Get(sGroup, GetGroupMenuExtraColors,		iPosBegin, iPosEnd ,PosGroupMenuExtraColors);
	Get(sGroup, GetGroupMenuExtraPreference,	iPosBegin, iPosEnd ,PosGroupMenuExtraPreference);
	Get(sGroup, GetGroupMenuExtraProxy,			iPosBegin, iPosEnd ,PosGroupMenuExtraProxy);

	Get(sGroup, GetGroupMenuExtraUpdateAll,		iPosBegin, iPosEnd ,PosGroupMenuExtraUpdateAll);
	Get(sGroup, GetGroupMenuExtraStartup,		iPosBegin, iPosEnd ,PosGroupMenuExtraStartup);
	Get(sGroup, GetGroupMenuExtraAllowNetwork,	iPosBegin, iPosEnd ,PosGroupMenuExtraAllowNetwork);
	Get(sGroup, GetGroupMenuExtraAllowToRun,	iPosBegin, iPosEnd ,PosGroupMenuExtraAllowToRun);
	Get(sGroup, GetGroupMenuExtraAllowToRunGpu,	iPosBegin, iPosEnd ,PosGroupMenuExtraAllowToRunGpu);
	Get(sGroup, GetGroupMenuExtraBenchmark,		iPosBegin, iPosEnd ,PosGroupMenuExtraBenchmark);
	Get(sGroup, GetGroupMenuExtraBenchmarkLast,	iPosBegin, iPosEnd ,PosGroupMenuExtraBenchmarkLast);
	
	Get(sGroup, GetGroupMenuExtraEditConfig,	iPosBegin, iPosEnd ,PosGroupMenuExtraEditConfig);
	Get(sGroup, GetGroupMenuExtraEditAppInfo,	iPosBegin, iPosEnd, PosGroupMenuExtraEditAppInfo);
	Get(sGroup, GetGroupMenuExtraReadConfig,	iPosBegin, iPosEnd ,PosGroupMenuExtraReadConfig);

//	Get(sGroup, GetGroupMenuExtraSelComputer,	iPosBegin, iPosEnd, PosGroupMenuExtraSelComputer);

	Get(sGroup, GetGroupMenuWindow,				iPosBegin, iPosEnd ,PosGroupMenuWindow);
	Get(sGroup, GetGroupMenuWindowCascade,		iPosBegin, iPosEnd ,PosGroupMenuWindowCascade);
	Get(sGroup, GetGroupMenuWindowCascadeS,		iPosBegin, iPosEnd ,PosGroupMenuWindowCascadeS);
	Get(sGroup, GetGroupMenuWindowTile,			iPosBegin, iPosEnd ,PosGroupMenuWindowTile);
	Get(sGroup, GetGroupMenuWindowTileS,		iPosBegin, iPosEnd ,PosGroupMenuWindowTileS);
	Get(sGroup, GetGroupMenuWindowActivateS,	iPosBegin, iPosEnd ,PosGroupMenuWindowActivateS);

	Get(sGroup, GetGroupMenuHelp,				iPosBegin, iPosEnd ,PosGroupMenuHelp);
	Get(sGroup, GetGroupMenuHelpManual,			iPosBegin, iPosEnd ,PosGroupMenuHelpManual);
	Get(sGroup, GetGroupMenuHelpForum,			iPosBegin, iPosEnd ,PosGroupMenuHelpForum);
	Get(sGroup, GetGroupMenuHelpBoincWebsite,	iPosBegin, iPosEnd ,PosGroupMenuHelpBoincWebsite);
	Get(sGroup, GetGroupMenuHelpAbout,			iPosBegin, iPosEnd ,PosGroupMenuHelpAbout);
	Get(sGroup, GetGroupMenuHelpUpdate,			iPosBegin, iPosEnd ,PosGroupMenuHelpUpdate);
	Get(sGroup, GetGroupMenuHelpLog,			iPosBegin, iPosEnd ,PosGroupMenuHelpLog);
	Get(sGroup, GetGroupMenuHelpLogRules,		iPosBegin, iPosEnd ,PosGroupMenuHelpLogRules);

	Get(sGroup, GetGroupMenuCommonAllComputers,	iPosBegin, iPosEnd ,PosGroupMenuCommonAllComputers);
	Get(sGroup, GetGroupMenuCommonAlways,		iPosBegin, iPosEnd ,PosGroupMenuCommonAlways);
	Get(sGroup, GetGroupMenuCommonAuto,			iPosBegin, iPosEnd ,PosGroupMenuCommonAuto);
	Get(sGroup, GetGroupMenuCommonSnooze,		iPosBegin, iPosEnd ,PosGroupMenuCommonSnooze);
	Get(sGroup, GetGroupMenuCommonNever,		iPosBegin, iPosEnd ,PosGroupMenuCommonNever);

	sGroup = GroupDialogLogging; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogLoggingTitle,			iPosBegin, iPosEnd ,PosDialogLoggingTitle);
	Get(sGroup, GetDialogLoggingRulesTitle,		iPosBegin, iPosEnd ,PosDialogLoggingRulesTitle);
	Get(sGroup, GetDialogLoggingEnableDebug,	iPosBegin, iPosEnd ,PosDialogLoggingEnableDebug);

	sGroup = GroupPopUpTasks; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetGroupPopUpTasksSuspend,			iPosBegin, iPosEnd ,PosGroupPopUpTasksSuspend);
	Get(sGroup, GetGroupPopUpTasksSuspendCp,		iPosBegin, iPosEnd ,PosGroupPopUpTasksSuspendCp);
	Get(sGroup, GetGroupPopUpTasksResume,			iPosBegin, iPosEnd ,PosGroupPopUpTasksResume);
	Get(sGroup, GetGroupPopUpTasksAbort,			iPosBegin, iPosEnd ,PosGroupPopUpTasksAbort);
	Get(sGroup, GetGroupPopUpTasksPoperties,		iPosBegin, iPosEnd ,PosGroupPopUpTasksPoperties);

	Get(sGroup, GetGroupPopUpTasksProjectUpdate,	iPosBegin, iPosEnd ,PosGroupPopUpTasksProjectUpdate);
	Get(sGroup, GetGroupPopUpTasksProjectSuspend,	iPosBegin, iPosEnd ,PosGroupPopUpTasksProjectSuspend);
	Get(sGroup, GetGroupPopUpTasksProjectResume,	iPosBegin, iPosEnd ,PosGroupPopUpTasksProjectResume);
	Get(sGroup, GetGroupPopUpTasksFilter,			iPosBegin, iPosEnd ,PosGroupPopUpTasksFilter);
	Get(sGroup, GetGroupPopUpTasksFilterReadyTR,	iPosBegin, iPosEnd ,PosGroupPopUpTasksFilterReadyTR);
	Get(sGroup, GetGroupPopUpTasksFilterWaitingTR,	iPosBegin, iPosEnd ,PosGroupPopUpTasksFilterWaitingTR);
	Get(sGroup, GetGroupPopUpTasksFilterReadyTS,	iPosBegin, iPosEnd ,PosGroupPopUpTasksFilterReadyTS);
	Get(sGroup, GetGroupPopUpTasksFilterUpload,		iPosBegin, iPosEnd ,PosGroupPopUpTasksFilterUpload);
	Get(sGroup, GetGroupPopUpTasksFilterDownload,	iPosBegin, iPosEnd ,PosGroupPopUpTasksFilterDownload);
	Get(sGroup, GetGroupPopUpTasksFilterComputation,iPosBegin, iPosEnd ,PosGroupPopUpTasksFilterComputation);
	Get(sGroup, GetGroupPopUpTasksFilterSuspended,	iPosBegin, iPosEnd ,PosGroupPopUpTasksFilterSuspended);
	Get(sGroup, GetGroupPopUpTasksFilterAborted,	iPosBegin, iPosEnd ,PosGroupPopUpTasksFilterAborted);
	Get(sGroup, GetGroupPopUpTasksFilterRunning,	iPosBegin, iPosEnd ,PosGroupPopUpTasksFilterRunning);
	Get(sGroup, GetGroupPopUpTasksShowActive,		iPosBegin, iPosEnd ,PosGroupPopUpTasksShowActive);
	Get(sGroup, GetGroupPopUpTasksShowNonCpuIntensive,iPosBegin, iPosEnd ,PosGroupPopUpTasksShowNonCpuIntensive);
	Get(sGroup, GetGroupPopUpTasksShowCpu,			iPosBegin, iPosEnd ,PosGroupPopUpTasksShowCpu);
	Get(sGroup, GetGroupPopUpTasksShowGpu,			iPosBegin, iPosEnd ,PosGroupPopUpTasksShowGpu);
	Get(sGroup, GetGroupPopUpTasksShowGraphic,		iPosBegin, iPosEnd ,PosGroupPopUpTasksShowGraphic);
	Get(sGroup, GetGroupPopUpTasksAddRule,			iPosBegin, iPosEnd ,PosGroupPopUpTasksAddRule);

	sGroup = GroupPopUpProjects; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetGroupPopUpProjectsUpdate,		iPosBegin, iPosEnd ,PosGroupPopUpProjectsUpdate);
	Get(sGroup, GetGroupPopUpProjectsSuspend,		iPosBegin, iPosEnd ,PosGroupPopUpProjectsSuspend);
	Get(sGroup, GetGroupPopUpProjectsResume,		iPosBegin, iPosEnd ,PosGroupPopUpProjectsResume);
	Get(sGroup, GetGroupPopUpProjectsNoNewTasks,	iPosBegin, iPosEnd ,PosGroupPopUpProjectsNoNewTasks);
	Get(sGroup, GetGroupPopUpProjectsAllowNewTasks,	iPosBegin, iPosEnd ,PosGroupPopUpProjectsAllowNewTasks);
	Get(sGroup, GetGroupPopUpProjectsProperties,	iPosBegin, iPosEnd ,PosGroupPopUpProjectsProperties);
	Get(sGroup, GetGroupPopUpProjectsAddAddRemove,	iPosBegin, iPosEnd ,PosGroupPopUpProjectsAddAddRemove);
	Get(sGroup, GetGroupPopUpProjectsAddAddNew,		iPosBegin, iPosEnd ,PosGroupPopUpProjectsAddAddNew);
	Get(sGroup, GetGroupPopUpProjectsAddReset,		iPosBegin, iPosEnd ,PosGroupPopUpProjectsAddReset);
	Get(sGroup, GetGroupPopUpProjectsAddDetach,		iPosBegin, iPosEnd ,PosGroupPopUpProjectsAddDetach);

	sGroup = GroupPopUpTransfers; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetPopUpTransfersRetry,				iPosBegin, iPosEnd ,PosPopUpTransfersRetry);
	Get(sGroup, GetPopUpTransfersRetryAll,			iPosBegin, iPosEnd ,PosPopUpTransfersRetryAll);
	Get(sGroup, GetPopUpTransfersAbort,				iPosBegin, iPosEnd ,PosPopUpTransfersAbort);

	sGroup = GroupPopUpMessages; GetGroup(sGroup, &iPosBegin, &iPosEnd);

	Get(sGroup, GetPopUpMessagesShowOnly,			iPosBegin, iPosEnd ,PosPopUpMessagesShowOnly);
	Get(sGroup, GetPopUpMessagesShowOnlyAll,		iPosBegin, iPosEnd ,PosPopUpMessagesShowOnlyAll);
	Get(sGroup, GetPopUpMessagesIncludeGeneral,		iPosBegin, iPosEnd ,PosPopUpMessagesIncludeGeneral);
	Get(sGroup, GetPopUpMessagesCopyAll,			iPosBegin, iPosEnd ,PosPopUpMessagesCopyAll);
	Get(sGroup, GetPopUpMessagesSearch,				iPosBegin, iPosEnd ,PosPopUpMessagesSearch);
	Get(sGroup, GetPopUpMessagesCopySelected,		iPosBegin, iPosEnd ,PosPopUpMessagesCopySelected);

	sGroup = GroupPopUpTaskbar; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetPopUpTaskbarOpen,				iPosBegin, iPosEnd ,PosPopUpTaskbarOpen);
	Get(sGroup, GetPopUpTaskbarUpdate,				iPosBegin, iPosEnd ,PosPopUpTaskbarUpdate);
	Get(sGroup, GetPopUpTaskbarAbout,				iPosBegin, iPosEnd ,PosPopUpTaskbarAbout);
	Get(sGroup, GetPopUpTaskbarSnoozeCpu,			iPosBegin, iPosEnd ,PosPopUpTaskbarSnoozeCpu);
	Get(sGroup, GetPopUpTaskbarSnoozeGpu,			iPosBegin, iPosEnd ,PosPopUpTaskbarSnoozeGpu);
	Get(sGroup, GetPopUpTaskbarExit,				iPosBegin, iPosEnd ,PosPopUpTaskbarExit);

	sGroup = GroupPopUpHeader; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetPopUpHeaderEnable,				iPosBegin, iPosEnd ,PosPopUpHeaderEnable);

	sGroup = GroupPopUpComputer; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetPopUpComputerProperties,			iPosBegin, iPosEnd ,PosPopUpComputerProperties);

	sGroup = GroupToolbarGraph; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetGroupToolbarGraphCredit,			iPosBegin, iPosEnd ,PosGroupToolbarGraphCredit);
	Get(sGroup, GetGroupToolbarGraphTasks,			iPosBegin, iPosEnd ,PosGroupToolbarGraphTasks);
	Get(sGroup, GetGroupToolbarGraphTemperature,	iPosBegin, iPosEnd ,PosGroupToolbarGraphTemperature);

	sGroup = GroupToolbarOperations; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetGroupToolbarOperationsSuspend,			iPosBegin, iPosEnd ,PosGroupToolbarOperationsSuspend);
	Get(sGroup, GetGroupToolbarOperationsResume,			iPosBegin, iPosEnd ,PosGroupToolbarOperationsResume);
	Get(sGroup, GetGroupToolbarOperationsAbort,				iPosBegin, iPosEnd ,PosGroupToolbarOperationsAbort);
	Get(sGroup, GetGroupToolbarOperationsUpdateProject,		iPosBegin, iPosEnd ,PosGroupToolbarOperationsUpdateProject);
	Get(sGroup, GetGroupToolbarOperationsSuspendProject,	iPosBegin, iPosEnd ,PosGroupToolbarOperationsSuspendProject);
	Get(sGroup, GetGroupToolbarOperationsResumeProject,		iPosBegin, iPosEnd ,PosGroupToolbarOperationsResumeProject);
	Get(sGroup, GetGroupToolbarOperationsNoNewWorkProject,	iPosBegin, iPosEnd ,PosGroupToolbarOperationsNoNewWorkProject);
	Get(sGroup, GetGroupToolbarOperationsAllowNewProject,	iPosBegin, iPosEnd ,PosGroupToolbarOperationsAllowNewProject);

	sGroup = GroupToolbarAllow; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetGroupToolbarAllowNetworkPref,			iPosBegin, iPosEnd ,PosGroupToolbarAllowNetworkPref);
	Get(sGroup, GetGroupToolbarAllowNetworkNever,			iPosBegin, iPosEnd ,PosGroupToolbarAllowNetworkNever);
	Get(sGroup, GetGroupToolbarAllowCpuPref,				iPosBegin, iPosEnd ,PosGroupToolbarAllowCpuPref);
	Get(sGroup, GetGroupToolbarAllowCpuSnooze,				iPosBegin, iPosEnd ,PosGroupToolbarAllowCpuSnooze);
	Get(sGroup, GetGroupToolbarAllowCpuNever,				iPosBegin, iPosEnd ,PosGroupToolbarAllowCpuNever);
	Get(sGroup, GetGroupToolbarAllowGpuPref,				iPosBegin, iPosEnd ,PosGroupToolbarAllowGpuPref);
	Get(sGroup, GetGroupToolbarAllowGpuSnooze,				iPosBegin, iPosEnd ,PosGroupToolbarAllowGpuSnooze);
	Get(sGroup, GetGroupToolbarAllowGpuNever,				iPosBegin, iPosEnd ,PosGroupToolbarAllowGpuNever);

	sGroup = GroupDialogSettings; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetGroupDialogSettingsTitle,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTitle);
	Get(sGroup, GetGroupDialogSettingsGeneralTitle,			iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralTitle);
	Get(sGroup, GetGroupDialogSettingsGeneralStartAtLogin,	iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralStartAtLogin);
	Get(sGroup, GetGroupDialogSettingsGeneralStartBOINC,	iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralStartBOINC);
	Get(sGroup, GetGroupDialogSettingsGeneralStartBOINCDelay,iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralStartBOINCDelay);

	Get(sGroup, GetGroupDialogSettingsGeneralStopOnExit,	iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralStopOnExit);
	Get(sGroup, GetGroupDialogSettingsGeneralConTThrottle,	iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralConTThrottle);

	Get(sGroup, GetGroupDialogSettingsGeneralHideStartup,	iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralHideStartup);
	Get(sGroup, GetGroupDialogSettingsGeneralPassword,		iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralPassword);
	Get(sGroup, GetGroupDialogSettingsGeneralPasswordShow,	iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralPasswordShow);

	Get(sGroup, GetGroupDialogSettingsGeneralLanguage,		iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralLanguage);
	Get(sGroup, GetGroupDialogGeneralHotKeyMain,			iPosBegin, iPosEnd ,PosGroupDialogGeneralHotKeyMain);

	Get(sGroup, GetGroupDialogGeneralOs,					iPosBegin, iPosEnd ,PosGroupDialogGeneralOs);
	Get(sGroup, GetGroupDialogGeneralOsQuestionW,			iPosBegin, iPosEnd ,PosGroupDialogGeneralOsQuestionW);
	Get(sGroup, GetGroupDialogGeneralOsQuestionL,			iPosBegin, iPosEnd ,PosGroupDialogGeneralOsQuestionL);
	Get(sGroup, GetGroupDialogGeneralOsQuestionM,			iPosBegin, iPosEnd ,PosGroupDialogGeneralOsQuestionM);
	Get(sGroup, GetGroupDialogGeneralWarningBoinc,			iPosBegin, iPosEnd ,PosGroupDialogGeneralWarningBoinc);

	Get(sGroup, GetGroupDialogSettingsViewTitle,				iPosBegin, iPosEnd ,PosGroupDialogSettingsViewTitle);
	Get(sGroup, GetGroupDialogSettingsViewTimeFormat,			iPosBegin, iPosEnd ,PosGroupDialogSettingsViewTimeFormat);
	Get(sGroup, GetGroupDialogSettingsViewTimeFComboSystem,		iPosBegin, iPosEnd ,PosGroupDialogSettingsViewTimeFComboSystem);
	Get(sGroup, GetGroupDialogSettingsViewTimeFCombo24h,		iPosBegin, iPosEnd ,PosGroupDialogSettingsViewTimeFCombo24h);
	Get(sGroup, GetGroupDialogSettingsViewTimeFCombo12h,		iPosBegin, iPosEnd ,PosGroupDialogSettingsViewTimeFCombo12h);
	Get(sGroup, GetGroupDialogSettingsViewTimeFComboUser,		iPosBegin, iPosEnd ,PosGroupDialogSettingsViewTimeFComboUser);

	Get(sGroup, GetGroupDialogSettingsViewRefreshRate,			iPosBegin, iPosEnd ,PosGroupDialogSettingsViewRefreshRate);
	Get(sGroup, GetGroupDialogSettingsViewRRateComboSlow,		iPosBegin, iPosEnd ,PosGroupDialogSettingsViewRRateComboSlow);
	Get(sGroup, GetGroupDialogSettingsViewRRateComboNormal,		iPosBegin, iPosEnd ,PosGroupDialogSettingsViewRRateComboNormal);
	Get(sGroup, GetGroupDialogSettingsViewRRRateComboQuick,		iPosBegin, iPosEnd ,PosGroupDialogSettingsViewRRateComboQuick);
	Get(sGroup, GetGroupDialogSettingsViewRRateComboManual,		iPosBegin, iPosEnd ,PosGroupDialogSettingsViewRRateComboManual);
	Get(sGroup, GetGroupDialogSettingsViewSeconds,				iPosBegin, iPosEnd ,PosGroupDialogSettingsViewSeconds);
	Get(sGroup, GetGroupDialogSettingsViewSkin,					iPosBegin, iPosEnd ,PosGroupDialogSettingsViewSkin);
	Get(sGroup, GetGroupDialogSettingsViewGridHorizontal,		iPosBegin, iPosEnd ,PosGroupDialogSettingsViewGridHorizontal);
	Get(sGroup, GetGroupDialogSettingsViewGridVertical,			iPosBegin, iPosEnd ,PosGroupDialogSettingsViewGridVertical);
	Get(sGroup, GetGroupDialogSettingsViewNumberFormat,			iPosBegin, iPosEnd ,PosGroupDialogSettingsViewNumberFormat);
	Get(sGroup, GetGroupDialogSettingsViewPercRect,				iPosBegin, iPosEnd ,PosGroupDialogSettingsViewPercRect);
	Get(sGroup, GetGroupDialogSettingsViewAdjustTime,			iPosBegin, iPosEnd ,PosGroupDialogSettingsViewAdjustTime);
	Get(sGroup, GetGroupDialogSettingsViewTranslatable,			iPosBegin, iPosEnd ,PosGroupDialogSettingsViewTranslatable);

	Get(sGroup, GetGroupDialogSettingsProjectsTitle,			iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectsTitle);
	Get(sGroup, GetGroupDialogSettingsProjectsGroupName,		iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectsGroupName);
	Get(sGroup, GetGroupDialogSettingsProjectProject,			iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectProject);
	Get(sGroup, GetGroupDialogSettingsProjectAccount,			iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectAccount);
	Get(sGroup, GetGroupDialogSettingsProjectTeam,				iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectTeam);
	Get(sGroup, GetGroupDialogSettingsProjectUserCredits,		iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectUserCredits);
	Get(sGroup, GetGroupDialogSettingsProjectUserACredits,		iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectUserACredits);
	Get(sGroup, GetGroupDialogSettingsProjectHostCredits,		iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectHostCredits);
	Get(sGroup, GetGroupDialogSettingsProjectHostACredits,		iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectHostACredits);
	Get(sGroup, GetGroupDialogSettingsProjectShare,				iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectShare);
	Get(sGroup, GetGroupDialogSettingsProjectTasks,				iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectTasks);
	Get(sGroup, GetGroupDialogSettingsProjectTimeLeft,			iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectTimeLeft);
	Get(sGroup, GetGroupDialogSettingsProjectStatus,			iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectStatus);
	Get(sGroup, GetGroupDialogSettingsProjectComputer,			iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectComputer);
	Get(sGroup, GetGroupDialogSettingsProjectVenue,				iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectVenue);
	Get(sGroup, GetGroupDialogSettingsProjectTasksDay,			iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectTasksDay);
	Get(sGroup, GetGroupDialogSettingsProjectTasksWeek,			iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectTasksWeek);
	Get(sGroup, GetGroupDialogSettingsProjectRank,				iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectRank);
	Get(sGroup, GetGroupDialogSettingsProjectTeamRank,			iPosBegin, iPosEnd ,PosGroupDialogSettingsProjectTeamRank);

	Get(sGroup, GetGroupDialogSettingsTasksTitle,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksTitle);
	Get(sGroup, GetGroupDialogSettingsTasksGroupName,			iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksGroupName);
	Get(sGroup, GetGroupDialogSettingsTasksButtonDefault,		iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksButtonDefault);
	Get(sGroup, GetGroupDialogSettingsTasksCheckUserFriendly,	iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksCheckUserFriendly);
	Get(sGroup, GetGroupDialogSettingsTasksCheckCondense,		iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksCheckCondense);
	Get(sGroup, GetGroupDialogSettingsTasksRatioLong,			iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksRatioLong);
	Get(sGroup, GetGroupDialogSettingsTasksDeadlineRemaining,	iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksDeadlineRemaining);

	Get(sGroup, GetGroupDialogSettingsTasksApplicationSortOnNr, iPosBegin, iPosEnd, PosGroupDialogSettingsTasksApplicationSortOnNr);

	Get(sGroup, GetGroupDialogSettingsTasksSortStatus,			iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksSortStatus);
	Get(sGroup, GetGroupDialogSettingsTasksCpuPercDigits,		iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksCpuPercDigits);
	Get(sGroup, GetGroupDialogSettingsTasksCpuProgressDigits,	iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksCpuProgressDigits);
	Get(sGroup, GetGroupDialogSettingsTasksAlternatingStripes,	iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksAlternatingStripes);
	Get(sGroup, GetGroupDialogSettingsTasksMsgBoxExclude,		iPosBegin, iPosEnd ,PosGroupDialogSettingsTasksMsgBoxExclude);

	Get(sGroup, GetGroupDialogSettingsTaskProject,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskProject);
	Get(sGroup, GetGroupDialogSettingsTaskApplication,			iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskApplication);
	Get(sGroup, GetGroupDialogSettingsTaskName,					iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskName);
	Get(sGroup, GetGroupDialogSettingsTaskElapsedTime,			iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskElapsedTime);
	Get(sGroup, GetGroupDialogSettingsTaskCpuPerc,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskCpuPerc);
	Get(sGroup, GetGroupDialogSettingsTaskProgress,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskProgress);
	Get(sGroup, GetGroupDialogSettingsTaskTimeLeft,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskTimeLeft);
	Get(sGroup, GetGroupDialogSettingsTaskDeadline,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskDeadline);
	Get(sGroup, GetGroupDialogSettingsTaskUse,					iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskUse);
	Get(sGroup, GetGroupDialogSettingsTaskStatus,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskStatus);
	Get(sGroup, GetGroupDialogSettingsTaskThrottle,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskThrottle);
	Get(sGroup, GetGroupDialogSettingsTaskTemperature,			iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskTemperature);
	Get(sGroup, GetGroupDialogSettingsTaskComputer,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskComputer);
	Get(sGroup, GetGroupDialogSettingsTaskCheckpoint,			iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskCheckpoint);
	Get(sGroup, GetGroupDialogSettingsTaskDebt,					iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskDebt);
	Get(sGroup, GetGroupDialogSettingsTaskReceived,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskReceived);
	Get(sGroup, GetGroupDialogSettingsTaskAccount,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskAccount);
	Get(sGroup, GetGroupDialogSettingsTaskVirtual,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskVirtual);
	Get(sGroup, GetGroupDialogSettingsTaskMemory,				iPosBegin, iPosEnd ,PosGroupDialogSettingsTaskMemory);

	Get(sGroup, GetGroupDialogSettingsTransferTitle,			iPosBegin, iPosEnd ,PosGroupDialogSettingsTransferTitle);

	Get(sGroup, GetGroupDialogSettingsMessageTitle,				iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageTitle);
	Get(sGroup, GetGroupDialogSettingsMessagesKeep,				iPosBegin, iPosEnd ,PosGroupDialogSettingsMessagesKeep);
	Get(sGroup, GetGroupDialogSettingsMessageProject,			iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageProject);
	Get(sGroup, GetGroupDialogSettingsMessageMessage,			iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageMessage);
	Get(sGroup, GetGroupDialogSettingsMessageHighlight1,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageHighlight1);
	Get(sGroup, GetGroupDialogSettingsMessageHighlight2,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageHighlight2);
	Get(sGroup, GetGroupDialogSettingsMessageHighlight3,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageHighlight3);
	Get(sGroup, GetGroupDialogSettingsMessageHighlight4,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageHighlight4);
	Get(sGroup, GetGroupDialogSettingsMessageHighlight5,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageHighlight5);
	Get(sGroup, GetGroupDialogSettingsMessageHighlight6,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageHighlight6);
	Get(sGroup, GetGroupDialogSettingsMessageHighlight7,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageHighlight7);
	Get(sGroup, GetGroupDialogSettingsMessageHighlight8,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageHighlight8);
	Get(sGroup, GetGroupDialogSettingsMessageHighlight9,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageHighlight9);
	Get(sGroup, GetGroupDialogSettingsMessageHighlight10,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMessageHighlight10);

	Get(sGroup, GetGroupDialogSettingsHistoryTitle,				iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryTitle);

	Get(sGroup, GetGroupDialogSettingsHistoryGroupName,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryGroupName);
	Get(sGroup, GetGroupDialogSettingsHistoryProject,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryProject);
	Get(sGroup, GetGroupDialogSettingsHistoryApplication,		iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryApplication);
	Get(sGroup, GetGroupDialogSettingsHistoryName,				iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryName);
	Get(sGroup, GetGroupDialogSettingsHistoryElapsedTime,		iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryElapsedTim);
	Get(sGroup, GetGroupDialogSettingsHistoryCompleted,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryCompleted);
	Get(sGroup, GetGroupDialogSettingsHistoryReported,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryReported);
	Get(sGroup, GetGroupDialogSettingsHistoryUse,				iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryUse);
	Get(sGroup, GetGroupDialogSettingsHistoryCpuPerc,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryCpuPerc);
	Get(sGroup, GetGroupDialogSettingsHistoryStatus,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryStatus);
	Get(sGroup, GetGroupDialogSettingsHistoryComputer,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryComputer);
	Get(sGroup, GetGroupDialogSettingsHistoryVirtual,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryVirtual);
	Get(sGroup, GetGroupDialogSettingsHistoryMemory,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryMemory);

	Get(sGroup, GetGroupDialogSettingsHistoryEnable,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryEnable);
	Get(sGroup, GetGroupDialogSettingsHistorySmart,				iPosBegin, iPosEnd ,PosGroupDialogSettingsHistorySmart);
	Get(sGroup, GetGroupDialogSettingsHistoryAccurate,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryAccurate);
	Get(sGroup, GetGroupDialogSettingsHistoryRemove,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryRemove);
	Get(sGroup, GetGroupDialogSettingsHistoryRemoveDays,		iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryRemoveDays);
	Get(sGroup, GetGroupDialogSettingsHistoryRemoveHours,		iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryRemoveHours);
	Get(sGroup, GetGroupDialogSettingsHistoryDayWarning,		iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryDayWarning);
	Get(sGroup, GetGroupDialogSettingsHistoryMinUpdate,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryMinUpdate);
	Get(sGroup, GetGroupDialogSettingsHistoryMaxUpdate,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryMaxUpdate);
	Get(sGroup, GetGroupDialogSettingsHistoryMaxUpdateSec,		iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryMaxUpdateSec);
	Get(sGroup, GetGroupDialogSettingsHistoryMoveLong,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryMoveLong);
	Get(sGroup, GetGroupDialogSettingsHistoryBackup,			iPosBegin, iPosEnd ,PosGroupDialogSettingsHistoryBackup);
	
	Get(sGroup, GetGroupDialogSettingsNoticesTitle,				iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesTitle);
	Get(sGroup, GetGroupDialogSettingsNoticesHideAfter,			iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesHideAfter);
	Get(sGroup, GetGroupDialogSettingsNoticesCheckEvery,		iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesCheckEvery);
	Get(sGroup, GetGroupDialogSettingsNoticesCheckAlert,		iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesCheckAlert);
	Get(sGroup, GetGroupDialogSettingsNoticesMinutes,			iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesMinutes);
	Get(sGroup, GetGroupDialogSettingsNoticesDays,				iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesDays);
	Get(sGroup, GetGroupDialogSettingsNoticesSort,				iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesSort);
	Get(sGroup, GetGroupDialogSettingsNoticesSortCategory,		iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesSortCategory);
	Get(sGroup, GetGroupDialogSettingsNoticesSortProject,		iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesSortProject);
	Get(sGroup, GetGroupDialogSettingsNoticesSortCreation,		iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesSortCreation);
	Get(sGroup, GetGroupDialogSettingsNoticesSortArrival,		iPosBegin, iPosEnd ,PosGroupDialogSettingsNoticesSortArrival);

	Get(sGroup, GetGroupDialogSettingsWarningsTitle,		iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningsTitle);
	Get(sGroup, GetGroupDialogSettingsWarningCheckpoint,	iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningCheckpoint);
	Get(sGroup, GetGroupDialogSettingsWarningDeadline,		iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningDeadline);
	Get(sGroup, GetGroupDialogSettingsWarningDeadlineDays,	iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningDeadlineDays);
	Get(sGroup, GetGroupDialogSettingsWarningDeadlinehours,	iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningDeadlinehours);
	Get(sGroup, GetGroupDialogSettingsWarningIsActiveWhen,	iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningIsActiveWhen);
	Get(sGroup, GetGroupDialogSettingsWarningCpuBetween,	iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningCpuBetween);
	Get(sGroup, GetGroupDialogSettingsWarningGpuBetween,	iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningGpuBetween);
	Get(sGroup, GetGroupDialogSettingsWarningCpuPBetween,	iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningCpuPBetween);
	Get(sGroup, GetGroupDialogSettingsWarningCpuTasks,		iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningCpuTasks);
	Get(sGroup, GetGroupDialogSettingsWarningGpuTasks,		iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningGpuTasks);
	Get(sGroup, GetGroupDialogSettingsWarningProject,		iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningProject);
	Get(sGroup, GetGroupDialogSettingsWarningComputer,		iPosBegin, iPosEnd ,PosGroupDialogSettingsWarningComputer);

	Get(sGroup, GetGroupDialogSettingsGadgetTitle,			iPosBegin, iPosEnd ,PosGroupDialogSettingsGadgetTitle);
	Get(sGroup, GetGroupDialogSettingsGadgetModeTasks,		iPosBegin, iPosEnd ,PosGroupDialogSettingsGadgetModeTasks);
	Get(sGroup, GetGroupDialogSettingsGadgetModeComputers,	iPosBegin, iPosEnd ,PosGroupDialogSettingsGadgetModeComputers);
	Get(sGroup, GetGroupDialogSettingsGadgetRadioShowTop,	iPosBegin, iPosEnd ,PosGroupDialogSettingsGadgetRadioShowTop);
	Get(sGroup, GetGroupDialogSettingsGadgetRadioShow,		iPosBegin, iPosEnd ,PosGroupDialogSettingsGadgetRadioShow);
	Get(sGroup, GetGroupDialogSettingsGadgetRadioShowIcon,	iPosBegin, iPosEnd ,PosGroupDialogSettingsGadgetRadioShowIcon);
	Get(sGroup, GetGroupDialogSettingsGadgetRadioShowFor,	iPosBegin, iPosEnd ,PosGroupDialogSettingsGadgetRadioShowFor);

	Get(sGroup, GetGroupDialogSettingsRulesTitle,			iPosBegin, iPosEnd ,PosGroupDialogSettingsRulesTitle);

	Get(sGroup, GetGroupDialogSettingsRulesColumnComputer,		iPosBegin, iPosEnd ,PosGroupDialogSettingsRulesColumnComputer);
	Get(sGroup, GetGroupDialogSettingsRulesColumnProject,		iPosBegin, iPosEnd ,PosGroupDialogSettingsRulesColumnProject);
	Get(sGroup, GetGroupDialogSettingsRulesColumnApplication,	iPosBegin, iPosEnd ,PosGroupDialogSettingsRulesColumnApplication);
	Get(sGroup, GetGroupDialogSettingsRulesColumnRule,			iPosBegin, iPosEnd ,PosGroupDialogSettingsRulesColumnRule);
	Get(sGroup, GetGroupDialogSettingsRulesDeleteYesNo,		iPosBegin, iPosEnd ,PosGroupDialogSettingsRulesDeleteYesNo);

	Get(sGroup, GetGroupDialogSettingsCloudTitle,			iPosBegin, iPosEnd ,PosGroupDialogSettingsCloudTitle);

	Get(sGroup, GetGroupDialogSettingsWWWGroup,				iPosBegin, iPosEnd ,PosGroupDialogSettingsWWWGroup);
	Get(sGroup, GetGroupDialogSettingsWWWEnable,			iPosBegin, iPosEnd ,PosGroupDialogSettingsWWWEnable);

	Get(sGroup, GetGroupDialogSettingsMobileTitle,			iPosBegin, iPosEnd ,PosGroupDialogSettingsMobileTitle);
	Get(sGroup, GetGroupDialogSettingsMobileEnable,			iPosBegin, iPosEnd ,PosGroupDialogSettingsMobileEnable);
	Get(sGroup, GetGroupDialogSettingsMobilePassword,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMobilePassword);
	Get(sGroup, GetGroupDialogSettingsMobileHeaderFont,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMobileHeaderFont);
	Get(sGroup, GetGroupDialogSettingsMobileFont,			iPosBegin, iPosEnd ,PosGroupDialogSettingsMobileFont);
	Get(sGroup, GetGroupDialogSettingsMobileWarning,		iPosBegin, iPosEnd ,PosGroupDialogSettingsMobileWarning);

	Get(sGroup, GetGroupDialogSettingsExpertTitle,			iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertTitle);
	Get(sGroup, GetGroupDialogSettingsExpertOverride,		iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertOverride);
	Get(sGroup, GetGroupDialogSettingsExpertEncryption,		iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertEncryption);
	Get(sGroup, GetGroupDialogSettingsExpertManualCPID,		iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertManualCPID);
	Get(sGroup, GetGroupDialogSettingsExpertOverrideWarning,iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertOverrideWarning);
	Get(sGroup, GetGroupDialogSettingsExpertInvalidFolder,	iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertInvalidFolder);
	Get(sGroup, GetGroupDialogSettingsExpertReconnect,		iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertReconnect);
	Get(sGroup, GetGroupDialogSettingsExpertThreadTimeout,	iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertThreadTimeout);
	Get(sGroup, GetGroupDialogSettingsExpertSilentReboot,	iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertSilentReboot);
	Get(sGroup, GetGroupDialogSettingsExpertTimes,			iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertTimes);
	Get(sGroup, GetGroupDialogSettingsExpertTimesGraph,		iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertTimesGraph);

	Get(sGroup, GetGroupDialogSettingsExpertPriority,		iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertPriority);
	Get(sGroup, GetGroupDialogSettingsExpertPriorityNormal,	iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertPriorityNormal);
	Get(sGroup, GetGroupDialogSettingsExpertPriorityBelow,	iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertPriorityBelow);
	Get(sGroup, GetGroupDialogSettingsExpertPriorityLow,	iPosBegin, iPosEnd ,PosGroupDialogSettingsExpertPriorityLow);

	Get(sGroup, GetGroupDialogSettingsGeneralSeconds,		iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralSeconds);
	Get(sGroup, GetGroupDialogSettingsGeneralRefresh,		iPosBegin, iPosEnd ,PosGroupDialogSettingsGeneralRefresh);

	sGroup = GroupDialogColor; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogColorTitle,				iPosBegin, iPosEnd ,PosDialogColorTitle);

	Get(sGroup, GetDialogColorTitleGeneral,			iPosBegin, iPosEnd ,PosDialogColorTitleGeneral);
	Get(sGroup, GetDialogColorGeneralGrid,			iPosBegin, iPosEnd ,PosDialogColorGeneralGrid);
	Get(sGroup, GetDialogColorGeneralFont,			iPosBegin, iPosEnd ,PosDialogColorGeneralFont);
	Get(sGroup, GetDialogColorGeneralFontSelected,	iPosBegin, iPosEnd ,PosDialogColorGeneralFontSelected);
	Get(sGroup, GetDialogColorGeneralBackSelected,	iPosBegin, iPosEnd ,PosDialogColorGeneralBackSelected);
	Get(sGroup, GetDialogColorGeneralBar,			iPosBegin, iPosEnd ,PosDialogColorGeneralBar);
	Get(sGroup, GetDialogColorGeneralBarOutline,	iPosBegin, iPosEnd ,PosDialogColorGeneralBarOutline);

	Get(sGroup, GetDialogColorTitleMessages,		iPosBegin, iPosEnd ,PosDialogColorTitleMessages);
	Get(sGroup, GetDialogColorMessageNormal,		iPosBegin, iPosEnd ,PosDialogColorMessageNormal);
	Get(sGroup, GetDialogColorMessageHigh,			iPosBegin, iPosEnd ,PosDialogColorMessageHigh);
	Get(sGroup, GetDialogColorMessageHighest,		iPosBegin, iPosEnd ,PosDialogColorMessageHighest);
	Get(sGroup, GetDialogColorMessageHighlight1,	iPosBegin, iPosEnd ,PosDialogColorMessageHighlight1);
	Get(sGroup, GetDialogColorMessageHighlight2,	iPosBegin, iPosEnd ,PosDialogColorMessageHighlight2);
	Get(sGroup, GetDialogColorMessageHighlight3,	iPosBegin, iPosEnd ,PosDialogColorMessageHighlight3);
	Get(sGroup, GetDialogColorMessageHighlight4,	iPosBegin, iPosEnd ,PosDialogColorMessageHighlight4);
	Get(sGroup, GetDialogColorMessageHighlight5,	iPosBegin, iPosEnd ,PosDialogColorMessageHighlight5);
	Get(sGroup, GetDialogColorMessageHighlight6,	iPosBegin, iPosEnd ,PosDialogColorMessageHighlight6);
	Get(sGroup, GetDialogColorMessageHighlight7,	iPosBegin, iPosEnd ,PosDialogColorMessageHighlight7);
	Get(sGroup, GetDialogColorMessageHighlight8,	iPosBegin, iPosEnd ,PosDialogColorMessageHighlight8);
	Get(sGroup, GetDialogColorMessageHighlight9,	iPosBegin, iPosEnd ,PosDialogColorMessageHighlight9);
	Get(sGroup, GetDialogColorMessageHighlight10,	iPosBegin, iPosEnd ,PosDialogColorMessageHighlight10);

	Get(sGroup, GetDialogColorTitleProjects,		iPosBegin, iPosEnd ,PosDialogColorTitleProjects);
	Get(sGroup, GetDialogColorProjectsRunning,		iPosBegin, iPosEnd ,PosDialogColorProjectsRunning);
	Get(sGroup, GetDialogColorProjectsSuspended,	iPosBegin, iPosEnd ,PosDialogColorProjectsSuspended);
	Get(sGroup, GetDialogColorProjectsNoNewTasks,	iPosBegin, iPosEnd ,PosDialogColorProjectsNoNewTasks);

	Get(sGroup, GetDialogColorTitleTasks,			iPosBegin, iPosEnd ,PosDialogColorTitleTasks);
	Get(sGroup, GetDialogColorUploadDownload,		iPosBegin, iPosEnd ,PosDialogColorUploadDownload);
	Get(sGroup, GetDialogColorUploadDownloadGpu,	iPosBegin, iPosEnd ,PosDialogColorUploadDownloadGpu);
	Get(sGroup, GetDialogColorRunning,				iPosBegin, iPosEnd ,PosDialogColorRunning);
	Get(sGroup, GetDialogColorRunningGpu,			iPosBegin, iPosEnd ,PosDialogColorRunningGpu);
	Get(sGroup, GetDialogColorHighPriority,			iPosBegin, iPosEnd ,PosDialogColorHighPriority);
	Get(sGroup, GetDialogColorHighPriorityGpu,		iPosBegin, iPosEnd ,PosDialogColorHighPriorityGpu);
	Get(sGroup, GetDialogColorAbort,				iPosBegin, iPosEnd ,PosDialogColorAbort);
	Get(sGroup, GetDialogColorWaitingToRun,			iPosBegin, iPosEnd ,PosDialogColorWaitingToRun);
	Get(sGroup, GetDialogColorWaitingToRunGpu,		iPosBegin, iPosEnd ,PosDialogColorWaitingToRunGpu);
	Get(sGroup, GetDialogColorReadyToStart,			iPosBegin, iPosEnd ,PosDialogColorReadyToStart);
	Get(sGroup, GetDialogColorReadyToStartGpu,		iPosBegin, iPosEnd ,PosDialogColorReadyToStartGpu);
	Get(sGroup, GetDialogColorError,				iPosBegin, iPosEnd ,PosDialogColorError);
	Get(sGroup, GetDialogColorErrorGpu,				iPosBegin, iPosEnd ,PosDialogColorErrorGpu);
	Get(sGroup, GetDialogColorSuspended,			iPosBegin, iPosEnd ,PosDialogColorSuspended);
	Get(sGroup, GetDialogColorSuspendedGpu,			iPosBegin, iPosEnd ,PosDialogColorSuspendedGpu);
	Get(sGroup, GetDialogColorMissed,				iPosBegin, iPosEnd ,PosDialogColorMissed);

	Get(sGroup, GetDialogColorTitleFont,			iPosBegin, iPosEnd ,PosDialogColorTitleFont);
	Get(sGroup, GetDialogColorFontUseSystem,		iPosBegin, iPosEnd ,PosDialogColorFontUseSystem);
	Get(sGroup, GetDialogColorFontGroupGeneral,		iPosBegin, iPosEnd ,PosDialogColorFontGroupGeneral);
	Get(sGroup, GetDialogColorFontGroupGadget,		iPosBegin, iPosEnd ,PosDialogColorFontGroupGadget);

	Get(sGroup, GetDialogColorTitleHistory,			iPosBegin, iPosEnd ,PosDialogColorTitleHistory);
	Get(sGroup, GetDialogColorUpload,				iPosBegin, iPosEnd ,PosDialogColorUpload);
	Get(sGroup, GetDialogColorUploadGpu,			iPosBegin, iPosEnd ,PosDialogColorUploadGpu);
	Get(sGroup, GetDialogColorReadyToReport,		iPosBegin, iPosEnd ,PosDialogColorReadyToReport);
	Get(sGroup, GetDialogColorReadyToReportGpu,		iPosBegin, iPosEnd ,PosDialogColorReadyToReportGpu);
	Get(sGroup, GetDialogColorReported,				iPosBegin, iPosEnd ,PosDialogColorReported);
	Get(sGroup, GetDialogColorReportedGpu,			iPosBegin, iPosEnd ,PosDialogColorReportedGpu);
	Get(sGroup, GetDialogColorReportedError,		iPosBegin, iPosEnd ,PosDialogColorReportedError);
	Get(sGroup, GetDialogColorReportedErrorGpu,		iPosBegin, iPosEnd ,PosDialogColorReportedErrorGpu);

	Get(sGroup, GetDialogColorTitleTemperature,		iPosBegin, iPosEnd ,PosDialogColorTitleTemperature);
	Get(sGroup, GetDialogColorTemperatureCore,		iPosBegin, iPosEnd ,PosDialogColorTemperatureCore);
	Get(sGroup, GetDialogColorTemperatureGpu,		iPosBegin, iPosEnd ,PosDialogColorTemperatureGpu);
	Get(sGroup, GetDialogColorTemperatureTCpu,		iPosBegin, iPosEnd ,PosDialogColorTemperatureTCpu);
	Get(sGroup, GetDialogColorTemperatureTGpu,		iPosBegin, iPosEnd ,PosDialogColorTemperatureTGpu);
	Get(sGroup, GetDialogColorTemperatureMax,		iPosBegin, iPosEnd ,PosDialogColorTemperatureMax);
	Get(sGroup, GetDialogColorTemperatureBackground,iPosBegin, iPosEnd ,PosDialogColorTemperatureBackground);
	Get(sGroup, GetDialogColorTemperatureGridFine,	iPosBegin, iPosEnd ,PosDialogColorTemperatureGridFine);
	Get(sGroup, GetDialogColorTemperatureGrid,		iPosBegin, iPosEnd ,PosDialogColorTemperatureGrid);
	Get(sGroup, GetDialogColorTemperatureText,		iPosBegin, iPosEnd ,PosDialogColorTemperatureText);

	Get(sGroup, GetDialogColorTitleWarnings,		iPosBegin, iPosEnd ,PosDialogColorTitleWarnings);
	Get(sGroup, GetDialogColorCpuRatio,				iPosBegin, iPosEnd ,PosDialogColorCpuRatio);
	Get(sGroup, GetDialogColorGpuRatio,				iPosBegin, iPosEnd ,PosDialogColorGpuRatio);
	Get(sGroup, GetDialogColorDeadline,				iPosBegin, iPosEnd ,PosDialogColorDeadline);
	Get(sGroup, GetDialogColorDeadlineRunning,		iPosBegin, iPosEnd ,PosDialogColorDeadlineRunning);
	Get(sGroup, GetDialogColorTasks,				iPosBegin, iPosEnd ,PosDialogColorTasks);
	Get(sGroup, GetDialogColorCheckpoint,			iPosBegin, iPosEnd ,PosDialogColorCheckpoint);
	Get(sGroup, GetDialogColorReadSettingsWarning,	iPosBegin, iPosEnd ,PosDialogColorReadSettingsWarning);
	Get(sGroup, GetDialogColorWriteSettingsWarning,	iPosBegin, iPosEnd ,PosDialogColorWriteSettingsWarning);
	Get(sGroup, GetDialogColorFailedSettingsWarning,iPosBegin, iPosEnd ,PosDialogColorFailedSettingsWarning);

	sGroup = GroupDialogBoincSettings; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogBoincSettingsTitle,					iPosBegin, iPosEnd ,PosDialogBoincSettingsTitle);
	Get(sGroup, GetDialogBoincSettingsGeneralAnd,				iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralAnd);
	Get(sGroup, GetDialogBoincSettingsGeneralMinutes,			iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralMinutes);
	Get(sGroup, GetDialogBoincSettingsGeneralNoRestrictions,	iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralNoRestrictions);
	Get(sGroup, GetDialogBoincSettingsGeneralMonday,			iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralMonday);
	Get(sGroup, GetDialogBoincSettingsGeneralTuesday,			iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralTuesday);
	Get(sGroup, GetDialogBoincSettingsGeneralWednesday,			iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralWednesday);
	Get(sGroup, GetDialogBoincSettingsGeneralThursday,			iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralThursday);
	Get(sGroup, GetDialogBoincSettingsGeneralFriday,			iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralFriday);
	Get(sGroup, GetDialogBoincSettingsGeneralSaturday,			iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralSaturday);
	Get(sGroup, GetDialogBoincSettingsGeneralSunday,			iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralSunday);
	Get(sGroup, GetDialogBoincSettingsGeneralHourMinutes,		iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralHourMinutes);
	Get(sGroup, GetDialogBoincSettingsGeneralGraphic,			iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralGraphic);
	Get(sGroup, GetDialogBoincSettingsGeneralKBsec,				iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralKBsec);
//	Get(sGroup, GetDialogBoincSettingsGeneralDays,				iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralDays);
	Get(sGroup, GetDialogBoincSettingsGeneralDayOfWeekTitle,	iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralDayOfWeekTitle);

	Get(sGroup, GetDialogBoincSettingsGeneralDayBetween,		iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralDayBetween);
	Get(sGroup, GetDialogBoincSettingsGeneralSeconds,			iPosBegin, iPosEnd ,PosDialogBoincSettingsGeneralSeconds);
	Get(sGroup, GetDialogBoincSettingsTitleProcessor,			iPosBegin, iPosEnd ,PosDialogBoincSettingsTitleProcessor);
	Get(sGroup, GetDialogBoincSettingsPComputingTitle,			iPosBegin, iPosEnd ,PosDialogBoincSettingsPComputingTitle);
	Get(sGroup, GetDialogBoincSettingsPComputingBatteries,		iPosBegin, iPosEnd ,PosDialogBoincSettingsPComputingBatteries);
	Get(sGroup, GetDialogBoincSettingsPComputingUseCpu,			iPosBegin, iPosEnd ,PosDialogBoincSettingsPComputingUseCpu);
	Get(sGroup, GetDialogBoincSettingsPComputingUseGpu,			iPosBegin, iPosEnd ,PosDialogBoincSettingsPComputingUseGpu);
	Get(sGroup, GetDialogBoincSettingsPComputingOnlyIdle,		iPosBegin, iPosEnd ,PosDialogBoincSettingsPComputingOnlyIdle);
	Get(sGroup, GetDialogBoincSettingsPComputingUsageLess,		iPosBegin, iPosEnd ,PosDialogBoincSettingsPComputingUsageLess);
	Get(sGroup, GetDialogBoincSettingsPComputingUsageLessP,		iPosBegin, iPosEnd ,PosDialogBoincSettingsPComputingUsageLessP);

	Get(sGroup, GetDialogBoincSettingsPOtherTitle,				iPosBegin, iPosEnd ,PosDialogBoincSettingsPOtherTitle);
	Get(sGroup, GetDialogBoincSettingsPOtherSwitchEvery,		iPosBegin, iPosEnd ,PosDialogBoincSettingsPOtherSwitchEvery);
	Get(sGroup, GetDialogBoincSettingsPOtherMultiprocessors,	iPosBegin, iPosEnd ,PosDialogBoincSettingsPOtherMultiprocessors);
	Get(sGroup, GetDialogBoincSettingsPOtherUseAtMost,			iPosBegin, iPosEnd ,PosDialogBoincSettingsPOtherUseAtMost);
	Get(sGroup, GetDialogBoincSettingsPOtherPercProcessor,		iPosBegin, iPosEnd ,PosDialogBoincSettingsPOtherPercProcessor);
	Get(sGroup, GetDialogBoincSettingsPOtherPercCpu,			iPosBegin, iPosEnd ,PosDialogBoincSettingsPOtherPercCpu);
	Get(sGroup, GetDialogBoincSettingsTitleNetwork,				iPosBegin, iPosEnd ,PosDialogBoincSettingsTitleNetwork);
	Get(sGroup, GetDialogBoincSettingsNetGeneralTitle,			iPosBegin, iPosEnd ,PosDialogBoincSettingsNetGeneralTitle);
	Get(sGroup, GetDialogBoincSettingsNetGeneralMaxDownload,	iPosBegin, iPosEnd ,PosDialogBoincSettingsNetGeneralMaxDownload);
	Get(sGroup, GetDialogBoincSettingsNGeneralMaxUpload,		iPosBegin, iPosEnd ,PosDialogBoincSettingsNGeneralMaxUpload);
	Get(sGroup, GetDialogBoincSettingsTransferAtMost,			iPosBegin, iPosEnd ,PosDialogBoincSettingsTransferAtMost);
	Get(sGroup, GetDialogBoincSettingsTransferAtMostDays,		iPosBegin, iPosEnd ,PosDialogBoincSettingsTransferAtMostDays);

	Get(sGroup, GetDialogBoincSettingsMBytesEvery,				iPosBegin, iPosEnd ,PosDialogBoincSettingsMBytesEvery);
	Get(sGroup, GetDialogBoincSettingsNGeneralConnectEvery,		iPosBegin, iPosEnd ,PosDialogBoincSettingsNGeneralConnectEvery);
	Get(sGroup, GetDialogBoincSettingsNGeneralConnectEveryDays,	iPosBegin, iPosEnd ,PosDialogBoincSettingsNGeneralConnectEveryDays);

	Get(sGroup, GetDialogBoincSettingsNGeneralMinWorkBuffer,	iPosBegin, iPosEnd ,PosDialogBoincSettingsNGeneralMinWorkBuffer);
	Get(sGroup, GetDialogBoincSettingsNGeneralWorkBuffer,		iPosBegin, iPosEnd ,PosDialogBoincSettingsNGeneralWorkBuffer);
	Get(sGroup, GetDialogBoincSettingsNetGeneralWURegulator,	iPosBegin, iPosEnd ,PosDialogBoincSettingsNetGeneralWURegulator);
	Get(sGroup, GetDialogBoincSettingsNetGeneralDaysMax,		iPosBegin, iPosEnd ,PosDialogBoincSettingsNetGeneralDaysMax);

	Get(sGroup, GetDialogBoincSettingsNetGeneralSkipImage,		iPosBegin, iPosEnd ,PosDialogBoincSettingsNetGeneralSkipImage);
	Get(sGroup, GetDialogBoincSettingsNetConnectionTitle,		iPosBegin, iPosEnd ,PosDialogBoincSettingsNetConnectionTitle);
	Get(sGroup, GetDialogBoincSettingsNetConnectionConfirm,		iPosBegin, iPosEnd ,PosDialogBoincSettingsNetConnectionConfirm);
	Get(sGroup, GetDialogBoincSettingsNetConnectionDWD,			iPosBegin, iPosEnd ,PosDialogBoincSettingsNetConnectionDWD);
	Get(sGroup, GetDialogBoincSettingsNetNetworkTitle,			iPosBegin, iPosEnd ,PosDialogBoincSettingsNetNetworkTitle);
	Get(sGroup, GetDialogBoincSettingsTitleDisk,				iPosBegin, iPosEnd ,PosDialogBoincSettingsTitleDisk);
	Get(sGroup, GetDialogBoincSettingsDiskDiskTitle,			iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskDiskTitle);
	Get(sGroup, GetDialogBoincSettingsDiskDiskAtMost,			iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskDiskAtMost);
	Get(sGroup, GetDialogBoincSettingsDiskDiskAtLeast,			iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskDiskAtLeast);
	Get(sGroup, GetDialogBoincSettingsDiskDiskWriteEvery,		iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskDiskWriteEvery);
	Get(sGroup, GetDialogBoincSettingsDiskDiskGigaDisk,			iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskDiskGigaDisk);
	Get(sGroup, GetDialogBoincSettingsDiskDiskGigaDiskFree,		iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskDiskGigaDiskFree);
	Get(sGroup, GetDialogBoincSettingsDiskDiskGigaDiskTotal,	iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskDiskGigaDiskTotal);
	Get(sGroup, GetDialogBoincSettingsDiskDiskPage,				iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskDiskPage);
	Get(sGroup, GetDialogBoincSettingsDiskMemoryTitle,			iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskMemoryTitle);
	Get(sGroup, GetDialogBoincSettingsDiskMemoryInUse,			iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskMemoryInUse);
	Get(sGroup, GetDialogBoincSettingsDiskMemoryIdle,			iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskMemoryIdle);
	Get(sGroup, GetDialogBoincSettingsDiskMemoryLeaveIn,		iPosBegin, iPosEnd ,PosDialogBoincSettingsDiskMemoryLeaveIn);

	Get(sGroup, GetDialogBoincSettingsExclusiveTitle,			iPosBegin, iPosEnd ,PosDialogBoincSettingsExclusiveTitle);
	Get(sGroup, GetDialogBoincSettingsExclusiveTitleAdd,		iPosBegin, iPosEnd ,PosDialogBoincSettingsExclusiveTitleAdd);
	Get(sGroup, GetDialogBoincSettingsExclusiveSelect,			iPosBegin, iPosEnd ,PosDialogBoincSettingsExclusiveSelect);
	Get(sGroup, GetDialogBoincSettingsExclusiveGpu,				iPosBegin, iPosEnd ,PosDialogBoincSettingsExclusiveGpu);

	Get(sGroup, GetDialogBoincSettingsButtonClearWarning,		iPosBegin, iPosEnd ,PosDialogBoincSettingsButtonClearWarning);

	sGroup = GroupDialogGraphic; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogGraphicTitle,					iPosBegin, iPosEnd ,PosDialogGraphicTitle);
	Get(sGroup, GetDialogGraphicTitleComputers,			iPosBegin, iPosEnd ,PosDialogGraphicTitleComputers);
	Get(sGroup, GetDialogGraphicTitleProjects,			iPosBegin, iPosEnd ,PosDialogGraphicTitleProjects);
	Get(sGroup, GetDialogGraphicRadioUser,				iPosBegin, iPosEnd ,PosDialogGraphicRadioUser);
	Get(sGroup, GetDialogGraphicRadioComputer,			iPosBegin, iPosEnd ,PosDialogGraphicRadioComputer);
	Get(sGroup, GetDialogGraphicRadioTotal,				iPosBegin, iPosEnd ,PosDialogGraphicRadioTotal);
	Get(sGroup, GetDialogGraphicRadioAverage,			iPosBegin, iPosEnd ,PosDialogGraphicRadioAverage);
	Get(sGroup, GetDialogGraphicButtonUpdate,			iPosBegin, iPosEnd ,PosDialogGraphicButtonUpdate);
	Get(sGroup, GetDialogGraphicEmptyComputer,			iPosBegin, iPosEnd ,PosDialogGraphicEmptyComputer);
	Get(sGroup, GetDialogGraphicEmptyProject,			iPosBegin, iPosEnd ,PosDialogGraphicEmptyProject);
	Get(sGroup, GetDialogGraphicNoData,					iPosBegin, iPosEnd ,PosDialogGraphicNoData);
	Get(sGroup, GetDialogGraphicDoubleClick,			iPosBegin, iPosEnd ,PosDialogGraphicDoubleClick);

	Get(sGroup, GetDialogGraphicMultiple,				iPosBegin, iPosEnd ,PosDialogGraphicMultiple);
	Get(sGroup, GetDialogGraphicAverage,				iPosBegin, iPosEnd ,PosDialogGraphicAverage);

	Get(sGroup, GetDialogGraphicLineThickness,			iPosBegin, iPosEnd ,PosDialogGraphicLineThickness);
	Get(sGroup, GetDialogGraphicPeriod,					iPosBegin, iPosEnd ,PosDialogGraphicPeriod);
	Get(sGroup, GetDialogGraphicPeriodDays,				iPosBegin, iPosEnd ,PosDialogGraphicPeriodDays);
	Get(sGroup, GetDialogGraphicCombineProject,			iPosBegin, iPosEnd ,PosDialogGraphicCombineProject);
	Get(sGroup, GetDialogGraphicExpanded,				iPosBegin, iPosEnd ,PosDialogGraphicExpanded);

	sGroup = GroupDialogGraphicTasks; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogGraphicTasksTitle,				iPosBegin, iPosEnd ,PosDialogGraphicTasksTitle);

	sGroup = GroupDialogGraphicDeadline; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogGraphicDeadlineTitle,			iPosBegin, iPosEnd ,PosDialogGraphicDeadlineTitle);

	sGroup = GroupDialogGraphicTransfer; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogGraphicTransferTitle,			iPosBegin, iPosEnd ,PosDialogGraphicTransferTitle);
	Get(sGroup, GetDialogGraphicTransferUp,				iPosBegin, iPosEnd ,PosDialogGraphicTransferUp);
	Get(sGroup, GetDialogGraphicTransferDown,			iPosBegin, iPosEnd ,PosDialogGraphicTransferDown);


	sGroup = GroupDialogHistoryLong; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogHistoryLongTitle,				iPosBegin, iPosEnd ,PosDialogHistoryLongTitle);

	sGroup = GroupDialogGraphicColors; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogGraphicColorsTitle,			iPosBegin, iPosEnd ,PosDialogGraphicColorsTitle);
	Get(sGroup, GetDialogGraphicColorsButtonBackground,	iPosBegin, iPosEnd ,PosDialogGraphicColorsButtonBackground);
	Get(sGroup, GetDialogGraphicColorsButtonProjectText,iPosBegin, iPosEnd ,PosDialogGraphicColorsButtonProjectText);
	Get(sGroup, GetDialogGraphicColorsButtonScaleText,	iPosBegin, iPosEnd ,PosDialogGraphicColorsButtonScaleText);
	Get(sGroup, GetDialogGraphicColorsButtonGrid,		iPosBegin, iPosEnd ,PosDialogGraphicColorsButtonGrid);
	Get(sGroup, GetDialogGraphicColorsButtonColor,		iPosBegin, iPosEnd ,PosDialogGraphicColorsButtonColor);
	Get(sGroup, GetDialogGraphicColorFixed,				iPosBegin, iPosEnd ,PosDialogGraphicColorFixed);
	Get(sGroup, GetDialogGraphicColorsAverage,			iPosBegin, iPosEnd ,PosDialogGraphicColorsAverage);

	sGroup = GroupDialogTemperatureGraphic; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogTemperatureGraphicTitle,		iPosBegin, iPosEnd ,PosDialogTemperatureGraphicTitle);
	Get(sGroup, GetDialogTemperatureGraphicC1Minute,	iPosBegin, iPosEnd ,PosDialogTemperatureGraphicC1Minute);
	Get(sGroup, GetDialogTemperatureGraphicC5Minute,	iPosBegin, iPosEnd ,PosDialogTemperatureGraphicC5Minute);
	Get(sGroup, GetDialogTemperatureGraphicC10Minute,	iPosBegin, iPosEnd ,PosDialogTemperatureGraphicC10Minute);
	Get(sGroup, GetDialogTemperatureGraphicC30Minute,	iPosBegin, iPosEnd ,PosDialogTemperatureGraphicC30Minute);
	Get(sGroup, GetDialogTemperatureGraphicC60Minute,	iPosBegin, iPosEnd ,PosDialogTemperatureGraphicC60Minute);
	Get(sGroup, GetDialogTemperatureGraphicC3Hour,		iPosBegin, iPosEnd ,PosDialogTemperatureGraphicC3Hour);
	Get(sGroup, GetDialogTemperatureGraphicC6Hour,		iPosBegin, iPosEnd ,PosDialogTemperatureGraphicC6Hour);
	Get(sGroup, GetDialogTemperatureGraphicC12Hour,		iPosBegin, iPosEnd ,PosDialogTemperatureGraphicC12Hour);
	Get(sGroup, GetDialogTemperatureGraphicC24Hour,		iPosBegin, iPosEnd ,PosDialogTemperatureGraphicC24Hour);
	Get(sGroup, GetDialogTemperatureGraphicCore,		iPosBegin, iPosEnd ,PosDialogTemperatureGraphicCore);
	Get(sGroup, GetDialogTemperatureGraphicGpu,			iPosBegin, iPosEnd ,PosDialogTemperatureGraphicGpu);
	Get(sGroup, GetDialogTemperatureGraphicCpuPerc,		iPosBegin, iPosEnd ,PosDialogTemperatureGraphicCpuPerc);
	Get(sGroup, GetDialogTemperatureGraphicGpuPerc,		iPosBegin, iPosEnd ,PosDialogTemperatureGraphicGpuPerc);
	Get(sGroup, GetDialogTemperatureGraphicCpuMax,		iPosBegin, iPosEnd ,PosDialogTemperatureGraphicCpuMax);
	Get(sGroup, GetDialogTemperatureGraphicGpuMax,		iPosBegin, iPosEnd ,PosDialogTemperatureGraphicGpuMax);

	sGroup = GroupDialogThreadGraphic; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogThreadGraphicTitle,			iPosBegin, iPosEnd ,PosDialogThreadGraphicTitle);

	sGroup = GroupDialogTThrottle; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogTThrottleTitleComputer,		iPosBegin, iPosEnd ,PosDialogTThrottleTitleComputer);
	Get(sGroup, GetDialogTThrottleTitleTemperature,		iPosBegin, iPosEnd ,PosDialogTThrottleTitleTemperature);
	Get(sGroup, GetDialogTThrottleTitleRunPercentage,	iPosBegin, iPosEnd ,PosDialogTThrottleTitleRunPercentage);
	Get(sGroup, GetDialogTThrottleCore,					iPosBegin, iPosEnd ,PosDialogTThrottleCore);
	Get(sGroup, GetDialogTThrottleSetCore,				iPosBegin, iPosEnd ,PosDialogTThrottleSetCore);
	Get(sGroup, GetDialogTThrottleGpu,					iPosBegin, iPosEnd ,PosDialogTThrottleGpu);
	Get(sGroup, GetDialogTThrottleSetGpu,				iPosBegin, iPosEnd ,PosDialogTThrottleSetGpu);
	Get(sGroup, GetDialogTThrottleRunningCpu,			iPosBegin, iPosEnd ,PosDialogTThrottleRunningCpu);
	Get(sGroup, GetDialogTThrottleMaxCpu,				iPosBegin, iPosEnd ,PosDialogTThrottleMaxCpu);
	Get(sGroup, GetDialogTThrottleMinCpu,				iPosBegin, iPosEnd ,PosDialogTThrottleMinCpu);
	Get(sGroup, GetDialogTThrottleRunningGpu,			iPosBegin, iPosEnd ,PosDialogTThrottleRunningGpu);
	Get(sGroup, GetDialogTThrottleAutoActive,			iPosBegin, iPosEnd ,PosDialogTThrottleAutoActive);

	sGroup = GroupDialogSetDebt; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogSetDebtTitle,					iPosBegin, iPosEnd ,PosDialogSetDebtTitle);
	Get(sGroup, GetDialogSetDebtCpuLong,				iPosBegin, iPosEnd ,PosDialogSetDebtCpuLong);
	Get(sGroup, GetDialogSetDebtNVidiaLong,				iPosBegin, iPosEnd ,PosDialogSetDebtNVidiaLong);
	Get(sGroup, GetDialogSetDebtAtiLong,				iPosBegin, iPosEnd ,PosDialogSetDebtAtiLong);
	Get(sGroup, GetDialogSetDebtCpuShort,				iPosBegin, iPosEnd ,PosDialogSetDebtCpuShort);
	Get(sGroup, GetDialogSetDebtNVidiaShort,			iPosBegin, iPosEnd ,PosDialogSetDebtNVidiaShort);
	Get(sGroup, GetDialogSetDebtAtiShort,				iPosBegin, iPosEnd ,PosDialogSetDebtAtiShort);

	sGroup = GroupDialogAddProject; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogAddProjectTitle,				iPosBegin, iPosEnd ,PosDialogAddProjectTitle);
	Get(sGroup, GetDialogAddProjectMain,				iPosBegin, iPosEnd ,PosDialogAddProjectMain);
	Get(sGroup, GetDialogAddProjectLoginName,			iPosBegin, iPosEnd ,PosDialogAddProjectLoginName);
	Get(sGroup, GetDialogAddProjectPassword,			iPosBegin, iPosEnd ,PosDialogAddProjectPassword);
	Get(sGroup, GetDialogAddProjectUserName,			iPosBegin, iPosEnd ,PosDialogAddProjectUserName);
	Get(sGroup, GetDialogAddProjectCreateWhile,			iPosBegin, iPosEnd ,PosDialogAddProjectCreateWhile);
	Get(sGroup, GetDialogAddProjectComputers,			iPosBegin, iPosEnd ,PosDialogAddProjectComputers);
	Get(sGroup, GetDialogAddProjectMsgInvalidLoginName,	iPosBegin, iPosEnd ,PosDialogAddProjectMsgInvalidLoginName);
	Get(sGroup, GetDialogAddProjectMsgPasswordsEqual,	iPosBegin, iPosEnd ,PosDialogAddProjectMsgPasswordsEqual);
	Get(sGroup, GetDialogAddProjectMsgUsernameMissing,	iPosBegin, iPosEnd ,PosDialogAddProjectMsgUsernameMissing);
	Get(sGroup, GetDialogAddProjectMsgProjectUrlShort,	iPosBegin, iPosEnd ,PosDialogAddProjectMsgProjectUrlShort);
	Get(sGroup, GetDialogAddProjectMsgBusy,				iPosBegin, iPosEnd ,PosDialogAddProjectMsgBusy);
	Get(sGroup, GetDialogAddProjectMsgAddedSuccessfully,iPosBegin, iPosEnd ,PosDialogAddProjectMsgAddedSuccessfully);
	Get(sGroup, GetDialogAddProjectMsgErrorCreating,	iPosBegin, iPosEnd ,PosDialogAddProjectMsgErrorCreating);
	Get(sGroup, GetDialogAddProjectMsgErrorUrl,			iPosBegin, iPosEnd ,PosDialogAddProjectMsgErrorUrl);
	Get(sGroup, GetDialogAddProjectMsgErrorFailedContact,iPosBegin, iPosEnd ,PosDialogAddProjectMsgErrorFailedContact);
	Get(sGroup, GetDialogAddProjectMsgErrorPassword,	iPosBegin, iPosEnd ,PosDialogAddProjectMsgErrorPassword);
	Get(sGroup, GetDialogAddProjectMsgErrorAlreadyAtt,	iPosBegin, iPosEnd ,PosDialogAddProjectMsgErrorAlreadyAtt);
	Get(sGroup, GetDialogAddProjectMsgErrorNotDatabase,	iPosBegin, iPosEnd ,PosDialogAddProjectMsgErrorNotDatabase);
	Get(sGroup, GetDialogAddProjectMsgErrorUnableConnect,iPosBegin, iPosEnd ,PosDialogAddProjectMsgErrorUnableConnect);
	Get(sGroup, GetDialogAddProjectMsgErrorNotFound,	iPosBegin, iPosEnd ,PosDialogAddProjectMsgErrorNotFound);
	Get(sGroup, GetDialogAddProjectMsgErrorEmail,	iPosBegin, iPosEnd ,PosDialogAddProjectMsgErrorEmail);

	Get(sGroup, GetDialogAddProjectButtonWeb,			iPosBegin, iPosEnd ,PosDialogAddProjectButtonWeb);
	Get(sGroup, GetDialogAddProjectButtonAdd,			iPosBegin, iPosEnd ,PosDialogAddProjectButtonAdd);

	sGroup = GroupDialogAddAccountManager; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogAddAccountManagerTitle,		iPosBegin, iPosEnd ,PosDialogAddAccountManagerTitle);

	Get(sGroup, GetDialogAddAccountManagerUserName,		iPosBegin, iPosEnd ,PosDialogAddAccountManagerUserName);
	Get(sGroup, GetDialogAddAccountManagerPassword,		iPosBegin, iPosEnd ,PosDialogAddAccountManagerPassword);
	Get(sGroup, GetDialogAddAccountManagerButtonWeb,	iPosBegin, iPosEnd ,PosDialogAddAccountManagerButtonWeb);
	Get(sGroup, GetDialogAddAccountManagerButtonAdd,	iPosBegin, iPosEnd ,PosDialogAddAccountManagerButtonAdd);
	Get(sGroup, GetDialogAddAccountManagerButtonRemove,	iPosBegin, iPosEnd ,PosDialogAddAccountManagerButtonRemove);
	Get(sGroup, GetDialogAddAccountManagerAlready,		iPosBegin, iPosEnd ,PosDialogAddAccountManagerAlready);

	sGroup = GroupDialogProjectProperties; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogProjectPropComputer,			iPosBegin, iPosEnd ,PosDialogProjectPropComputer);
	Get(sGroup, GetDialogProjectPropTitleProjectP,		iPosBegin, iPosEnd ,PosDialogProjectPropTitleProjectP);
	Get(sGroup, GetDialogProjectPropProject,			iPosBegin, iPosEnd ,PosDialogProjectPropProject);
	Get(sGroup, GetDialogProjectPropGeneral,			iPosBegin, iPosEnd ,PosDialogProjectPropGeneral);
	Get(sGroup, GetDialogProjectPropMasterUrl,			iPosBegin, iPosEnd ,PosDialogProjectPropMasterUrl);
	Get(sGroup, GetDialogProjectPropUserName,			iPosBegin, iPosEnd ,PosDialogProjectPropUserName);
	Get(sGroup, GetDialogProjectPropTeamName,			iPosBegin, iPosEnd ,PosDialogProjectPropTeamName);
	Get(sGroup, GetPosDialogProjectPropVenue,			iPosBegin, iPosEnd ,PosDialogProjectPropVenue);
	Get(sGroup, GetPosDialogProjectPropTasksCompleted,	iPosBegin, iPosEnd ,PosDialogProjectPropTasksCompleted);
	Get(sGroup, GetPosDialogProjectTasksError,			iPosBegin, iPosEnd ,PosDialogProjectTasksError);
	Get(sGroup, GetDialogProjectPropResourseShare,		iPosBegin, iPosEnd ,PosDialogProjectPropResourseShare);
	Get(sGroup, GetDialogProjectPropFileUploadDeferred,	iPosBegin, iPosEnd ,PosDialogProjectPropFileUploadDeferred);
	Get(sGroup, GetDialogProjectPropFileDownloadDeferred,iPosBegin, iPosEnd ,PosDialogProjectPropFileDownloadDeferred);
	Get(sGroup, GetDialogProjectPropComputerId,			iPosBegin,iPosEnd ,PosDialogProjectPropComputerId);
	Get(sGroup, GetDialogProjectPropProjectId,			iPosBegin,iPosEnd ,PosDialogProjectPropProjectId);
	Get(sGroup, GetDialogProjectPropNonCpuIntensive,	iPosBegin, iPosEnd ,PosDialogProjectPropNonCpuIntensive);
	Get(sGroup, GetDialogProjectPropSuspendedGui,		iPosBegin, iPosEnd ,PosDialogProjectPropSuspendedGui);
	Get(sGroup, GetDialogProjectPropDontReqWork,		iPosBegin, iPosEnd ,PosDialogProjectPropDontReqWork);
	Get(sGroup, GetDialogProjectPropSchedulerInProg,	iPosBegin, iPosEnd ,PosDialogProjectPropSchedulerInProg);
	Get(sGroup, GetDialogProjectPropAttachedAcountM,	iPosBegin, iPosEnd ,PosDialogProjectPropAttachedAcountM);
	Get(sGroup, GetDialogProjectPropDetachWD,			iPosBegin, iPosEnd ,PosDialogProjectPropDetachWD);
	Get(sGroup, GetDialogProjectPropEnded,				iPosBegin, iPosEnd ,PosDialogProjectPropEnded);
	Get(sGroup, GetDialogProjectPropCredit,				iPosBegin, iPosEnd ,PosDialogProjectPropCredit);
	Get(sGroup, GetDialogProjectPropUserCredit,			iPosBegin, iPosEnd ,PosDialogProjectPropUserCredit);
	Get(sGroup, GetDialogProjectPropHostCredit,			iPosBegin, iPosEnd ,PosDialogProjectPropHostCredit);
	Get(sGroup, GetDialogProjectPropMore,				iPosBegin, iPosEnd ,PosDialogProjectPropMore);
	Get(sGroup, GetDialogProjectPropDurationF,			iPosBegin, iPosEnd ,PosDialogProjectPropDurationF);
	Get(sGroup, GetDialogProjectPropSchedulingPriority,	iPosBegin, iPosEnd ,PosDialogProjectPropSchedulingPriority);
	Get(sGroup, GetDialogProjectPropCpuBackoff,			iPosBegin, iPosEnd ,PosDialogProjectPropCpuBackoff);
	Get(sGroup, GetDialogProjectPropCpuDontFetch,		iPosBegin, iPosEnd ,PosDialogProjectPropCpuDontFetch);
	Get(sGroup, GetDialogProjectPropCpuBackoffInterval,	iPosBegin, iPosEnd ,PosDialogProjectPropCpuBackoffInterval);
	Get(sGroup, GetDialogProjectPropCudaBackoff,		iPosBegin, iPosEnd ,PosDialogProjectPropCudaBackoff);
	Get(sGroup, GetDialogProjectPropCudaDontFetch,		iPosBegin, iPosEnd ,PosDialogProjectPropCudaDontFetch);
	Get(sGroup, GetDialogProjectPropAtiBackoff,			iPosBegin, iPosEnd ,PosDialogProjectPropAtiBackoff);
	Get(sGroup, GetDialogProjectPropAtiDontFetch,		iPosBegin, iPosEnd ,PosDialogProjectPropAtiDontFetch);
	Get(sGroup, GetDialogProjectPropSchedulePending,	iPosBegin, iPosEnd ,PosDialogProjectPropSchedulePending);
	Get(sGroup, GetDialogProjectPropCpuShortTimeDebt,	iPosBegin, iPosEnd ,PosDialogProjectPropCpuShortTimeDebt);
	Get(sGroup, GetDialogProjectPropCpuLongTimeDebt,	iPosBegin, iPosEnd ,PosDialogProjectPropCpuLongTimeDebt);
	Get(sGroup, GetDialogProjectPropCudaShortTimeDebt,	iPosBegin, iPosEnd ,PosDialogProjectPropCudaShortTimeDebt);
	Get(sGroup, GetDialogProjectPropCudaLongTimeDebt,	iPosBegin, iPosEnd ,PosDialogProjectPropCudaLongTimeDebt);
	Get(sGroup, GetDialogProjectPropAtiShortTimeDebt,	iPosBegin, iPosEnd ,PosDialogProjectPropAtiShortTimeDebt);
	Get(sGroup, GetDialogProjectPropAtiLongTimeDebt,	iPosBegin, iPosEnd ,PosDialogProjectPropAtiLongTimeDebt);
	Get(sGroup, GetDialogProjectPropDisk,				iPosBegin, iPosEnd ,PosDialogProjectPropDisk);
	Get(sGroup, GetDialogProjectPropDiskUse,			iPosBegin, iPosEnd ,PosDialogProjectPropDiskUse);
	Get(sGroup, GetDialogProjectPropDiskAll,			iPosBegin, iPosEnd ,PosDialogProjectPropDiskAll);
	Get(sGroup, GetDialogProjectPropDiskAllowed,		iPosBegin, iPosEnd ,PosDialogProjectPropDiskAllowed);
	Get(sGroup, GetDialogProjectPropDiskBoinc,			iPosBegin, iPosEnd ,PosDialogProjectPropDiskBoinc);
	Get(sGroup, GetDialogProjectPropDiskFree,			iPosBegin, iPosEnd ,PosDialogProjectPropDiskFree);
	Get(sGroup, GetDialogProjectPropDiskTotal,			iPosBegin, iPosEnd ,PosDialogProjectPropDiskTotal);
	Get(sGroup, GetDialogProjectPropTotal,				iPosBegin, iPosEnd ,PosDialogProjectPropTotal);
	Get(sGroup, GetDialogProjectPropAverage,			iPosBegin, iPosEnd ,PosDialogProjectPropAverage);
	Get(sGroup, GetDialogProjectPropYes,				iPosBegin, iPosEnd ,PosDialogProjectPropYes);
	Get(sGroup, GetDialogProjectPropNo,					iPosBegin, iPosEnd ,PosDialogProjectPropNo);

	sGroup = GroupDialogTaskProperties; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogTaskPropComputer,			iPosBegin, iPosEnd ,PosDialogTaskPropComputer);
	Get(sGroup, GetDialogTaskPropTitleProjectP,		iPosBegin, iPosEnd ,PosDialogTaskPropTitleProjectP);
	Get(sGroup, GetDialogTaskPropProject,			iPosBegin, iPosEnd ,PosDialogTaskPropProject);
	Get(sGroup, GetDialogTaskPropApplication,		iPosBegin, iPosEnd ,PosDialogTaskPropApplication);
	Get(sGroup, GetDialogTaskPropName,				iPosBegin, iPosEnd ,PosDialogTaskPropName);
	Get(sGroup, GetDialogTaskPropWorkunitName,		iPosBegin, iPosEnd ,PosDialogTaskPropWorkunitName);
	Get(sGroup, GetDialogTaskPropState,				iPosBegin, iPosEnd ,PosDialogTaskPropState);
	Get(sGroup, GetDialogTaskPropRecieved,			iPosBegin, iPosEnd ,PosDialogTaskPropRecieved);
	Get(sGroup, GetDialogTaskPropDeadline,			iPosBegin, iPosEnd ,PosDialogTaskPropDeadline);
	Get(sGroup, GetDialogTaskPropEstAppSpeed,		iPosBegin, iPosEnd ,PosDialogTaskPropEstAppSpeed);
	Get(sGroup, GetDialogTaskPropEstTaskSize,		iPosBegin, iPosEnd ,PosDialogTaskPropEstTaskSize);
	Get(sGroup, GetDialogTaskPropResources,			iPosBegin, iPosEnd ,PosDialogTaskPropResources);
	Get(sGroup, GetDialogTaskPropCheckpointTime,	iPosBegin, iPosEnd ,PosDialogTaskPropCheckpointTime);
	Get(sGroup, GetDialogTaskPropCpuTime,			iPosBegin, iPosEnd ,PosDialogTaskPropCpuTime);
	Get(sGroup, GetDialogTaskPropElapsedTime,		iPosBegin, iPosEnd ,PosDialogTaskPropElapsedTime);
	Get(sGroup, GetDialogTaskPropEstTimeRemaining,	iPosBegin, iPosEnd ,PosDialogTaskPropEstTimeRemaining);
	Get(sGroup, GetDialogTaskPropFractionDone,		iPosBegin, iPosEnd ,PosDialogTaskPropFractionDone);
	Get(sGroup, GetDialogTaskPropVirtualMemory,		iPosBegin, iPosEnd ,PosDialogTaskPropVirtualMemory);
	Get(sGroup, GetDialogTaskPropWorkingSetSize,	iPosBegin, iPosEnd ,PosDialogTaskPropWorkingSetSize);
	Get(sGroup, GetDialogTaskPropDirectoy,			iPosBegin, iPosEnd ,PosDialogTaskPropDirectoy);
	Get(sGroup, GetDialogTaskPropPid,				iPosBegin, iPosEnd ,PosDialogTaskPropPid);

	sGroup = GroupDialogComputerProperties; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogComputerPropComputer,		iPosBegin, iPosEnd ,PosDialogComputerPropComputer);
	Get(sGroup, GetDialogComputerPropTitle,			iPosBegin, iPosEnd ,PosDialogComputerPropTitle);
	Get(sGroup, GetDialogComputerPropCPU,			iPosBegin, iPosEnd ,PosDialogComputerPropCPU);
	Get(sGroup, GetDialogComputerPropOs,			iPosBegin, iPosEnd ,PosDialogComputerPropOs);
	Get(sGroup, GetDialogComputerPropMemory,		iPosBegin, iPosEnd ,PosDialogComputerPropMemory);
	Get(sGroup, GetDialogComputerPropDisk,			iPosBegin, iPosEnd ,PosDialogComputerPropDisk);
	Get(sGroup, GetDialogComputerPropGPU,			iPosBegin, iPosEnd ,PosDialogComputerPropGPU);

	Get(sGroup, GetDialogComputerPropTime,			iPosBegin, iPosEnd ,PosDialogComputerPropTime);
	Get(sGroup, GetDialogComputerPropTimeBoinc,		iPosBegin, iPosEnd ,PosDialogComputerPropTimeBoinc);
	Get(sGroup, GetDialogComputerPropTimeInternet,	iPosBegin, iPosEnd ,PosDialogComputerPropTimeInternet);
	Get(sGroup, GetDialogComputerPropTimeCPU,		iPosBegin, iPosEnd ,PosDialogComputerPropTimeCPU);
	Get(sGroup, GetDialogComputerPropTimeGPU,		iPosBegin, iPosEnd ,PosDialogComputerPropTimeGPU);
	Get(sGroup, GetDialogComputerPropTimeStart,		iPosBegin, iPosEnd ,PosDialogComputerPropTimeStart);
	Get(sGroup, GetDialogComputerPropTimeUpTime,	iPosBegin, iPosEnd ,PosDialogComputerPropTimeUpTime);

	sGroup = GroupDialogProjectOperation; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogProjectOQuestionReset,			iPosBegin, iPosEnd ,PosDialogProjectOQuestionReset);
	Get(sGroup, GetDialogProjectOQuestionDetach,		iPosBegin, iPosEnd ,PosDialogProjectOQuestionDetach);
//	Get(sGroup, GetDialogProjectOMsgBoxSelectProject,	iPosBegin, iPosEnd ,PosDialogProjectOMsgBoxSelectProject);

	sGroup = GroupDialogTaskAbort; GetGroup(GroupDialogTaskAbort,	&iPosBegin, &iPosEnd);
	Get(sGroup, GetDialogTaskAbortColumnTask,			iPosBegin, iPosEnd ,PosDialogTaskAbortColumnTask);
	Get(sGroup, GetDialogTaskAbortColumnProject,		iPosBegin, iPosEnd ,PosDialogTaskAbortColumnProject);
	Get(sGroup, GetDialogTaskAbortColumnDeadline,		iPosBegin, iPosEnd ,PosDialogTaskAbortColumnDeadline);
	Get(sGroup, GetDialogTaskAbortColumnState,			iPosBegin, iPosEnd ,PosDialogTaskAbortColumnState);
	Get(sGroup, GetDialogTaskAbortColumnComputer,		iPosBegin, iPosEnd ,PosDialogTaskAbortColumnComputer);
	Get(sGroup, GetDialogTaskAbortQuestionAbort,		iPosBegin, iPosEnd ,PosDialogTaskAbortQuestionAbort);

	sGroup = GroupDialogTransferAbort; GetGroup(sGroup,	&iPosBegin, &iPosEnd);
	Get(sGroup,	GetDialogTransferAbortQuestionAbort,	iPosBegin, iPosEnd ,PosDialogTransferAbortQuestionAbort);

	sGroup = GroupDialogSearchComputer; GetGroup(sGroup,	&iPosBegin, &iPosEnd);
	Get(sGroup,	GetDialogSearchComputerTitle,				iPosBegin, iPosEnd ,PosDialogSearchComputerTitle);
	Get(sGroup,	GetDialogSearchComputerComputer,			iPosBegin, iPosEnd ,PosDialogSearchComputerComputer);
	Get(sGroup,	GetDialogSearchComputerIP,					iPosBegin, iPosEnd ,PosDialogSearchComputerIP);
	Get(sGroup,	GetDialogSearchComputerAddress,				iPosBegin, iPosEnd ,PosDialogSearchComputerAddress);
	Get(sGroup,	GetDialogSearchComputerPassword,			iPosBegin, iPosEnd ,PosDialogSearchComputerPassword);
	Get(sGroup,	GetDialogSearchComputerCopyIp,				iPosBegin, iPosEnd ,PosDialogSearchComputerCopyIp);
	Get(sGroup,	GetDialogSearchComputerCopyMAC,				iPosBegin, iPosEnd ,PosDialogSearchComputerCopyMAC);
	Get(sGroup,	GetDialogSearchComputerCopyAll,				iPosBegin, iPosEnd ,PosDialogSearchComputerCopyAll);

	Get(sGroup,	GetDialogSearchComputerButtonAddressRange,	iPosBegin, iPosEnd ,PosDialogSearchComputerButtonAddressRange);
	Get(sGroup,	GetDialogSearchComputerButtonStop,			iPosBegin, iPosEnd ,PosDialogSearchComputerButtonStop);
	Get(sGroup,	GetDialogSearchComputerButtonSearchAgain,	iPosBegin, iPosEnd ,PosDialogSearchComputerButtonSearchAgain);
	Get(sGroup,	GetDialogSearchComputerButtonAdd,			iPosBegin, iPosEnd ,PosDialogSearchComputerButtonAdd);
	Get(sGroup,	GetDialogSearchComputerYes,					iPosBegin, iPosEnd ,PosDialogSearchComputerYes);

	sGroup = GroupDialogAbout; GetGroup(sGroup,	&iPosBegin, &iPosEnd);
	Get(sGroup,	GetDialogAboutTitle,						iPosBegin, iPosEnd ,PosDialogAboutTitle);
	Get(sGroup,	GetDialogAboutText1,						iPosBegin, iPosEnd ,PosDialogAboutText1);
	Get(sGroup,	GetDialogAboutTextProgrammer,				iPosBegin, iPosEnd ,PosDialogAboutTextProgrammer);
	Get(sGroup,	GetDialogAboutTextTesters,					iPosBegin, iPosEnd ,PosDialogAboutTextTesters);
	Get(sGroup,	GetDialogAboutTextSupport,					iPosBegin, iPosEnd ,PosDialogAboutTextSupport);
	Get(sGroup,	GetDialogAboutText2,						iPosBegin, iPosEnd ,PosDialogAboutText2);
	Get(sGroup,	GetDialogAboutCredits,						iPosBegin, iPosEnd ,PosDialogAboutCredits);
	Get(sGroup,	GetDialogAboutCreditsLink,					iPosBegin, iPosEnd ,PosDialogAboutCreditsLink);
	Get(sGroup,	GetDialogAboutCreditsLinkHttp,				iPosBegin, iPosEnd ,PosDialogAboutCreditsLinkHttp);

	sGroup = GroupDialogUpdate; GetGroup(sGroup,	&iPosBegin, &iPosEnd);
	Get(sGroup,	GetDialogUpdateTitle,						iPosBegin, iPosEnd ,PosDialogUpdateTitle);
	Get(sGroup,	GetDialogUpdateCheckNew,					iPosBegin, iPosEnd ,PosDialogUpdateCheckNew);
	Get(sGroup,	GetDialogUpdateUsesHttp,					iPosBegin, iPosEnd ,PosDialogUpdateUsesHttp);
	Get(sGroup,	GetDialogUpdateCurrentVersion,				iPosBegin, iPosEnd ,PosDialogUpdateCurrentVersion);
	Get(sGroup,	GetDialogUpdateLatestVersion,				iPosBegin, iPosEnd ,PosDialogUpdateLatestVersion);
	Get(sGroup,	GetDialogUpdateLatestBetaVersion,			iPosBegin, iPosEnd ,PosDialogUpdateLatestBetaVersion);
	Get(sGroup,	GetDialogUpdateNewVersionFound,				iPosBegin, iPosEnd ,PosDialogUpdateNewVersionFound);
	Get(sGroup,	GetDialogUpdateNewBetaVersionFound,			iPosBegin, iPosEnd ,PosDialogUpdateNewBetaVersionFound);
	Get(sGroup,	GetDialogUpdateClickUpdate,					iPosBegin, iPosEnd ,PosDialogUpdateClickUpdate);
	Get(sGroup,	GetDialogUpdateAlreadyCurrent,				iPosBegin, iPosEnd ,PosDialogUpdateAlreadyCurrent);
	Get(sGroup,	GetDialogUpdateCantConnect,					iPosBegin, iPosEnd ,PosDialogUpdateCantConnect);
	Get(sGroup,	GetDialogUpdateCheckManually,				iPosBegin, iPosEnd ,PosDialogUpdateCheckManually);

	Get(sGroup,	GetDialogUpdateDirect,						iPosBegin, iPosEnd ,PosDialogUpdateDirect);
	Get(sGroup,	GetDialogUpdateLocation,					iPosBegin, iPosEnd ,PosDialogUpdateLocation);
	Get(sGroup,	GetDialogUpdateEurope,						iPosBegin, iPosEnd ,PosDialogUpdateEurope);
	Get(sGroup,	GetDialogUpdateNorthAmerica,				iPosBegin, iPosEnd ,PosDialogUpdateNorthAmerica);

	Get(sGroup,	GetDialogUpdateButtonCheck,					iPosBegin, iPosEnd ,PosDialogUpdateButtonCheck);
	Get(sGroup,	GetDialogUpdateButtonUpdate,				iPosBegin, iPosEnd ,PosDialogUpdateButtonUpdate);
	Get(sGroup,	GetDialogUpdateCheckAuto,					iPosBegin, iPosEnd ,PosDialogUpdateCheckAuto);
	Get(sGroup,	GetDialogUpdateCheckBeta,					iPosBegin, iPosEnd ,PosDialogUpdateCheckBeta);

	Get(sGroup,	GetDialogUpdateCheckUseBrowser,				iPosBegin, iPosEnd ,PosDialogUpdateCheckUseBrowser);
	Get(sGroup,	GetDialogUpdateTempFolder,					iPosBegin, iPosEnd ,PosDialogUpdateTempFolder);
	Get(sGroup,	GetDialogUpdateUnableToDownload,			iPosBegin, iPosEnd ,PosDialogUpdateUnableToDownload);
	Get(sGroup,	GetDialogUpdateUnableToStore,				iPosBegin, iPosEnd ,PosDialogUpdateUnableToStore);

	sGroup = GroupDialogProxy; GetGroup(sGroup,	&iPosBegin, &iPosEnd);
	Get(sGroup,	GetDialogProxyTitle,						iPosBegin, iPosEnd ,PosDialogProxyTitle);
	Get(sGroup,	GetDialogProxyHttpTitle,					iPosBegin, iPosEnd ,PosDialogProxyHttpTitle);
	Get(sGroup,	GetDialogProxyHttpConnect,					iPosBegin, iPosEnd ,PosDialogProxyHttpConnect);
	Get(sGroup,	GetDialogProxyHttpGroup,					iPosBegin, iPosEnd ,PosDialogProxyHttpGroup);
	Get(sGroup,	GetDialogProxyHttpServer,					iPosBegin, iPosEnd ,PosDialogProxyHttpServer);
	Get(sGroup,	GetDialogProxyHttpPort,						iPosBegin, iPosEnd ,PosDialogProxyHttpPort);
	Get(sGroup,	GetDialogProxyHttpNoFor,					iPosBegin, iPosEnd ,PosDialogProxyHttpNoFor);
	Get(sGroup,	GetDialogProxyHttpGroupUser,				iPosBegin, iPosEnd ,PosDialogProxyHttpGroupUser);
	Get(sGroup,	GetDialogProxyHttpUser,						iPosBegin, iPosEnd ,PosDialogProxyHttpUser);
	Get(sGroup,	GetDialogProxyHttpPassword,					iPosBegin, iPosEnd ,PosDialogProxyHttpPassword);

	Get(sGroup,	GetDialogProxySocksTitle,					iPosBegin, iPosEnd ,PosDialogProxySocksTitle);
	Get(sGroup,	GetDialogProxySocksConnect,					iPosBegin, iPosEnd ,PosDialogProxySocksConnect);
	Get(sGroup,	GetDialogProxySocksGroup,					iPosBegin, iPosEnd ,PosDialogProxySocksGroup);
	Get(sGroup,	GetDialogProxySocksServer,					iPosBegin, iPosEnd ,PosDialogProxySocksServer);
	Get(sGroup,	GetDialogProxySocksPort,					iPosBegin, iPosEnd ,PosDialogProxySocksPort);
	Get(sGroup,	GetDialogProxySocksNoFor,					iPosBegin, iPosEnd ,PosDialogProxySocksNoFor);
	Get(sGroup,	GetDialogProxySocksGroupUser,				iPosBegin, iPosEnd ,PosDialogProxySocksGroupUser);
	Get(sGroup,	GetDialogProxySocksUser,					iPosBegin, iPosEnd ,PosDialogProxySocksUser);
	Get(sGroup,	GetDialogProxySocksPassword,				iPosBegin, iPosEnd ,PosDialogProxySocksPassword);

	sGroup = GroupDialogRules; GetGroup(sGroup,	&iPosBegin, &iPosEnd);
	Get(sGroup,	GetDialogRulesTitle,						iPosBegin, iPosEnd ,PosDialogRulesTitle);
	Get(sGroup,	GetDialogRulesName,							iPosBegin, iPosEnd ,PosDialogRulesName);
	Get(sGroup,	GetDialogRulesComputer,						iPosBegin, iPosEnd ,PosDialogRulesComputer);
	Get(sGroup,	GetDialogRulesProject,						iPosBegin, iPosEnd ,PosDialogRulesProject);
	Get(sGroup,	GetDialogRulesApplication,					iPosBegin, iPosEnd ,PosDialogRulesApplication);

	Get(sGroup,	GetDialogRulesGroupType,					iPosBegin, iPosEnd ,PosDialogRulesGroupType);
	Get(sGroup,	GetDialogRulesGroupOperator,				iPosBegin, iPosEnd ,PosDialogRulesGroupOperator);
	Get(sGroup,	GetDialogRulesGroupValue,					iPosBegin, iPosEnd ,PosDialogRulesGroupValue);
	Get(sGroup,	GetDialogRulesGroupTime,					iPosBegin, iPosEnd ,PosDialogRulesGroupTime);
	Get(sGroup,	GetDialogRulesGroupEvent,					iPosBegin, iPosEnd ,PosDialogRulesGroupEvent);

	Get(sGroup,	GetDialogRulesTypeElapsed,					iPosBegin, iPosEnd ,PosDialogRulesTypeElapsed);
	Get(sGroup,	GetDialogRulesTypeCpuP,						iPosBegin, iPosEnd ,PosDialogRulesTypeCpuP);
	Get(sGroup,	GetDialogRulesTypeProgress,					iPosBegin, iPosEnd ,PosDialogRulesTypeProgress);
	Get(sGroup,	GetDialogRulesTypeProgressDelta,			iPosBegin, iPosEnd ,PosDialogRulesTypeProgressDelta);

	Get(sGroup,	GetDialogRulesTypeTimeLeft,					iPosBegin, iPosEnd ,PosDialogRulesTypeTimeLeft);
	Get(sGroup,	GetDialogRulesTypeConnection,				iPosBegin, iPosEnd ,PosDialogRulesTypeConnection);
	Get(sGroup,	GetDialogRulesTypeUse,						iPosBegin, iPosEnd ,PosDialogRulesTypeUse);
	Get(sGroup,	GetDialogRulesTypeTemperature,				iPosBegin, iPosEnd ,PosDialogRulesTypeTemperature);
	Get(sGroup,	GetDialogRulesTypeStatus,					iPosBegin, iPosEnd ,PosDialogRulesTypeStatus);
	Get(sGroup,	GetDialogRulesTypeTime,						iPosBegin, iPosEnd ,PosDialogRulesTypeTime);
	Get(sGroup,	GetDialogRulesTypeDeadline,					iPosBegin, iPosEnd ,PosDialogRulesTypeDeadline);
	Get(sGroup,	GetDialogRulesTypeTimeLeftProject,			iPosBegin, iPosEnd ,PosDialogRulesTypeTimeLeftProject);

	Get(sGroup,	GetDialogRulesEventSuspendProject,			iPosBegin, iPosEnd ,PosDialogRulesEventSuspendProject);
	Get(sGroup,	GetDialogRulesEventResumeProject,			iPosBegin, iPosEnd ,PosDialogRulesEventResumeProject);
	Get(sGroup,	GetDialogRulesEventSnooze,					iPosBegin, iPosEnd ,PosDialogRulesEventSnooze);
	Get(sGroup,	GetDialogRulesEventSnoozeGpu,				iPosBegin, iPosEnd ,PosDialogRulesEventSnoozeGpu);

	Get(sGroup,	GetDialogRulesEventNoNewWork,				iPosBegin, iPosEnd ,PosDialogRulesEventNoNewWork);
	Get(sGroup,	GetDialogRulesEventAllowNewWork,			iPosBegin, iPosEnd ,PosDialogRulesEventAllowNewWork);
	Get(sGroup,	GetDialogRulesEventSuspendTask,				iPosBegin, iPosEnd ,PosDialogRulesEventSuspendTask);

	Get(sGroup,	GetDialogRulesEventMessageNone,				iPosBegin, iPosEnd ,PosDialogRulesEventMessageNone);
	Get(sGroup,	GetDialogRulesEventMessageNotice,			iPosBegin, iPosEnd ,PosDialogRulesEventMessageNotice);
	Get(sGroup,	GetDialogRulesEventMessage,					iPosBegin, iPosEnd ,PosDialogRulesEventMessage);
	Get(sGroup,	GetDialogRulesEventProgram,					iPosBegin, iPosEnd ,PosDialogRulesEventProgram);

	Get(sGroup, GetDialogRulesEventSuspendNetwork,			iPosBegin, iPosEnd, PosDialogRulesEventSuspendNetwork);
	Get(sGroup, GetDialogRulesEventResumeNetwork,			iPosBegin, iPosEnd, PosDialogRulesEventResumeNetwork);

	Get(sGroup,	GetDialogRulesMsgIdenticalType,				iPosBegin, iPosEnd ,PosDialogRulesMsgIdenticalType);
	Get(sGroup,	GetDialogRulesMsgNotRules,					iPosBegin, iPosEnd ,PosDialogRulesMsgNotRules);
	Get(sGroup,	GetDialogRulesMsgRuleNameEmpty,				iPosBegin, iPosEnd ,PosDialogRulesMsgRuleNameEmpty);
	Get(sGroup,	GetDialogRulesMsgRuleNameInUse,				iPosBegin, iPosEnd ,PosDialogRulesMsgRuleNameInUse);
	Get(sGroup,	GetDialogRulesMsgNoProgram,					iPosBegin, iPosEnd ,PosDialogRulesMsgNoProgram);
	Get(sGroup,	GetDialogRulesMsgEmpty,						iPosBegin, iPosEnd ,PosDialogRulesMsgEmpty);
	Get(sGroup,	GetDialogRulesMsgOperatorNotSelected,		iPosBegin, iPosEnd ,PosDialogRulesMsgOperatorNotSelected);
	Get(sGroup,	GetDialogRulesMsgInvalidTime,				iPosBegin, iPosEnd ,PosDialogRulesMsgInvalidTime);
	Get(sGroup,	GetDialogRulesMsgTempRuleComputer,			iPosBegin, iPosEnd ,PosDialogRulesMsgTempRuleComputer);
	Get(sGroup,	GetDialogRulesMsgInvalidUse,				iPosBegin, iPosEnd ,PosDialogRulesMsgInvalidUse);
	Get(sGroup,	GetDialogRulesMsgInvalidStatus,				iPosBegin, iPosEnd ,PosDialogRulesMsgInvalidStatus);
	Get(sGroup,	GetDialogRulesMsgInvalidStatus2,			iPosBegin, iPosEnd ,PosDialogRulesMsgInvalidStatus2);
	Get(sGroup,	GetDialogRulesMsgOnlyFirstType,				iPosBegin, iPosEnd ,PosDialogRulesMsgOnlyFirstType);
	Get(sGroup,	GetDialogRulesMsgSnoozeFor,					iPosBegin, iPosEnd ,PosDialogRulesMsgSnoozeFor);
	Get(sGroup,	GetDialogRulesNotAllowed,					iPosBegin, iPosEnd ,PosDialogRulesNotAllowed);

	Get(sGroup,	GetDialogRulesButtonCheck,					iPosBegin, iPosEnd ,PosDialogRulesButtonCheck);
	Get(sGroup,	GetDialogRulesButtonColor,					iPosBegin, iPosEnd ,PosDialogRulesButtonColor);

	sGroup = GroupDialogMessageSearch; GetGroup(sGroup,	&iPosBegin, &iPosEnd);
	Get(sGroup,	GetDialogMessageSearchTitle,				iPosBegin, iPosEnd ,PosDialogMessageSearchTitle);

	sGroup = GroupDialogCommonButtons; GetGroup(sGroup,	&iPosBegin, &iPosEnd);
	Get(sGroup,	GetDialogCommonButtonsYesButton,			iPosBegin, iPosEnd ,PosDialogCommonButtonsYesButton);
	Get(sGroup,	GetDialogCommonButtonsNoButton,				iPosBegin, iPosEnd ,PosDialogCommonButtonsNoButton);
	Get(sGroup,	GetDialogCommonButtonsOk,					iPosBegin, iPosEnd ,PosDialogCommonButtonsOk);
	Get(sGroup,	GetDialogCommonButtonsCancel,				iPosBegin, iPosEnd ,PosDialogCommonButtonsCancel);
	Get(sGroup,	GetDialogCommonButtonsClear,				iPosBegin, iPosEnd ,PosDialogCommonButtonsClear);
	Get(sGroup,	GetDialogCommonButtonsEdit,					iPosBegin, iPosEnd ,PosDialogCommonButtonsEdit);
	Get(sGroup,	GetDialogCommonButtonsAdd,					iPosBegin, iPosEnd ,PosDialogCommonButtonsAdd);
	Get(sGroup,	GetDialogCommonButtonsDelete,				iPosBegin, iPosEnd ,PosDialogCommonButtonsDelete);
	Get(sGroup,	GetDialogCommonButtonsApply,				iPosBegin, iPosEnd ,PosDialogCommonButtonsApply);

	sGroup = GroupMessageBox; GetGroup(sGroup,				&iPosBegin, &iPosEnd);
	Get(sGroup,	GetClientQuitQuestion,						iPosBegin, iPosEnd ,PosClientQuitQuestion);\
	Get(sGroup,	GetClientCouldntShutdown,					iPosBegin, iPosEnd ,PosClientCouldntShutdown);
	Get(sGroup,	GetClientNotStartedBT,						iPosBegin, iPosEnd ,PosClientNotStartedBT);

	sGroup = GroupWindowStatusMessage; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetWindowStatusMessageTasksUpdateInSeconds,	iPosBegin, iPosEnd ,PosWindowStatusMessageTasksUpdateInSeconds);
	Get(sGroup, GetWindowStatusMessageTasksWaiting,			iPosBegin, iPosEnd ,PosWindowStatusMessageTasksWaiting);
	Get(sGroup, GetWindowStatusMessageStatusUpdateInSec,	iPosBegin, iPosEnd ,PosWindowStatusMessageStatusUpdateInSec);
	Get(sGroup, GetWindowStatusMessageStatusWaiting,		iPosBegin, iPosEnd ,PosWindowStatusMessageStatusWaiting);
	Get(sGroup, GetWindowStatusMessageStatusUpdating,		iPosBegin, iPosEnd ,PosWindowStatusMessageStatusUpdating);
	Get(sGroup, GetWindowStatusMessageStatusSwitched,		iPosBegin, iPosEnd ,PosWindowStatusMessageStatusSwitched);
	Get(sGroup, GetWindowStatusMessageStatusClosing,		iPosBegin, iPosEnd ,PosWindowStatusMessageStatusClosing);

	Get(sGroup, GetWindowStatusMessageStatusHistoryDisabled,iPosBegin, iPosEnd ,PosWindowStatusMessageStatusHistoryDisabled);

//	Get(sGroup, GetWindowStatusMessageCombined,				iPosBegin, iPosEnd ,PosWindowStatusMessageCombined);
	Get(sGroup, GetWindowStatusMessageCombinedNotConnected,	iPosBegin, iPosEnd ,PosWindowStatusMessageCombinedNotConnected);
	Get(sGroup, GetWindowStatusMessageCombinedPassWord,		iPosBegin, iPosEnd ,PosWindowStatusMessageCombinedPassWord);
	Get(sGroup, GetWindowStatusMessageCombinedState,		iPosBegin, iPosEnd ,PosWindowStatusMessageCombinedState);
	Get(sGroup, GetWindowStatusMessageCombinedConnecting,	iPosBegin, iPosEnd ,PosWindowStatusMessageCombinedConnecting);
	Get(sGroup, GetWindowStatusMessageCombinedConnected,	iPosBegin, iPosEnd ,PosWindowStatusMessageCombinedConnected);
	Get(sGroup, GetWindowStatusMessageCombinedLostConnection,iPosBegin, iPosEnd ,PosWindowStatusMessageCombinedLostConnection);

	Get(sGroup, GetWindowStatusMessageMessages,				iPosBegin, iPosEnd ,PosWindowStatusMessageMessages);
	Get(sGroup, GetWindowStatusMessageTasks,				iPosBegin, iPosEnd ,PosWindowStatusMessageTasks);
	Get(sGroup, GetWindowStatusMessageProjects,				iPosBegin, iPosEnd ,PosWindowStatusMessageProjects);
	Get(sGroup, GetWindowStatusMessageTransfer,				iPosBegin, iPosEnd ,PosWindowStatusMessageTransfer);
	Get(sGroup, GetWindowStatusMessageHistory,				iPosBegin, iPosEnd ,PosWindowStatusMessageHistory);
	Get(sGroup, GetWindowStatusMessageComputer,				iPosBegin, iPosEnd ,PosWindowStatusMessageComputer);
	Get(sGroup, GetWindowStatusMessageNotices,				iPosBegin, iPosEnd ,PosWindowStatusMessageNotices);

	sGroup = GroupTaskbarMessage; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetGroupTaskbarMessageComputers,			iPosBegin, iPosEnd ,PosGroupTaskbarMessageComputers);
	Get(sGroup, GetGroupTaskbarMessageRunning,				iPosBegin, iPosEnd ,PosGroupTaskbarMessageRunning);

	sGroup = GroupMobile; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetGroupMobileLoginHeader,					iPosBegin, iPosEnd ,PosGroupMobileLoginHeader);
	Get(sGroup, GetGroupMobileLoginHeaderAuto,				iPosBegin, iPosEnd ,PosGroupMobileLoginHeaderAuto);
	Get(sGroup, GetGroupMobileLoginHeaderFailed,			iPosBegin, iPosEnd ,PosGroupMobileLoginHeaderFailed);
	Get(sGroup, GetGroupMobileLoginLoginAuto,				iPosBegin, iPosEnd ,PosGroupMobileLoginLoginAuto);
	Get(sGroup, GetGroupMobileLoginKeep,					iPosBegin, iPosEnd ,PosGroupMobileLoginKeep);
	Get(sGroup, GetGroupMobileLoginStore,					iPosBegin, iPosEnd ,PosGroupMobileLoginStore);
	Get(sGroup, GetGroupMobileLoginPassword,				iPosBegin, iPosEnd ,PosGroupMobileLoginPassword);
	Get(sGroup, GetGroupMobileLoginNeedPassword,			iPosBegin, iPosEnd ,PosGroupMobileLoginNeedPassword);
	Get(sGroup, GetGroupMobileLogout,						iPosBegin, iPosEnd ,PosGroupMobileLogout);
	Get(sGroup, GetGroupMobileButtonOptions,				iPosBegin, iPosEnd ,PosGroupMobileButtonOptions);
	Get(sGroup, GetGroupMobileButtonMore,					iPosBegin, iPosEnd ,PosGroupMobileButtonMore);
	Get(sGroup, GetGroupMobileButtonLess,					iPosBegin, iPosEnd ,PosGroupMobileButtonLess);
	Get(sGroup, GetGroupMobileButtonFontSize,				iPosBegin, iPosEnd ,PosGroupMobileButtonFontSize);
	Get(sGroup, GetGroupMobileButtonMultipleSelections,		iPosBegin, iPosEnd ,PosGroupMobileButtonMultipleSelections);
	Get(sGroup, GetGroupMobileSort,							iPosBegin, iPosEnd ,PosGroupMobileSort);
	Get(sGroup, GetGroupMobileSort1,						iPosBegin, iPosEnd ,PosGroupMobileSort1);
	Get(sGroup, GetGroupMobileSort2,						iPosBegin, iPosEnd ,PosGroupMobileSort2);
	Get(sGroup, GetGroupMobileSort3,						iPosBegin, iPosEnd ,PosGroupMobileSort3);
	Get(sGroup, GetGroupMobileButonFilter,					iPosBegin, iPosEnd ,PosGroupMobileButtonFilter);
	Get(sGroup, GetGroupMobileButtonReport,					iPosBegin, iPosEnd ,PosGroupMobileButtonReport);
	Get(sGroup, GetGroupMobileButtonGraph,					iPosBegin, iPosEnd ,PosGroupMobileButtonGraph);
	Get(sGroup, GetGroupMobileSingleSelection,				iPosBegin, iPosEnd ,PosGroupMobileSingleSelection);
	Get(sGroup, GetGroupMobileButtonToggleFilter,			iPosBegin, iPosEnd ,PosGroupMobileButtonToggleFilter);
	Get(sGroup, GetGroupMobileButtonToggleFullScreen,		iPosBegin, iPosEnd ,PosGroupMobileButtonToggleFullScreen);
	Get(sGroup, GetGroupMobileButtonGraphSComputer,			iPosBegin, iPosEnd ,PosGroupMobileButtonGraphSComputer);
	Get(sGroup, GetGroupMobileButtonGraphSProject,			iPosBegin, iPosEnd ,PosGroupMobileButtonGraphSProject);
	Get(sGroup, GetGroupMobileButtonGraphDone,				iPosBegin, iPosEnd ,PosGroupMobileButtonGraphDone);

	sGroup = GroupNoticesMessage; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetNoticesMessageMore,						iPosBegin, iPosEnd ,PosNoticesMessageMore);
	Get(sGroup, GetNoticesMessageThereAreNew,				iPosBegin, iPosEnd ,PosNoticesMessageThereAreNew);
	Get(sGroup, GetNoticesMessageBalloonTitle,				iPosBegin, iPosEnd ,PosNoticesMessageBalloonTitle);
	Get(sGroup, GetNoticesCatClient,						iPosBegin, iPosEnd ,PosNoticesCatClient);
	Get(sGroup, GetNoticesCatScheduler,						iPosBegin, iPosEnd ,PosNoticesCatScheduler);
	Get(sGroup, GetNoticesCatBoincTasks,					iPosBegin, iPosEnd ,PosNoticesCatBoincTasks);
	Get(sGroup, GetNoticesCatTThrottle,						iPosBegin, iPosEnd ,PosNoticesCatTThrottle);
	Get(sGroup, GetNoticeNoNotices,							iPosBegin, iPosEnd ,PosNoticeNoNotices);
	Get(sGroup, GetNoticeDisabledUser,						iPosBegin, iPosEnd ,PosNoticeDisabledUser);

	sGroup = GroupTranslatableBoincText; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetTranslatableBoincText0,iPosBegin, iPosEnd ,PosTranslatableBoincText0);
	Get(sGroup, GetTranslatableBoincText1,iPosBegin, iPosEnd ,PosTranslatableBoincText1);
	Get(sGroup, GetTranslatableBoincText2,iPosBegin, iPosEnd ,PosTranslatableBoincText2);
	Get(sGroup, GetTranslatableBoincText3,iPosBegin, iPosEnd ,PosTranslatableBoincText3);
	Get(sGroup, GetTranslatableBoincText4,iPosBegin, iPosEnd ,PosTranslatableBoincText4);
	Get(sGroup, GetTranslatableBoincText5,iPosBegin, iPosEnd ,PosTranslatableBoincText5);
	Get(sGroup, GetTranslatableBoincText6,iPosBegin, iPosEnd ,PosTranslatableBoincText6);
	Get(sGroup, GetTranslatableBoincText7,iPosBegin, iPosEnd ,PosTranslatableBoincText7);
	Get(sGroup, GetTranslatableBoincText8,iPosBegin, iPosEnd ,PosTranslatableBoincText8);
	Get(sGroup, GetTranslatableBoincText9,iPosBegin, iPosEnd ,PosTranslatableBoincText9);

	Get(sGroup, GetTranslatableBoincText10,iPosBegin, iPosEnd ,PosTranslatableBoincText10);
	Get(sGroup, GetTranslatableBoincText11,iPosBegin, iPosEnd ,PosTranslatableBoincText11);
	Get(sGroup, GetTranslatableBoincText12,iPosBegin, iPosEnd ,PosTranslatableBoincText12);
	Get(sGroup, GetTranslatableBoincText13,iPosBegin, iPosEnd ,PosTranslatableBoincText13);
	Get(sGroup, GetTranslatableBoincText14,iPosBegin, iPosEnd ,PosTranslatableBoincText14);
	Get(sGroup, GetTranslatableBoincText15,iPosBegin, iPosEnd ,PosTranslatableBoincText15);
	Get(sGroup, GetTranslatableBoincText16,iPosBegin, iPosEnd ,PosTranslatableBoincText16);
	Get(sGroup, GetTranslatableBoincText17,iPosBegin, iPosEnd ,PosTranslatableBoincText17);
	Get(sGroup, GetTranslatableBoincText18,iPosBegin, iPosEnd ,PosTranslatableBoincText18);
	Get(sGroup, GetTranslatableBoincText19,iPosBegin, iPosEnd ,PosTranslatableBoincText19);

	Get(sGroup, GetTranslatableBoincText20,iPosBegin, iPosEnd ,PosTranslatableBoincText20);
	Get(sGroup, GetTranslatableBoincText21,iPosBegin, iPosEnd ,PosTranslatableBoincText21);
	Get(sGroup, GetTranslatableBoincText22,iPosBegin, iPosEnd ,PosTranslatableBoincText22);
	Get(sGroup, GetTranslatableBoincText23,iPosBegin, iPosEnd ,PosTranslatableBoincText23);
	Get(sGroup, GetTranslatableBoincText24,iPosBegin, iPosEnd ,PosTranslatableBoincText24);
	Get(sGroup, GetTranslatableBoincText25,iPosBegin, iPosEnd ,PosTranslatableBoincText25);
	Get(sGroup, GetTranslatableBoincText26,iPosBegin, iPosEnd ,PosTranslatableBoincText26);
	Get(sGroup, GetTranslatableBoincText27,iPosBegin, iPosEnd ,PosTranslatableBoincText27);
	Get(sGroup, GetTranslatableBoincText28,iPosBegin, iPosEnd ,PosTranslatableBoincText28);
	Get(sGroup, GetTranslatableBoincText29,iPosBegin, iPosEnd ,PosTranslatableBoincText29);

	sGroup = GroupTranslatableWWWText; GetGroup(sGroup, &iPosBegin, &iPosEnd);
	Get(sGroup, GetTranslatableWWWText0,iPosBegin, iPosEnd ,PosTranslatableWWWText0);
	Get(sGroup, GetTranslatableWWWText1,iPosBegin, iPosEnd ,PosTranslatableWWWText1);
	Get(sGroup, GetTranslatableWWWText2,iPosBegin, iPosEnd ,PosTranslatableWWWText2);
	Get(sGroup, GetTranslatableWWWText3,iPosBegin, iPosEnd ,PosTranslatableWWWText3);
	Get(sGroup, GetTranslatableWWWText4,iPosBegin, iPosEnd ,PosTranslatableWWWText4);
	Get(sGroup, GetTranslatableWWWText5,iPosBegin, iPosEnd ,PosTranslatableWWWText5);
	Get(sGroup, GetTranslatableWWWText6,iPosBegin, iPosEnd ,PosTranslatableWWWText6);
	Get(sGroup, GetTranslatableWWWText7,iPosBegin, iPosEnd ,PosTranslatableWWWText7);
	Get(sGroup, GetTranslatableWWWText8,iPosBegin, iPosEnd ,PosTranslatableWWWText8);
	Get(sGroup, GetTranslatableWWWText9,iPosBegin, iPosEnd ,PosTranslatableWWWText9);

	Get(sGroup, GetTranslatableWWWText10,iPosBegin, iPosEnd ,PosTranslatableWWWText10);
	Get(sGroup, GetTranslatableWWWText11,iPosBegin, iPosEnd ,PosTranslatableWWWText11);
	Get(sGroup, GetTranslatableWWWText12,iPosBegin, iPosEnd ,PosTranslatableWWWText12);
	Get(sGroup, GetTranslatableWWWText13,iPosBegin, iPosEnd ,PosTranslatableWWWText13);
	Get(sGroup, GetTranslatableWWWText14,iPosBegin, iPosEnd ,PosTranslatableWWWText14);
	Get(sGroup, GetTranslatableWWWText15,iPosBegin, iPosEnd ,PosTranslatableWWWText15);
	Get(sGroup, GetTranslatableWWWText16,iPosBegin, iPosEnd ,PosTranslatableWWWText16);
	Get(sGroup, GetTranslatableWWWText17,iPosBegin, iPosEnd ,PosTranslatableWWWText17);
	Get(sGroup, GetTranslatableWWWText18,iPosBegin, iPosEnd ,PosTranslatableWWWText18);
	Get(sGroup, GetTranslatableWWWText19,iPosBegin, iPosEnd ,PosTranslatableWWWText19);

	Get(sGroup, GetTranslatableWWWText20,iPosBegin, iPosEnd ,PosTranslatableWWWText20);
	Get(sGroup, GetTranslatableWWWText21,iPosBegin, iPosEnd ,PosTranslatableWWWText21);
	Get(sGroup, GetTranslatableWWWText22,iPosBegin, iPosEnd ,PosTranslatableWWWText22);
	Get(sGroup, GetTranslatableWWWText23,iPosBegin, iPosEnd ,PosTranslatableWWWText23);
	Get(sGroup, GetTranslatableWWWText24,iPosBegin, iPosEnd ,PosTranslatableWWWText24);
	Get(sGroup, GetTranslatableWWWText25,iPosBegin, iPosEnd ,PosTranslatableWWWText25);
	Get(sGroup, GetTranslatableWWWText26,iPosBegin, iPosEnd ,PosTranslatableWWWText26);
	Get(sGroup, GetTranslatableWWWText27,iPosBegin, iPosEnd ,PosTranslatableWWWText27);
	Get(sGroup, GetTranslatableWWWText28,iPosBegin, iPosEnd ,PosTranslatableWWWText28);
	Get(sGroup, GetTranslatableWWWText29,iPosBegin, iPosEnd ,PosTranslatableWWWText29);

	return m_bError;
}

bool CTranslation::Get(CString sGroup, CString sGet, int iPosBegin, int iPosEnd, int iPosArray)
{
	int		iPosFound, iPosFoundEnd, iStringLen;
	CString sGetFind, sString, sFormat;
	char	*szString;
	m_iNrTranslations++;


	if (iPosBegin < 0) return false;
	if (iPosEnd < 0) return false;

	sGetFind = sGet;
	sGetFind.MakeLower();

	sGetFind = "<" + sGetFind + ">";
	iPosFound = m_sTextLower.Find(sGetFind,iPosBegin);
	if (iPosFound >= 0)
	{
		if ((iPosFound >= iPosBegin) & (iPosFound < iPosEnd))
		{
			iPosFound += sGetFind.GetLength();
			iPosFoundEnd = m_sTextLower.Find("</",iPosFound);
			if (iPosFoundEnd >=0)
			{
				iStringLen = iPosFoundEnd - iPosFound;
				sString = m_sText.Mid(iPosFound,iStringLen);
				szString = new char [iStringLen+1];
				strcpy_s(szString,iStringLen+1,sString.GetBuffer()); 

				if (m_bOverride) 
				{
					if (gszTranslation[iPosArray] != NULL)
					{
						delete (gszTranslation[iPosArray]);
						gszTranslation[iPosArray] = NULL;
					}
				}

				if (gszTranslation[iPosArray] == NULL) gszTranslation[iPosArray] = szString;
				else
				{
					sFormat.Format("Group <%s>: String on position %d for %s, string [%s] is not empty! [%s]\r\n",sGroup, iPosArray, sGetFind, szString, gszTranslation[iPosArray]);
					m_sLog += sFormat;
					delete szString;
					szString = NULL;
					m_bError = true;
				}
				return true;
			}
		}
	}

	if (!m_bOverride)
	{
		sFormat.Format("Group: <%s> string header in pos %d, <%s> not found\r\n" ,sGroup, iPosArray, sGet);
		m_sLog += sFormat;
		m_bError = true;
		return false;
	}
	return true;
}


bool CTranslation::GetGroup(CString sGroup, int *piPosBegin, int *piPosEnd)
{
	bool	bOk = false;

	CString sGroupBegin, sGroupEnd;

	sGroupBegin = sGroup;
	sGroupBegin.MakeLower();
	sGroupEnd = sGroupBegin;

	sGroupBegin = "<" + sGroupBegin + ">";
	sGroupEnd = "</" + sGroupEnd + ">"; 
	
	*piPosBegin = m_sTextLower.Find(sGroupBegin, 0);
	*piPosEnd = m_sTextLower.Find(sGroupEnd, 0);

	if (*piPosBegin < 0)
	{
		if (!m_bOverride)
		{
			sGroupBegin = "Group begin not found: " + sGroupBegin + "\r\n";
			m_sLog += sGroupBegin;
			m_bError = true;
			return false;
		}
		return true;
	}
	if (*piPosEnd < 0)
	{
		sGroupEnd = "Group end not found: " + sGroupEnd + "\r\n";
		m_sLog += sGroupEnd;
		m_bError = true;
		return false;
	}
	if (*piPosBegin < *piPosBegin)
	{
		sGroupEnd = "Group out of sequence: " + sGroupEnd + "\r\n";
		m_sLog += sGroupEnd;
		m_bError = true;
		return false;
	}
	
	return true;

}


void CTranslation::Close()
{
	if (m_bOpen) m_file.Close();
	m_sText = "";
	m_sTextLower = "";
	m_bOpen = false;
}