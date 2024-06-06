#pragma once

// CodePage

#define GroupCodePage						"GroupCodePage"
#define GetCodePage							"Page"
#define PosCodePage							0					// dummy location, will be cleared after use
#define GetCharSet							"CharSet"
#define PosCharSet							1	

// GroupViewProjects
#define GroupViewProjects					"GroupViewProjects"
#define GetViewProjecsRowProject			"RowProject"
#define PosViewProjecsRowProject			PosCharSet+1
#define GetViewProjecsRowAccount			"RowAccount"
#define PosViewProjecsRowAccount			PosViewProjecsRowProject+1
#define GetViewProjecsRowTeam				"RowTeam"
#define PosViewProjecsRowTeam				PosViewProjecsRowProject+2
#define GetViewProjecsRowCredits			"RowCredits"
#define PosViewProjecsRowCredits			PosViewProjecsRowProject+3
#define GetViewProjecsRowAvgCredits			"RowAvgCredits"
#define PosViewProjecsRowAvgCredits			PosViewProjecsRowProject+4
#define GetViewProjecsRowHostCredits		"RowHostCredits"
#define PosViewProjecsRowHostCredits		PosViewProjecsRowProject+5
#define GetViewProjecsRowAvgHostCredits		"RowAvgHostCredits"
#define PosViewProjecsRowAvgHostCredits		PosViewProjecsRowProject+6
#define GetViewProjecsRowShareP				"RowShareP"
#define PosViewProjecsRowShareP				PosViewProjecsRowProject+7
#define GetViewProjecsRowTasks				"RowTasks"
#define PosViewProjecsRowTasks				PosViewProjecsRowProject+8
#define GetViewProjecsRowTimeLeft			"RowTimeLeft"
#define PosViewProjecsRowTimeLeft			PosViewProjecsRowProject+9
#define GetViewProjecsRowStatus				"RowStatus"
#define PosViewProjecsRowStatus				PosViewProjecsRowProject+10
#define GetViewProjecsRowComputer			"RowComputer"
#define PosViewProjecsRowComputer			PosViewProjecsRowProject+11
#define GetViewProjecsRowVenue				"RowVenue"
#define PosViewProjecsRowVenue				PosViewProjecsRowProject+12
#define GetViewProjecsRowTasksDay			"RowTasksDay"
#define PosViewProjecsRowTasksDay			PosViewProjecsRowProject+13
#define GetViewProjecsRowTasksWeek			"RowTasksWeek"
#define PosViewProjecsRowTasksWeek			PosViewProjecsRowProject+14
#define GetViewProjecsRowProjectRank		"RowProjectRank"
#define PosViewProjecsRowProjectRank		PosViewProjecsRowProject+15
#define GetViewProjecsRowTeamRank			"RowTeamRank"
#define PosViewProjecsRowTeamRank			PosViewProjecsRowProject+16

#define GetViewProjecsStatusSuspendedByGui	"StatusSuspendedByGui"
#define PosViewProjecsStatusSuspendedByGui	PosViewProjecsRowProject+17
#define GetViewProjecsStatusNoNewTasks		"StatusNoNewTasks"
#define PosViewProjecsStatusNoNewTasks		PosViewProjecsRowProject+18
#define GetViewProjecsStatusDefered			"StatusDefered"
#define PosViewProjecsStatusDefered			PosViewProjecsRowProject+19
#define GetViewProjecsStatusUpdating		"StatusUpdating"
#define PosViewProjecsStatusUpdating		PosViewProjecsRowProject+20
#define GetViewProjecsStatusUpdatingAM		"StatusUpdatingAM"
#define PosViewProjecsStatusUpdatingAM		PosViewProjecsRowProject+21
#define GetViewProjecsStatusAttaching		"StatusAttaching"
#define PosViewProjecsStatusAttaching		PosViewProjecsRowProject+22
#define GetViewProjecsStatusInitializing	"StatusInitializing"
#define PosViewProjecsStatusInitializing	PosViewProjecsRowProject+23
#define GetViewProjecsStatusInProgress		"StatusInProgress"
#define PosViewProjecsStatusInProgress		PosViewProjecsRowProject+24
#define GetViewProjecsStatusReport			"StatusReport"
#define PosViewProjecsStatusReport			PosViewProjecsRowProject+25
#define GetViewProjecsStatusNeedWork		"StatusNeedWork"
#define PosViewProjecsStatusNeedWork		PosViewProjecsRowProject+26
#define GetViewProjecsStatusTrickle			"StatusTrickle"
#define PosViewProjecsStatusTrickle			PosViewProjecsRowProject+27
#define GetViewProjecsStatusProjectReq		"StatusProjectReq"
#define PosViewProjecsStatusProjectReq		PosViewProjecsRowProject+28

// GroupViewTasks
#define GroupViewTasks					"GroupViewTasks"
#define	PosGroupViewTasks				PosViewProjecsStatusProjectReq+1
#define GetViewTasksRowProject			"RowProject"
#define PosViewTasksRowProject			PosGroupViewTasks
#define GetViewTasksRowApplication		"RowApplication"
#define PosViewTasksRowApplication		PosGroupViewTasks+1
#define GetViewTasksRowName				"RowName"
#define PosViewTasksRowName				PosGroupViewTasks+2
#define GetViewTasksRowElapsed			"RowElapsed"
#define PosViewTasksRowElapsed			PosGroupViewTasks+3
#define GetViewTasksRowCpuP				"RowCpuP"
#define PosViewTasksRowCpuP				PosGroupViewTasks+4
#define GetViewTasksRowProgress			"RowProgress"
#define PosViewTasksRowProgress			PosGroupViewTasks+5
#define GetViewTasksRowTimeLeft			"RowTimeLeft"
#define PosViewTasksRowTimeLeft			PosGroupViewTasks+6
#define GetViewTasksRowDeadline			"RowDeadline"
#define PosViewTasksRowDeadline			PosGroupViewTasks+7
#define GetViewTasksRowUse				"RowUse"
#define PosViewTasksRowUse				PosGroupViewTasks+8
#define GetViewTasksRowStatus			"RowStatus"
#define PosViewTasksRowStatus			PosGroupViewTasks+9
#define GetViewTasksRowThrottle			"RowThrottle"
#define PosViewTasksRowThrottle			PosGroupViewTasks+10
#define GetViewTasksRowTemperature		"RowTemperature"
#define PosViewTasksRowTemperature		PosGroupViewTasks+11
#define GetViewTasksRowComputer			"RowComputer"
#define PosViewTasksRowComputer			PosGroupViewTasks+12
#define GetViewTasksRowCheckpoint		"RowCheckpoint"
#define PosViewTasksRowCheckpoint		PosGroupViewTasks+13
#define GetViewTasksRowDebt				"RowDebt"
#define PosViewTasksRowDebt				PosGroupViewTasks+14
#define GetViewTasksRowReceived			"RowReceived"
#define PosViewTasksRowReceived			PosGroupViewTasks+15
#define GetViewTasksRowAccount			"RowAccount"
#define PosViewTasksRowAccount			PosGroupViewTasks+16
#define GetViewTasksRowVirtual			"RowVirtual"
#define PosViewTasksRowVirtual			PosGroupViewTasks+17
#define GetViewTasksRowMemory			"RowMemory"
#define PosViewTasksRowMemory			PosGroupViewTasks+18
#define GetViewTasksRowShare			"RowShare"
#define PosViewTasksRowShare			PosGroupViewTasks+19

#define GetViewTasksStatusComputation	"StatusComputation"			// must be the first (sorting tasks status column)
#define PosViewTasksStatusComputation	PosGroupViewTasks+20
#define GetViewTasksStatusAbortedProject "StatusAbortedByProject"
#define PosViewTasksStatusAbortedProject PosGroupViewTasks+21
#define GetViewTasksStatusAbortedUser	"StatusAbortedByUser"
#define PosViewTasksStatusAbortedUser	PosGroupViewTasks+22
#define GetViewTasksStatusDownloading	"StatusDownloading"	
#define PosViewTasksStatusDownloading	PosGroupViewTasks+23
#define GetViewTasksStatusDownloadError	"StatusDownloadError"
#define PosViewTasksStatusDownloadError	PosGroupViewTasks+24
#define GetViewTasksStatusUploading		"StatusUploading"
#define PosViewTasksStatusUploading		PosGroupViewTasks+25
#define GetViewTasksStatusReadyTR		"StatusReadyTR"
#define PosViewTasksStatusReadyTR		PosGroupViewTasks+26
#define GetViewTasksStatusReadyTS		"StatusReadyTS"
#define PosViewTasksStatusReadyTS		PosGroupViewTasks+27
#define GetViewTasksStatusWaitingTR		"StatusWaitingTR"
#define PosViewTasksStatusWaitingTR		PosGroupViewTasks+28

#define GetViewTasksStatusRunning		"StatusRunning"
#define PosViewTasksStatusRunning		PosGroupViewTasks+29
#define GetViewTasksStatusRunningHP		"StatusRunningHP"
#define PosViewTasksStatusRunningHP		PosGroupViewTasks+30
#define GetViewTasksStatusRunningLP		"StatusRunningLP"
#define PosViewTasksStatusRunningLP		PosGroupViewTasks+31
#define GetViewTasksStatusAborted		"StatusAborted"
#define PosViewTasksStatusAborted		PosGroupViewTasks+32
#define GetViewTasksStatusNew			"StatusNew"
#define PosViewTasksStatusNew			PosGroupViewTasks+33
#define GetViewTasksSuspendProject		"SuspendProject"
#define PosViewTasksSuspendProject		PosGroupViewTasks+34
#define GetViewTasksSuspendPrefix		"SuspendPrefix"
#define PosViewTasksSuspendPrefix		PosGroupViewTasks+35
#define GetViewTasksStatusSuspended		"StatusSuspended"
#define PosViewTasksStatusSuspended		PosGroupViewTasks+36	// must be the last (sorting tasks status column)	Any new items makes the list invalid

#define GetViewTasksSuspendOnBatt		"SuspendOnBatt"
#define PosViewTasksSuspendOnBatt		PosGroupViewTasks+37
#define GetViewTasksSuspendUserAct		"SuspendUserAct"	
#define PosViewTasksSuspendUserAct		PosGroupViewTasks+38
#define GetViewTasksSuspendUserRq		"SuspendUserRq"
#define PosViewTasksSuspendUserRq		PosGroupViewTasks+39
#define GetViewTasksSuspendTimeOD		"SuspendTimeOD"
#define PosViewTasksSuspendTimeOD		PosGroupViewTasks+40
#define GetViewTasksSuspendBench		"SuspendBench"
#define PosViewTasksSuspendBench		PosGroupViewTasks+41
#define GetViewTasksSuspendDisk			"SuspendDisk"
#define PosViewTasksSuspendDisk			PosGroupViewTasks+42
#define GetViewTasksSuspendNoRI			"SuspendNoRI"
#define PosViewTasksSuspendNoRI			PosGroupViewTasks+43
#define GetViewTasksSuspendInitial		"SuspendInitial"
#define PosViewTasksSuspendInitial		PosGroupViewTasks+44
#define GetViewTasksSuspendEx			"SuspendEx"
#define PosViewTasksSuspendEx			PosGroupViewTasks+45
#define GetViewTasksSuspendCpuUsage		"SuspendCpuUsage"
#define PosViewTasksSuspendCpuUsage		PosGroupViewTasks+46
#define GetViewTasksSuspendMemory		"Memory"
#define PosViewTasksSuspendMemory		PosGroupViewTasks+47

#define GetViewTasksWarningDeadline		"WarningDeadline"
#define PosViewTasksWarningDeadline		PosGroupViewTasks+48
#define GetViewTasksInitializing		"Initializing"
#define PosViewTasksInitializing		PosGroupViewTasks+49
#define GetViewTasksFilterTasks			"FilterTasks"
#define PosViewTasksFilterTasks			PosGroupViewTasks+50

// GroupViewTransfers
#define GroupViewTransfers					"GroupViewTransfers"
#define	PosGroupViewTasksTransfer			PosViewTasksFilterTasks+1
#define GetViewTransfersRowProject			"RowProject"
#define PosViewTransfersRowProject			PosGroupViewTasksTransfer
#define GetViewTransfersRowFile				"RowFile"	
#define PosViewTransfersRowFile				PosGroupViewTasksTransfer+1
#define GetViewTransfersRowProgress			"RowProgress"
#define PosViewTransfersRowProgress			PosGroupViewTasksTransfer+2
#define GetViewTransfersRowSize				"RowSize"	
#define PosViewTransfersRowSize				PosGroupViewTasksTransfer+3
#define GetViewTransfersRowElapsed			"RowElapsed"	
#define PosViewTransfersRowElapsed			PosGroupViewTasksTransfer+4
#define GetViewTransfersRowSpeed			"RowSpeed"
#define PosViewTransfersRowSpeed			PosGroupViewTasksTransfer+5
#define GetViewTransfersRowStatus			"RowStatus"
#define PosViewTransfersRowStatus			PosGroupViewTasksTransfer+6
#define GetViewTransfersRowComputer			"RowComputer"	
#define PosViewTransfersRowComputer			PosGroupViewTasksTransfer+7

#define GetViewTransfersStatusUploading		"StatusUploading"
#define PosViewTransfersStatusUploading		PosGroupViewTasksTransfer+8
#define GetViewTransfersStatusDownloading	"StatusDownloading"
#define PosViewTransfersStatusDownloading	PosGroupViewTasksTransfer+9
#define GetViewTransfersStatusRetry			"StatusRetry"
#define PosViewTransfersStatusRetry			PosGroupViewTasksTransfer+10
#define GetViewTransfersStatusUploadP		"StatusUploadP"
#define PosViewTransfersStatusUploadP		PosGroupViewTasksTransfer+11
#define GetViewTransfersStatusDownloadP		"StatusDownloadP"
#define PosViewTransfersStatusDownloadP		PosGroupViewTasksTransfer+12
#define GetViewTransfersStatusRetryCount	"StatusRetryCount"
#define PosViewTransfersStatusRetryCount	PosGroupViewTasksTransfer+13

#define GetViewTransfersStatusProjectBack	"StatusProjectBackoff"
#define PosViewTransfersStatusProjectBack	PosGroupViewTasksTransfer+14

// GroupViewMessages
#define GroupViewMessages				"GroupViewMessages"
#define	PosGroupViewTasksMessages		PosViewTransfersStatusProjectBack+1
#define GetViewMessagesRowNr			"RowNr"
#define PosMessagesRowNr				PosGroupViewTasksMessages
#define GetViewMessagesRowProject		"RowProject"
#define PosViewMessagesRowProject		PosGroupViewTasksMessages+1
#define GetViewMessagesRowTime			"RowTime"
#define PosViewMessagesRowTime			PosGroupViewTasksMessages+2
#define GetViewMessagesRowMessage		"RowMessage"
#define PosViewMessagesRowMessage		PosGroupViewTasksMessages+3
#define GetViewMessagesRowComputer		"RowComputer"
#define PosViewMessagesRowComputer		PosGroupViewTasksMessages+4
#define GetViewMessagesStatusSelectOne	"StatusSelectOne"
#define PosViewMessagesStatusSelectOne	PosGroupViewTasksMessages+5

// GroupViewHistory
#define GroupViewHistory					"GroupViewHistory"
#define	PosGroupViewTasksHistory			PosViewMessagesStatusSelectOne+1
#define GetViewHistoryRowProject			"RowProject"
#define PosViewHistoryRowProject			PosGroupViewTasksHistory
#define GetViewHistoryRowApplication		"RowApplication"
#define PosViewHistoryRowApplication		PosGroupViewTasksHistory+1
#define GetViewHistoryRowName				"RowName"
#define PosViewHistoryRowName				PosGroupViewTasksHistory+2
#define GetViewHistoryRowElapsed			"RowElapsed"
#define PosViewHistoryRowElapsed			PosGroupViewTasksHistory+3
#define GetViewHistoryRowCompleted			"RowCompleted"
#define PosViewHistoryRowCompleted			PosGroupViewTasksHistory+4
#define GetViewHistoryRowReported			"RowReported"
#define PosViewHistoryRowReported			PosGroupViewTasksHistory+5
#define GetViewHistoryRowUse				"RowUse"
#define PosViewHistoryRowUse				PosGroupViewTasksHistory+6
#define GetViewHistoryRowCpuP				"RowCpuP"
#define PosViewHistoryRowCpuP				PosGroupViewTasksHistory+7
#define GetViewHistoryRowStatus				"RowStatus"
#define PosViewHistoryRowStatus				PosGroupViewTasksHistory+8
#define GetViewHistoryRowComputer			"RowComputer"
#define PosViewHistoryRowComputer			PosGroupViewTasksHistory+9
#define GetViewHistoryRowVirtual			"RowVirtual"
#define PosViewHistoryRowVirtual			PosGroupViewTasksHistory+10
#define GetViewHistoryRowMemory				"RowMemory"
#define PosViewHistoryRowMemory				PosGroupViewTasksHistory+11

#define GetViewHistoryStatusReportedOk		"StatusReportedOk"
#define PosViewHistoryStatusReportedOk		PosGroupViewTasksHistory+12
#define GetViewHistoryStatusReportedAb		"StatusReportedAb"
#define PosViewHistoryStatusReportedAb		PosGroupViewTasksHistory+13
#define GetViewHistoryStatusReportedCompErr	"StatusReportedCompErr"
#define PosViewHistoryStatusReportedCompErr	PosGroupViewTasksHistory+14
#define GetViewHistoryStatusCompErr			"StatusCompErr"
#define PosViewHistoryStatusCompErr			PosGroupViewTasksHistory+15
#define GetViewHistoryMessageMissed			"StatusMissed"
#define PosViewHistoryMessageMissed			PosGroupViewTasksHistory+16

#define GetViewHistoryStatusWaitingForData	"StatusWaitingForData"
#define PosViewHistoryStatusWaitingForData	PosGroupViewTasksHistory+17
#define GetViewHistoryStatusToEnable		"StatusToEnable"
#define PosViewHistoryStatusToEnable		PosGroupViewTasksHistory+18
#define GetViewHistoryStatusToEnableMenu	"StatusToEnableMenu"
#define PosViewHistoryStatusToEnableMenu	PosGroupViewTasksHistory+19
//#define GetViewHistoryStatusNotSupported	"StatusNotSupported"
//#define PosViewHistoryStatusNotSupported	PosGroupViewTasksHistory+18
#define GetViewHistoryMessageTooLarge		"MessageTooLarge"
#define PosViewHistoryMessageTooLarge		PosGroupViewTasksHistory+20

// GroupViewComputer
#define GroupViewComputer					"GroupViewComputer"
#define PosGroupViewComputer				PosViewHistoryMessageTooLarge+1
#define GetViewComputerGroup				"Group"
#define PosViewComputerGroup				PosGroupViewComputer
#define GetViewComputerTitle				"Title"
#define PosViewComputerTitle				PosGroupViewComputer+1
#define GetViewComputerEncrypted			"Encrypted"
#define PosViewComputerEncrypted			PosGroupViewComputer+2
#define GetViewComputerIp					"Ip"
#define PosViewComputerIp					PosGroupViewComputer+3
#define GetViewComputerMAC					"MAC"
#define PosViewComputerMAC					PosGroupViewComputer+4
#define GetViewComputerPort					"Port"
#define PosViewComputerPort					PosGroupViewComputer+5
#define GetViewComputerPassword				"Password"
#define PosViewComputerPassword				PosGroupViewComputer+6
#define GetViewComputerBoinc				"Boinc"
#define PosViewComputerBoinc				PosGroupViewComputer+7
#define GetViewComputerBoincPlatform		"BoincPlatform"
#define PosViewComputerBoincPlatform		PosGroupViewComputer+8
#define GetViewComputerTThrottle			"TThrottle"
#define PosViewComputerTThrottle			PosGroupViewComputer+9
#define GetViewComputerStatus				"Status"
#define PosViewComputerStatus				PosGroupViewComputer+10
//#define GetViewComputerDaysWork				"DaysWork"
//#define PosViewComputerDaysWork				PosGroupViewComputer+11
//#define GetViewComputerWantedTaks			"WantedTaks"
//#define PosViewComputerWantedTaks			PosGroupViewComputer+12

#define GetViewComputerAddNew				"AddNew"
#define PosViewComputerAddNew				PosGroupViewComputer+12
#define GetViewComputerAddThis				"AddThis"
#define PosViewComputerAddThis				PosGroupViewComputer+13
#define GetViewComputerErrorEmptyId			"ErrorEmptyId"
#define PosViewComputerErrorEmptyId			PosGroupViewComputer+14
#define GetViewComputerErrorIdenticalId		"ErrorIdenticalId"
#define PosViewComputerErrorIdenticalId		PosGroupViewComputer+15
		
#define GetViewComputerApplyChanges			"ApplyChanges"
#define PosViewComputerApplyChanges			PosGroupViewComputer+16

// GroupBarViewSelect
#define GroupBarViewSelect					"GroupBarViewSelect"
#define	PosGroupBarViewSelect				PosViewComputerApplyChanges+1
#define GetBarViewSelectHistory				"History"
#define PosBarViewSelectHistory				PosGroupBarViewSelect
#define GetBarViewSelectMessages			"Messages"	
#define PosBarViewSelectMessages			PosGroupBarViewSelect+1
#define GetBarViewSelectTransfers			"Transfers"
#define PosBarViewSelectTransfers			PosGroupBarViewSelect+2
#define GetBarViewSelectTasks				"Tasks"
#define PosBarViewSelectTasks				PosGroupBarViewSelect+3
#define GetBarViewSelectProjects			"Projects"
#define PosBarViewSelectProjects			PosGroupBarViewSelect+4
#define GetBarViewSelectComputers			"Computers"
#define PosBarViewSelectComputers			PosGroupBarViewSelect+5
#define GetBarViewSelectNotices				"Notices"
#define PosBarViewSelectNotices				PosGroupBarViewSelect+6

// GroupBarComputers

#define GroupBarComputers					"GroupBarComputers"
#define PosGroupBarComputers				PosBarViewSelectNotices+1
#define GetBarComputersAllComputers			"AllComputers"
#define PosBarComputersAllComputers			PosGroupBarComputers

#define GroupBarProjects					"GroupBarProjects"
#define PosGroupBarProjects					PosBarComputersAllComputers+1
#define GetBarProjectsAllProjects			"AllProjects"
#define PosBarProjectsAllProjects			PosGroupBarProjects

// GroupMenuComputer
#define GroupMenuComputer					"GroupMenuComputer"
#define PosGroupMenuComputer				PosBarProjectsAllProjects+1
#define GetMenuComputer						"Computers"
#define PosMenuComputer						PosGroupMenuComputer
#define GetMenuComputerDetect				"Detect"
#define PosMenuComputerDetect				PosGroupMenuComputer+1
#define GetMenuComputerAdd					"Add"
#define PosMenuComputerAdd					PosGroupMenuComputer+2
#define GetMenuComputerRemove				"Remove"
#define PosMenuComputerRemove				PosGroupMenuComputer+3
//#define GetMenuComputerConnectCombined		"ConnectCombined"
//#define PosMenuComputerConnectCombined		PosGroupMenuComputer+4
#define GetMenuComputerConnect				"Connect"
#define PosMenuComputerConnect				PosGroupMenuComputer+4

// GroupMenuMain
#define GroupMenuMain						"GroupMenuMain"
#define	PosGroupMenuMain					PosMenuComputerConnect+1

#define	GetGroupMenuFile					"File"
#define	PosGroupMenuFile					PosGroupMenuMain
#define	GetGroupMenuFileStartBoinc			"FileStartBoinc"
#define	PosGroupMenuFileStartBoinc			PosGroupMenuMain+1
#define	GetGroupMenuFileStartBoincS			"FileStartBoincS"
#define	PosGroupMenuFileStartBoincS			PosGroupMenuMain+2
#define	GetGroupMenuFileStopBoinc			"FileStopBoinc"
#define	PosGroupMenuFileStopBoinc			PosGroupMenuMain+3
#define	GetGroupMenuFileStopBoincS			"FileStopBoincS"
#define	PosGroupMenuFileStopBoincS			PosGroupMenuMain+4
#define GetGroupMenuFileReadColorConfig		"FileReadColorConfig"
#define PosGroupMenuFileReadColorConfig		PosGroupMenuMain+5
#define GetGroupMenuFileReadColorConfigS	"FileReadColorConfigS"
#define PosGroupMenuFileReadColorConfigS	PosGroupMenuMain+6
#define GetGroupMenuFileWriteColorConfig	"FileWriteColorConfig"
#define PosGroupMenuFileWriteColorConfig	PosGroupMenuMain+7
#define GetGroupMenuFileWriteColorConfigS	"FileWriteColorConfigS"
#define PosGroupMenuFileWriteColorConfigS	PosGroupMenuMain+8
#define	GetGroupMenuFileExit				"FileExit"
#define	PosGroupMenuFileExit				PosGroupMenuMain+9
#define	GetGroupMenuFileExitS				"FileExitS"
#define	PosGroupMenuFileExitS				PosGroupMenuMain+10

#define	GetGroupMenuView					"View"
#define	PosGroupMenuView					PosGroupMenuMain+11
#define	GetGroupMenuViewToolGraphic			"ViewToolGraphic"
#define	PosGroupMenuViewToolGraphic			PosGroupMenuMain+12
#define	GetGroupMenuViewToolGraphicS		"ViewToolGraphicS"
#define	PosGroupMenuViewToolGraphicS		PosGroupMenuMain+13
#define	GetGroupMenuViewToolOperations		"ViewToolOperations"
#define	PosGroupMenuViewToolOperations		PosGroupMenuMain+14
#define	GetGroupMenuViewToolOperationsS		"ViewToolOperationsS"
#define	PosGroupMenuViewToolOperationsS		PosGroupMenuMain+15
#define	GetGroupMenuViewViewSelection		"ViewViewSelection"
#define	PosGroupMenuViewViewSelection		PosGroupMenuMain+16
#define	GetGroupMenuViewViewSelectionS		"ViewViewSelectionS"
#define	PosGroupMenuViewViewSelectionS		PosGroupMenuMain+17
#define	GetGroupMenuViewComputer			"ViewComputer"
#define	PosGroupMenuViewComputer			PosGroupMenuMain+18
#define	GetGroupMenuViewComputerS			"ViewComputerS"
#define	PosGroupMenuViewComputerS			PosGroupMenuMain+19
#define	GetGroupMenuViewProject				"ViewProject"
#define	PosGroupMenuViewProject				PosGroupMenuMain+20
#define	GetGroupMenuViewProjectS			"ViewProjectS"
#define	PosGroupMenuViewProjectS			PosGroupMenuMain+21

#define	GetGroupMenuViewAllow				"ViewToolAllow"
#define	PosGroupMenuViewAllow				PosGroupMenuMain+22
#define	GetGroupMenuViewAllowS				"ViewToolAllowS"
#define	PosGroupMenuViewAllowS				PosGroupMenuMain+23

#define	GetGroupMenuViewStatus				"ViewStatus"
#define	PosGroupMenuViewStatus				PosGroupMenuMain+24
#define	GetGroupMenuViewStatusS				"ViewStatusS"
#define	PosGroupMenuViewStatusS				PosGroupMenuMain+25

#define	GetGroupMenuShow					"Show"
#define	PosGroupMenuShow					PosGroupMenuMain+26

#define	GetGroupMenuShowComputers			"ShowComputers"
#define	PosGroupMenuShowComputers			PosGroupMenuMain+27
#define	GetGroupMenuShowComputersS			"ShowComputersS"
#define	PosGroupMenuShowComputersS			PosGroupMenuMain+28

#define	GetGroupMenuShowProjects			"ShowProjects"
#define	PosGroupMenuShowProjects			PosGroupMenuMain+29
#define	GetGroupMenuShowProjectsS			"ShowProjectsS"
#define	PosGroupMenuShowProjectsS			PosGroupMenuMain+30
#define	GetGroupMenuShowTasks				"ShowTasks"
#define	PosGroupMenuShowTasks				PosGroupMenuMain+31
#define	GetGroupMenuShowTasksS				"ShowTasksS"
#define	PosGroupMenuShowTasksS				PosGroupMenuMain+32
#define	GetGroupMenuShowTransfers			"ShowTransfers"
#define	PosGroupMenuShowTransfers			PosGroupMenuMain+33
#define	GetGroupMenuShowTransfersS			"ShowTransfersS"
#define	PosGroupMenuShowTransfersS			PosGroupMenuMain+34
#define	GetGroupMenuShowMessages			"ShowMessages"
#define	PosGroupMenuShowMessages			PosGroupMenuMain+35
#define	GetGroupMenuShowMessagesS			"ShowMessagesS"
#define	PosGroupMenuShowMessagesS			PosGroupMenuMain+36
#define	GetGroupMenuShowHistory				"ShowHistory"
#define	PosGroupMenuShowHistory				PosGroupMenuMain+37
#define	GetGroupMenuShowHistoryS			"ShowHistoryS"
#define	PosGroupMenuShowHistoryS			PosGroupMenuMain+38

#define	GetGroupMenuShowHistoryLong			"ShowHistoryLong"
#define	PosGroupMenuShowHistoryLong			PosGroupMenuMain+39
#define	GetGroupMenuShowHistoryLongS		"ShowHistoryLongS"
#define	PosGroupMenuShowHistoryLongS		PosGroupMenuMain+40

#define	GetGroupMenuShowNotices				"ShowNotices"
#define	PosGroupMenuShowNotices				PosGroupMenuMain+41
#define	GetGroupMenuShowNoticesS			"ShowNoticesS"
#define	PosGroupMenuShowNoticesS			PosGroupMenuMain+42

#define	GetGroupMenuShowStatistics			"ShowStatistics"
#define	PosGroupMenuShowStatistics			PosGroupMenuMain+43
#define	GetGroupMenuShowStatisticsS			"ShowStatisticsS"
#define	PosGroupMenuShowStatisticsS			PosGroupMenuMain+44

#define	GetGroupMenuShowStatisticsTasks		"ShowStatisticsTasks"
#define	PosGroupMenuShowStatisticsTasks		PosGroupMenuMain+45
#define	GetGroupMenuShowStatisticsTasksS	"ShowStatisticsTasksS"
#define	PosGroupMenuShowStatisticsTasksS	PosGroupMenuMain+46
#define	GetGroupMenuShowTemperature			"ShowTemperature"
#define	PosGroupMenuShowTemperature			PosGroupMenuMain+47
#define	GetGroupMenuShowTemperatureS		"ShowTemperatureS"
#define	PosGroupMenuShowTemperatureS		PosGroupMenuMain+48
#define	GetGroupMenuShowGraphTransfer		"ShowGraphTransfer"
#define	PosGroupMenuShowGraphTransfer		PosGroupMenuMain+49
#define	GetGroupMenuShowGraphTransferS		"ShowGraphTransferS"
#define	PosGroupMenuShowGraphTransferS		PosGroupMenuMain+50
#define	GetGroupMenuShowGraphDeadline		"ShowGraphDeadline"
#define	PosGroupMenuShowGraphDeadline		PosGroupMenuMain+51
#define	GetGroupMenuShowGraphDeadlineS		"ShowGraphDeadlineS"
#define	PosGroupMenuShowGraphDeadlineS		PosGroupMenuMain+52

#define	GetGroupMenuProjects				"Projects"
#define	PosGroupMenuProjects				PosGroupMenuMain+53
#define	GetGroupMenuProjectsAdd				"ProjectsAdd"
#define	PosGroupMenuProjectsAdd				PosGroupMenuMain+54
#define	GetGroupMenuProjectsAddAccountM		"ProjectsAddAccountM"
#define	PosGroupMenuProjectsAddAccountM		PosGroupMenuMain+55
#define	GetGroupMenuProjectsSynchronize		"ProjectsSynchronize"
#define	PosGroupMenuProjectsSynchronize		PosGroupMenuMain+56

#define	GetGroupMenuProjectsSetDebt			"ProjectsSetDebt"
#define	PosGroupMenuProjectsSetDebt			PosGroupMenuMain+57

#define	GetGroupMenuExtra					"Extra"
#define	PosGroupMenuExtra					PosGroupMenuMain+58
#define	GetGroupMenuExtraSetting			"ExtraSetting"
#define	PosGroupMenuExtraSetting			PosGroupMenuMain+59
#define	GetGroupMenuExtraColors				"ExtraColors"
#define	PosGroupMenuExtraColors				PosGroupMenuMain+60
#define	GetGroupMenuExtraPreference			"ExtraPreference"
#define	PosGroupMenuExtraPreference			PosGroupMenuMain+61
#define	GetGroupMenuExtraProxy				"ExtraProxy"
#define	PosGroupMenuExtraProxy				PosGroupMenuMain+62
#define	GetGroupMenuExtraUpdateAll			"ExtraUpdateAll"
#define	PosGroupMenuExtraUpdateAll			PosGroupMenuMain+63
#define	GetGroupMenuExtraStartup			"ExtraStartup"
#define	PosGroupMenuExtraStartup			PosGroupMenuMain+64
#define	GetGroupMenuExtraAllowNetwork		"ExtraAllowNetwork"
#define	PosGroupMenuExtraAllowNetwork		PosGroupMenuMain+65
#define	GetGroupMenuExtraAllowToRun			"ExtraAllowToRun"
#define	PosGroupMenuExtraAllowToRun			PosGroupMenuMain+66
#define	GetGroupMenuExtraAllowToRunGpu		"ExtraAllowToRunGpu"
#define	PosGroupMenuExtraAllowToRunGpu		PosGroupMenuMain+67
#define	GetGroupMenuExtraBenchmark			"ExtraRunBenchmark"
#define	PosGroupMenuExtraBenchmark			PosGroupMenuMain+68
#define	GetGroupMenuExtraBenchmarkLast		"ExtraRunBenchmarkLast"
#define	PosGroupMenuExtraBenchmarkLast		PosGroupMenuMain+69
#define	GetGroupMenuExtraEditConfig			"ExtraEditConfig"
#define	PosGroupMenuExtraEditConfig			PosGroupMenuMain+70
#define	GetGroupMenuExtraEditAppInfo		"ExtraEditAppInfo"
#define	PosGroupMenuExtraEditAppInfo		PosGroupMenuMain+71
#define	GetGroupMenuExtraReadConfig			"ExtraReadConfig"
#define	PosGroupMenuExtraReadConfig			PosGroupMenuMain+72

#define	GetGroupMenuWindow					"Window"
#define	PosGroupMenuWindow					PosGroupMenuMain+73
#define	GetGroupMenuWindowCascade			"WindowCascade"
#define	PosGroupMenuWindowCascade			PosGroupMenuMain+74
#define	GetGroupMenuWindowCascadeS			"WindowCascadeS"
#define	PosGroupMenuWindowCascadeS			PosGroupMenuMain+75
#define	GetGroupMenuWindowTile				"WindowTile"
#define	PosGroupMenuWindowTile				PosGroupMenuMain+76
#define	GetGroupMenuWindowTileS				"WindowTileS"
#define	PosGroupMenuWindowTileS				PosGroupMenuMain+77
#define GetGroupMenuWindowActivateS			"WindowActivateS"
#define PosGroupMenuWindowActivateS			PosGroupMenuMain+78

#define	GetGroupMenuHelp					"Help"
#define	PosGroupMenuHelp					PosGroupMenuMain+79
#define	GetGroupMenuHelpManual				"HelpManual"
#define	PosGroupMenuHelpManual				PosGroupMenuMain+80
#define	GetGroupMenuHelpForum				"HelpForum"
#define	PosGroupMenuHelpForum				PosGroupMenuMain+81
#define	GetGroupMenuHelpBoincWebsite		"HelpBoincWebsite"
#define	PosGroupMenuHelpBoincWebsite		PosGroupMenuMain+82
#define	GetGroupMenuHelpAbout				"HelpAbout"
#define	PosGroupMenuHelpAbout				PosGroupMenuMain+83
#define	GetGroupMenuHelpUpdate				"HelpUpdate"
#define	PosGroupMenuHelpUpdate				PosGroupMenuMain+84
#define	GetGroupMenuHelpLog					"HelpLog"
#define	PosGroupMenuHelpLog					PosGroupMenuMain+85
#define	GetGroupMenuHelpLogRules			"HelpLogRules"
#define	PosGroupMenuHelpLogRules			PosGroupMenuMain+86

#define	GetGroupMenuCommonAllComputers		"CommonAllComputers"
#define	PosGroupMenuCommonAllComputers		PosGroupMenuMain+87
#define	GetGroupMenuCommonAlways			"CommonAlways"
#define	PosGroupMenuCommonAlways			PosGroupMenuMain+88
#define	GetGroupMenuCommonAuto				"CommonAuto"
#define	PosGroupMenuCommonAuto				PosGroupMenuMain+89
#define	GetGroupMenuCommonSnooze			"CommonSnooze"
#define	PosGroupMenuCommonSnooze			PosGroupMenuMain+90
#define	GetGroupMenuCommonNever				"CommonNever"
#define	PosGroupMenuCommonNever				PosGroupMenuMain+91

// GroupPopUpTasks
#define GroupPopUpTasks						"GroupPopUpTasks"
#define	PosGroupPopUpTasks					PosGroupMenuCommonNever+1
#define GetGroupPopUpTasksSuspend			"Suspend"
#define PosGroupPopUpTasksSuspend			PosGroupPopUpTasks
#define GetGroupPopUpTasksSuspendCp			"SuspendCp"
#define PosGroupPopUpTasksSuspendCp			PosGroupPopUpTasks+1
#define GetGroupPopUpTasksResume			"Resume"
#define PosGroupPopUpTasksResume			PosGroupPopUpTasks+2
#define GetGroupPopUpTasksAbort				"Abort"
#define PosGroupPopUpTasksAbort				PosGroupPopUpTasks+3
#define GetGroupPopUpTasksPoperties			"Properties"
#define PosGroupPopUpTasksPoperties			PosGroupPopUpTasks+4
#define GetGroupPopUpTasksProjectUpdate		"ProjectUpdate"
#define PosGroupPopUpTasksProjectUpdate		PosGroupPopUpTasks+5
#define GetGroupPopUpTasksProjectSuspend	"ProjectSuspend"
#define PosGroupPopUpTasksProjectSuspend	PosGroupPopUpTasks+6
#define GetGroupPopUpTasksProjectResume		"ProjectResume"
#define PosGroupPopUpTasksProjectResume		PosGroupPopUpTasks+7
#define GetGroupPopUpTasksFilter			"Filter"
#define PosGroupPopUpTasksFilter			PosGroupPopUpTasks+8
#define GetGroupPopUpTasksFilterReadyTR		"FilterReadyTR"
#define PosGroupPopUpTasksFilterReadyTR		PosGroupPopUpTasks+9
#define GetGroupPopUpTasksFilterWaitingTR	"FilterWaitingTR"
#define PosGroupPopUpTasksFilterWaitingTR	PosGroupPopUpTasks+10
#define GetGroupPopUpTasksFilterReadyTS		"FilterReadyTS"
#define PosGroupPopUpTasksFilterReadyTS		PosGroupPopUpTasks+11
#define GetGroupPopUpTasksFilterUpload		"FilterUpload"
#define PosGroupPopUpTasksFilterUpload		PosGroupPopUpTasks+12
#define GetGroupPopUpTasksFilterDownload	"FilterDownload"
#define PosGroupPopUpTasksFilterDownload	PosGroupPopUpTasks+13
#define GetGroupPopUpTasksFilterComputation	"FilterComputation"
#define PosGroupPopUpTasksFilterComputation	PosGroupPopUpTasks+14
#define GetGroupPopUpTasksFilterSuspended	"FilterSuspended"
#define PosGroupPopUpTasksFilterSuspended	PosGroupPopUpTasks+15
#define GetGroupPopUpTasksFilterAborted		"FilterAborted"
#define PosGroupPopUpTasksFilterAborted		PosGroupPopUpTasks+16
#define GetGroupPopUpTasksFilterRunning		"FilterRunning"
#define PosGroupPopUpTasksFilterRunning		PosGroupPopUpTasks+17
#define GetGroupPopUpTasksShowActive		"ShowActive"
#define PosGroupPopUpTasksShowActive		PosGroupPopUpTasks+18
#define GetGroupPopUpTasksShowNonCpuIntensive	"ShowNonCpuIntensive"
#define PosGroupPopUpTasksShowNonCpuIntensive	PosGroupPopUpTasks+19
#define GetGroupPopUpTasksShowCpu			"ShowCpu"
#define PosGroupPopUpTasksShowCpu			PosGroupPopUpTasks+20
#define GetGroupPopUpTasksShowGpu			"ShowGpu"
#define PosGroupPopUpTasksShowGpu			PosGroupPopUpTasks+21
#define GetGroupPopUpTasksShowGraphic		"ShowGraphic"
#define PosGroupPopUpTasksShowGraphic		PosGroupPopUpTasks+22
#define GetGroupPopUpTasksAddRule			"AddRule"
#define PosGroupPopUpTasksAddRule			PosGroupPopUpTasks+23

// GroupPopUpProjects
#define GroupPopUpProjects					"GroupPopUpProjects"
#define	PosGroupPopUpProjects				PosGroupPopUpTasksAddRule+1
#define GetGroupPopUpProjectsUpdate			"Update"
#define PosGroupPopUpProjectsUpdate			PosGroupPopUpProjects
#define GetGroupPopUpProjectsSuspend		"Suspend"
#define PosGroupPopUpProjectsSuspend		PosGroupPopUpProjects+1
#define GetGroupPopUpProjectsResume			"Resume"
#define PosGroupPopUpProjectsResume			PosGroupPopUpProjects+2
#define GetGroupPopUpProjectsNoNewTasks		"NoNewTasks"
#define PosGroupPopUpProjectsNoNewTasks		PosGroupPopUpProjects+3
#define GetGroupPopUpProjectsAllowNewTasks	"AllowNewTasks"
#define PosGroupPopUpProjectsAllowNewTasks	PosGroupPopUpProjects+4
#define GetGroupPopUpProjectsProperties		"Properties"
#define PosGroupPopUpProjectsProperties		PosGroupPopUpProjects+5
#define GetGroupPopUpProjectsAddAddRemove	"AddRemove"
#define PosGroupPopUpProjectsAddAddRemove	PosGroupPopUpProjects+6
#define GetGroupPopUpProjectsAddAddNew		"AddNew"
#define PosGroupPopUpProjectsAddAddNew		PosGroupPopUpProjects+7
#define GetGroupPopUpProjectsAddReset		"Reset"
#define PosGroupPopUpProjectsAddReset		PosGroupPopUpProjects+8
#define GetGroupPopUpProjectsAddDetach		"Detach"
#define PosGroupPopUpProjectsAddDetach		PosGroupPopUpProjects+9

// GroupPopUpTransfers
#define GroupPopUpTransfers					"GroupPopUpTransfers"
#define PosGroupPopUpTransfers				PosGroupPopUpProjectsAddDetach+1
#define GetPopUpTransfersRetry				"Retry"
#define PosPopUpTransfersRetry				PosGroupPopUpTransfers
#define GetPopUpTransfersRetryAll			"RetryAll"
#define PosPopUpTransfersRetryAll			PosGroupPopUpTransfers+1
#define GetPopUpTransfersAbort				"Abort"
#define PosPopUpTransfersAbort				PosGroupPopUpTransfers+2

// GroupPopUpMessages
#define GroupPopUpMessages					"GroupPopUpMessages"
#define PosGroupPopUpMessages				PosPopUpTransfersAbort+1
#define GetPopUpMessagesShowOnly			"ShowOnlyProjects"
#define PosPopUpMessagesShowOnly			PosGroupPopUpMessages
#define GetPopUpMessagesShowOnlyAll			"ShowOnlyProjectsAll"
#define PosPopUpMessagesShowOnlyAll			PosGroupPopUpMessages+1
#define GetPopUpMessagesIncludeGeneral		"IncludeGeneral"
#define PosPopUpMessagesIncludeGeneral		PosGroupPopUpMessages+2
#define GetPopUpMessagesSearch				"Search"
#define PosPopUpMessagesSearch				PosGroupPopUpMessages+4
#define GetPopUpMessagesCopyAll				"CopyAll"
#define PosPopUpMessagesCopyAll				PosGroupPopUpMessages+5
#define GetPopUpMessagesCopySelected		"CopySelected"
#define PosPopUpMessagesCopySelected		PosGroupPopUpMessages+6

// GroupPopUpTaskbar
#define GroupPopUpTaskbar					"GroupPopUpTaskbar"
#define PosGroupPopUpTaskbar				PosPopUpMessagesCopySelected+1
#define GetPopUpTaskbarOpen					"Open"
#define PosPopUpTaskbarOpen					PosGroupPopUpTaskbar
#define GetPopUpTaskbarUpdate				"Update"
#define PosPopUpTaskbarUpdate				PosGroupPopUpTaskbar+1
#define GetPopUpTaskbarAbout				"About"
#define PosPopUpTaskbarAbout				PosGroupPopUpTaskbar+2
#define GetPopUpTaskbarSnoozeCpu			"Snooze"
#define PosPopUpTaskbarSnoozeCpu			PosGroupPopUpTaskbar+3
#define GetPopUpTaskbarSnoozeGpu			"SnoozeGpu"
#define PosPopUpTaskbarSnoozeGpu			PosGroupPopUpTaskbar+4
#define GetPopUpTaskbarExit					"Exit"
#define PosPopUpTaskbarExit					PosGroupPopUpTaskbar+5

// GroupPopUpHeader
#define GroupPopUpHeader					"GroupPopUpHeader"
#define PosGroupPopUpHeader					PosPopUpTaskbarExit+1
#define GetPopUpHeaderEnable				"Enable"
#define PosPopUpHeaderEnable				PosGroupPopUpHeader+1

// GroupPopUpComputer
#define GroupPopUpComputer					"GroupPopUpComputer"
#define PosGroupPopUpComputer				PosPopUpHeaderEnable+1
#define GetPopUpComputerProperties			"Properties"
#define PosPopUpComputerProperties			PosGroupPopUpComputer+1

//GroupToolbarGraph
#define GroupToolbarGraph					"GroupToolbarGraphic"
#define PosGroupToolbarGraph				PosPopUpComputerProperties+1
#define GetGroupToolbarGraphCredit			"GraphCredit"
#define PosGroupToolbarGraphCredit			PosGroupToolbarGraph
#define GetGroupToolbarGraphTasks			"GraphTasks"
#define PosGroupToolbarGraphTasks			PosGroupToolbarGraph+1
#define GetGroupToolbarGraphTemperature		"GraphTemperature"
#define PosGroupToolbarGraphTemperature		PosGroupToolbarGraph+2

//GroupToolbarOperations
#define GroupToolbarOperations						"GroupToolbarOperations"
#define PosGroupToolbarOperations					PosGroupToolbarGraphTemperature+1
#define GetGroupToolbarOperationsSuspend			"Suspend"
#define PosGroupToolbarOperationsSuspend			PosGroupToolbarOperations
#define GetGroupToolbarOperationsResume				"Resume"
#define PosGroupToolbarOperationsResume				PosGroupToolbarOperations+1
#define GetGroupToolbarOperationsAbort				"Abort"
#define PosGroupToolbarOperationsAbort				PosGroupToolbarOperations+2
#define GetGroupToolbarOperationsUpdateProject		"UpdateProject"
#define PosGroupToolbarOperationsUpdateProject		PosGroupToolbarOperations+3
#define GetGroupToolbarOperationsSuspendProject		"SuspendProject"
#define PosGroupToolbarOperationsSuspendProject		PosGroupToolbarOperations+4
#define GetGroupToolbarOperationsResumeProject		"ResumeProject"
#define PosGroupToolbarOperationsResumeProject		PosGroupToolbarOperations+5
#define GetGroupToolbarOperationsNoNewWorkProject	"NoNewWorkProject"
#define PosGroupToolbarOperationsNoNewWorkProject	PosGroupToolbarOperations+6
#define GetGroupToolbarOperationsAllowNewProject	"AllowNewProject"
#define PosGroupToolbarOperationsAllowNewProject	PosGroupToolbarOperations+7

//GroupToolbarAllow
#define GroupToolbarAllow							"GroupToolbarAllow"
#define PosGroupToolbarAllow						PosGroupToolbarOperationsAllowNewProject+1
#define GetGroupToolbarAllowNetworkPref				"NetworkPref"
#define PosGroupToolbarAllowNetworkPref				PosGroupToolbarAllow
#define GetGroupToolbarAllowNetworkNever			"NetworkNever"
#define PosGroupToolbarAllowNetworkNever			PosGroupToolbarAllow+1
#define GetGroupToolbarAllowCpuPref					"CpuGpuPref"
#define PosGroupToolbarAllowCpuPref					PosGroupToolbarAllow+2
#define GetGroupToolbarAllowCpuSnooze				"CpuGpuSnooze"
#define PosGroupToolbarAllowCpuSnooze				PosGroupToolbarAllow+3
#define GetGroupToolbarAllowCpuNever				"CpuGpuNever"
#define PosGroupToolbarAllowCpuNever				PosGroupToolbarAllow+4
#define GetGroupToolbarAllowGpuPref					"GpuPref"
#define PosGroupToolbarAllowGpuPref					PosGroupToolbarAllow+5
#define GetGroupToolbarAllowGpuSnooze				"GpuSnooze"
#define PosGroupToolbarAllowGpuSnooze				PosGroupToolbarAllow+6
#define GetGroupToolbarAllowGpuNever				"GpuNever"
#define PosGroupToolbarAllowGpuNever				PosGroupToolbarAllow+7

//GroupDialogSettings
#define GroupDialogSettings							"GroupDialogSettings"
#define	PosGroupDialogSettings						PosGroupToolbarAllowGpuNever+1
#define GetGroupDialogSettingsTitle					"Title"
#define PosGroupDialogSettingsTitle					PosGroupDialogSettings
#define GetGroupDialogSettingsGeneralTitle			"GeneralTitle"
#define PosGroupDialogSettingsGeneralTitle			PosGroupDialogSettings+1
#define GetGroupDialogSettingsGeneralStartAtLogin	"GeneralStartAtLogin"
#define PosGroupDialogSettingsGeneralStartAtLogin	PosGroupDialogSettings+2
#define GetGroupDialogSettingsGeneralStartBOINC		"GeneralStartBOINC"
#define PosGroupDialogSettingsGeneralStartBOINC		PosGroupDialogSettings+3
#define GetGroupDialogSettingsGeneralStartBOINCDelay "GeneralStartBOINCDelay"
#define PosGroupDialogSettingsGeneralStartBOINCDelay PosGroupDialogSettings+4
#define GetGroupDialogSettingsGeneralStopOnExit		"GeneralStopBOINCOnExit"
#define PosGroupDialogSettingsGeneralStopOnExit		PosGroupDialogSettings+5
#define GetGroupDialogSettingsGeneralConTThrottle	"GeneralConTThrottle"
#define PosGroupDialogSettingsGeneralConTThrottle	PosGroupDialogSettings+6
#define GetGroupDialogSettingsGeneralHideStartup	"GeneralHideAtStartup"
#define	PosGroupDialogSettingsGeneralHideStartup	PosGroupDialogSettings+7

#define	GetGroupDialogSettingsGeneralPassword		"GeneralPassword"
#define	PosGroupDialogSettingsGeneralPassword		PosGroupDialogSettings+8
#define	GetGroupDialogSettingsGeneralPasswordShow	"GeneralShow"
#define	PosGroupDialogSettingsGeneralPasswordShow	PosGroupDialogSettings+9

#define GetGroupDialogSettingsGeneralLanguage		"GeneralLanguage"
#define PosGroupDialogSettingsGeneralLanguage		PosGroupDialogSettings+10
#define GetGroupDialogGeneralHotKeyMain				"GeneralHotKeyMain"
#define PosGroupDialogGeneralHotKeyMain				PosGroupDialogSettings+11

#define GetGroupDialogGeneralOs						"GeneralOs"
#define PosGroupDialogGeneralOs						PosGroupDialogSettings+12
#define GetGroupDialogGeneralOsQuestionW			"GeneralOsQuestionW"
#define PosGroupDialogGeneralOsQuestionW			PosGroupDialogSettings+13
#define GetGroupDialogGeneralOsQuestionL			"GeneralOsQuestionL"
#define PosGroupDialogGeneralOsQuestionL			PosGroupDialogSettings+14
#define GetGroupDialogGeneralOsQuestionM			"GeneralOsQuestionM"
#define PosGroupDialogGeneralOsQuestionM			PosGroupDialogSettings+15
#define GetGroupDialogGeneralWarningBoinc			"GeneralWarningBoinc"
#define PosGroupDialogGeneralWarningBoinc			PosGroupDialogSettings+16

#define GetGroupDialogSettingsViewTitle				"ViewTitle"
#define PosGroupDialogSettingsViewTitle				PosGroupDialogSettings+17
#define GetGroupDialogSettingsViewTimeFormat		"ViewTimeFormat"
#define PosGroupDialogSettingsViewTimeFormat		PosGroupDialogSettings+18
#define GetGroupDialogSettingsViewTimeFComboSystem	"ViewTimeFComboSystem"
#define PosGroupDialogSettingsViewTimeFComboSystem	PosGroupDialogSettings+19
#define GetGroupDialogSettingsViewTimeFCombo24h		"ViewTimeFCombo24h"
#define PosGroupDialogSettingsViewTimeFCombo24h		PosGroupDialogSettings+20
#define GetGroupDialogSettingsViewTimeFCombo12h		"ViewTimeFCombo12h"
#define PosGroupDialogSettingsViewTimeFCombo12h		PosGroupDialogSettings+21
#define GetGroupDialogSettingsViewTimeFComboUser	"ViewTimeFComboUser"
#define PosGroupDialogSettingsViewTimeFComboUser	PosGroupDialogSettings+22

#define GetGroupDialogSettingsViewRefreshRate		"ViewRefreshRate"
#define PosGroupDialogSettingsViewRefreshRate		PosGroupDialogSettings+23
#define GetGroupDialogSettingsViewRRateComboSlow	"ViewRRateComboSlow"
#define PosGroupDialogSettingsViewRRateComboSlow	PosGroupDialogSettings+24
#define GetGroupDialogSettingsViewRRateComboNormal	"ViewRRateComboNormal"
#define PosGroupDialogSettingsViewRRateComboNormal	PosGroupDialogSettings+25
#define GetGroupDialogSettingsViewRRRateComboQuick	"ViewRRateComboQuick"
#define PosGroupDialogSettingsViewRRateComboQuick	PosGroupDialogSettings+26
#define GetGroupDialogSettingsViewRRateComboManual	"ViewRRateComboManual"
#define PosGroupDialogSettingsViewRRateComboManual	PosGroupDialogSettings+27
#define GetGroupDialogSettingsViewSeconds			"ViewSeconds"
#define PosGroupDialogSettingsViewSeconds			PosGroupDialogSettings+28
#define GetGroupDialogSettingsViewSkin				"ViewSkin"
#define PosGroupDialogSettingsViewSkin				PosGroupDialogSettings+29
#define GetGroupDialogSettingsViewGridHorizontal	"ViewGridHorizontal"
#define PosGroupDialogSettingsViewGridHorizontal	PosGroupDialogSettings+30
#define GetGroupDialogSettingsViewGridVertical		"ViewGridVertical"
#define PosGroupDialogSettingsViewGridVertical		PosGroupDialogSettings+31
#define GetGroupDialogSettingsViewNumberFormat		"ViewNumberFormat"
#define PosGroupDialogSettingsViewNumberFormat		PosGroupDialogSettings+32
#define GetGroupDialogSettingsViewPercRect			"ViewPercRect"
#define PosGroupDialogSettingsViewPercRect			PosGroupDialogSettings+33
#define GetGroupDialogSettingsViewAdjustTime		"ViewAdjustTime"
#define PosGroupDialogSettingsViewAdjustTime		PosGroupDialogSettings+34
#define GetGroupDialogSettingsViewTranslatable		"ViewTranslatable"
#define PosGroupDialogSettingsViewTranslatable		PosGroupDialogSettings+35

#define GetGroupDialogSettingsProjectsTitle			"ProjectsTitle"
#define PosGroupDialogSettingsProjectsTitle			PosGroupDialogSettings+36
#define GetGroupDialogSettingsProjectsGroupName		"ProjectGroupName"
#define PosGroupDialogSettingsProjectsGroupName		PosGroupDialogSettings+37	
#define GetGroupDialogSettingsProjectProject		"ProjectProject"
#define PosGroupDialogSettingsProjectProject		PosGroupDialogSettings+38
#define GetGroupDialogSettingsProjectAccount		"ProjectAccount"
#define PosGroupDialogSettingsProjectAccount		PosGroupDialogSettings+39
#define GetGroupDialogSettingsProjectTeam			"ProjectTeam"
#define PosGroupDialogSettingsProjectTeam			PosGroupDialogSettings+40
#define GetGroupDialogSettingsProjectUserCredits	"ProjectUserCredits"
#define PosGroupDialogSettingsProjectUserCredits	PosGroupDialogSettings+41
#define GetGroupDialogSettingsProjectUserACredits	"ProjectUserACredits"
#define PosGroupDialogSettingsProjectUserACredits	PosGroupDialogSettings+42
#define GetGroupDialogSettingsProjectHostCredits	"ProjectHostCredits"
#define PosGroupDialogSettingsProjectHostCredits	PosGroupDialogSettings+43
#define GetGroupDialogSettingsProjectHostACredits	"ProjectHostACredits"
#define PosGroupDialogSettingsProjectHostACredits	PosGroupDialogSettings+44
#define GetGroupDialogSettingsProjectShare			"ProjectShare"
#define PosGroupDialogSettingsProjectShare			PosGroupDialogSettings+45
#define GetGroupDialogSettingsProjectTasks			"ProjectTasks"
#define PosGroupDialogSettingsProjectTasks			PosGroupDialogSettings+46
#define GetGroupDialogSettingsProjectTimeLeft		"ProjectTimeLeft"
#define PosGroupDialogSettingsProjectTimeLeft		PosGroupDialogSettings+47
#define GetGroupDialogSettingsProjectStatus			"ProjectStatus"
#define PosGroupDialogSettingsProjectStatus			PosGroupDialogSettings+48
#define GetGroupDialogSettingsProjectComputer		"ProjectComputer"
#define PosGroupDialogSettingsProjectComputer		PosGroupDialogSettings+49
#define GetGroupDialogSettingsProjectVenue			"ProjectVenue"
#define PosGroupDialogSettingsProjectVenue			PosGroupDialogSettings+50
#define GetGroupDialogSettingsProjectTasksDay		"ProjectTasksDay"
#define PosGroupDialogSettingsProjectTasksDay		PosGroupDialogSettings+51
#define GetGroupDialogSettingsProjectTasksWeek		"ProjectTasksWeek"
#define PosGroupDialogSettingsProjectTasksWeek		PosGroupDialogSettings+52
#define GetGroupDialogSettingsProjectRank			"ProjectRank"
#define PosGroupDialogSettingsProjectRank			PosGroupDialogSettings+53
#define GetGroupDialogSettingsProjectTeamRank		"ProjectTeamRank"
#define PosGroupDialogSettingsProjectTeamRank		PosGroupDialogSettings+54

#define GetGroupDialogSettingsTasksTitle			"TasksTitle"
#define PosGroupDialogSettingsTasksTitle			PosGroupDialogSettings+55
#define GetGroupDialogSettingsTasksGroupName		"TasksGroupName"
#define PosGroupDialogSettingsTasksGroupName		PosGroupDialogSettings+56
#define GetGroupDialogSettingsTasksButtonDefault	"TasksButtonDefault"
#define PosGroupDialogSettingsTasksButtonDefault	PosGroupDialogSettings+57
#define GetGroupDialogSettingsTasksCheckUserFriendly "TasksCheckUserFriendly"
#define PosGroupDialogSettingsTasksCheckUserFriendly PosGroupDialogSettings+58
#define GetGroupDialogSettingsTasksCheckCondense	"TasksCheckCondense"
#define PosGroupDialogSettingsTasksCheckCondense	PosGroupDialogSettings+59
#define GetGroupDialogSettingsTasksRatioLong		"TasksCheckRatioLong"
#define PosGroupDialogSettingsTasksRatioLong		PosGroupDialogSettings+60
#define GetGroupDialogSettingsTasksDeadlineRemaining "TasksDeadlineRemaining"	
#define PosGroupDialogSettingsTasksDeadlineRemaining PosGroupDialogSettings+61

#define GetGroupDialogSettingsTasksApplicationSortOnNr "TasksAppSortOnNr"	
#define PosGroupDialogSettingsTasksApplicationSortOnNr PosGroupDialogSettings+62

#define GetGroupDialogSettingsTasksSortStatus		 "TasksSortStatus"	
#define PosGroupDialogSettingsTasksSortStatus		 PosGroupDialogSettings+63

#define GetGroupDialogSettingsTasksCpuPercDigits	"TasksCpuPercDigits"	
#define PosGroupDialogSettingsTasksCpuPercDigits	PosGroupDialogSettings+64
#define GetGroupDialogSettingsTasksCpuProgressDigits "TasksCpuProgressDigits"	
#define PosGroupDialogSettingsTasksCpuProgressDigits PosGroupDialogSettings+65

#define GetGroupDialogSettingsTasksAlternatingStripes "TasksAlternatingStripes"
#define PosGroupDialogSettingsTasksAlternatingStripes PosGroupDialogSettings+66
#define GetGroupDialogSettingsTasksMsgBoxExclude	"TasksMsgBoxExclude"
#define PosGroupDialogSettingsTasksMsgBoxExclude	 PosGroupDialogSettings+67

#define GetGroupDialogSettingsTaskProject			"TaskProject"
#define PosGroupDialogSettingsTaskProject			PosGroupDialogSettings+68
#define GetGroupDialogSettingsTaskApplication		"TaskApplication"
#define PosGroupDialogSettingsTaskApplication		PosGroupDialogSettings+69
#define GetGroupDialogSettingsTaskName				"TaskName"
#define PosGroupDialogSettingsTaskName				PosGroupDialogSettings+70
#define GetGroupDialogSettingsTaskElapsedTime		"TaskElapsedTime"
#define PosGroupDialogSettingsTaskElapsedTime		PosGroupDialogSettings+71
#define GetGroupDialogSettingsTaskCpuPerc			"TaskCpuPerc"	
#define PosGroupDialogSettingsTaskCpuPerc			PosGroupDialogSettings+72
#define GetGroupDialogSettingsTaskProgress			"TaskProgress"	
#define PosGroupDialogSettingsTaskProgress			PosGroupDialogSettings+73
#define GetGroupDialogSettingsTaskTimeLeft			"TaskTimeLeft"
#define PosGroupDialogSettingsTaskTimeLeft			PosGroupDialogSettings+74
#define GetGroupDialogSettingsTaskDeadline			"TaskDeadline"
#define PosGroupDialogSettingsTaskDeadline			PosGroupDialogSettings+75
#define GetGroupDialogSettingsTaskUse				"TaskUse"
#define PosGroupDialogSettingsTaskUse				PosGroupDialogSettings+76
#define GetGroupDialogSettingsTaskStatus			"TaskStatus"
#define PosGroupDialogSettingsTaskStatus			PosGroupDialogSettings+77
#define GetGroupDialogSettingsTaskThrottle			"TaskThrottle"
#define PosGroupDialogSettingsTaskThrottle			PosGroupDialogSettings+78
#define GetGroupDialogSettingsTaskTemperature		"TaskTemperature"
#define PosGroupDialogSettingsTaskTemperature		PosGroupDialogSettings+79
#define GetGroupDialogSettingsTaskComputer			"TaskComputer"
#define PosGroupDialogSettingsTaskComputer			PosGroupDialogSettings+80
#define GetGroupDialogSettingsTaskCheckpoint		"TaskCheckpoint"
#define PosGroupDialogSettingsTaskCheckpoint		PosGroupDialogSettings+81
#define GetGroupDialogSettingsTaskDebt				"TaskDebt"
#define PosGroupDialogSettingsTaskDebt				PosGroupDialogSettings+82
#define GetGroupDialogSettingsTaskReceived			"TaskReceived"
#define PosGroupDialogSettingsTaskReceived			PosGroupDialogSettings+83
#define GetGroupDialogSettingsTaskAccount			"TaskAccount"
#define PosGroupDialogSettingsTaskAccount			PosGroupDialogSettings+84
#define GetGroupDialogSettingsTaskVirtual			"TaskVirtual"
#define PosGroupDialogSettingsTaskVirtual			PosGroupDialogSettings+85
#define GetGroupDialogSettingsTaskMemory			"TaskMemory"
#define PosGroupDialogSettingsTaskMemory			PosGroupDialogSettings+86

#define GetGroupDialogSettingsTransferTitle			"TransferTitle"
#define PosGroupDialogSettingsTransferTitle			PosGroupDialogSettings+87

#define GetGroupDialogSettingsMessageTitle			"MessageTitle"
#define PosGroupDialogSettingsMessageTitle			PosGroupDialogSettings+88
#define GetGroupDialogSettingsMessagesKeep			"MessageKeep"
#define PosGroupDialogSettingsMessagesKeep			PosGroupDialogSettings+89	
#define GetGroupDialogSettingsMessageProject		"MessageProject"
#define PosGroupDialogSettingsMessageProject		PosGroupDialogSettings+90	
#define GetGroupDialogSettingsMessageMessage		"MessageMessage"
#define PosGroupDialogSettingsMessageMessage		PosGroupDialogSettings+91	
#define GetGroupDialogSettingsMessageHighlight1		"MessageHighlight1"
#define PosGroupDialogSettingsMessageHighlight1		PosGroupDialogSettings+92
#define GetGroupDialogSettingsMessageHighlight2		"MessageHighlight2"
#define PosGroupDialogSettingsMessageHighlight2		PosGroupDialogSettings+93
#define GetGroupDialogSettingsMessageHighlight3		"MessageHighlight3"
#define PosGroupDialogSettingsMessageHighlight3		PosGroupDialogSettings+94
#define GetGroupDialogSettingsMessageHighlight4		"MessageHighlight4"
#define PosGroupDialogSettingsMessageHighlight4		PosGroupDialogSettings+95
#define GetGroupDialogSettingsMessageHighlight5		"MessageHighlight5"
#define PosGroupDialogSettingsMessageHighlight5		PosGroupDialogSettings+96
#define GetGroupDialogSettingsMessageHighlight6		"MessageHighlight6"
#define PosGroupDialogSettingsMessageHighlight6		PosGroupDialogSettings+97
#define GetGroupDialogSettingsMessageHighlight7		"MessageHighlight7"
#define PosGroupDialogSettingsMessageHighlight7		PosGroupDialogSettings+98
#define GetGroupDialogSettingsMessageHighlight8		"MessageHighlight8"
#define PosGroupDialogSettingsMessageHighlight8		PosGroupDialogSettings+99
#define GetGroupDialogSettingsMessageHighlight9		"MessageHighlight9"
#define PosGroupDialogSettingsMessageHighlight9		PosGroupDialogSettings+100
#define GetGroupDialogSettingsMessageHighlight10	"MessageHighlight10"
#define PosGroupDialogSettingsMessageHighlight10	PosGroupDialogSettings+101

#define GetGroupDialogSettingsHistoryTitle			"HistoryTitle"
#define PosGroupDialogSettingsHistoryTitle			PosGroupDialogSettings+102

#define GetGroupDialogSettingsHistoryGroupName		"HistoryGroupName"
#define PosGroupDialogSettingsHistoryGroupName		PosGroupDialogSettings+103
#define GetGroupDialogSettingsHistoryProject		"HistoryProject"
#define PosGroupDialogSettingsHistoryProject		PosGroupDialogSettings+104
#define GetGroupDialogSettingsHistoryApplication	"HistoryApplication"
#define PosGroupDialogSettingsHistoryApplication	PosGroupDialogSettings+105
#define GetGroupDialogSettingsHistoryName			"HistoryName"
#define PosGroupDialogSettingsHistoryName			PosGroupDialogSettings+106
#define GetGroupDialogSettingsHistoryElapsedTime	"HistoryElapsedTime"
#define PosGroupDialogSettingsHistoryElapsedTim		PosGroupDialogSettings+107
#define GetGroupDialogSettingsHistoryCompleted		"HistoryCompleted"
#define PosGroupDialogSettingsHistoryCompleted		PosGroupDialogSettings+108
#define GetGroupDialogSettingsHistoryReported		"HistoryReported"
#define PosGroupDialogSettingsHistoryReported		PosGroupDialogSettings+109
#define GetGroupDialogSettingsHistoryUse			"HistoryUse"
#define PosGroupDialogSettingsHistoryUse			PosGroupDialogSettings+110
#define GetGroupDialogSettingsHistoryCpuPerc		"HistoryCpuPerc"
#define PosGroupDialogSettingsHistoryCpuPerc		PosGroupDialogSettings+111
#define GetGroupDialogSettingsHistoryStatus			"HistoryStatus"
#define PosGroupDialogSettingsHistoryStatus			PosGroupDialogSettings+112
#define GetGroupDialogSettingsHistoryComputer		"HistoryComputer"
#define PosGroupDialogSettingsHistoryComputer		PosGroupDialogSettings+113
#define GetGroupDialogSettingsHistoryVirtual		"HistoryVirtual"
#define PosGroupDialogSettingsHistoryVirtual		PosGroupDialogSettings+114
#define GetGroupDialogSettingsHistoryMemory			"HistoryMemory"
#define PosGroupDialogSettingsHistoryMemory			PosGroupDialogSettings+115

#define GetGroupDialogSettingsHistoryEnable			"HistoryEnable"
#define PosGroupDialogSettingsHistoryEnable			PosGroupDialogSettings+116
#define GetGroupDialogSettingsHistorySmart			"HistorySmart"
#define PosGroupDialogSettingsHistorySmart			PosGroupDialogSettings+117
#define GetGroupDialogSettingsHistoryAccurate		"HistoryAccurate"
#define PosGroupDialogSettingsHistoryAccurate		PosGroupDialogSettings+118
#define GetGroupDialogSettingsHistoryRemove			"HistoryRemove"
#define PosGroupDialogSettingsHistoryRemove			PosGroupDialogSettings+119
#define GetGroupDialogSettingsHistoryRemoveDays		"HistoryRemoveDays"
#define PosGroupDialogSettingsHistoryRemoveDays		PosGroupDialogSettings+120
#define GetGroupDialogSettingsHistoryRemoveHours	"HistoryRemoveHours"
#define PosGroupDialogSettingsHistoryRemoveHours	PosGroupDialogSettings+121
#define GetGroupDialogSettingsHistoryDayWarning		"HistoryDayWarning"
#define PosGroupDialogSettingsHistoryDayWarning		PosGroupDialogSettings+122
#define GetGroupDialogSettingsHistoryMinUpdate		"HistoryMinUpdate"
#define PosGroupDialogSettingsHistoryMinUpdate		PosGroupDialogSettings+123
#define GetGroupDialogSettingsHistoryMaxUpdate		"HistoryMaxUpdate"
#define PosGroupDialogSettingsHistoryMaxUpdate		PosGroupDialogSettings+124
#define GetGroupDialogSettingsHistoryMaxUpdateSec	"HistoryMaxUpdateSec"
#define PosGroupDialogSettingsHistoryMaxUpdateSec	PosGroupDialogSettings+125
#define GetGroupDialogSettingsHistoryMoveLong		"HistoryMoveLong"
#define PosGroupDialogSettingsHistoryMoveLong		PosGroupDialogSettings+126
#define GetGroupDialogSettingsHistoryBackup			"HistoryBackup"	
#define PosGroupDialogSettingsHistoryBackup			PosGroupDialogSettings+127

#define GetGroupDialogSettingsNoticesTitle			"NoticesTitle"
#define PosGroupDialogSettingsNoticesTitle			PosGroupDialogSettings+128
#define GetGroupDialogSettingsNoticesHideAfter		"HideAfter"
#define PosGroupDialogSettingsNoticesHideAfter		PosGroupDialogSettings+129
#define GetGroupDialogSettingsNoticesCheckEvery		"CheckEvery"
#define PosGroupDialogSettingsNoticesCheckEvery		PosGroupDialogSettings+130
#define GetGroupDialogSettingsNoticesCheckAlert		"Alert"
#define PosGroupDialogSettingsNoticesCheckAlert		PosGroupDialogSettings+131
#define GetGroupDialogSettingsNoticesMinutes		"Minutes"
#define PosGroupDialogSettingsNoticesMinutes		PosGroupDialogSettings+132
#define GetGroupDialogSettingsNoticesDays			"Days"
#define PosGroupDialogSettingsNoticesDays			PosGroupDialogSettings+133
#define GetGroupDialogSettingsNoticesSort			"NoticesSort"
#define PosGroupDialogSettingsNoticesSort			PosGroupDialogSettings+134
#define GetGroupDialogSettingsNoticesSortCategory	"NoticesSortCategory"
#define PosGroupDialogSettingsNoticesSortCategory	PosGroupDialogSettings+135
#define GetGroupDialogSettingsNoticesSortProject	"NoticesSortProject"
#define PosGroupDialogSettingsNoticesSortProject	PosGroupDialogSettings+136
#define GetGroupDialogSettingsNoticesSortCreation	"NoticesSortCreation"
#define PosGroupDialogSettingsNoticesSortCreation	PosGroupDialogSettings+137
#define GetGroupDialogSettingsNoticesSortArrival	"NoticesSortArrival"
#define PosGroupDialogSettingsNoticesSortArrival	PosGroupDialogSettings+138

#define GetGroupDialogSettingsWarningsTitle			"WarningsTitle"
#define PosGroupDialogSettingsWarningsTitle			PosGroupDialogSettings+139
#define GetGroupDialogSettingsWarningCheckpoint		"WarningCheckpoint"
#define PosGroupDialogSettingsWarningCheckpoint		PosGroupDialogSettings+140
#define GetGroupDialogSettingsWarningDeadline		"WarningDeadline"
#define PosGroupDialogSettingsWarningDeadline		PosGroupDialogSettings+141
#define GetGroupDialogSettingsWarningDeadlineDays	"WarningDeadlineDays"
#define PosGroupDialogSettingsWarningDeadlineDays	PosGroupDialogSettings+142
#define GetGroupDialogSettingsWarningDeadlinehours	"WarningDeadlinehours"
#define PosGroupDialogSettingsWarningDeadlinehours	PosGroupDialogSettings+143
#define GetGroupDialogSettingsWarningIsActiveWhen	"WarningIsActiveWhen"
#define PosGroupDialogSettingsWarningIsActiveWhen	PosGroupDialogSettings+144
#define GetGroupDialogSettingsWarningCpuBetween		"WarningCpuBetween"
#define PosGroupDialogSettingsWarningCpuBetween		PosGroupDialogSettings+145
#define GetGroupDialogSettingsWarningGpuBetween		"WarningGpuBetween"
#define PosGroupDialogSettingsWarningGpuBetween		PosGroupDialogSettings+146
#define GetGroupDialogSettingsWarningCpuPBetween	"WarningCpuPBetween"
#define PosGroupDialogSettingsWarningCpuPBetween	PosGroupDialogSettings+147
#define GetGroupDialogSettingsWarningCpuTasks		"WarningCpuTasks"
#define PosGroupDialogSettingsWarningCpuTasks		PosGroupDialogSettings+148
#define GetGroupDialogSettingsWarningGpuTasks		"WarningGpuTasks"
#define PosGroupDialogSettingsWarningGpuTasks		PosGroupDialogSettings+149
#define GetGroupDialogSettingsWarningProject		"WarningProject"
#define PosGroupDialogSettingsWarningProject		PosGroupDialogSettings+150
#define GetGroupDialogSettingsWarningComputer		"WarningComputer"
#define PosGroupDialogSettingsWarningComputer		PosGroupDialogSettings+151

#define GetGroupDialogSettingsGadgetTitle			"GadgetTitle"
#define PosGroupDialogSettingsGadgetTitle			PosGroupDialogSettings+152
#define GetGroupDialogSettingsGadgetModeTasks		"GadgetModeTasks"
#define PosGroupDialogSettingsGadgetModeTasks		PosGroupDialogSettings+153
#define GetGroupDialogSettingsGadgetModeComputers	"GadgetModeComputers"
#define PosGroupDialogSettingsGadgetModeComputers	PosGroupDialogSettings+154
#define GetGroupDialogSettingsGadgetRadioShowTop	"GadgetRadioShowTop"
#define PosGroupDialogSettingsGadgetRadioShowTop	PosGroupDialogSettings+155
#define GetGroupDialogSettingsGadgetRadioShow		"GadgetRadioShow"
#define PosGroupDialogSettingsGadgetRadioShow		PosGroupDialogSettings+156
#define GetGroupDialogSettingsGadgetRadioShowIcon	"GadgetRadioShowIcon"
#define PosGroupDialogSettingsGadgetRadioShowIcon	PosGroupDialogSettings+157
#define GetGroupDialogSettingsGadgetRadioShowFor	"GadgetShowIconFor"
#define PosGroupDialogSettingsGadgetRadioShowFor	PosGroupDialogSettings+158

#define GetGroupDialogSettingsRulesTitle			"RulesTitle"
#define PosGroupDialogSettingsRulesTitle			PosGroupDialogSettings+159
#define GetGroupDialogSettingsRulesColumnComputer	"RulesColumnComputer"
#define PosGroupDialogSettingsRulesColumnComputer	PosGroupDialogSettings+160
#define GetGroupDialogSettingsRulesColumnProject	"RulesColumnProject"
#define PosGroupDialogSettingsRulesColumnProject	PosGroupDialogSettings+161
#define GetGroupDialogSettingsRulesColumnApplication "RulesColumnApplication"
#define PosGroupDialogSettingsRulesColumnApplication PosGroupDialogSettings+162
#define GetGroupDialogSettingsRulesColumnRule		"RulesColumnRule"
#define PosGroupDialogSettingsRulesColumnRule		PosGroupDialogSettings+163
#define GetGroupDialogSettingsRulesDeleteYesNo		"RulesDeleteYesNo"
#define PosGroupDialogSettingsRulesDeleteYesNo		PosGroupDialogSettings+164

#define GetGroupDialogSettingsCloudTitle			"CloudTitle"
#define PosGroupDialogSettingsCloudTitle			PosGroupDialogSettings+165
#define GetGroupDialogSettingsWWWGroup				"WWWGroup"
#define PosGroupDialogSettingsWWWGroup				PosGroupDialogSettings+166
#define GetGroupDialogSettingsWWWEnable				"WWWEnable"
#define PosGroupDialogSettingsWWWEnable				PosGroupDialogSettings+167

#define GetGroupDialogSettingsMobileTitle			"MobileTitle"
#define PosGroupDialogSettingsMobileTitle			PosGroupDialogSettings+168
#define GetGroupDialogSettingsMobileEnable			"MobileEnable"
#define PosGroupDialogSettingsMobileEnable			PosGroupDialogSettings+169
#define GetGroupDialogSettingsMobilePassword		"MobilePassword"
#define PosGroupDialogSettingsMobilePassword		PosGroupDialogSettings+170

#define GetGroupDialogSettingsMobileHeaderFont		"MobileHeaderFont"
#define PosGroupDialogSettingsMobileHeaderFont		PosGroupDialogSettings+171
#define GetGroupDialogSettingsMobileFont			"MobileFont"
#define PosGroupDialogSettingsMobileFont			PosGroupDialogSettings+172

#define GetGroupDialogSettingsMobileWarning			"MobileWarning"
#define PosGroupDialogSettingsMobileWarning			PosGroupDialogSettings+173

#define GetGroupDialogSettingsExpertTitle			"ExpertTitle"
#define PosGroupDialogSettingsExpertTitle			PosGroupDialogSettings+174
#define GetGroupDialogSettingsExpertOverride		"ExpertOverride"
#define PosGroupDialogSettingsExpertOverride		PosGroupDialogSettings+175
#define GetGroupDialogSettingsExpertEncryption		"ExpertEncryption"
#define PosGroupDialogSettingsExpertEncryption		PosGroupDialogSettings+176

#define GetGroupDialogSettingsExpertManualCPID		"ExpertCpidManual"
#define PosGroupDialogSettingsExpertManualCPID		PosGroupDialogSettings+177

#define GetGroupDialogSettingsExpertOverrideWarning	"ExpertOverrideWarning"
#define PosGroupDialogSettingsExpertOverrideWarning	PosGroupDialogSettings+178
#define GetGroupDialogSettingsExpertInvalidFolder	"ExpertInvalidFolder"
#define PosGroupDialogSettingsExpertInvalidFolder	PosGroupDialogSettings+179
#define GetGroupDialogSettingsExpertReconnect		"ExpertReconnect"
#define PosGroupDialogSettingsExpertReconnect		PosGroupDialogSettings+180
#define GetGroupDialogSettingsExpertThreadTimeout	"ExpertThreadTimeout"
#define PosGroupDialogSettingsExpertThreadTimeout	PosGroupDialogSettings+181
#define GetGroupDialogSettingsExpertSilentReboot	"ExpertSilentReboot"
#define PosGroupDialogSettingsExpertSilentReboot	PosGroupDialogSettings+182
#define GetGroupDialogSettingsExpertTimes			"ExpertRunTimes"
#define PosGroupDialogSettingsExpertTimes			PosGroupDialogSettings+183
#define GetGroupDialogSettingsExpertTimesGraph		"ExpertRunTimesGraph"
#define PosGroupDialogSettingsExpertTimesGraph		PosGroupDialogSettings+184

#define GetGroupDialogSettingsExpertPriority		"ExpertPriority"
#define PosGroupDialogSettingsExpertPriority		PosGroupDialogSettings+185
#define GetGroupDialogSettingsExpertPriorityNormal	"ExpertPriorityNormal"
#define PosGroupDialogSettingsExpertPriorityNormal	PosGroupDialogSettings+186
#define GetGroupDialogSettingsExpertPriorityBelow	"ExpertPriorityBelow"
#define PosGroupDialogSettingsExpertPriorityBelow	PosGroupDialogSettings+187
#define GetGroupDialogSettingsExpertPriorityLow		"ExpertPriorityLow"
#define PosGroupDialogSettingsExpertPriorityLow		PosGroupDialogSettings+188

#define GetGroupDialogSettingsGeneralSeconds		"GeneralSeconds"
#define PosGroupDialogSettingsGeneralSeconds		PosGroupDialogSettings+189
#define GetGroupDialogSettingsGeneralRefresh		"GeneralRefresh"
#define PosGroupDialogSettingsGeneralRefresh		PosGroupDialogSettings+190

// GroupDialogColor
#define GroupDialogColor							"GroupDialogColor"
#define PosGroupDialogColor							PosGroupDialogSettingsGeneralRefresh+1
#define GetDialogColorTitle							"Title"
#define PosDialogColorTitle							PosGroupDialogColor

#define GetDialogColorTitleGeneral					"TitleGeneral"
#define PosDialogColorTitleGeneral					PosGroupDialogColor+1
#define GetDialogColorGeneralGrid					"GeneralGrid"
#define PosDialogColorGeneralGrid					PosGroupDialogColor+2
#define GetDialogColorGeneralFont					"GeneralFont"
#define PosDialogColorGeneralFont					PosGroupDialogColor+3

#define GetDialogColorGeneralFontSelected			"GeneralFontSel"
#define PosDialogColorGeneralFontSelected			PosGroupDialogColor+4
#define GetDialogColorGeneralBackSelected			"GeneralBackSel"
#define PosDialogColorGeneralBackSelected			PosGroupDialogColor+5

#define GetDialogColorGeneralBar					"GeneralBar"
#define PosDialogColorGeneralBar					PosGroupDialogColor+6
#define GetDialogColorGeneralBarOutline				"GeneralBarOutline"
#define PosDialogColorGeneralBarOutline				PosGroupDialogColor+7

#define GetDialogColorTitleMessages					"TitleMessages"
#define PosDialogColorTitleMessages					PosGroupDialogColor+8
#define GetDialogColorMessageNormal					"MessageNormal"
#define PosDialogColorMessageNormal					PosGroupDialogColor+9
#define GetDialogColorMessageHigh					"MessageHigh"
#define PosDialogColorMessageHigh					PosGroupDialogColor+10
#define GetDialogColorMessageHighest				"MessageHighest"
#define PosDialogColorMessageHighest				PosGroupDialogColor+11

#define GetDialogColorMessageHighlight1				"MessageHighlight1"
#define PosDialogColorMessageHighlight1				PosGroupDialogColor+12
#define GetDialogColorMessageHighlight2				"MessageHighlight2"
#define PosDialogColorMessageHighlight2				PosGroupDialogColor+13
#define GetDialogColorMessageHighlight3				"MessageHighlight3"
#define PosDialogColorMessageHighlight3				PosGroupDialogColor+14
#define GetDialogColorMessageHighlight4				"MessageHighlight4"
#define PosDialogColorMessageHighlight4				PosGroupDialogColor+15
#define GetDialogColorMessageHighlight5				"MessageHighlight5"
#define PosDialogColorMessageHighlight5				PosGroupDialogColor+16
#define GetDialogColorMessageHighlight6				"MessageHighlight6"
#define PosDialogColorMessageHighlight6				PosGroupDialogColor+17
#define GetDialogColorMessageHighlight7				"MessageHighlight7"
#define PosDialogColorMessageHighlight7				PosGroupDialogColor+18
#define GetDialogColorMessageHighlight8				"MessageHighlight8"
#define PosDialogColorMessageHighlight8				PosGroupDialogColor+19
#define GetDialogColorMessageHighlight9				"MessageHighlight9"
#define PosDialogColorMessageHighlight9				PosGroupDialogColor+20
#define GetDialogColorMessageHighlight10			"MessageHighlight10"
#define PosDialogColorMessageHighlight10			PosGroupDialogColor+21

#define GetDialogColorTitleProjects					"TitleProjects"
#define PosDialogColorTitleProjects					PosGroupDialogColor+22
#define GetDialogColorProjectsRunning				"ProjectsRunning"
#define PosDialogColorProjectsRunning				PosGroupDialogColor+23
#define GetDialogColorProjectsSuspended				"ProjectsSuspended"
#define PosDialogColorProjectsSuspended				PosGroupDialogColor+24
#define GetDialogColorProjectsNoNewTasks			"ProjectsNoNewTasks"
#define PosDialogColorProjectsNoNewTasks			PosGroupDialogColor+25

#define GetDialogColorTitleTasks					"TitleTasks"
#define PosDialogColorTitleTasks					PosGroupDialogColor+26
#define GetDialogColorUploadDownload				"UploadDownload"
#define PosDialogColorUploadDownload				PosGroupDialogColor+27
#define GetDialogColorUploadDownloadGpu				"UploadDownloadGpu"
#define PosDialogColorUploadDownloadGpu				PosGroupDialogColor+28
#define GetDialogColorRunning						"Running"
#define PosDialogColorRunning						PosGroupDialogColor+29
#define GetDialogColorRunningGpu					"RunningGpu"
#define PosDialogColorRunningGpu					PosGroupDialogColor+30
#define GetDialogColorHighPriority					"HighPriority"
#define PosDialogColorHighPriority					PosGroupDialogColor+31
#define GetDialogColorHighPriorityGpu				"HighPriorityGpu"
#define PosDialogColorHighPriorityGpu				PosGroupDialogColor+32
#define GetDialogColorAbort							"Abort"
#define PosDialogColorAbort							PosGroupDialogColor+33
#define GetDialogColorWaitingToRun					"WaitingToRun"
#define PosDialogColorWaitingToRun					PosGroupDialogColor+34
#define GetDialogColorWaitingToRunGpu				"WaitingToRunGpu"
#define PosDialogColorWaitingToRunGpu				PosGroupDialogColor+35
#define GetDialogColorReadyToStart					"ReadyToStart"
#define PosDialogColorReadyToStart					PosGroupDialogColor+36
#define GetDialogColorReadyToStartGpu				"ReadyToStartGpu"
#define PosDialogColorReadyToStartGpu				PosGroupDialogColor+37
#define GetDialogColorError							"Error"
#define PosDialogColorError							PosGroupDialogColor+38
#define GetDialogColorErrorGpu						"ErrorGpu"
#define PosDialogColorErrorGpu						PosGroupDialogColor+39
#define GetDialogColorSuspended						"Suspended"
#define PosDialogColorSuspended						PosGroupDialogColor+40
#define GetDialogColorSuspendedGpu					"SuspendedGpu"
#define PosDialogColorSuspendedGpu					PosGroupDialogColor+41

#define GetDialogColorTitleFont						"TitleFont"
#define PosDialogColorTitleFont						PosGroupDialogColor+42
#define GetDialogColorFontUseSystem					"FontUseSystem"
#define PosDialogColorFontUseSystem					PosGroupDialogColor+43
#define GetDialogColorFontGroupGeneral				"FontGroupGeneral"
#define PosDialogColorFontGroupGeneral				PosGroupDialogColor+44
#define GetDialogColorFontGroupGadget				"FontGroupGadget"
#define PosDialogColorFontGroupGadget				PosGroupDialogColor+45

#define GetDialogColorTitleHistory					"TitleHistory"
#define PosDialogColorTitleHistory					PosGroupDialogColor+46
#define GetDialogColorUpload						"Upload"
#define PosDialogColorUpload						PosGroupDialogColor+47
#define GetDialogColorUploadGpu						"UploadGpu"
#define PosDialogColorUploadGpu						PosGroupDialogColor+48
#define GetDialogColorReadyToReport					"ReadyToReport"
#define PosDialogColorReadyToReport					PosGroupDialogColor+49
#define GetDialogColorReadyToReportGpu				"ReadyToReportGpu"
#define PosDialogColorReadyToReportGpu				PosGroupDialogColor+50
#define GetDialogColorReported						"Reported"
#define PosDialogColorReported						PosGroupDialogColor+51
#define GetDialogColorReportedGpu					"ReportedGpu"
#define PosDialogColorReportedGpu					PosGroupDialogColor+52
#define GetDialogColorReportedError					"ReportedError"
#define PosDialogColorReportedError					PosGroupDialogColor+53
#define GetDialogColorReportedErrorGpu				"ReportedErrorGpu"
#define PosDialogColorReportedErrorGpu				PosGroupDialogColor+54
#define GetDialogColorMissed						"Missed"
#define PosDialogColorMissed						PosGroupDialogColor+55

#define GetDialogColorTitleTemperature				"TitleTemperature"
#define PosDialogColorTitleTemperature				PosGroupDialogColor+56
#define GetDialogColorTemperatureCore				"TemperatureCore"
#define PosDialogColorTemperatureCore				PosGroupDialogColor+57	
#define GetDialogColorTemperatureGpu				"TemperatureGpu"
#define PosDialogColorTemperatureGpu				PosGroupDialogColor+58
#define GetDialogColorTemperatureTCpu				"TemperatureTCpu"
#define PosDialogColorTemperatureTCpu				PosGroupDialogColor+59
#define GetDialogColorTemperatureTGpu				"TemperatureTGpu"
#define PosDialogColorTemperatureTGpu				PosGroupDialogColor+60
#define GetDialogColorTemperatureMax				"TemperatureMax"
#define PosDialogColorTemperatureMax				PosGroupDialogColor+61
#define GetDialogColorTemperatureBackground			"TemperatureBackground"
#define PosDialogColorTemperatureBackground			PosGroupDialogColor+62
#define GetDialogColorTemperatureGridFine			"TemperatureGridFine"
#define PosDialogColorTemperatureGridFine			PosGroupDialogColor+63
#define GetDialogColorTemperatureGrid				"TemperatureGrid"
#define PosDialogColorTemperatureGrid				PosGroupDialogColor+64
#define GetDialogColorTemperatureText				"TemperatureText"
#define PosDialogColorTemperatureText				PosGroupDialogColor+65

//efine PosDialogColorGraphicAverage				


#define GetDialogColorTitleWarnings					"TitleWarnings"
#define PosDialogColorTitleWarnings					PosGroupDialogColor+66
#define GetDialogColorCpuRatio						"CpuRatio"
#define PosDialogColorCpuRatio						PosGroupDialogColor+67
#define GetDialogColorGpuRatio						"GpuRatio"
#define PosDialogColorGpuRatio						PosGroupDialogColor+68
#define GetDialogColorDeadline						"Deadline"
#define PosDialogColorDeadline						PosGroupDialogColor+69
#define GetDialogColorDeadlineRunning				"DeadlineRunning"
#define PosDialogColorDeadlineRunning				PosGroupDialogColor+70
#define GetDialogColorTasks							"Tasks"
#define PosDialogColorTasks							PosGroupDialogColor+71
#define GetDialogColorCheckpoint					"Checkpoint"
#define PosDialogColorCheckpoint					PosGroupDialogColor+72
#define GetDialogColorReadSettingsWarning			"ReadSettingsWarning"
#define PosDialogColorReadSettingsWarning			PosGroupDialogColor+73
#define GetDialogColorWriteSettingsWarning			"WriteSettingsWarning"
#define PosDialogColorWriteSettingsWarning			PosGroupDialogColor+74
#define GetDialogColorFailedSettingsWarning			"FailedSettingsWarning"
#define PosDialogColorFailedSettingsWarning			PosGroupDialogColor+75

// GroupDialogBoincSettings
#define GroupDialogBoincSettings					"GroupDialogBoincSettings"
#define PosGroupDialogBoincSettings					PosDialogColorFailedSettingsWarning+1
#define GetDialogBoincSettingsTitle					"Title"
#define PosDialogBoincSettingsTitle					PosGroupDialogBoincSettings
#define GetDialogBoincSettingsGeneralAnd			"GeneralAnd"
#define PosDialogBoincSettingsGeneralAnd			PosGroupDialogBoincSettings+1
#define GetDialogBoincSettingsGeneralMinutes		"GeneralMinutes"
#define PosDialogBoincSettingsGeneralMinutes		PosGroupDialogBoincSettings+2
#define GetDialogBoincSettingsGeneralNoRestrictions	"GeneralNoRestrictions"
#define PosDialogBoincSettingsGeneralNoRestrictions	PosGroupDialogBoincSettings+3

#define GetDialogBoincSettingsGeneralSunday			"GeneralSunday"
#define PosDialogBoincSettingsGeneralSunday			PosGroupDialogBoincSettings+4
#define GetDialogBoincSettingsGeneralMonday			"GeneralMonday"
#define PosDialogBoincSettingsGeneralMonday			PosGroupDialogBoincSettings+5
#define GetDialogBoincSettingsGeneralTuesday		"GeneralTuesday"
#define PosDialogBoincSettingsGeneralTuesday		PosGroupDialogBoincSettings+6
#define GetDialogBoincSettingsGeneralWednesday		"GeneralWednesday"
#define PosDialogBoincSettingsGeneralWednesday		PosGroupDialogBoincSettings+7
#define GetDialogBoincSettingsGeneralThursday		"GeneralThursday"
#define PosDialogBoincSettingsGeneralThursday		PosGroupDialogBoincSettings+8
#define GetDialogBoincSettingsGeneralFriday			"GeneralFriday"
#define PosDialogBoincSettingsGeneralFriday			PosGroupDialogBoincSettings+9
#define GetDialogBoincSettingsGeneralSaturday		"GeneralSaturday"
#define PosDialogBoincSettingsGeneralSaturday		PosGroupDialogBoincSettings+10

#define GetDialogBoincSettingsGeneralHourMinutes	"GeneralHourMinutes"
#define PosDialogBoincSettingsGeneralHourMinutes	PosGroupDialogBoincSettings+11
#define GetDialogBoincSettingsGeneralGraphic		"GeneralGraphic"
#define PosDialogBoincSettingsGeneralGraphic		PosGroupDialogBoincSettings+12
#define GetDialogBoincSettingsGeneralKBsec			"GeneralKBsec"
#define PosDialogBoincSettingsGeneralKBsec			PosGroupDialogBoincSettings+13
#define GetDialogBoincSettingsGeneralDayBetween		"GeneralDayBetween"
#define PosDialogBoincSettingsGeneralDayBetween		PosGroupDialogBoincSettings+14
#define GetDialogBoincSettingsGeneralSeconds		"GeneralSeconds"
#define PosDialogBoincSettingsGeneralSeconds		PosGroupDialogBoincSettings+15
#define GetDialogBoincSettingsGeneralDayOfWeekTitle	"GeneralDayOfWeekTitle"
#define PosDialogBoincSettingsGeneralDayOfWeekTitle	PosGroupDialogBoincSettings+16

#define GetDialogBoincSettingsTitleProcessor		"TitleProcessor"
#define PosDialogBoincSettingsTitleProcessor		PosGroupDialogBoincSettings+17
#define GetDialogBoincSettingsPComputingTitle		"PComputingTitle"
#define PosDialogBoincSettingsPComputingTitle		PosGroupDialogBoincSettings+18
#define GetDialogBoincSettingsPComputingBatteries	"PComputingBatteries"
#define PosDialogBoincSettingsPComputingBatteries	PosGroupDialogBoincSettings+19
#define GetDialogBoincSettingsPComputingUseCpu		"PComputingUseCpu"
#define PosDialogBoincSettingsPComputingUseCpu		PosGroupDialogBoincSettings+20
#define GetDialogBoincSettingsPComputingUseGpu		"PComputingUseGpu"
#define PosDialogBoincSettingsPComputingUseGpu		PosGroupDialogBoincSettings+21
#define GetDialogBoincSettingsPComputingOnlyIdle	"PComputingOnlyIdle"
#define PosDialogBoincSettingsPComputingOnlyIdle	PosGroupDialogBoincSettings+22
#define GetDialogBoincSettingsPComputingUsageLess	"PComputingUsageLess"
#define PosDialogBoincSettingsPComputingUsageLess	PosGroupDialogBoincSettings+23
#define GetDialogBoincSettingsPComputingUsageLessP	"PComputingUsageLessP"
#define PosDialogBoincSettingsPComputingUsageLessP	PosGroupDialogBoincSettings+24

#define GetDialogBoincSettingsPOtherTitle			"POtherTitle"
#define PosDialogBoincSettingsPOtherTitle			PosGroupDialogBoincSettings+25
#define GetDialogBoincSettingsPOtherSwitchEvery		"POtherSwitchEvery"
#define PosDialogBoincSettingsPOtherSwitchEvery		PosGroupDialogBoincSettings+26
#define GetDialogBoincSettingsPOtherMultiprocessors	"POtherMultiprocessors"
#define PosDialogBoincSettingsPOtherMultiprocessors	PosGroupDialogBoincSettings+27
#define GetDialogBoincSettingsPOtherUseAtMost		"POtherUseAtMost"
#define PosDialogBoincSettingsPOtherUseAtMost		PosGroupDialogBoincSettings+28
#define GetDialogBoincSettingsPOtherPercProcessor	"POtherPercProcessor"
#define PosDialogBoincSettingsPOtherPercProcessor	PosGroupDialogBoincSettings+29
#define GetDialogBoincSettingsPOtherPercCpu			"POtherPercCpu"
#define PosDialogBoincSettingsPOtherPercCpu			PosGroupDialogBoincSettings+30

#define GetDialogBoincSettingsTitleNetwork			"TitleNetwork"
#define PosDialogBoincSettingsTitleNetwork			PosGroupDialogBoincSettings+31
#define GetDialogBoincSettingsNetGeneralTitle		"NetGeneralTitle"
#define PosDialogBoincSettingsNetGeneralTitle		PosGroupDialogBoincSettings+32
#define GetDialogBoincSettingsNetGeneralMaxDownload	"NetGeneralMaxDownload"
#define PosDialogBoincSettingsNetGeneralMaxDownload	PosGroupDialogBoincSettings+33
#define GetDialogBoincSettingsNGeneralMaxUpload		"NetGeneralMaxUpload"
#define PosDialogBoincSettingsNGeneralMaxUpload		PosGroupDialogBoincSettings+34

#define	GetDialogBoincSettingsTransferAtMost		"NetTransferAtMost"
#define	PosDialogBoincSettingsTransferAtMost		PosGroupDialogBoincSettings+35
#define GetDialogBoincSettingsMBytesEvery			"NetMBytesEvery"
#define PosDialogBoincSettingsMBytesEvery			PosGroupDialogBoincSettings+36
#define GetDialogBoincSettingsTransferAtMostDays	"NetTransferAtMostDays"
#define PosDialogBoincSettingsTransferAtMostDays	PosGroupDialogBoincSettings+37

#define GetDialogBoincSettingsNGeneralConnectEvery		"NetGeneralConnectEvery"
#define PosDialogBoincSettingsNGeneralConnectEvery		PosGroupDialogBoincSettings+38
#define GetDialogBoincSettingsNGeneralConnectEveryDays	"NetGeneralConnectEveryDays"
#define PosDialogBoincSettingsNGeneralConnectEveryDays	PosGroupDialogBoincSettings+39


#define GetDialogBoincSettingsNGeneralMinWorkBuffer	"NetGeneralMinWorkBuffer"
#define PosDialogBoincSettingsNGeneralMinWorkBuffer	PosGroupDialogBoincSettings+40
#define GetDialogBoincSettingsNGeneralWorkBuffer	"NetGeneralWorkBuffer"
#define PosDialogBoincSettingsNGeneralWorkBuffer	PosGroupDialogBoincSettings+41
#define GetDialogBoincSettingsNetGeneralWURegulator	"NetGeneralWURegulator"
#define PosDialogBoincSettingsNetGeneralWURegulator	PosGroupDialogBoincSettings+42
#define GetDialogBoincSettingsNetGeneralDaysMax		"NetGeneralDaysMax"
#define PosDialogBoincSettingsNetGeneralDaysMax		PosGroupDialogBoincSettings+43

#define GetDialogBoincSettingsNetGeneralSkipImage	"NetGeneralSkipImage"
#define PosDialogBoincSettingsNetGeneralSkipImage	PosGroupDialogBoincSettings+44

#define GetDialogBoincSettingsNetConnectionTitle	"NetConnectionTitle"
#define PosDialogBoincSettingsNetConnectionTitle	PosGroupDialogBoincSettings+45
#define GetDialogBoincSettingsNetConnectionConfirm	"NetConnectionConfirm"
#define PosDialogBoincSettingsNetConnectionConfirm	PosGroupDialogBoincSettings+46
#define GetDialogBoincSettingsNetConnectionDWD		"NetConnectionDWD"
#define PosDialogBoincSettingsNetConnectionDWD		PosGroupDialogBoincSettings+47

#define GetDialogBoincSettingsNetNetworkTitle		"NetNetworkTitle"
#define PosDialogBoincSettingsNetNetworkTitle		PosGroupDialogBoincSettings+48

#define GetDialogBoincSettingsTitleDisk				"TitleDisk"
#define PosDialogBoincSettingsTitleDisk				PosGroupDialogBoincSettings+49
#define GetDialogBoincSettingsDiskDiskTitle			"DiskDiskTitle"
#define PosDialogBoincSettingsDiskDiskTitle			PosGroupDialogBoincSettings+50
#define GetDialogBoincSettingsDiskDiskAtMost		"DiskDiskAtMost"
#define PosDialogBoincSettingsDiskDiskAtMost		PosGroupDialogBoincSettings+51
#define GetDialogBoincSettingsDiskDiskAtLeast		"DiskDiskAtLeast"
#define PosDialogBoincSettingsDiskDiskAtLeast		PosGroupDialogBoincSettings+52
#define GetDialogBoincSettingsDiskDiskWriteEvery	"DiskDiskWriteEvery"
#define PosDialogBoincSettingsDiskDiskWriteEvery	PosGroupDialogBoincSettings+53
#define GetDialogBoincSettingsDiskDiskGigaDisk		"DiskDiskGigaDisk"
#define PosDialogBoincSettingsDiskDiskGigaDisk		PosGroupDialogBoincSettings+54
#define GetDialogBoincSettingsDiskDiskGigaDiskFree	"DiskDiskGigaDiskFree"
#define PosDialogBoincSettingsDiskDiskGigaDiskFree	PosGroupDialogBoincSettings+55
#define GetDialogBoincSettingsDiskDiskGigaDiskTotal	"DiskDiskGigaDiskTotal"
#define PosDialogBoincSettingsDiskDiskGigaDiskTotal	PosGroupDialogBoincSettings+56
#define GetDialogBoincSettingsDiskDiskPage			"DiskDiskPage"
#define PosDialogBoincSettingsDiskDiskPage			PosGroupDialogBoincSettings+57

#define GetDialogBoincSettingsDiskMemoryTitle		"DiskMemoryTitle"
#define PosDialogBoincSettingsDiskMemoryTitle		PosGroupDialogBoincSettings+58
#define GetDialogBoincSettingsDiskMemoryInUse		"DiskMemoryInUse"
#define PosDialogBoincSettingsDiskMemoryInUse		PosGroupDialogBoincSettings+59
#define GetDialogBoincSettingsDiskMemoryIdle		"DiskMemoryIdle"
#define PosDialogBoincSettingsDiskMemoryIdle		PosGroupDialogBoincSettings+60
#define GetDialogBoincSettingsDiskMemoryLeaveIn		"DiskMemoryLeaveIn"
#define PosDialogBoincSettingsDiskMemoryLeaveIn		PosGroupDialogBoincSettings+61

#define	GetDialogBoincSettingsExclusiveTitle		"TitleExclusive"
#define	PosDialogBoincSettingsExclusiveTitle		PosGroupDialogBoincSettings+62
#define	GetDialogBoincSettingsExclusiveTitleAdd		"TitleExclusiveAdd"
#define	PosDialogBoincSettingsExclusiveTitleAdd		PosGroupDialogBoincSettings+63
#define	GetDialogBoincSettingsExclusiveSelect		"ExclusiveSelect"
#define	PosDialogBoincSettingsExclusiveSelect		PosGroupDialogBoincSettings+64
#define	GetDialogBoincSettingsExclusiveGpu			"ExclusiveGpu"
#define	PosDialogBoincSettingsExclusiveGpu			PosGroupDialogBoincSettings+65

#define GetDialogBoincSettingsButtonClearWarning	"ButtonClearWarning"
#define PosDialogBoincSettingsButtonClearWarning	PosGroupDialogBoincSettings+66

// GroupDialogGraphic
#define GroupDialogGraphic					"GroupDialogGraphic"
#define PosGroupDialogGraphic				PosDialogBoincSettingsButtonClearWarning+1
#define GetDialogGraphicTitle				"Title"
#define PosDialogGraphicTitle				PosGroupDialogGraphic
#define GetDialogGraphicTitleComputers		"TitleComputers"
#define PosDialogGraphicTitleComputers		PosGroupDialogGraphic+1
#define GetDialogGraphicTitleProjects		"TitleProjects"
#define PosDialogGraphicTitleProjects		PosGroupDialogGraphic+2
#define GetDialogGraphicRadioUser			"RadioUser"
#define PosDialogGraphicRadioUser			PosGroupDialogGraphic+3
#define GetDialogGraphicRadioComputer		"RadioComputer"
#define PosDialogGraphicRadioComputer		PosGroupDialogGraphic+4
#define GetDialogGraphicRadioTotal			"RadioTotal"
#define PosDialogGraphicRadioTotal			PosGroupDialogGraphic+5
#define GetDialogGraphicRadioAverage		"RadioAverage"
#define PosDialogGraphicRadioAverage		PosGroupDialogGraphic+6
#define GetDialogGraphicButtonUpdate		"ButtonUpdate"
#define PosDialogGraphicButtonUpdate		PosGroupDialogGraphic+7
#define GetDialogGraphicEmptyComputer		"EmptyComputer"
#define PosDialogGraphicEmptyComputer		PosGroupDialogGraphic+8
#define GetDialogGraphicEmptyProject		"EmptyProject"
#define PosDialogGraphicEmptyProject		PosGroupDialogGraphic+9
#define GetDialogGraphicNoData				"NoData"
#define PosDialogGraphicNoData				PosGroupDialogGraphic+10
#define GetDialogGraphicDoubleClick			"DoubleClick"
#define PosDialogGraphicDoubleClick			PosGroupDialogGraphic+11

#define GetDialogGraphicMultiple			"MultipleSelections"
#define PosDialogGraphicMultiple			PosGroupDialogGraphic+12
#define GetDialogGraphicAverage				"Average"
#define PosDialogGraphicAverage				PosGroupDialogGraphic+13

#define GetDialogGraphicLineThickness		"LineThickness"
#define PosDialogGraphicLineThickness		PosGroupDialogGraphic+14
#define GetDialogGraphicPeriod				"Period"
#define PosDialogGraphicPeriod				PosGroupDialogGraphic+15
#define GetDialogGraphicPeriodDays			"Days"
#define PosDialogGraphicPeriodDays			PosGroupDialogGraphic+16
#define GetDialogGraphicCombineProject		"CombineProjects"
#define PosDialogGraphicCombineProject		PosGroupDialogGraphic+17
#define GetDialogGraphicExpanded			"Expanded"
#define PosDialogGraphicExpanded			PosGroupDialogGraphic+18

#define GroupDialogGraphicTasks				"GroupDialogGraphicTasks"
#define PosGroupDialogGraphicTasks			PosDialogGraphicExpanded+1
#define GetDialogGraphicTasksTitle			"Title"
#define PosDialogGraphicTasksTitle			PosGroupDialogGraphicTasks

#define GroupDialogGraphicDeadline			"GroupDialogGraphicDeadline"
#define PosGroupDialogGraphicDeadline		PosDialogGraphicTasksTitle+1
#define GetDialogGraphicDeadlineTitle		"Title"
#define PosDialogGraphicDeadlineTitle		PosGroupDialogGraphicDeadline

#define GroupDialogGraphicTransfer			"GroupDialogGraphicTransfer"
#define PosGroupDialogGraphicTransfer		PosDialogGraphicDeadlineTitle+1
#define GetDialogGraphicTransferTitle		"Title"
#define PosDialogGraphicTransferTitle		PosGroupDialogGraphicTransfer
#define GetDialogGraphicTransferUp			"Up"
#define PosDialogGraphicTransferUp			PosGroupDialogGraphicTransfer+1
#define GetDialogGraphicTransferDown		"Down"
#define PosDialogGraphicTransferDown		PosGroupDialogGraphicTransfer+2

#define GroupDialogHistoryLong				"GroupDialogHistoryLong"
#define PosGroupDialogHistoryLong			PosDialogGraphicTransferDown+1
#define GetDialogHistoryLongTitle			"Title"
#define PosDialogHistoryLongTitle			PosGroupDialogHistoryLong

// GroupDialogGraphicColors
#define GroupDialogGraphicColors				"GroupDialogGraphicColors"
#define PosGroupDialogGraphicColors				PosDialogHistoryLongTitle+1
#define GetDialogGraphicColorsTitle				"Title"
#define PosDialogGraphicColorsTitle				PosGroupDialogGraphicColors
#define GetDialogGraphicColorsButtonBackground	"ButtonBackground"
#define PosDialogGraphicColorsButtonBackground	PosGroupDialogGraphicColors+1
#define GetDialogGraphicColorsButtonProjectText	"ButtonProjectText"
#define PosDialogGraphicColorsButtonProjectText	PosGroupDialogGraphicColors+2
#define GetDialogGraphicColorsButtonScaleText	"ButtonScaleText"
#define PosDialogGraphicColorsButtonScaleText	PosGroupDialogGraphicColors+3
#define GetDialogGraphicColorsButtonGrid		"ButtonGrid"
#define PosDialogGraphicColorsButtonGrid		PosGroupDialogGraphicColors+4
#define GetDialogGraphicColorsButtonColor		"ButtonColor"
#define PosDialogGraphicColorsButtonColor		PosGroupDialogGraphicColors+5
#define GetDialogGraphicColorFixed				"CheckFixed"
#define PosDialogGraphicColorFixed				PosGroupDialogGraphicColors+7
#define GetDialogGraphicColorsAverage			"ButtonAverage"
#define PosDialogGraphicColorsAverage			PosGroupDialogGraphicColors+8

// GroupDialogTemperatureGraphic
#define GroupDialogTemperatureGraphic			"GroupDialogTemperatureGraphic"
#define PosGroupDialogTemperatureGraphic		PosDialogGraphicColorsAverage+1
#define GetDialogTemperatureGraphicTitle		"Title"
#define PosDialogTemperatureGraphicTitle		PosGroupDialogTemperatureGraphic
#define GetDialogTemperatureGraphicC1Minute		"Combo1Minute"
#define PosDialogTemperatureGraphicC1Minute		PosGroupDialogTemperatureGraphic+1
#define GetDialogTemperatureGraphicC5Minute		"Combo5Minute"
#define PosDialogTemperatureGraphicC5Minute		PosGroupDialogTemperatureGraphic+2
#define GetDialogTemperatureGraphicC10Minute	"Combo10Minute"
#define PosDialogTemperatureGraphicC10Minute	PosGroupDialogTemperatureGraphic+3
#define GetDialogTemperatureGraphicC30Minute	"Combo30Minute"
#define PosDialogTemperatureGraphicC30Minute	PosGroupDialogTemperatureGraphic+4
#define GetDialogTemperatureGraphicC60Minute	"Combo60Minute"
#define PosDialogTemperatureGraphicC60Minute	PosGroupDialogTemperatureGraphic+5
#define GetDialogTemperatureGraphicC3Hour		"Combo3Hour"
#define PosDialogTemperatureGraphicC3Hour		PosGroupDialogTemperatureGraphic+6
#define GetDialogTemperatureGraphicC6Hour		"Combo6Hour"
#define PosDialogTemperatureGraphicC6Hour		PosGroupDialogTemperatureGraphic+7
#define GetDialogTemperatureGraphicC12Hour		"Combo12Hour"
#define PosDialogTemperatureGraphicC12Hour		PosGroupDialogTemperatureGraphic+8
#define GetDialogTemperatureGraphicC24Hour		"Combo24Hour"
#define PosDialogTemperatureGraphicC24Hour		PosGroupDialogTemperatureGraphic+9
#define GetDialogTemperatureGraphicCore			"Core"
#define PosDialogTemperatureGraphicCore			PosGroupDialogTemperatureGraphic+10
#define GetDialogTemperatureGraphicGpu			"Gpu"
#define PosDialogTemperatureGraphicGpu			PosGroupDialogTemperatureGraphic+11
#define GetDialogTemperatureGraphicCpuPerc		"CpuPerc"
#define PosDialogTemperatureGraphicCpuPerc		PosGroupDialogTemperatureGraphic+12
#define GetDialogTemperatureGraphicGpuPerc		"GpuPerc"
#define PosDialogTemperatureGraphicGpuPerc		PosGroupDialogTemperatureGraphic+13
#define GetDialogTemperatureGraphicCpuMax		"CoreMax"
#define PosDialogTemperatureGraphicCpuMax		PosGroupDialogTemperatureGraphic+14
#define GetDialogTemperatureGraphicGpuMax		"GpuMax"
#define PosDialogTemperatureGraphicGpuMax		PosGroupDialogTemperatureGraphic+15

// GroupDialogThreadGraphic
#define GroupDialogThreadGraphic				"GroupDialogThreadGraphic"
#define PosGroupDialogThreadGraphic				PosDialogTemperatureGraphicGpuMax+1
#define GetDialogThreadGraphicTitle				"Title"
#define PosDialogThreadGraphicTitle				PosGroupDialogThreadGraphic

#define GroupDialogTThrottle					"GroupDialogTThrottle"
#define PosGroupDialogTThrottle					PosDialogThreadGraphicTitle+1
#define GetDialogTThrottleTitleComputer			"TitleComputers"
#define PosDialogTThrottleTitleComputer			PosGroupDialogTThrottle
#define GetDialogTThrottleTitleTemperature		"TitleTemperature"
#define PosDialogTThrottleTitleTemperature		PosGroupDialogTThrottle+1
#define GetDialogTThrottleTitleRunPercentage	"TitleRunPercentage"
#define PosDialogTThrottleTitleRunPercentage	PosGroupDialogTThrottle+2
#define GetDialogTThrottleCore					"Core"
#define PosDialogTThrottleCore					PosGroupDialogTThrottle+3
#define GetDialogTThrottleSetCore				"SetCore"
#define PosDialogTThrottleSetCore				PosGroupDialogTThrottle+4
#define GetDialogTThrottleGpu					"Gpu"
#define PosDialogTThrottleGpu					PosGroupDialogTThrottle+5
#define GetDialogTThrottleSetGpu				"SetGpu"
#define PosDialogTThrottleSetGpu				PosGroupDialogTThrottle+6
#define GetDialogTThrottleRunningCpu			"RunningCpu"
#define PosDialogTThrottleRunningCpu			PosGroupDialogTThrottle+7
#define GetDialogTThrottleMaxCpu				"MaxCpu"
#define PosDialogTThrottleMaxCpu				PosGroupDialogTThrottle+8
#define GetDialogTThrottleMinCpu				"MinCpu"
#define PosDialogTThrottleMinCpu				PosGroupDialogTThrottle+9
#define GetDialogTThrottleRunningGpu			"RunningGpu"
#define PosDialogTThrottleRunningGpu			PosGroupDialogTThrottle+10
#define GetDialogTThrottleAutoActive			"AutoActive"
#define PosDialogTThrottleAutoActive			PosGroupDialogTThrottle+11

// GroupDialogSetDebt
#define GroupDialogSetDebt							"GroupDialogSetDebt"
#define PosGroupDialogSetDebt						PosDialogTThrottleAutoActive+1
#define GetDialogSetDebtTitle						"Title"
#define PosDialogSetDebtTitle						PosGroupDialogSetDebt
#define GetDialogSetDebtCpuLong						"CpuLong"
#define PosDialogSetDebtCpuLong						PosGroupDialogSetDebt+1
#define GetDialogSetDebtNVidiaLong					"NVidiaLong"
#define PosDialogSetDebtNVidiaLong					PosGroupDialogSetDebt+2
#define GetDialogSetDebtAtiLong						"AtiLong"
#define PosDialogSetDebtAtiLong						PosGroupDialogSetDebt+3
#define GetDialogSetDebtCpuShort					"CpuShort"
#define PosDialogSetDebtCpuShort					PosGroupDialogSetDebt+4
#define GetDialogSetDebtNVidiaShort					"NVidiaShort"
#define PosDialogSetDebtNVidiaShort					PosGroupDialogSetDebt+5
#define GetDialogSetDebtAtiShort					"AtiShort"
#define PosDialogSetDebtAtiShort					PosGroupDialogSetDebt+6

// GroupDialogAddProject
#define GroupDialogAddProject						"GroupDialogAddProject"
#define PosGroupDialogAddProject					PosDialogSetDebtAtiShort+1
#define GetDialogAddProjectTitle					"Title"
#define PosDialogAddProjectTitle					PosGroupDialogAddProject
#define GetDialogAddProjectMain						"ComputerMain"
#define PosDialogAddProjectMain						PosGroupDialogAddProject+1
#define GetDialogAddProjectLoginName				"LoginName"
#define PosDialogAddProjectLoginName				PosGroupDialogAddProject+2
#define GetDialogAddProjectPassword					"Password"
#define PosDialogAddProjectPassword					PosGroupDialogAddProject+3
#define GetDialogAddProjectUserName					"UserName"
#define PosDialogAddProjectUserName					PosGroupDialogAddProject+4
#define GetDialogAddProjectCreateWhile				"CreateWhile"
#define PosDialogAddProjectCreateWhile				PosGroupDialogAddProject+5
#define GetDialogAddProjectComputers				"AddComputers"
#define PosDialogAddProjectComputers				PosGroupDialogAddProject+6

#define GetDialogAddProjectMsgInvalidLoginName		"MsgInvalidLoginName"
#define PosDialogAddProjectMsgInvalidLoginName		PosGroupDialogAddProject+7
#define GetDialogAddProjectMsgPasswordsEqual		"MsgPasswordsEqual"
#define PosDialogAddProjectMsgPasswordsEqual		PosGroupDialogAddProject+8
#define GetDialogAddProjectMsgUsernameMissing		"MsgUsernameMissing"
#define PosDialogAddProjectMsgUsernameMissing		PosGroupDialogAddProject+9
#define GetDialogAddProjectMsgProjectUrlShort		"MsgProjectUrlShort"
#define PosDialogAddProjectMsgProjectUrlShort		PosGroupDialogAddProject+10
#define GetDialogAddProjectMsgBusy					"MsgBusy"
#define PosDialogAddProjectMsgBusy					PosGroupDialogAddProject+11
#define GetDialogAddProjectMsgAddedSuccessfully		"MsgAddedSuccessfully"
#define PosDialogAddProjectMsgAddedSuccessfully		PosGroupDialogAddProject+12
#define GetDialogAddProjectMsgErrorCreating			"MsgErrorCreating"
#define PosDialogAddProjectMsgErrorCreating			PosGroupDialogAddProject+13
#define GetDialogAddProjectMsgErrorUrl				"MsgErrorUrl"
#define PosDialogAddProjectMsgErrorUrl				PosGroupDialogAddProject+14
#define GetDialogAddProjectMsgErrorFailedContact	"MsgErrorFailedContact"
#define PosDialogAddProjectMsgErrorFailedContact	PosGroupDialogAddProject+15
#define GetDialogAddProjectMsgErrorPassword			"MsgErrorPassword"
#define PosDialogAddProjectMsgErrorPassword			PosGroupDialogAddProject+16
#define GetDialogAddProjectMsgErrorAlreadyAtt		"MsgErrorAlreadyAtt"
#define PosDialogAddProjectMsgErrorAlreadyAtt		PosGroupDialogAddProject+17
#define GetDialogAddProjectMsgErrorNotDatabase		"MsgErrorNotDatabase"
#define PosDialogAddProjectMsgErrorNotDatabase		PosGroupDialogAddProject+18
#define GetDialogAddProjectMsgErrorUnableConnect	"MsgErrorUnableConnect"
#define PosDialogAddProjectMsgErrorUnableConnect	PosGroupDialogAddProject+19
#define GetDialogAddProjectMsgErrorNotFound			"MsgErrorNotFound"
#define PosDialogAddProjectMsgErrorNotFound			PosGroupDialogAddProject+20
#define GetDialogAddProjectMsgErrorEmail			"MsgErrorBadEmail"
#define PosDialogAddProjectMsgErrorEmail			PosGroupDialogAddProject+21

#define GetDialogAddProjectButtonWeb				"ButtonWeb"
#define PosDialogAddProjectButtonWeb				PosGroupDialogAddProject+22
#define GetDialogAddProjectButtonAdd				"ButtonAdd"
#define PosDialogAddProjectButtonAdd				PosGroupDialogAddProject+23

// GroupDialogAddAccountManager
#define GroupDialogAddAccountManager				"GroupDialogAddAccountManager"
#define PosGroupDialogAddAccountManager				PosDialogAddProjectButtonAdd+1
#define GetDialogAddAccountManagerTitle				"Title"
#define PosDialogAddAccountManagerTitle				PosGroupDialogAddAccountManager
#define GetDialogAddAccountManagerUserName			"UserName"
#define PosDialogAddAccountManagerUserName			PosGroupDialogAddAccountManager+1
#define GetDialogAddAccountManagerPassword			"Password"
#define PosDialogAddAccountManagerPassword			PosGroupDialogAddAccountManager+2
#define GetDialogAddAccountManagerButtonWeb			"ButtonWeb"
#define PosDialogAddAccountManagerButtonWeb			PosGroupDialogAddAccountManager+3
#define GetDialogAddAccountManagerButtonAdd			"ButtonAdd"
#define PosDialogAddAccountManagerButtonAdd			PosGroupDialogAddAccountManager+4
#define GetDialogAddAccountManagerButtonRemove		"ButtonRemove"
#define PosDialogAddAccountManagerButtonRemove		PosGroupDialogAddAccountManager+5
#define GetDialogAddAccountManagerAlready			"MsgAlready"
#define PosDialogAddAccountManagerAlready			PosGroupDialogAddAccountManager+6

// GroupDialogProjectProperties
#define GroupDialogProjectProperties			"GroupDialogProjectProperties"
#define PosGroupDialogProjectProperties			PosDialogAddAccountManagerAlready+1
#define GetDialogProjectPropComputer			"Computer"
#define PosDialogProjectPropComputer			PosGroupDialogProjectProperties
#define GetDialogProjectPropTitleProjectP		"TitleProjectP"
#define PosDialogProjectPropTitleProjectP		PosGroupDialogProjectProperties+1
#define GetDialogProjectPropProject				"Project"
#define PosDialogProjectPropProject				PosGroupDialogProjectProperties+2
#define GetDialogProjectPropGeneral				"General"
#define PosDialogProjectPropGeneral				PosGroupDialogProjectProperties+3
#define GetDialogProjectPropMasterUrl			"MasterUrl"
#define PosDialogProjectPropMasterUrl			PosGroupDialogProjectProperties+4
#define GetDialogProjectPropUserName			"UserName"
#define PosDialogProjectPropUserName			PosGroupDialogProjectProperties+5
#define GetDialogProjectPropTeamName			"Team"
#define PosDialogProjectPropTeamName			PosGroupDialogProjectProperties+6
#define GetPosDialogProjectPropVenue			"Venue"
#define PosDialogProjectPropVenue				PosGroupDialogProjectProperties+7
#define GetPosDialogProjectPropTasksCompleted	"TasksCompleted"
#define PosDialogProjectPropTasksCompleted		PosGroupDialogProjectProperties+8
#define GetPosDialogProjectTasksError			"TasksError"
#define PosDialogProjectTasksError				PosGroupDialogProjectProperties+9
#define GetDialogProjectPropResourseShare		"ResourseShare"
#define PosDialogProjectPropResourseShare		PosGroupDialogProjectProperties+10
#define GetDialogProjectPropFileUploadDeferred	"FileUploadDeferred"
#define PosDialogProjectPropFileUploadDeferred	PosGroupDialogProjectProperties+11
#define GetDialogProjectPropFileDownloadDeferred "FileDownloadDeferred"
#define PosDialogProjectPropFileDownloadDeferred PosGroupDialogProjectProperties+12
#define GetDialogProjectPropComputerId			"ComputerId"
#define PosDialogProjectPropComputerId			PosGroupDialogProjectProperties+13
#define GetDialogProjectPropProjectId			"ExternalProjectId"
#define PosDialogProjectPropProjectId			PosGroupDialogProjectProperties+14
#define GetDialogProjectPropNonCpuIntensive		"NonCpuIntensive"
#define PosDialogProjectPropNonCpuIntensive		PosGroupDialogProjectProperties+15
#define GetDialogProjectPropSuspendedGui		"SuspendedGui"
#define PosDialogProjectPropSuspendedGui		PosGroupDialogProjectProperties+16
#define GetDialogProjectPropDontReqWork			"DontReqWork"
#define PosDialogProjectPropDontReqWork			PosGroupDialogProjectProperties+17
#define GetDialogProjectPropSchedulerInProg		"SchedulerInProg"
#define PosDialogProjectPropSchedulerInProg		PosGroupDialogProjectProperties+18
#define GetDialogProjectPropAttachedAcountM		"AttachedAcountM"
#define PosDialogProjectPropAttachedAcountM		PosGroupDialogProjectProperties+19
#define GetDialogProjectPropDetachWD			"DetachWD"
#define PosDialogProjectPropDetachWD			PosGroupDialogProjectProperties+20
#define GetDialogProjectPropEnded				"Ended"
#define PosDialogProjectPropEnded				PosGroupDialogProjectProperties+21
#define GetDialogProjectPropCredit				"Credit"
#define PosDialogProjectPropCredit				PosGroupDialogProjectProperties+22
#define GetDialogProjectPropUserCredit			"UserCredit"
#define PosDialogProjectPropUserCredit			PosGroupDialogProjectProperties+23
#define GetDialogProjectPropHostCredit			"HostCredit"
#define PosDialogProjectPropHostCredit			PosGroupDialogProjectProperties+24
#define GetDialogProjectPropMore				"More"
#define PosDialogProjectPropMore				PosGroupDialogProjectProperties+25
#define GetDialogProjectPropDurationF			"DurationF"
#define PosDialogProjectPropDurationF			PosGroupDialogProjectProperties+26
#define GetDialogProjectPropSchedulingPriority	"Priority"
#define PosDialogProjectPropSchedulingPriority	PosGroupDialogProjectProperties+27
#define GetDialogProjectPropCpuBackoff			"CpuBackoff"
#define PosDialogProjectPropCpuBackoff			PosGroupDialogProjectProperties+28
#define	GetDialogProjectPropCpuDontFetch		"CpuDontFetch"
#define PosDialogProjectPropCpuDontFetch		PosGroupDialogProjectProperties+29
#define GetDialogProjectPropCpuBackoffInterval	"CpuBackoffInterval"
#define PosDialogProjectPropCpuBackoffInterval	PosGroupDialogProjectProperties+30
#define GetDialogProjectPropCudaBackoff			"CudaBackoff"
#define PosDialogProjectPropCudaBackoff			PosGroupDialogProjectProperties+31
#define GetDialogProjectPropCudaDontFetch		"CudaDontFetch"
#define PosDialogProjectPropCudaDontFetch		PosGroupDialogProjectProperties+32
#define GetDialogProjectPropAtiBackoff			"AtiBackoff"	
#define PosDialogProjectPropAtiBackoff			PosGroupDialogProjectProperties+33
#define GetDialogProjectPropAtiDontFetch		"AtiDontFetch"	
#define PosDialogProjectPropAtiDontFetch		PosGroupDialogProjectProperties+34

#define GetDialogProjectPropSchedulePending		"SchedulePending"
#define PosDialogProjectPropSchedulePending		PosGroupDialogProjectProperties+35

#define GetDialogProjectPropCpuShortTimeDebt	"CpuShortTimeDebt"
#define PosDialogProjectPropCpuShortTimeDebt	PosGroupDialogProjectProperties+36
#define GetDialogProjectPropCpuLongTimeDebt		"CpuLongTimeDebt"
#define PosDialogProjectPropCpuLongTimeDebt		PosGroupDialogProjectProperties+37

#define GetDialogProjectPropCudaShortTimeDebt	"CudaShortTimeDebt"
#define PosDialogProjectPropCudaShortTimeDebt	PosGroupDialogProjectProperties+38
#define GetDialogProjectPropCudaLongTimeDebt	"CudaLongTimeDebt"
#define PosDialogProjectPropCudaLongTimeDebt	PosGroupDialogProjectProperties+39

#define GetDialogProjectPropAtiShortTimeDebt	"AtiShortTimeDebt"
#define PosDialogProjectPropAtiShortTimeDebt	PosGroupDialogProjectProperties+40
#define GetDialogProjectPropAtiLongTimeDebt		"AtiLongTimeDebt"
#define PosDialogProjectPropAtiLongTimeDebt		PosGroupDialogProjectProperties+41

#define GetDialogProjectPropDisk				"Disk"
#define PosDialogProjectPropDisk				PosGroupDialogProjectProperties+42
#define GetDialogProjectPropDiskUse				"DiskUse"
#define PosDialogProjectPropDiskUse				PosGroupDialogProjectProperties+43
#define GetDialogProjectPropDiskAll				"DiskAll"
#define PosDialogProjectPropDiskAll				PosGroupDialogProjectProperties+44
#define GetDialogProjectPropDiskAllowed			"DiskAllowed"
#define PosDialogProjectPropDiskAllowed			PosGroupDialogProjectProperties+45
#define GetDialogProjectPropDiskBoinc			"DiskBoinc"
#define PosDialogProjectPropDiskBoinc			PosGroupDialogProjectProperties+46
#define GetDialogProjectPropDiskFree			"DiskFree"
#define PosDialogProjectPropDiskFree			PosGroupDialogProjectProperties+47
#define GetDialogProjectPropDiskTotal			"DiskTotal"
#define PosDialogProjectPropDiskTotal			PosGroupDialogProjectProperties+48
#define GetDialogProjectPropTotal				"Total"
#define PosDialogProjectPropTotal				PosGroupDialogProjectProperties+49
#define GetDialogProjectPropAverage				"Average"
#define PosDialogProjectPropAverage				PosGroupDialogProjectProperties+50
#define GetDialogProjectPropYes					"Yes"
#define PosDialogProjectPropYes					PosGroupDialogProjectProperties+51
#define GetDialogProjectPropNo					"No"
#define PosDialogProjectPropNo					PosGroupDialogProjectProperties+52

// GroupDialogTaskProperties
#define GroupDialogTaskProperties			"GroupDialogTaskProperties"
#define PosGroupDialogTaskProperties		PosDialogProjectPropNo+1
#define GetDialogTaskPropComputer			"Computer"
#define PosDialogTaskPropComputer			PosGroupDialogTaskProperties
#define GetDialogTaskPropTitleProjectP		"TitleTaskP"
#define PosDialogTaskPropTitleProjectP		PosGroupDialogTaskProperties+1
#define GetDialogTaskPropProject			"Project"
#define PosDialogTaskPropProject			PosGroupDialogTaskProperties+2
#define GetDialogTaskPropApplication		"Application"
#define PosDialogTaskPropApplication		PosGroupDialogTaskProperties+3
#define GetDialogTaskPropName				"Name"
#define PosDialogTaskPropName				PosGroupDialogTaskProperties+4
#define GetDialogTaskPropWorkunitName		"WorkunitName"
#define PosDialogTaskPropWorkunitName		PosGroupDialogTaskProperties+5
#define GetDialogTaskPropState				"State"
#define PosDialogTaskPropState				PosGroupDialogTaskProperties+6
#define GetDialogTaskPropRecieved			"Recieved"
#define PosDialogTaskPropRecieved			PosGroupDialogTaskProperties+7
#define GetDialogTaskPropDeadline			"Deadline"
#define PosDialogTaskPropDeadline			PosGroupDialogTaskProperties+8
#define GetDialogTaskPropEstAppSpeed		"EstAppSpeed"
#define PosDialogTaskPropEstAppSpeed		PosGroupDialogTaskProperties+9
#define GetDialogTaskPropEstTaskSize		"EstTaskSize"
#define PosDialogTaskPropEstTaskSize		PosGroupDialogTaskProperties+10
#define GetDialogTaskPropResources			"Resources"
#define PosDialogTaskPropResources			PosGroupDialogTaskProperties+11
#define GetDialogTaskPropCheckpointTime		"CheckpointTime"
#define PosDialogTaskPropCheckpointTime		PosGroupDialogTaskProperties+12
#define GetDialogTaskPropCpuTime			"CpuTime"
#define PosDialogTaskPropCpuTime			PosGroupDialogTaskProperties+13
#define GetDialogTaskPropElapsedTime		"ElapsedTime"
#define PosDialogTaskPropElapsedTime		PosGroupDialogTaskProperties+14
#define GetDialogTaskPropEstTimeRemaining	"EstTimeRemaining"
#define PosDialogTaskPropEstTimeRemaining	PosGroupDialogTaskProperties+15
#define GetDialogTaskPropFractionDone		"FractionDone"
#define PosDialogTaskPropFractionDone		PosGroupDialogTaskProperties+16
#define GetDialogTaskPropVirtualMemory		"VirtualMemory"
#define PosDialogTaskPropVirtualMemory		PosGroupDialogTaskProperties+17
#define GetDialogTaskPropWorkingSetSize		"WorkingSetSize"
#define PosDialogTaskPropWorkingSetSize		PosGroupDialogTaskProperties+18
#define GetDialogTaskPropDirectoy			"Directoy"
#define PosDialogTaskPropDirectoy			PosGroupDialogTaskProperties+19
#define GetDialogTaskPropPid				"Pid"
#define PosDialogTaskPropPid				PosGroupDialogTaskProperties+20

// GroupDialogComputerProperties
#define GroupDialogComputerProperties		"GroupDialogComputerProperties"
#define PosGroupDialogComputerProperties	PosDialogTaskPropPid+1
#define GetDialogComputerPropComputer		"Computer"
#define PosDialogComputerPropComputer		PosGroupDialogComputerProperties
#define GetDialogComputerPropTitle			"TitleComputerP"
#define PosDialogComputerPropTitle			PosGroupDialogComputerProperties+1
#define GetDialogComputerPropCPU			"CPU"
#define PosDialogComputerPropCPU			PosGroupDialogComputerProperties+2
#define GetDialogComputerPropOs				"Os"
#define PosDialogComputerPropOs				PosGroupDialogComputerProperties+3
#define GetDialogComputerPropMemory			"Memory"
#define PosDialogComputerPropMemory			PosGroupDialogComputerProperties+4
#define GetDialogComputerPropDisk			"Disk"
#define PosDialogComputerPropDisk			PosGroupDialogComputerProperties+5
#define GetDialogComputerPropGPU			"GPU"
#define PosDialogComputerPropGPU			PosGroupDialogComputerProperties+6
#define GetDialogComputerPropTime			"TimeHeader"
#define PosDialogComputerPropTime			PosGroupDialogComputerProperties+7
#define GetDialogComputerPropTimeBoinc		"TimeBOINC"
#define PosDialogComputerPropTimeBoinc		PosGroupDialogComputerProperties+8
#define GetDialogComputerPropTimeInternet	"TimeInternet"
#define PosDialogComputerPropTimeInternet	PosGroupDialogComputerProperties+9
#define GetDialogComputerPropTimeCPU		"TimeCPU"
#define PosDialogComputerPropTimeCPU		PosGroupDialogComputerProperties+10
#define GetDialogComputerPropTimeGPU		"TimeGPU"
#define PosDialogComputerPropTimeGPU		PosGroupDialogComputerProperties+11
#define GetDialogComputerPropTimeStart		"TimeStart"
#define PosDialogComputerPropTimeStart		PosGroupDialogComputerProperties+12
#define GetDialogComputerPropTimeUpTime		"TimeUp"
#define PosDialogComputerPropTimeUpTime		PosGroupDialogComputerProperties+13

#define GroupDialogProjectOperation				"GroupDialogProjectOperation"
#define PosGroupDialogProjectOperation			PosDialogComputerPropTimeUpTime+1
#define GetDialogProjectOQuestionReset			"QuestionReset"
#define PosDialogProjectOQuestionReset			PosGroupDialogProjectOperation
#define GetDialogProjectOQuestionDetach			"QuestionDetach"
#define PosDialogProjectOQuestionDetach			PosGroupDialogProjectOperation+1
//#define GetDialogProjectOMsgBoxSelectProject	"MsgBoxSelectProject"
//#define PosDialogProjectOMsgBoxSelectProject	PosGroupDialogProjectOperation+2

#define GroupDialogTaskAbort				"GroupDialogTaskAbort"
#define PosGroupDialogTaskAbort				PosDialogProjectOQuestionDetach+1
#define GetDialogTaskAbortColumnTask		"ColumnTask"
#define PosDialogTaskAbortColumnTask		PosGroupDialogTaskAbort
#define	GetDialogTaskAbortColumnProject		"ColumnProject"
#define	PosDialogTaskAbortColumnProject		PosGroupDialogTaskAbort+1
#define GetDialogTaskAbortColumnDeadline	"ColumnDeadline"
#define PosDialogTaskAbortColumnDeadline	PosGroupDialogTaskAbort+2
#define	GetDialogTaskAbortColumnState		"ColumnState"
#define	PosDialogTaskAbortColumnState		PosGroupDialogTaskAbort+3
#define	GetDialogTaskAbortColumnComputer	"ColumnComputer"
#define	PosDialogTaskAbortColumnComputer	PosGroupDialogTaskAbort+4
#define	GetDialogTaskAbortQuestionAbort		"QuestionAbort"
#define	PosDialogTaskAbortQuestionAbort		PosGroupDialogTaskAbort+5

#define GroupDialogTransferAbort			"GroupDialogTransferAbort"
#define PosGroupDialogTransferAbort			PosDialogTaskAbortQuestionAbort+1
#define GetDialogTransferAbortQuestionAbort	"QuestionAbort"
#define PosDialogTransferAbortQuestionAbort	PosGroupDialogTransferAbort

#define GroupDialogSearchComputer					"GroupDialogSearchComputer"
#define PosGroupDialogSearchComputer				PosDialogTransferAbortQuestionAbort+1
#define GetDialogSearchComputerTitle				"Title"
#define PosDialogSearchComputerTitle				PosGroupDialogSearchComputer
#define GetDialogSearchComputerComputer				"Computer"
#define PosDialogSearchComputerComputer				PosGroupDialogSearchComputer+1
#define GetDialogSearchComputerIP					"IP"
#define PosDialogSearchComputerIP					PosGroupDialogSearchComputer+2
#define GetDialogSearchComputerAddress				"Address"
#define PosDialogSearchComputerAddress				PosGroupDialogSearchComputer+3
#define GetDialogSearchComputerPassword				"Password"
#define PosDialogSearchComputerPassword				PosGroupDialogSearchComputer+4

#define GetDialogSearchComputerCopyIp				"CopyIP"
#define PosDialogSearchComputerCopyIp				PosGroupDialogSearchComputer+5
#define GetDialogSearchComputerCopyMAC				"CopyMAC"
#define PosDialogSearchComputerCopyMAC				PosGroupDialogSearchComputer+6
#define GetDialogSearchComputerCopyAll				"CopyAll"
#define PosDialogSearchComputerCopyAll				PosGroupDialogSearchComputer+7

#define GetDialogSearchComputerButtonAddressRange	"ButtonAddressRange"
#define PosDialogSearchComputerButtonAddressRange	PosGroupDialogSearchComputer+8
#define GetDialogSearchComputerButtonStop			"ButtonStop"
#define PosDialogSearchComputerButtonStop			PosGroupDialogSearchComputer+9
#define GetDialogSearchComputerButtonSearchAgain	"ButtonSearchAgain"
#define PosDialogSearchComputerButtonSearchAgain	PosGroupDialogSearchComputer+10
#define GetDialogSearchComputerButtonAdd			"ButtonAdd"
#define PosDialogSearchComputerButtonAdd			PosGroupDialogSearchComputer+11
#define GetDialogSearchComputerYes					"Yes"
#define PosDialogSearchComputerYes					PosGroupDialogSearchComputer+12

// GroupDialogLogging
#define GroupDialogLogging							"GroupDialogLogging"
#define PosGroupDialogLogging						PosDialogSearchComputerYes+1
#define GetDialogLoggingTitle						"Title"
#define PosDialogLoggingTitle						PosGroupDialogLogging
#define GetDialogLoggingRulesTitle					"TitleRules"
#define PosDialogLoggingRulesTitle					PosGroupDialogLogging+1
#define GetDialogLoggingEnableDebug					"EnableDebug"
#define PosDialogLoggingEnableDebug					PosGroupDialogLogging+2

// GroupDialogAbout
#define GroupDialogAbout							"GroupDialogAbout"
#define PosGroupDialogAbout							PosDialogLoggingEnableDebug+1
#define GetDialogAboutTitle							"Title"
#define PosDialogAboutTitle							PosGroupDialogAbout
#define GetDialogAboutText1							"Text1"
#define PosDialogAboutText1							PosGroupDialogAbout+1
#define GetDialogAboutTextProgrammer				"TextProgrammer"
#define PosDialogAboutTextProgrammer				PosGroupDialogAbout+2
#define GetDialogAboutTextTesters					"TextTesters"
#define PosDialogAboutTextTesters					PosGroupDialogAbout+3
#define GetDialogAboutTextSupport					"TextSupport"
#define PosDialogAboutTextSupport					PosGroupDialogAbout+4
#define GetDialogAboutText2							"Text2"
#define PosDialogAboutText2							PosGroupDialogAbout+5
#define GetDialogAboutCredits						"Credits"
#define PosDialogAboutCredits						PosGroupDialogAbout+6
#define GetDialogAboutCreditsLink					"CreditsLink"
#define PosDialogAboutCreditsLink					PosGroupDialogAbout+7
#define GetDialogAboutCreditsLinkHttp				"CreditsLinkHttp"
#define PosDialogAboutCreditsLinkHttp				PosGroupDialogAbout+8

// GroupDialogUpdate
#define GroupDialogUpdate							"GroupDialogUpdate"
#define PosGroupDialogUpdate						PosDialogAboutCreditsLinkHttp+1
#define GetDialogUpdateTitle						"Title"
#define PosDialogUpdateTitle						PosGroupDialogUpdate
#define GetDialogUpdateCheckNew						"CheckNew"
#define PosDialogUpdateCheckNew						PosGroupDialogUpdate+1
#define GetDialogUpdateUsesHttp						"UsesHttp"
#define PosDialogUpdateUsesHttp						PosGroupDialogUpdate+2
#define GetDialogUpdateCurrentVersion				"CurrentVersion"
#define PosDialogUpdateCurrentVersion				PosGroupDialogUpdate+3
#define GetDialogUpdateLatestVersion				"LatestVersion"
#define PosDialogUpdateLatestVersion				PosGroupDialogUpdate+4
#define GetDialogUpdateLatestBetaVersion			"LatestBetaVersion"
#define PosDialogUpdateLatestBetaVersion			PosGroupDialogUpdate+5
#define GetDialogUpdateNewVersionFound				"NewVersionFound"
#define PosDialogUpdateNewVersionFound				PosGroupDialogUpdate+6
#define GetDialogUpdateNewBetaVersionFound			"NewBetaVersionFound"
#define PosDialogUpdateNewBetaVersionFound			PosGroupDialogUpdate+7
#define GetDialogUpdateClickUpdate					"ClickUpdate"
#define PosDialogUpdateClickUpdate					PosGroupDialogUpdate+8
#define GetDialogUpdateAlreadyCurrent				"AlreadyCurrent"
#define PosDialogUpdateAlreadyCurrent				PosGroupDialogUpdate+9
#define GetDialogUpdateCantConnect					"CantConnect"
#define PosDialogUpdateCantConnect					PosGroupDialogUpdate+10
#define GetDialogUpdateCheckManually				"CheckManually"
#define PosDialogUpdateCheckManually				PosGroupDialogUpdate+11

#define GetDialogUpdateDirect						"UpdateDirect"
#define PosDialogUpdateDirect						PosGroupDialogUpdate+12

#define GetDialogUpdateLocation						"Location"
#define PosDialogUpdateLocation						PosGroupDialogUpdate+13
#define GetDialogUpdateEurope						"Europe"
#define PosDialogUpdateEurope						PosGroupDialogUpdate+14
#define GetDialogUpdateNorthAmerica					"NorthAmerica"
#define PosDialogUpdateNorthAmerica					PosGroupDialogUpdate+15

#define GetDialogUpdateButtonCheck					"ButtonCheck"
#define PosDialogUpdateButtonCheck					PosGroupDialogUpdate+16
#define GetDialogUpdateButtonUpdate					"ButtonUpdate"
#define PosDialogUpdateButtonUpdate					PosGroupDialogUpdate+17

#define GetDialogUpdateCheckAuto					"CheckAuto"
#define PosDialogUpdateCheckAuto					PosGroupDialogUpdate+18
#define GetDialogUpdateCheckBeta					"CheckBeta"
#define PosDialogUpdateCheckBeta					PosGroupDialogUpdate+19

#define GetDialogUpdateCheckUseBrowser				"CheckUseBrowser"
#define PosDialogUpdateCheckUseBrowser				PosGroupDialogUpdate+20
#define GetDialogUpdateTempFolder					"TempFolder"
#define PosDialogUpdateTempFolder					PosGroupDialogUpdate+21
#define GetDialogUpdateUnableToDownload				"UnableToDownload"
#define PosDialogUpdateUnableToDownload				PosGroupDialogUpdate+22
#define GetDialogUpdateUnableToStore				"UnableToStore"
#define PosDialogUpdateUnableToStore				PosGroupDialogUpdate+23

// GroupDialogProxy
#define GroupDialogProxy							"GroupDialogProxy"
#define PosGroupDialogProxy							PosDialogUpdateUnableToStore+1
#define GetDialogProxyTitle							"Title"
#define PosDialogProxyTitle							PosGroupDialogProxy

#define GetDialogProxyHttpTitle						"HttpTitle"
#define PosDialogProxyHttpTitle						PosGroupDialogProxy+1
#define GetDialogProxyHttpConnect					"HttpConnect"
#define PosDialogProxyHttpConnect					PosGroupDialogProxy+2
#define GetDialogProxyHttpGroup						"HttpGroup"
#define PosDialogProxyHttpGroup						PosGroupDialogProxy+3
#define GetDialogProxyHttpServer					"HttpServer"
#define PosDialogProxyHttpServer					PosGroupDialogProxy+4
#define GetDialogProxyHttpPort						"HttpPort"
#define PosDialogProxyHttpPort						PosGroupDialogProxy+5
#define GetDialogProxyHttpNoFor						"HttpNoFor"
#define PosDialogProxyHttpNoFor						PosGroupDialogProxy+6
#define GetDialogProxyHttpGroupUser					"HttpGroupUser"
#define PosDialogProxyHttpGroupUser					PosGroupDialogProxy+7
#define GetDialogProxyHttpUser						"HttpUser"
#define PosDialogProxyHttpUser						PosGroupDialogProxy+8
#define GetDialogProxyHttpPassword					"HttpPassword"
#define PosDialogProxyHttpPassword					PosGroupDialogProxy+9

#define GetDialogProxySocksTitle					"SocksTitle"
#define PosDialogProxySocksTitle					PosGroupDialogProxy+10

#define GetDialogProxySocksConnect					"SocksConnect"
#define PosDialogProxySocksConnect					PosGroupDialogProxy+11
#define GetDialogProxySocksGroup					"SocksGroup"
#define PosDialogProxySocksGroup					PosGroupDialogProxy+12
#define GetDialogProxySocksServer					"SocksServer"
#define PosDialogProxySocksServer					PosGroupDialogProxy+13
#define GetDialogProxySocksPort						"SocksPort"
#define PosDialogProxySocksPort						PosGroupDialogProxy+14
#define GetDialogProxySocksNoFor					"SocksNoFor"
#define PosDialogProxySocksNoFor					PosGroupDialogProxy+15
#define GetDialogProxySocksGroupUser				"SocksGroupUser"
#define PosDialogProxySocksGroupUser				PosGroupDialogProxy+16
#define GetDialogProxySocksUser						"SocksUser"
#define PosDialogProxySocksUser						PosGroupDialogProxy+17
#define GetDialogProxySocksPassword					"SocksPassword"
#define PosDialogProxySocksPassword					PosGroupDialogProxy+18

// GroupDialogRules

#define GroupDialogRules							"GroupDialogRules"
#define PosGroupDialogRules							PosDialogProxySocksPassword+1
#define GetDialogRulesTitle							"Title"
#define PosDialogRulesTitle							PosGroupDialogRules
#define GetDialogRulesName							"Name"
#define PosDialogRulesName							PosGroupDialogRules+1
#define GetDialogRulesComputer						"Computer"
#define PosDialogRulesComputer						PosGroupDialogRules+2
#define GetDialogRulesProject						"Project"
#define PosDialogRulesProject						PosGroupDialogRules+3
#define GetDialogRulesApplication					"Application"
#define PosDialogRulesApplication					PosGroupDialogRules+4

#define GetDialogRulesGroupType						"GroupType"
#define PosDialogRulesGroupType						PosGroupDialogRules+5
#define GetDialogRulesGroupOperator					"GroupOperator"
#define PosDialogRulesGroupOperator					PosGroupDialogRules+6
#define GetDialogRulesGroupValue					"GroupValue"
#define PosDialogRulesGroupValue					PosGroupDialogRules+7
#define GetDialogRulesGroupTime						"GroupTime"
#define PosDialogRulesGroupTime						PosGroupDialogRules+8
#define GetDialogRulesGroupEvent					"GroupEvent"
#define PosDialogRulesGroupEvent					PosGroupDialogRules+9

#define GetDialogRulesTypeElapsed					"TypeElapsed"
#define PosDialogRulesTypeElapsed					PosGroupDialogRules+10
#define GetDialogRulesTypeCpuP						"TypeCpuP"
#define PosDialogRulesTypeCpuP						PosGroupDialogRules+11
#define GetDialogRulesTypeProgress					"TypeProgress"
#define PosDialogRulesTypeProgress					PosGroupDialogRules+12
#define GetDialogRulesTypeProgressDelta				"TypeProgressDelta"
#define PosDialogRulesTypeProgressDelta				PosGroupDialogRules+13
#define GetDialogRulesTypeTimeLeft					"TypeTimeLeft"
#define PosDialogRulesTypeTimeLeft					PosGroupDialogRules+14
#define GetDialogRulesTypeConnection				"TypeConnection"
#define PosDialogRulesTypeConnection				PosGroupDialogRules+15
#define GetDialogRulesTypeUse						"TypeUse"
#define PosDialogRulesTypeUse						PosGroupDialogRules+16
#define GetDialogRulesTypeTemperature				"TypeTemperature"
#define PosDialogRulesTypeTemperature				PosGroupDialogRules+17
#define GetDialogRulesTypeStatus					"TypeStatus"
#define PosDialogRulesTypeStatus					PosGroupDialogRules+18
#define GetDialogRulesTypeTime						"TypeTime"
#define PosDialogRulesTypeTime						PosGroupDialogRules+19
#define GetDialogRulesTypeDeadline					"TypeDeadline"
#define PosDialogRulesTypeDeadline					PosGroupDialogRules+20
#define GetDialogRulesTypeTimeLeftProject			"TypeTimeLeftProject"
#define PosDialogRulesTypeTimeLeftProject			PosGroupDialogRules+21

#define GetDialogRulesEventSuspendProject			"EventSuspendProject"
#define PosDialogRulesEventSuspendProject			PosGroupDialogRules+22
#define GetDialogRulesEventResumeProject			"EventResumeProject"
#define PosDialogRulesEventResumeProject			PosGroupDialogRules+23
#define GetDialogRulesEventSnooze					"EventSnooze"
#define PosDialogRulesEventSnooze					PosGroupDialogRules+24
#define GetDialogRulesEventSnoozeGpu				"EventSnoozeGpu"
#define PosDialogRulesEventSnoozeGpu				PosGroupDialogRules+25

#define GetDialogRulesEventNoNewWork				"EventNoNewWorkProject"
#define PosDialogRulesEventNoNewWork				PosGroupDialogRules+26
#define GetDialogRulesEventAllowNewWork				"EventAllowNewWorkProject"
#define PosDialogRulesEventAllowNewWork				PosGroupDialogRules+27
#define GetDialogRulesEventSuspendTask				"EventSuspendTask"
#define PosDialogRulesEventSuspendTask				PosGroupDialogRules+28

#define GetDialogRulesEventMessageNone				"EventMessageNone"
#define PosDialogRulesEventMessageNone				PosGroupDialogRules+29
#define GetDialogRulesEventMessageNotice			"EventMessageNotice"
#define PosDialogRulesEventMessageNotice			PosGroupDialogRules+30
#define GetDialogRulesEventMessage					"EventMessage"
#define PosDialogRulesEventMessage					PosGroupDialogRules+31

#define GetDialogRulesEventProgram					"EventProgram"
#define PosDialogRulesEventProgram					PosGroupDialogRules+32

#define GetDialogRulesEventSuspendNetwork			"EventSuspendNetwork"
#define PosDialogRulesEventSuspendNetwork			PosGroupDialogRules+33
#define GetDialogRulesEventResumeNetwork			"EventResumeNetwork"
#define PosDialogRulesEventResumeNetwork			PosGroupDialogRules+34

#define GetDialogRulesMsgIdenticalType				"MsgIdenticalType"
#define PosDialogRulesMsgIdenticalType				PosGroupDialogRules+35
#define GetDialogRulesMsgNotRules					"MsgNotRules"
#define PosDialogRulesMsgNotRules					PosGroupDialogRules+36
#define GetDialogRulesMsgRuleNameEmpty				"MsgRuleNameEmpty"
#define PosDialogRulesMsgRuleNameEmpty				PosGroupDialogRules+37
#define GetDialogRulesMsgRuleNameInUse				"MsgRuleNameInUse"
#define PosDialogRulesMsgRuleNameInUse				PosGroupDialogRules+38
#define GetDialogRulesMsgNoProgram					"MsgNoProgram"
#define PosDialogRulesMsgNoProgram					PosGroupDialogRules+39
#define GetDialogRulesMsgEmpty						"MsgEmpty"
#define PosDialogRulesMsgEmpty						PosGroupDialogRules+40
#define GetDialogRulesMsgOperatorNotSelected		"MsgOperatorNotSelected"
#define PosDialogRulesMsgOperatorNotSelected		PosGroupDialogRules+41
#define GetDialogRulesMsgInvalidTime				"MsgInvalidTime"
#define PosDialogRulesMsgInvalidTime				PosGroupDialogRules+42
#define GetDialogRulesMsgTempRuleComputer			"MsgTempRuleComputer"
#define PosDialogRulesMsgTempRuleComputer			PosGroupDialogRules+43
#define GetDialogRulesMsgInvalidUse					"MsgInvalidUse"
#define PosDialogRulesMsgInvalidUse					PosGroupDialogRules+44
#define GetDialogRulesMsgInvalidStatus				"MsgInvalidStatus"
#define PosDialogRulesMsgInvalidStatus				PosGroupDialogRules+45
#define GetDialogRulesMsgInvalidStatus2				"MsgInvalidStatus2"
#define PosDialogRulesMsgInvalidStatus2				PosGroupDialogRules+46
#define GetDialogRulesMsgOnlyFirstType				"MsgOnlyFirstType"
#define PosDialogRulesMsgOnlyFirstType				PosGroupDialogRules+47
#define GetDialogRulesMsgSnoozeFor					"MsgSnoozeFor"
#define PosDialogRulesMsgSnoozeFor					PosGroupDialogRules+48
#define GetDialogRulesNotAllowed					"MsgNotAllowed"
#define PosDialogRulesNotAllowed					PosGroupDialogRules+49

#define GetDialogRulesButtonCheck					"CheckButton"
#define PosDialogRulesButtonCheck					PosGroupDialogRules+50
#define GetDialogRulesButtonColor					"ColorButton"
#define PosDialogRulesButtonColor					PosGroupDialogRules+51

// GroupDialogMessageSearch

#define GroupDialogMessageSearch					"GroupDialogMessageSearch"
#define PosGroupDialogMessageSearch					PosDialogRulesButtonColor+1
#define GetDialogMessageSearchTitle					"Title"
#define PosDialogMessageSearchTitle					PosGroupDialogMessageSearch

// GroupDialogCommonButtons
#define GroupDialogCommonButtons					"GroupDialogCommonButtons"
#define PosGroupDialogCommonButtons					PosDialogMessageSearchTitle+1
#define GetDialogCommonButtonsYesButton				"YesButton"
#define PosDialogCommonButtonsYesButton				PosGroupDialogCommonButtons
#define GetDialogCommonButtonsNoButton				"NoButton"
#define PosDialogCommonButtonsNoButton				PosGroupDialogCommonButtons+1
#define GetDialogCommonButtonsOk					"OkButton"
#define PosDialogCommonButtonsOk					PosGroupDialogCommonButtons+2		
#define GetDialogCommonButtonsCancel				"CancelButton"
#define PosDialogCommonButtonsCancel				PosGroupDialogCommonButtons+3
#define GetDialogCommonButtonsClear					"ClearButton"
#define PosDialogCommonButtonsClear					PosGroupDialogCommonButtons+4
#define GetDialogCommonButtonsEdit					"EditButton"
#define PosDialogCommonButtonsEdit					PosGroupDialogCommonButtons+5
#define GetDialogCommonButtonsAdd					"AddButton"
#define PosDialogCommonButtonsAdd					PosGroupDialogCommonButtons+6
#define GetDialogCommonButtonsDelete				"DeleteButton"
#define PosDialogCommonButtonsDelete				PosGroupDialogCommonButtons+7
#define GetDialogCommonButtonsApply					"ApplyButton"
#define PosDialogCommonButtonsApply					PosGroupDialogCommonButtons+8

#define GroupMessageBox								"GroupMessageBox"
#define PosGroupMessageBox							PosDialogCommonButtonsApply+1
#define GetClientQuitQuestion						"ClientQuitQuestion"
#define PosClientQuitQuestion						PosGroupMessageBox
#define GetClientCouldntShutdown					"ClientCouldntShutdown"
#define PosClientCouldntShutdown					PosGroupMessageBox+1
#define GetClientNotStartedBT						"ClientNotStartedBT"
#define PosClientNotStartedBT						PosGroupMessageBox+2

#define GroupWindowStatusMessage					"GroupWindowStatusMessage"
#define PosGroupWindowStatusMessage					PosClientNotStartedBT+1
#define GetWindowStatusMessageTasksUpdateInSeconds	"TasksUpdateInSeconds" 
#define PosWindowStatusMessageTasksUpdateInSeconds	PosGroupWindowStatusMessage
#define GetWindowStatusMessageTasksWaiting			"TasksWaiting" 
#define	PosWindowStatusMessageTasksWaiting			PosGroupWindowStatusMessage+1
#define GetWindowStatusMessageStatusUpdateInSec		"StatusUpdateInSec" 
#define PosWindowStatusMessageStatusUpdateInSec		PosGroupWindowStatusMessage+2
#define GetWindowStatusMessageStatusWaiting			"StatusWaiting" 
#define PosWindowStatusMessageStatusWaiting			PosGroupWindowStatusMessage+3
#define GetWindowStatusMessageStatusUpdating		"StatusUpdating"
#define PosWindowStatusMessageStatusUpdating		PosGroupWindowStatusMessage+4
#define GetWindowStatusMessageStatusSwitched		"StatusSwitched"
#define PosWindowStatusMessageStatusSwitched		PosGroupWindowStatusMessage+5
#define GetWindowStatusMessageStatusClosing			"StatusClosing"
#define PosWindowStatusMessageStatusClosing			PosGroupWindowStatusMessage+6
#define GetWindowStatusMessageStatusHistoryDisabled	"StatusHistoryDisabled"
#define PosWindowStatusMessageStatusHistoryDisabled	PosGroupWindowStatusMessage+7

//#define GetWindowStatusMessageCombined				"Combined"
//#define PosWindowStatusMessageCombined				PosGroupWindowStatusMessage+8
#define GetWindowStatusMessageCombinedNotConnected	"CombinedNotConnected"
#define PosWindowStatusMessageCombinedNotConnected	PosGroupWindowStatusMessage+8
#define GetWindowStatusMessageCombinedPassWord		"CombinedPassWord"
#define PosWindowStatusMessageCombinedPassWord		PosGroupWindowStatusMessage+9
#define GetWindowStatusMessageCombinedState			"CombinedState"
#define PosWindowStatusMessageCombinedState			PosGroupWindowStatusMessage+10
#define GetWindowStatusMessageCombinedConnecting	"CombinedConnecting"
#define PosWindowStatusMessageCombinedConnecting	PosGroupWindowStatusMessage+11
#define GetWindowStatusMessageCombinedConnected		"CombinedConnected"
#define PosWindowStatusMessageCombinedConnected		PosGroupWindowStatusMessage+12
#define GetWindowStatusMessageCombinedLostConnection "CombinedLostConnection"
#define PosWindowStatusMessageCombinedLostConnection PosGroupWindowStatusMessage+13
#define GetWindowStatusMessageMessages				"Messages"
#define PosWindowStatusMessageMessages				PosGroupWindowStatusMessage+14
#define GetWindowStatusMessageTasks					"Tasks"
#define PosWindowStatusMessageTasks					PosGroupWindowStatusMessage+15
#define GetWindowStatusMessageProjects				"Projects"
#define PosWindowStatusMessageProjects				PosGroupWindowStatusMessage+16
#define GetWindowStatusMessageTransfer				"Transfer"
#define PosWindowStatusMessageTransfer				PosGroupWindowStatusMessage+17
#define GetWindowStatusMessageHistory				"History"
#define PosWindowStatusMessageHistory				PosGroupWindowStatusMessage+18
#define GetWindowStatusMessageComputer				"Computer"
#define PosWindowStatusMessageComputer				PosGroupWindowStatusMessage+19
#define GetWindowStatusMessageNotices				"Notices"
#define PosWindowStatusMessageNotices				PosGroupWindowStatusMessage+20

#define GroupTaskbarMessage							"GroupTaskbarMessage"
#define PosGroupTaskbarMessage						PosWindowStatusMessageNotices+1
#define GetGroupTaskbarMessageComputers				"Computers" 
#define PosGroupTaskbarMessageComputers				PosGroupTaskbarMessage
#define GetGroupTaskbarMessageRunning				"Running" 
#define PosGroupTaskbarMessageRunning				PosGroupTaskbarMessage+1

#define GroupMobile									"GroupMobile"
#define PosGroupMobile								PosGroupTaskbarMessageRunning+1
#define GetGroupMobileLoginHeader					"LoginHeader" 
#define PosGroupMobileLoginHeader					PosGroupMobile
#define GetGroupMobileLoginHeaderAuto				"LoginHeaderAuto" 
#define PosGroupMobileLoginHeaderAuto				PosGroupMobile+1
#define GetGroupMobileLoginHeaderFailed				"LoginHeaderFailed" 
#define PosGroupMobileLoginHeaderFailed				PosGroupMobile+2
#define GetGroupMobileLoginLoginAuto				"LoginAuto"
#define PosGroupMobileLoginLoginAuto				PosGroupMobile+3
#define GetGroupMobileLoginKeep						"LoginKeep" 
#define PosGroupMobileLoginKeep						PosGroupMobile+4
#define GetGroupMobileLoginStore					"LoginStore" 
#define PosGroupMobileLoginStore					PosGroupMobile+5
#define GetGroupMobileLoginPassword					"LoginPassword" 
#define PosGroupMobileLoginPassword					PosGroupMobile+6
#define GetGroupMobileLoginNeedPassword				"LoginNeedPassword" 
#define PosGroupMobileLoginNeedPassword				PosGroupMobile+7
#define GetGroupMobileLogout						"Logout" 
#define PosGroupMobileLogout						PosGroupMobile+8
#define GetGroupMobileButtonOptions					"Options" 
#define PosGroupMobileButtonOptions					PosGroupMobile+9
#define GetGroupMobileButtonMore					"More" 
#define PosGroupMobileButtonMore					PosGroupMobile+10
#define GetGroupMobileButtonLess					"Less" 
#define PosGroupMobileButtonLess					PosGroupMobile+11
#define GetGroupMobileButtonFontSize				"FontSize" 
#define PosGroupMobileButtonFontSize				PosGroupMobile+12
#define GetGroupMobileButtonMultipleSelections		"MultipleSelections" 
#define PosGroupMobileButtonMultipleSelections		PosGroupMobile+13
#define GetGroupMobileSort							"Sort"
#define PosGroupMobileSort							PosGroupMobile+14
#define GetGroupMobileSort1							"Sort1"
#define PosGroupMobileSort1							PosGroupMobile+15
#define GetGroupMobileSort2							"Sort2"
#define PosGroupMobileSort2							PosGroupMobile+16
#define GetGroupMobileSort3							"Sort3"
#define PosGroupMobileSort3							PosGroupMobile+17
#define GetGroupMobileButonFilter					"Filter"
#define PosGroupMobileButtonFilter					PosGroupMobile+18
#define GetGroupMobileButtonReport					"Report"
#define PosGroupMobileButtonReport					PosGroupMobile+19
#define GetGroupMobileButtonGraph					"Graph"
#define PosGroupMobileButtonGraph					PosGroupMobile+20
#define GetGroupMobileSingleSelection				"SingleSelection"
#define PosGroupMobileSingleSelection				PosGroupMobile+21
#define GetGroupMobileButtonToggleFilter			"ToggleFilter"
#define PosGroupMobileButtonToggleFilter			PosGroupMobile+22
#define GetGroupMobileButtonToggleFullScreen		"ToggleFullScreen"
#define PosGroupMobileButtonToggleFullScreen		PosGroupMobile+23
#define GetGroupMobileButtonGraphSComputer			"GraphSComputer"
#define PosGroupMobileButtonGraphSComputer			PosGroupMobile+24
#define GetGroupMobileButtonGraphSProject			"GraphSProject"
#define PosGroupMobileButtonGraphSProject			PosGroupMobile+25
#define GetGroupMobileButtonGraphDone				"GraphDone"
#define PosGroupMobileButtonGraphDone				PosGroupMobile+26

#define GroupNoticesMessage							"GroupNoticesMessage"
#define PosGroupNoticesMessage						PosGroupMobileButtonGraphDone+1
#define GetNoticesMessageMore						"More"
#define PosNoticesMessageMore						PosGroupNoticesMessage
#define GetNoticesMessageThereAreNew				"ThereAreNew"
#define PosNoticesMessageThereAreNew				PosGroupNoticesMessage+1
#define GetNoticesMessageBalloonTitle				"BalloonTitle"
#define PosNoticesMessageBalloonTitle				PosGroupNoticesMessage+2
#define GetNoticesCatClient							"CatClient"
#define PosNoticesCatClient							PosGroupNoticesMessage+3
#define GetNoticesCatScheduler						"CatScheduler"
#define PosNoticesCatScheduler						PosGroupNoticesMessage+4
#define GetNoticesCatBoincTasks						"CatBoincTasks"
#define PosNoticesCatBoincTasks						PosGroupNoticesMessage+5
#define GetNoticesCatTThrottle						"CatTThrottle"
#define PosNoticesCatTThrottle						PosGroupNoticesMessage+6
#define GetNoticeNoNotices							"NoNotices"
#define PosNoticeNoNotices							PosGroupNoticesMessage+7
#define GetNoticeDisabledUser						"DisabledUser"
#define PosNoticeDisabledUser						PosGroupNoticesMessage+8

#define GroupTranslatableBoincText					"TranslatableBoincText"
#define PosGroupTranslatableBoincText				PosNoticeDisabledUser+1
#define GetTranslatableBoincText0					"0" 
#define PosTranslatableBoincText0					PosGroupTranslatableBoincText
#define GetTranslatableBoincText1					"1" 
#define PosTranslatableBoincText1					PosGroupTranslatableBoincText+1
#define GetTranslatableBoincText2					"2" 
#define PosTranslatableBoincText2					PosGroupTranslatableBoincText+2
#define GetTranslatableBoincText3					"3" 
#define PosTranslatableBoincText3					PosGroupTranslatableBoincText+3
#define GetTranslatableBoincText4					"4" 
#define PosTranslatableBoincText4					PosGroupTranslatableBoincText+4
#define GetTranslatableBoincText5					"5" 
#define PosTranslatableBoincText5					PosGroupTranslatableBoincText+5
#define GetTranslatableBoincText6					"6" 
#define PosTranslatableBoincText6					PosGroupTranslatableBoincText+6
#define GetTranslatableBoincText7					"7" 
#define PosTranslatableBoincText7					PosGroupTranslatableBoincText+7
#define GetTranslatableBoincText8					"8" 
#define PosTranslatableBoincText8					PosGroupTranslatableBoincText+8
#define GetTranslatableBoincText9					"9" 
#define PosTranslatableBoincText9					PosGroupTranslatableBoincText+9
#define GetTranslatableBoincText10					"10" 
#define PosTranslatableBoincText10					PosGroupTranslatableBoincText+10
#define GetTranslatableBoincText11					"11" 
#define PosTranslatableBoincText11					PosGroupTranslatableBoincText+11
#define GetTranslatableBoincText12					"12" 
#define PosTranslatableBoincText12					PosGroupTranslatableBoincText+12
#define GetTranslatableBoincText13					"13" 
#define PosTranslatableBoincText13					PosGroupTranslatableBoincText+13
#define GetTranslatableBoincText14					"14" 
#define PosTranslatableBoincText14					PosGroupTranslatableBoincText+14
#define GetTranslatableBoincText15					"15" 
#define PosTranslatableBoincText15					PosGroupTranslatableBoincText+15
#define GetTranslatableBoincText16					"16" 
#define PosTranslatableBoincText16					PosGroupTranslatableBoincText+16
#define GetTranslatableBoincText17					"17" 
#define PosTranslatableBoincText17					PosGroupTranslatableBoincText+17
#define GetTranslatableBoincText18					"18" 
#define PosTranslatableBoincText18					PosGroupTranslatableBoincText+18
#define GetTranslatableBoincText19					"19" 
#define PosTranslatableBoincText19					PosGroupTranslatableBoincText+19
#define GetTranslatableBoincText20					"20" 
#define PosTranslatableBoincText20					PosGroupTranslatableBoincText+20
#define GetTranslatableBoincText21					"21" 
#define PosTranslatableBoincText21					PosGroupTranslatableBoincText+21
#define GetTranslatableBoincText22					"22" 
#define PosTranslatableBoincText22					PosGroupTranslatableBoincText+22
#define GetTranslatableBoincText23					"23" 
#define PosTranslatableBoincText23					PosGroupTranslatableBoincText+23
#define GetTranslatableBoincText24					"24" 
#define PosTranslatableBoincText24					PosGroupTranslatableBoincText+24
#define GetTranslatableBoincText25					"25" 
#define PosTranslatableBoincText25					PosGroupTranslatableBoincText+25
#define GetTranslatableBoincText26					"26" 
#define PosTranslatableBoincText26					PosGroupTranslatableBoincText+26
#define GetTranslatableBoincText27					"27" 
#define PosTranslatableBoincText27					PosGroupTranslatableBoincText+27
#define GetTranslatableBoincText28					"28" 
#define PosTranslatableBoincText28					PosGroupTranslatableBoincText+28
#define GetTranslatableBoincText29					"29" 
#define PosTranslatableBoincText29					PosGroupTranslatableBoincText+29

#define GroupTranslatableWWWText					"TranslatableWWWText"
#define PosGroupTranslatableWWWText					PosTranslatableBoincText29+1
#define GetTranslatableWWWText0						"0" 
#define PosTranslatableWWWText0						PosGroupTranslatableWWWText
#define GetTranslatableWWWText1						"1" 
#define PosTranslatableWWWText1						PosGroupTranslatableWWWText+1
#define GetTranslatableWWWText2						"2" 
#define PosTranslatableWWWText2						PosGroupTranslatableWWWText+2
#define GetTranslatableWWWText3						"3" 
#define PosTranslatableWWWText3						PosGroupTranslatableWWWText+3
#define GetTranslatableWWWText4						"4" 
#define PosTranslatableWWWText4						PosGroupTranslatableWWWText+4
#define GetTranslatableWWWText5						"5" 
#define PosTranslatableWWWText5						PosGroupTranslatableWWWText+5
#define GetTranslatableWWWText6						"6" 
#define PosTranslatableWWWText6						PosGroupTranslatableWWWText+6
#define GetTranslatableWWWText7						"7" 
#define PosTranslatableWWWText7						PosGroupTranslatableWWWText+7
#define GetTranslatableWWWText8						"8" 
#define PosTranslatableWWWText8						PosGroupTranslatableWWWText+8
#define GetTranslatableWWWText9						"9" 
#define PosTranslatableWWWText9						PosGroupTranslatableWWWText+9
#define GetTranslatableWWWText10					"10" 
#define PosTranslatableWWWText10					PosGroupTranslatableWWWText+10
#define GetTranslatableWWWText11					"11" 
#define PosTranslatableWWWText11					PosGroupTranslatableWWWText+11
#define GetTranslatableWWWText12					"12" 
#define PosTranslatableWWWText12					PosGroupTranslatableWWWText+12
#define GetTranslatableWWWText13					"13" 
#define PosTranslatableWWWText13					PosGroupTranslatableWWWText+13
#define GetTranslatableWWWText14					"14" 
#define PosTranslatableWWWText14					PosGroupTranslatableWWWText+14
#define GetTranslatableWWWText15					"15" 
#define PosTranslatableWWWText15					PosGroupTranslatableWWWText+15
#define GetTranslatableWWWText16					"16" 
#define PosTranslatableWWWText16					PosGroupTranslatableWWWText+16
#define GetTranslatableWWWText17					"17" 
#define PosTranslatableWWWText17					PosGroupTranslatableWWWText+17
#define GetTranslatableWWWText18					"18" 
#define PosTranslatableWWWText18					PosGroupTranslatableWWWText+18
#define GetTranslatableWWWText19					"19" 
#define PosTranslatableWWWText19					PosGroupTranslatableWWWText+19
#define GetTranslatableWWWText20					"20" 
#define PosTranslatableWWWText20					PosGroupTranslatableWWWText+20
#define GetTranslatableWWWText21					"21" 
#define PosTranslatableWWWText21					PosGroupTranslatableWWWText+21
#define GetTranslatableWWWText22					"22" 
#define PosTranslatableWWWText22					PosGroupTranslatableWWWText+22
#define GetTranslatableWWWText23					"23" 
#define PosTranslatableWWWText23					PosGroupTranslatableWWWText+23
#define GetTranslatableWWWText24					"24" 
#define PosTranslatableWWWText24					PosGroupTranslatableWWWText+24
#define GetTranslatableWWWText25					"25" 
#define PosTranslatableWWWText25					PosGroupTranslatableWWWText+25
#define GetTranslatableWWWText26					"26" 
#define PosTranslatableWWWText26					PosGroupTranslatableWWWText+26
#define GetTranslatableWWWText27					"27" 
#define PosTranslatableWWWText27					PosGroupTranslatableWWWText+27
#define GetTranslatableWWWText28					"28" 
#define PosTranslatableWWWText28					PosGroupTranslatableWWWText+28
#define GetTranslatableWWWText29					"29" 
#define PosTranslatableWWWText29					PosGroupTranslatableWWWText+29

#define TRANSLATIONS_NR								PosTranslatableWWWText29+1

// CTranslation command target

extern char *gszTranslation[TRANSLATIONS_NR+1];
extern UINT gCodePage;

class CTranslation : public CObject
{
public:
	CTranslation();
	virtual ~CTranslation();
	bool	Open(CString *psLog, bool bOverride = false);
	bool	Read(CString *psLog);
	void	Close();

	CString m_sFile;
	LCID	m_lcid;
private:
	bool	m_bOpen;

	bool	Get(CString sGroup, CString sGet, int iPosBegin, int iPosEnd, int iPosArray);
	bool	GetGroup(CString sGroup, int *piPosBegin, int *piPosEnd);

	bool	ProcessView();

	bool	m_bWarning;

	bool	m_bOverride;

	CFile	m_file;
	
	CString m_sText;
	CString m_sTextLower;

	CString m_sLog;
	bool	m_bError;

	int		m_iNrTranslations;




};


