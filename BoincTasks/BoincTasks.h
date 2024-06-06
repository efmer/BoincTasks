// BoincTasks.h : main header file for the BoincTasks application
//
#pragma once

//#define ANDROID '192.168.10.66'

//#define TESTBUILD	"6"
//#define KEEP_ALIVE	1

#include <deque>
#include <string>

// http://msdn.microsoft.com/en-us/library/windows/desktop/aa370610(v=vs.85).aspx	net show users in groups

// 0.92	ThreadRpc removed all CString. Char new.
// 0.92 BoincLibary remove a lot of unused routines
// 0.93 BoincLibray integrated in the release
// 0.93 Search computer: Removed CString -> char new.

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#define RUN_BOINC_CLIENT_MUTEX           "Global\\BoincSingleInstance"

#include "wincrypt.h"

#define FILTER_ACTIVE 1
#define FILTER_INACTIVE 0

//#ifdef LARGE_COMPUTERS
//#define MAX_COMPUTERS_REMOTE 300
//#else
//#define MAX_COMPUTERS_REMOTE 60
//#endif

#define LOCALHOST_NAME	"localhost"
//#define MAC_NAME "use_mac"

#define RECONNECT_INTERVAL				120
#define TIMEOUT_THREAD_LOCKED_DEFAULT	120

#define KILO_BYTE	1024
#define MEGA_BYTE	1048576
#define GIGA_BYTE	(MEGA_BYTE * KILO_BYTE)

#define CORE00 0
#define CORE01 1
#define CORE02 2
#define CORE03 3
#define CORE04 4
#define CORE05 5
#define CORE06 6
#define CORE07 7
#define CORE08 8
#define CORE09 9
#define CORE10 10
#define CORE11 11
#define CORE12 12
#define CORE13 13
#define CORE14 14
#define CORE15 15

// GPU = last core!!
#define GPUOFFSET (CORE15+1)
#define GPU0	(0+GPUOFFSET)
#define GPU1	(1+GPUOFFSET)
#define GPU2	(2+GPUOFFSET)
#define GPU3	(3+GPUOFFSET)
#define GPU4	(4+GPUOFFSET)
#define GPU5	(5+GPUOFFSET)
#define GPU6	(6+GPUOFFSET)
#define GPU7	(7+GPUOFFSET)

// simulated temperature
#define THROTTLE_CPU (8+GPUOFFSET)
#define THROTTLE_GPU (9+GPUOFFSET)

#define LIMIT_CPU (10+GPUOFFSET)
#define LIMIT_GPU (11+GPUOFFSET)

#define TEMPERATURE_ARRAY (LIMIT_GPU+1)

#define V2_TEMPERATURE_NR_OF_CPU	12
#define V2_TEMPERATURE_NR_OF_GPU	8
#define V2_TEMPERATURE_NR_OF_CORES (TEMPERATURE_NR_OF_CPU + TEMPERATURE_NR_OF_GPU)
#define V2_TEMPERATURE_NR_OF_TOTAL	24
#define V2_TEMPERATURE_BUFFER_LEN	340

#define V3_TEMPERATURE_NR_OF_CPU	16
#define V3_TEMPERATURE_NR_OF_GPU	8
#define V3_TEMPERATURE_NR_OF_CORES (TEMPERATURE_NR_OF_CPU + TEMPERATURE_NR_OF_GPU)
#define V3_TEMPERATURE_NR_OF_TOTAL	28
#define V3_TEMPERATURE_BUFFER_LEN	280

#define INVALID_TIME_STRING		"--"

#define CRASH_FOLDER		"\\crash\\"

#define PROPERTIES_READY	0
#define PROPERTIES_DIALOG	1
#define PROPERTIES_MOBILE	2

#define DELAY_SHOW_UPDATE_TIMER			2

#define TAB_COMPUTER	0
#define TAB_PROJECTS	1
#define TAB_TASKS		2
#define TAB_TRANSFERS	3
#define TAB_MESSAGES	4
#define TAB_HISTORY		5

//#define MAX_COMPUTERS_MENU	3

class CThreadWebServer;
extern bool	g_bSilentReboot;

#define CLOUD_VERSION "1";

//extern bool g_bBoincClientDebtValid;

extern char	g_cFileNameLog[MAX_PATH+32];
extern VOID WriteDebugLog(char* pFileName, char* pMsg, bool bNew);

// c:\programma\BoincTasks\BoincTasks\boinc\boinc_source\boinc\lib\parse.cpp
// replace strtod into strtod_own ?? needed
// add double strtod_own(const char *str, char **endptr); ?? needed

// add #pragma warning(disable : 4996) to the coproc header file.


//C:\programma\BoincTasks\BoincTasks\boinc\boinc_source\boinc\lib\gui_rpc_client.cpp
// 
// Added:
//
//	// setup timeout
//	struct timeval tv;
//	int timeouts = 0;
//	tv.tv_sec = 30;
//	tv.tv_usec = 0;
//	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,  sizeof tv))
//	{
//		int iFailed;
//		iFailed = 1;
//		// failed to setup time
//	}
//	//

// release used: http://boinc.berkeley.edu/svn/tags/boinc_core_release_6_10_56/


#define TIME_SYSTEM	0
#define TIME_24		1
#define	TIME_12		2
#define TIME_USER	3

#define TIME_DHMS	0
#define TIME_HMS	1

#define REFRESH_SLOW	0
#define REFRESH_NORMAL	1
#define REFRESH_QUICK	2
#define REFRESH_MANUAL	3

#define TEXT_STATUS_UPDATING	gszTranslation[PosWindowStatusMessageStatusUpdating]
#define TEXT_STATUS_WAITING		gszTranslation[PosWindowStatusMessageStatusWaiting]

// 10 min trickle work unit timer
#define	TRICKLE_TIMER_WU_TRIGGER		(60*10)
// 1/2 hour trickle message timer
#define TRICKLE_TIMER_MESSAGE_TRIGGER	(60*60)
// 1 hour update check timer
#define UPDATE_TIMER_WU_TRIGGER			(60*60*1)
// 10 seconds history timer
//#define UPDATE_HISTORY_TRIGGER			12
#define UPDATE_HISTORY_TRIGGER_MIN		2
// 60 Seconds with new BOINC clients.
#define UPDATE_HISTORY_TRIGGER_MIN_DEFAULT		60
#define UPDATE_HISTORY_TRIGGER_MAX_DEFAULT		240

#define	MAX_REFRESH_ASAP	400

#define CLOUD_PORT	443
#define CLOUD_SERVER "cloud.seti.nl"

//#define CLOUD_PORT	80
//#define CLOUD_SERVER 'cloud.seti.nl'

extern char *registrySectionDebugLog;
extern char *registryDebugNotices;
extern char *registryDebugNoticesTimer;
extern char *registryDebugConnection;
extern char *registryDebugClosing;
extern char *registryDebugSearchComputers;
extern char *registryDebugTemperatureGpu;
extern char *registryDebugTThrottle;
extern char *registryDebugTThrottleData;
extern char *registryDebugTThrottleRead;
extern char *registryDebugTThrottleGraph;
extern char *registryDebugSnooze;
extern char *registryDebugHistoryFetching;
extern char *registryDebugTasks;
extern char *registryDebugTasksShow;
extern char *registryDebugLocalDirectory;
extern char *registryDebugDirectory;
extern char *registryDebugState;
extern char *registryDebugMobile;
extern char *registryDebugMobileScript;
extern char *registryDebugMobileConnect;
extern char *registryDebugMobileConnectDump;
extern char *registryDebugCloud;
extern char *registryDebugHeapCheck;
extern char *registryDebugFileOnly;

extern char *registrySectionSettings;
extern char *registryExitOk;
extern char *registryManualCPID;
extern char *registryCPID;
extern char *registryCPID2;
extern char *registryDebugLogging;
extern char *registryDebugLoggingRules;
extern char *registryPath;
extern char *registryTime;
extern char *registryTimeThread;
extern char *registryTimeFormat;
extern char *registryTimeDhms;
extern char *registryRefresh;
extern char *registryRefreshManual;
extern char *registryQuickAndDirty;
extern char *registryStartBoincClient;
extern char *registryStopBoincClient;
extern char *registryStartBoincClientDelay;
extern char *registryStartShow;
extern char *registrySilentReboot;
extern char *registryThreadPriority;
extern char *registryWarningDeadlineDays;
extern char *registryWarningDeadlineHours;
extern char *registryWarningCheckpoint;
extern char *registryHorizontalGrid;
extern char *registryVerticalGrid;
extern char *registryNumberSeperator;
extern char *registryBoincTranslatable;

extern char *registryReconnectTime;
extern char *registryThreadTimeout;

extern char *registryEnableThreadGraph;

extern char *registryUpdateAuto;
extern char *registryUpdateBeta;
extern char *registryUpdateFolder;
extern char *registryUpdateUseBrowser;
extern char *registryBoincSettingsProcGraph;

extern char *registryUsingOs;

extern char *registryHistoryLoggingEnable;
extern char *registryHistoryLoggingSmartMode;
extern char *registryHistoryLoggingAccurate;
extern char *registryHistoryRemoveDays;
extern char *registryHistoryRemoveHours;
extern char *registryHistoryMaxUpdateTime;
extern char *registryHistoryMinUpdateTime;
extern char *registryHistoryMoveLong;
extern char *registryHistoryMoveLongHours;
extern char *registryHistoryMoveLongDays;
extern char *registryHistoryBackup;

extern char *registryTasksyMaxUpdateTime;
extern char *registryTasksCpuDigits;
extern char *registryTasksProgressDigits;

extern char *registryMessagesKeep;
extern char *registryMessagesProjectHl1;
extern char *registryMessagesProjectHl2;
extern char *registryMessagesProjectHl3;
extern char *registryMessagesProjectHl4;
extern char *registryMessagesProjectHl5;
extern char *registryMessagesProjectHl6;
extern char *registryMessagesProjectHl7;
extern char *registryMessagesProjectHl8;
extern char *registryMessagesProjectHl9;
extern char *registryMessagesProjectHl10;

extern char *registryMessagesHl1;
extern char *registryMessagesHl2;
extern char *registryMessagesHl3;
extern char *registryMessagesHl4;
extern char *registryMessagesHl5;
extern char *registryMessagesHl6;
extern char *registryMessagesHl7;
extern char *registryMessagesHl8;
extern char *registryMessagesHl9;
extern char *registryMessagesHl10;

extern char *registryNoticesHideAfter;
extern char *registryNoticesAlertEvery;
extern char *registryNoticesUpdateEvery;

extern char *registryNoticesSort0;
extern char *registryNoticesSort1;
extern char *registryNoticesSort2;
extern char *registryNoticesSort3;
extern char *registryNoticesSort4;
extern char *registryNoticesSort5;

extern char *registryConnectTthrottle;
extern char *registryTTPassword;
extern char *registryHideAtStartup;
extern char *registryMobile;
extern char *registryMobilePort;
extern char *registryMobileHeaderFont;
extern char *registryMobileFont;

extern char *registryUserFriendly;
extern char *registryCondenseUse;
extern char *registryRatioLongTerm;
extern char *registryDeadlineShowRemaining;
extern char *registrySortApplicationOnNr;
extern char *registryAlternatingStripes;
extern char *registryPercentageRect;
extern char *registryAdjustTime;
extern char *registrySkin;
extern char *registrySkinNumber;
extern char *registryLanguage;
extern char *registryGadgetShow;
extern char	*registryGadgetTime;
extern char	*registryGadgetTimeAfter;
extern char *registryGadgetRefreshTime;
extern char *registryGadgetMode;
extern char *registryLegacy;

extern char *registryWWWEnable;
extern char *registryWWWPassword;
extern char *registryWWWRefreshTime;
extern char *registryCloudEnable;
extern char *registryCloudServer;
extern char *registryCloudServerDebug;
extern char *registryCloudEmail;
extern char *registryCloudPassword;

extern char *registryEncryptionPassword;


extern char *registrySectionFloating;
extern char *registryFloatingLeft;
extern char *registryFloatingTop;
extern char *registryFloatingWidth;
extern char *registryFloatingHeight;
extern char *registryColumnFloater;


extern char *registrySectionWarnings;
extern char *registryWarningCpuLow1;
extern char *registryWarningCpuLow2;
extern char *registryWarningCpuLow3;
extern char *registryWarningCpuLow4;
extern char *registryWarningCpuHigh1;
extern char *registryWarningCpuHigh2;
extern char *registryWarningCpuHigh3;
extern char *registryWarningCpuHigh4;
extern char *registryWarningGpuLow1;
extern char *registryWarningGpuLow2;
extern char *registryWarningGpuLow3;
extern char *registryWarningGpuLow4;
extern char *registryWarningGpuHigh1;
extern char *registryWarningGpuHigh2;
extern char *registryWarningGpuHigh3;
extern char *registryWarningGpuHigh4;
extern char *registryWarningPercLow1;
extern char *registryWarningPercLow2;
extern char *registryWarningPercLow3;
extern char *registryWarningPercLow4;
extern char *registryWarningPercHigh1;
extern char *registryWarningPercHigh2;
extern char *registryWarningPercHigh3;
extern char *registryWarningPercHigh4;

extern char *registryWarningCpuTasks1;
extern char *registryWarningCpuTasks2;
extern char *registryWarningCpuTasks3;
extern char *registryWarningCpuTasks4;
extern char *registryWarningGpuTasks1;
extern char *registryWarningGpuTasks2;
extern char *registryWarningGpuTasks3;
extern char *registryWarningGpuTasks4;
extern char *registryWarningProject1;
extern char *registryWarningProject2;
extern char *registryWarningProject3;
extern char *registryWarningProject4;
extern char *registryWarningComputer1;
extern char *registryWarningComputer2;
extern char *registryWarningComputer3;
extern char *registryWarningComputer4;

extern char *registrySectionNotices;
extern char *registryNoticesLocalhostTime;

extern char *registrySectionWindowPosition;
extern char *registryPositionMainBottom;
extern char *registryPositionMainRight;
extern char *registryPositionMainLeft;
extern char *registryPositionMainTop;
extern char *registryPositionMainShow;
extern char *registryPositionWindowBottom;
extern char *registryPositionWindowLeft;
extern char *registryPositionWindowRight;
extern char *registryPositionWindowTop;
extern char *registryPositionWindowShow;
extern char *registryPosBarComputerCx;
extern char *registryPosBarComputerCy;
extern char *registryPosBarProjectCx;
extern char *registryPosBarProjectCy;


extern char *registrySchedulerPosLeft;
extern char *registrySchedulerPosTop;
extern char *registrySchedulerPosRight;
extern char *registrySchedulerPosBottom;

extern char *registryGraphicPosLeft;
extern char *registryGraphicPosTop;
extern char *registryGraphicPosRight;
extern char *registryGraphicPosBottom;

extern char *registryGraphicThreadPosLeft;
extern char *registryGraphicThreadPosTop;
extern char *registryGraphicThreadPosRight;
extern char *registryGraphicThreadPosBottom;

extern char *registryGraphicTransferPosLeft;
extern char *registryGraphicTransferPosTop;
extern char *registryGraphicTransferPosRight;
extern char *registryGraphicTransferPosBottom;

extern char *registryEditConfigPosLeft;
extern char *registryEditConfigPosTop;
extern char *registryEditConfigPosRight;
extern char *registryEditConfigPosBottom;

extern char *registryEditAppInfoPosLeft;
extern char *registryEditAppInfoPosTop;
extern char *registryEditAppInfoPosRight;
extern char *registryEditAppInfoPosBottom;

extern char *registryGraphicTasksPosLeft;
extern char *registryGraphicTasksPosTop;
extern char *registryGraphicTasksPosRight;
extern char *registryGraphicTasksPosBottom;

extern char *registryGraphicDeadlinePosLeft;
extern char *registryGraphicDeadlinePosTop;
extern char *registryGraphicDeadlinePosRight;
extern char *registryGraphicDeadlinePosBottom;

extern char *registryGraphicTemperaturePosLeft;
extern char *registryGraphicTemperaturePosTop;
extern char *registryGraphicTemperaturePosRight;
extern char *registryGraphicTemperaturePosBottom;

extern char *registryTThrottlePosLeft;
extern char *registryTThrottlePosTop;
extern char *registryTThrottlePosRight;
extern char *registryTThrottlePosBottom;

extern char *registrySelectComputerPosLeft;
extern char *registrySelectComputerPosTop;
extern char *registrySelectComputerPosRight;
extern char *registrySelectComputerPosBottom;

extern char *registryLoggingPosLeft;
extern char *registryLoggingPosTop;
extern char *registryLoggingRulesPosLeft;
extern char *registryLoggingRulesPosTop;
extern char *registryPropertiesPosLeft;
extern char *registryPropertiesPosTop;
extern char *registryPropertiesPosRight;
extern char *registryPropertiesPosBottom;

extern char *registryHistoryPosLeft;
extern char *registryHistoryPosTop;
extern char *registryHistoryPosRight;
extern char *registryHistoryPosBottom;

extern char *registryPropertiesComputer;
extern char *registryPropertiesTask;
extern char *registryPropertiesProject;
extern char *registryPropertiesTasksGraph;

extern char *registryPosSettingsLeft;
extern char *registryPosSettingsTop;

extern char *registryPosPrefLeft;
extern char *registryPosPrefTop;

extern char *registryYesNoPosLeft;
extern char *registryYesNoPosTop;
extern char *registryYesNoPosRight;
extern char *registryYesNoPosBottom;

extern char *registrySectionGeneral;
extern char *registryUpdateCount;
extern char *registryFirstTime;
extern char *registryHideAllowToolbar;
extern char *registryGarbageTime;
extern char *registryUpdateTime;
extern char *registryReadStatsTime;

extern char *registrySectionColumn;
extern char *registrySectionSorting;
extern char *registrySectionFilter;
extern char *registrySectionGraphic;
extern char *registrySectionGraphicTemperature;
extern char *registrySectionGraphicTransfer;
extern char *registrySectionGraphicTasks;
extern char *registrySectionGraphicDeadline;
extern char *registrySectionColorGraphic;
extern char *registrySectionHistoryDialog;

extern char *registryColumnComputer;
extern char *registryColumnTasks;
extern char *registryColumnTransfers;
extern char *registryColumnProjects;
extern char *registryColumnMessages;
extern char *registryColumnHistory;
extern char *registryTasksColumn0;
extern char *registryTasksColumn1;
extern char *registryTasksColumn2;
extern char *registryTasksColumn3;
extern char *registryTasksColumn4;
extern char *registryTasksColumn5;
extern char *registryTasksColumn6;
extern char *registryTasksColumn7;
extern char *registryTasksColumn8;
extern char *registryTasksColumn9;
extern char *registryTasksColumn10;
extern char *registryTasksColumn11;
extern char *registryTasksColumn12;
extern char *registryTasksColumn13;
extern char *registryTasksColumn14;
extern char *registryTasksColumn15;
extern char *registryTasksColumn16;
extern char *registryTasksColumn17;
extern char* registryTasksColumn18;
extern char* registryTasksColumn19;

extern char *registryTasksStatusSort;

extern char *registryMessageColumn0;
extern char *registryMessageColumn1;
extern char *registryMessageColumn2;
extern char *registryMessageColumn3;
extern char *registryMessageColumn4;

extern char *registryTransferColumn0;
extern char *registryTransferColumn1;
extern char *registryTransferColumn2;
extern char *registryTransferColumn3;
extern char *registryTransferColumn4;
extern char *registryTransferColumn5;
extern char *registryTransferColumn6;
extern char *registryTransferColumn7;

extern char *registryHistoryColumn0;
extern char *registryHistoryColumn1;
extern char *registryHistoryColumn2;
extern char *registryHistoryColumn3;
extern char *registryHistoryColumn4;
extern char *registryHistoryColumn5;
extern char *registryHistoryColumn6;
extern char *registryHistoryColumn7;
extern char *registryHistoryColumn8;
extern char *registryHistoryColumn9;
extern char *registryHistoryColumn10;
extern char *registryHistoryColumn11;

extern char *registryProjectsColumn0;
extern char *registryProjectsColumn1;
extern char *registryProjectsColumn2;
extern char *registryProjectsColumn3;
extern char *registryProjectsColumn4;
extern char *registryProjectsColumn5;
extern char *registryProjectsColumn6;
extern char *registryProjectsColumn7;
extern char *registryProjectsColumn8;
extern char *registryProjectsColumn9;
extern char *registryProjectsColumn10;
extern char *registryProjectsColumn11;
extern char *registryProjectsColumn12;
extern char *registryProjectsColumn13;
extern char *registryProjectsColumn14;
extern char *registryProjectsColumn15;
extern char *registryProjectsColumn16;

extern char *registryColumnProperties;
extern char *registryColumnDlgHistory;
extern char *registryColumnLocation;
extern char *registryLocationPosLeft;
extern char *registryLocationPosTop;
extern char *registryLocationPosRight;
extern char *registryLocationPosBottom;

extern char	*registryColumnYesNo;

extern char *registrySortTasksPrimary;
extern char *registrySortComputerPrimary;
extern char *registrySortComputerSecondary;
extern char *registrySortComputerTertiary;
extern char *registrySortTasksSecondary;
extern char *registrySortTasksTertiary;
extern char *registrySortProjectPrimary;
extern char *registrySortProjectSecondary;
extern char *registrySortProjectTertiary;
extern char *registrySortTransferPrimary;
extern char *registrySortTransferSecondary;
extern char *registrySortTransferTertiary;
extern char *registrySortMessagesPrimary;
extern char *registrySortMessagesSecondary;
extern char *registrySortMessagesTertiary;
extern char *registrySortHistoryPrimary;
extern char *registrySortHistorySecondary;
extern char *registrySortHistoryTertiary;
extern char *registrySortHistoryLongPrimary;
extern char *registrySortHistoryLongSecondary;
extern char *registrySortHistoryLongTertiary;

extern char *registryFilterWaitingToRun;
extern char *registryFilterReadyToReport;
extern char *registryFilterReadyToStart;
extern char *registryFilterDownload;
extern char *registryFilterUpload;
extern char *registryFilterSuspend;
extern char *registryFilterAborted;
extern char *registryFilterRunning;
extern char *registryFilterComputation;

extern char *registryShowActiceTasks;
extern char *registryShowNonCpuIntensive;
extern char *registryShowCpu;
extern char *registryShowGpu;

extern char *registryGraphicUserHost;
extern char *registryGraphicTotalAverage;
extern char *registryGraphicLineThickness;
extern char *registryGraphicCombineProject;
extern char *registryGraphicPeriod;
extern char *registryGraphicPeriodValue;
extern char *registryBlockMode;
extern char *registrySingleMultiple;
extern char *registryExpanded;
extern char *registryAverage;
extern char *registryAverageThread;
extern char *registryCpu;
extern char *registryGpu;

extern char *registryGraphicTransferDown;
extern char *registryGraphicTransferUp;

extern char *registryColorBackground;
extern char *registryColorProjectText;
extern char *registryColorScaleText;
extern char *registryColorGrid;
extern char *registryColorGridFine;
extern char *registryColorAverage;
extern char *registryColorComputer1;
extern char *registryColorComputer2;
extern char *registryColorComputer3;
extern char *registryColorComputer4;
extern char *registryColorComputer5;
extern char *registryColorComputer6;
extern char *registryColorComputer7;
extern char *registryColorComputer8;
extern char *registryColorComputer9;
extern char *registryColorComputer10;
extern char *registryColorComputer11;
extern char *registryColorComputer12;
extern char *registryColorComputer13;
extern char *registryColorComputer14;
extern char *registryColorComputer15;
extern char *registryColorComputer16;
extern char *registryColorComputer17;
extern char *registryColorComputer18;
extern char *registryColorComputer19;
extern char *registryColorComputer20;
extern char *registryColorFixed;

extern char *registrySectionFont;
extern char *registryFontLf;
extern char *registryFontLfGadget;
extern char *registryFontUseSystem;

extern char *registrySectionColor;
extern char *registryColorGeneralGrid;
extern char *registryColorGeneralFont;
extern char *registryColorGeneralFontSelected;
extern char *registryColorGeneralBackSelected;

extern char *registryColorGeneralBar;
extern char *registryColorGeneralBarOutline;

extern char *registryColorTasksAbort;
extern char *registryColorTasksUploadDownload;
extern char *registryColorTasksUploadDownloadGPU;
extern char *registryColorTasksReadyToReport;
extern char *registryColorTasksReadyToReportGPU;
extern char *registryColorTasksRunning;
extern char *registryColorTasksRunningGPU;
extern char *registryColorTasksHighPriority;
extern char *registryColorTasksHighPriorityGpu;
extern char *registryColorTasksWaitingToRun;
extern char *registryColorTasksWaitingToRunGPU;
extern char *registryColorTasksReadyToStart;
extern char *registryColorTasksReadyToStartGPU;
extern char *registryColorTasksError;
extern char *registryColorTasksErrorGPU;
extern char *registryColorTasksSuspended;
extern char *registryColorTasksSuspendedGPU;
extern char *registryColorHistoryUpload;
extern char *registryColorHistoryUploadGPU;
extern char *registryColorHistoryReadyToReport;
extern char *registryColorHistoryReadyToReportGPU;
extern char *registryColorHistoryAbort;
extern char *registryColorHistoryError;
extern char *registryColorHistoryErrorGPU;
extern char *registryColorHistoryReported;
extern char *registryColorHistoryReportedGPU;
extern char *registryColorHistoryReportedError;
extern char *registryColorHistoryReportedErrorGPU;
extern char *registryColorHistoryMissed;
extern char *registryColorHistoryMissedGpu;

extern char *registryColorWarningsErrorRatio;
extern char *registryColorWarningsErrorRatioGpu;
extern char *registryColorWarningsDeadline;
extern char *registryColorWarningsDeadlineRunning;
extern char *registryColorWarningsTasks;
extern char *registryColorWarningsCheckpoint;

extern char *registryColorProjectNormal;
extern char *registryColorProjectSuspended;
extern char *registryColorProjectNoNewWork;

extern char *registryColorMessages1;
extern char *registryColorMessages2;
extern char *registryColorMessages3;
extern char *registryColorMessagesHl1;
extern char *registryColorMessagesHl2;
extern char *registryColorMessagesHl3;
extern char *registryColorMessagesHl4;
extern char *registryColorMessagesHl5;
extern char *registryColorMessagesHl6;
extern char *registryColorMessagesHl7;
extern char *registryColorMessagesHl8;
extern char *registryColorMessagesHl9;
extern char *registryColorMessagesHl10;

extern char *registryColorTemperatureCpu1;
extern char *registryColorTemperatureCpu2;
extern char *registryColorTemperatureCpu3;
extern char *registryColorTemperatureCpu4;
extern char *registryColorTemperatureCpu5;
extern char *registryColorTemperatureCpu6;
extern char *registryColorTemperatureCpu7;
extern char *registryColorTemperatureCpu8;
extern char *registryColorTemperatureCpu9;
extern char *registryColorTemperatureCpu10;
extern char *registryColorTemperatureCpu11;
extern char *registryColorTemperatureCpu12;
extern char *registryColorTemperatureCpu13;
extern char *registryColorTemperatureCpu14;
extern char *registryColorTemperatureCpu15;
extern char *registryColorTemperatureCpu16;

extern char *registryColorTemperatureGpu1;
extern char *registryColorTemperatureGpu2;
extern char *registryColorTemperatureGpu3;
extern char *registryColorTemperatureGpu4;
extern char *registryColorTemperatureGpu5;
extern char *registryColorTemperatureGpu6;
extern char *registryColorTemperatureGpu7;
extern char *registryColorTemperatureGpu8;
extern char *registryColorTemperatureThrottleCpu;
extern char *registryColorTemperatureThrottleGpu;
extern char *registryColorTemperatureMax;
extern char *registryColorTemperatureMaxGpu;
extern char *registryColorTemperatureBack;
extern char *registryColorTemperatureGridFine;
extern char *registryColorTemperatureGrid;
extern char *registryColorTemperatureText;

//extern char *registrySectionAutoStart;
//extern char *registryAutoStartComputer;
//extern char *registryAutoStartCombined;

extern char *registryHotkeyIdBoincTasksMain;

extern char *registryPathAllUsers;

extern char *StartupKeyName;
extern char *StartupKeyValueName;
extern char *StartupKeyValue;

#define TASK_VIEW_ACTIVE		1
#define MESSAGE_VIEW_ACTIVE		2
#define PROJECT_VIEW_ACTIVE		3
#define TRANSFER_VIEW_ACTIVE	4
#define HISTORY_VIEW_ACTIVE		5
#define COMPUTER_VIEW_ACTIVE	6
#define NOTICES_VIEW_ACTIVE		7

#define PARAM_MESSAGE			0
#define PARAM_MESSAGE_TRICKLE	1


// messages

#define	WM_TASKBAR									WM_USER
// warning the first 10 users are used by Windows (baloon notice)

#define UWM_TASKBAR_ACTIVATE						WM_USER+10
#define UWM_MSG_NOTICES_ALERT						WM_USER+12
#define UWM_MSG_NOTICES_BT_UPDATE					WM_USER+13
#define UWM_MSG_NOTICES_SET_COLLAPSE_DELETE			WM_USER+15
#define UWM_MSG_NOTICES_BT_RULE						WM_USER+16
#define UWM_MSG_NOTICES_REFRESH						WM_USER+17
#define UWM_MSG_NOTICES_NEW							WM_USER+18

#define UWM_MSG_ADD_NEW_PROJECT						WM_USER+20
#define UWM_MSG_ADD_NEW_ACCOUNT_MANAGER				WM_USER+21

#define UWM_MSG_THREAD_HOSTSEARCH					WM_USER+30
#define UWM_MSG_THREAD_HOSTSEARCH_READY				WM_USER+31
#define UWM_MSG_THREAD_HOSTSEARCH_QUIT				WM_USER+32

#define UWM_MSG_THREAD_THREAD_CONNECT				WM_USER+40
#define UWM_MSG_THREAD_THREAD_CONNECT_READY			WM_USER+41
#define UWM_MSG_THREAD_CONNECT_LOST					WM_USER+42

#define UWM_MSG_THREAD_QUIT							WM_USER+50
#define UWM_MSG_THREAD_QUIT_READY					WM_USER+51
#define UWM_MSG_THREAD_QUIT_CLIENT					WM_USER+52
#define UWM_MSG_THREAD_CONNECT_RPC					WM_USER+53
#define UWM_MSG_THREAD_CONNECT_RPC_READY			WM_USER+54
#define UWM_MSG_THREAD_RECONNECT_RPC				WM_USER+55
#define UWM_MSG_THREAD_RECONNECT_TIME_RPC			WM_USER+56
#define	UWM_MSG_THREAD_START_RPC_TASK				WM_USER+57
#define	UWM_MSG_THREAD_READY_RPC_TASK				WM_USER+58
#define UWM_MSG_THREAD_FILTER_TASK					WM_USER+59

#define UWM_MSG_THREAD_READ_STATUS					WM_USER+60
#define UWM_MSG_THREAD_READ_STATUS_READY			WM_USER+61

#define UWM_MSG_THREAD_TASK_OPERATION				WM_USER+70
#define UWM_MSG_THREAD_TASK_OPERATION_AAV			WM_USER+71
#define UWM_MSG_THREAD_TASK_OPERATION_SEND_READY	WM_USER+72
#define UWM_MSG_THREAD_TASK_OPERATION_READY			WM_USER+73
#define UWM_MSG_THREAD_TASK_READ					WM_USER+74
#define UWM_MSG_THREAD_TASK_READ_READY				WM_USER+75

#define UWM_MSG_THREAD_TASK_GRAPHIC					WM_USER+78
#define UWM_MSG_THREAD_TASK_GRAPHIC_READY			WM_USER+79

#define UWM_MSG_THREAD_RPC_FLOATER_DATA				WM_USER+80
#define UWM_MSG_THREAD_RPC_FLOATER_DATA_READY		WM_USER+81

#define	UWM_MSG_THREAD_START_RPC_MESSAGE			WM_USER+83
#define	UWM_MSG_THREAD_READY_RPC_MESSAGE			WM_USER+84

#define	UWM_MSG_THREAD_START_RPC_NOTICES			WM_USER+85
#define	UWM_MSG_THREAD_READY_RPC_NOTICES			WM_USER+86

#define UWM_MSG_THREAD_READY_RPC_COMPUTER			WM_USER+88

#define UWM_MSG_THREAD_START_RPC_TRANSFER			WM_USER+90
#define UWM_MSG_THREAD_READY_RPC_TRANSFER			WM_USER+91
#define UWM_MSG_THREAD_TRANSFER_OPERATION			WM_USER+92
#define UWM_MSG_THREAD_TRANSFER_OPERATION_BYNAME	WM_USER+93 
#define UWM_MSG_THREAD_REFRESH_TRANSFERS			WM_USER+94

#define UWM_MSG_THREAD_START_RPC_THREAD_TIMING		WM_USER+96
#define UWM_MSG_THREAD_READY_RPC_THREAD_TIMING		WM_USER+98

#define UWM_MSG_THREAD_START_STATS					WM_USER+100
#define UWM_MSG_THREAD_READY_STATS					WM_USER+101
#define UWM_MSG_THREAD_CPID_CHANGED					WM_USER+102

#define UWM_MSG_THREAD_START_RPC_PROJECT			WM_USER+105
#define UWM_MSG_THREAD_READY_RPC_PROJECT			WM_USER+106
#define UWM_MSG_THREAD_PROJECT_OPERATION			WM_USER+107
#define UWM_MSG_THREAD_PROJECT_OPERATION_BYNAME		WM_USER+108

#define UWM_MSG_THREAD_PROJECT_OPERATION_SEND_READY	WM_USER+110
#define UWM_MSG_THREAD_PROJECT_OPERATION_READY		WM_USER+111
#define UWM_MSG_THREAD_COMPUTER_OPERATION_SEND_READY WM_USER+112
#define UWM_MSG_THREAD_COMPUTER_OPERATION_READY		WM_USER+113

#define UWM_MSG_THREAD_START_RPC_READ_PROJECT		WM_USER+118
#define UWM_MSG_THREAD_READY_RPC_READ_PROJECT		WM_USER+119
#define UWM_MSG_THREAD_READY_RPC_READ_PROJECT_DISK  WM_USER+120

#define	UWM_MSG_THREAD_START_RPC_PROJECT_LIST		WM_USER+121
#define	UWM_MSG_THREAD_READY_RPC_PROJECT_LIST		WM_USER+122
#define UWM_MSG_THREAD_SET_RPC_PROJECT_LIST			WM_USER+123

#define UWM_MSG_SET_DEBT_SET_THREAD_ID				WM_USER+124
#define UWM_MSG_THREAD_RPC_SET_PROJECT_DEBT			WM_USER+125
#define UWM_MSG_THREAD_RPC_SET_PROJECT_DEBT_READY	WM_USER+126

#define UWM_MSG_THREAD_START_RPC_GRAPHIC			WM_USER+127
#define UWM_MSG_THREAD_READY_RPC_GRAPHIC			WM_USER+128
#define UWM_MSG_THREAD_START_RPC_GRAPHIC_TRANSFER	WM_USER+129
#define UWM_MSG_THREAD_READY_RPC_GRAPHIC_TRANSFER	WM_USER+130

#define UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS		WM_USER+131
#define UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS_L	WM_USER+132
#define UWM_MSG_THREAD_START_RPC_GRAPHIC_TASKS_P	WM_USER+133
#define UWM_MSG_THREAD_READY_RPC_GRAPHIC_TASKS		WM_USER+134

#define UWM_MSG_THREAD_START_RPC_GRAPHIC_DEADLINE	WM_USER+141
#define UWM_MSG_THREAD_START_RPC_GRAPHIC_DEADLINE_L	WM_USER+142
#define UWM_MSG_THREAD_START_RPC_GRAPHIC_DEADLINE_P	WM_USER+143
#define UWM_MSG_THREAD_READY_RPC_GRAPHIC_DEADLINE	WM_USER+144

#define	UWM_MSG_REMOTE_MESSAGE_SWITCH				WM_USER+146
#define	UWM_MSG_REMOTE_TASKS_SWITCH					WM_USER+147
#define UWM_MSG_REMOTE_PROJECT_SWITCH				WM_USER+148
#define UWM_MSG_REMOTE_TRANSFER_SWITCH				WM_USER+149
#define UWM_MSG_REMOTE_HISTORY_SWITCH				WM_USER+150
#define UWM_MSG_REMOTE_COMPUTER_SWITCH				WM_USER+151
#define UWM_MSG_REMOTE_NOTICES_SWITCH				WM_USER+152

#define UWM_MSG_REMOTE_WINDOW_SHOWN					WM_USER+153

#define UWM_MSG_SETTING_TASKS						WM_USER+156
#define UWM_MSG_THREAD_SETTING_TASKS				WM_USER+157

#define UWM_MSG_THREAD_RPC_HISTORY					WM_USER+158
#define UWM_MSG_THREAD_RPC_HISTORY_READY			WM_USER+159
#define UWM_MSG_LONG_HISTORY						WM_USER+160

#define UWM_MSG_HISTORY_TASKS_COUNT					WM_USER+161
#define UWM_MSG_HISTORY_TASKS						WM_USER+162		
#define UWM_MSG_HISTORY_SOMETHING_HAPPENED			WM_USER+163
#define UWM_MSG_WUS_THAT_ARE_READY					WM_USER+164

#define UWM_MSG_SET_NETWORK_MODE					WM_USER+166
#define UWM_MSG_THREAD_SET_NETWORK_MODE				WM_USER+167
#define UWM_MSG_THREAD_SET_NETWORK_MODE_READY		WM_USER+168

#define UWM_MSG_LOGGING_TEXT						WM_USER+169

#define UWM_MSG_DLG_SETTINGS_TASKS_GET				WM_USER+170
#define UWM_MSG_DLG_SETTINGS_TASKS_SET				WM_USER+171
#define UWM_MSG_DLG_SETTINGS_TASKS_SET_WIDTH		WM_USER+172
#define UWM_MSG_DLG_SETTINGS_MESSAGES_GET			WM_USER+173
#define UWM_MSG_DLG_SETTINGS_MESSAGES_SET			WM_USER+174
#define UWM_MSG_DLG_SETTINGS_MESSAGES_INVALIDATE	WM_USER+175
#define UWM_MSG_DLG_SETTINGS_TRANSFERS_GET			WM_USER+176
#define UWM_MSG_DLG_SETTINGS_TRANSFERS_SET			WM_USER+177
#define UWM_MSG_DLG_SETTINGS_PROJECTS_GET			WM_USER+178
#define UWM_MSG_DLG_SETTINGS_PROJECTS_SET			WM_USER+179
#define UWM_MSG_DLG_SETTINGS_PROJECTS_SET_WIDTH		WM_USER+180
#define UWM_MSG_DLG_SETTINGS_HISTORY_GET			WM_USER+181
#define UWM_MSG_DLG_SETTINGS_HISTORY_SET			WM_USER+182
#define UWM_MSG_DLG_SETTINGS_HISTORY_SET_WIDTH		WM_USER+183

#define UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_TASKS	WM_USER+186
#define UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_PROJECTS	WM_USER+187
#define UWM_MSG_DLG_SETTINGS_APPLY_CHANGES_HISTORY	WM_USER+188

#define UWM_MSG_DLG_SETTINGS_BOINC_GRAPHIC			WM_USER+190

#define UWM_MSG_DLG_PROPERTIES_SETUP				WM_USER+192
#define UWM_MSG_DLG_PROPERTIES_ADD_ROW				WM_USER+193
#define UWM_MSG_DLG_SETTINGS_MOBILE					WM_USER+194

#define UWM_MSG_DLG_COMPUTER_LOCATION_SETUP			WM_USER+195

#define UWM_MSG_THREAD_START_RPC_CLIENT				WM_USER+197
#define UWM_MSG_THREAD_START_RPC_CLIENT_SET			WM_USER+198
#define UWM_MSG_THREAD_READY_RPC_CLIENT				WM_USER+299
#define UWM_MSG_THREAD_START_RPC_CLIENT_TIME		WM_USER+200
#define UWM_MSG_THREAD_START_RPC_CLIENT_TEMPERATURE	WM_USER+201
#define UWM_MSG_THREAD_READY_RPC_CLIENT_TEMPERATURE	WM_USER+202
#define UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT			WM_USER+203
#define UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT_THREAD	WM_USER+204
#define UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT_READY	WM_USER+205
#define UWM_MSG_THREAD_IP_WITH_MAC					WM_USER+206
#define UWM_MSG_THREAD_START_IP_SCAN				WM_USER+207

#define UWM_MSG_THREAD_START_IP_LIST				WM_USER+208
#define UWM_MSG_THREAD_START_IP_LIST_READY			WM_USER+209

#define UWM_MSG_THREAD_START_RPC_COMPUTER_VIEW		WM_USER+210
#define UWM_MSG_THREAD_READY_RPC_COMPUTER_VIEW		WM_USER+211

#define UWM_MSG_THREAD_BENCHMARK					WM_USER+212
#define UWM_MSG_THREAD_READ_CONFIG					WM_USER+213

#define UWM_MSG_THREAD_STOP_BOINC_CLIENT			WM_USER+218
#define UWM_MSG_THREAD_START_BOINC_CLIENT			WM_USER+219

#define UWM_MSG_THREAD_GARBAGE_COLLECTOR			WM_USER+220
#define UWM_MSG_THREAD_GARBAGE_COLLECTOR_READY		WM_USER+221

#define UWM_MSG_DIALOG_BOINC_SETTINGS_FILL			WM_USER+222

#define UWM_MSG_READ_SETTINGS						WM_USER+223
#define UWM_MSG_WRITE_SETTINGS						WM_USER+224

#define	UWM_MSG_TIMER_DLG							WM_USER+225
#define	UWM_MSG_TIMER_MESSAGE						WM_USER+226
#define	UWM_MSG_TIMER_PROJECT						WM_USER+227
#define UWM_MSG_TIMER_TRANSFER						WM_USER+228
#define UWM_MSG_TIMER_HYSTORY						WM_USER+229
#define UWM_MSG_TIMER_COMPUTER						WM_USER+230
#define UWM_MSG_TIMER_NOTICES						WM_USER+231

#define UWM_MSG_REMOTE_COLUMN_TASKS					WM_USER+241
#define UWM_MSG_REMOTE_COLUMN_MESSAGES				WM_USER+242
#define UWM_MSG_REMOTE_COLUMN_PROJECTS				WM_USER+243
#define UWM_MSG_REMOTE_COLUMN_TRANSFERS				WM_USER+244
#define UWM_MSG_REMOTE_COLUMN_COMPUTERS				WM_USER+245
	
#define UWM_MSG_PREFERENCE_DIALOG					WM_USER+250
#define UWM_MSG_PREFERENCE_OPEN_DIALOG				WM_USER+251
#define UWM_MSG_THREAD_PREFERENCE_READ				WM_USER+252
#define UWM_MSG_THREAD_PREFERENCE_READ_READY		WM_USER+253
#define UWM_MSG_THREAD_PREFERENCE_WRITE				WM_USER+254

#define UWM_MSG_PROXY_DIALOG						WM_USER+255
#define UWM_MSG_PROXY_OPEN_DIALOG					WM_USER+256
#define UWM_MSG_THREAD_PROXY_READ					WM_USER+257
#define UWM_MSG_THREAD_PROXY_READ_READY				WM_USER+258
#define UWM_MSG_THREAD_PROXY_WRITE					WM_USER+259

#define UWM_MSG_THREAD_NETWORK_AVAILABLE			WM_USER+260
#define UWM_MSG_THREAD_UPDATE_DAYS					WM_USER+261
#define UWM_MSG_THREAD_UPDATE_DAYS_ASAP				WM_USER+262
#define UWM_MSG_THREAD_DAYS_LEFT					WM_USER+263
#define UWM_MSG_THREAD_WU_LEFT						WM_USER+264
	
#define UWM_MSG_TAB_VIEW_OPEN						WM_USER+270
#define UWM_MSG_TAB_VIEW_CLOSE						WM_USER+271
#define UWM_MSG_TAB_VIEW_SET_TAB					WM_USER+272
#define UWM_MSG_TAB_VIEW_SELECTED					WM_USER+273
#define UWM_MSG_TAB_VIEW_NOTICES_COUNT				WM_USER+274	
#define UWM_MSG_VIEW_REFRESH						WM_USER+275

#define UWM_MSG_BAR_COMPUTER_CHECK_SELECTED			WM_USER+280
#define UWM_MSG_BAR_COMPUTER_GET_SELECTED			WM_USER+281	
#define UWM_MSG_BAR_COMPUTER_SET_SELECTED			WM_USER+282
#define UWM_MSG_BAR_COMPUTER_TREE					WM_USER+283
#define UWM_MSG_BAR_COMPUTER_SELECTION_CHANGED		WM_USER+284
#define UWM_MSG_BAR_COMPUTER_SET_CONNECT			WM_USER+285
#define UWM_MSG_BAR_COMPUTER_SELECT_ALL				WM_USER+286
#define UWM_MSG_BAR_COMPUTER_GET_TREE				WM_USER+287

#define UWM_MSG_BAR_PROJECT_SELECTION_CHANGED		WM_USER+288

#define UWM_MSG_THREAD_ID							WM_USER+289

#define UWM_MSG_GET_FLOATER_DATA					WM_USER+290
#define UWM_MSG_GET_FLOATER_DATA_READY				WM_USER+291
#define UWM_MSG_SET_FLOATER							WM_USER+292
#define UWM_MSG_SET_FLOATER_MODE					WM_USER+293

#define UWM_MSG_SET_SNOOZE_CPU						WM_USER+295
#define UWM_MSG_SET_SNOOZE_GPU						WM_USER+296
#define UWM_MSG_SET_SNOOZE_CPU_READY				WM_USER+297
#define UWM_MSG_SET_SNOOZE_GPU_READY				WM_USER+298
#define UWM_MSG_THREAD_SET_SNOOZE_CPU				WM_USER+299
#define UWM_MSG_THREAD_SET_SNOOZE_CPU_READY			WM_USER+300
#define UWM_MSG_THREAD_SET_SNOOZE_GPU				WM_USER+301
#define UWM_MSG_THREAD_SET_SNOOZE_GPU_READY			WM_USER+302
#define UWM_MSG_THREAD_GET_SNOOZE_CPU_GPU			WM_USER+303

#define UWM_MSG_DOCUMENT_IS_CLOSING					WM_USER+304
#define UWM_MSG_DOCUMENT_HAS_CLOSED					WM_USER+305

#define UWM_MSG_FONT_INITIAL						WM_USER+310
#define UWM_MSG_FONT_CHANGE_ALL						WM_USER+311
#define UWM_MSG_FONT_HAS_CHANGED					WM_USER+312

#define UWM_MSG_THREAD_WEB_ID						WM_USER+320
#define UWM_MSG_THREAD_WEB_START					WM_USER+321
#define UWM_MSG_THREAD_WEB_SERVER					WM_USER+322
#define UWM_MSG_THREAD_WEB_EMAIL					WM_USER+323
#define UWM_MSG_THREAD_WEB_STATUS_TEXT				WM_USER+324
#define UWM_MSG_THREAD_WEB_STOP						WM_USER+325
#define UWM_MSG_THREAD_WEB_HTML						WM_USER+326
#define UWM_MSG_THREAD_WEB_STATUS					WM_USER+327

#define UWM_MSG_START_CLOUD							WM_USER+338
#define UWM_MSG_THREAD_CLOUD_STATUS					WM_USER+339
#define UWM_MSG_THREAD_CLOUD_WRITE					WM_USER+340
#define UWM_MSG_GET_CLOUD_DATA						WM_USER+341
#define UWM_MSG_SET_CLOUD_COLUMN					WM_USER+342

#define UWM_MSG_WEB_ITEM_SELECTED					WM_USER+345
#define UWM_MSG_WEB_OPERATION						WM_USER+346
#define UWM_MSG_WEB_FILTER							WM_USER+347
#define UWM_MSG_WEB_SORT							WM_USER+348
#define UWM_MSG_WEB_PROPERTIES						WM_USER+349

#define UWM_INITIAL_GRAPHIC							WM_USER+350
#define UWM_CLOSE_GRAPHIC							WM_USER+351

#define UWM_RESTART_GRAPHIC							WM_USER+352
#define UWM_RESTART_GRAPHIC_TASKS					WM_USER+353
#define UWM_RESTART_GRAPHIC_TASKS_READY				WM_USER+354

#define UWM_RESTART_GRAPHIC_DEADLINE				WM_USER+356
#define UWM_RESTART_GRAPHIC_DEADLINE_READY			WM_USER+357

#define UWM_GRAPHIC_COLOR_INIT						WM_USER+360
#define UWM_GRAPHIC_COLOR_INIT2						WM_USER+361
#define UWM_GRAPHIC_COLOR_CHANGED					WM_USER+362
#define UWM_MSG_GRAPHIC_WINDOW_LINE					WM_USER+363
#define UWM_MSG_GRAPHIC_PROJECTS_ONE				WM_USER+364 
#define UWM_MSG_GRAPHIC_AVERAGE						WM_USER+365
#define UWM_MSG_GRAPHIC_GET							WM_USER+366

#define UWM_MSG_EDIT_CONFIG_OPEN_DIALOG				WM_USER+367
#define UWM_MSG_EDIT_APPP_INFO_OPEN_DIALOG			WM_USER+368

#define UWM_UPDATE_COMPUTER_LIST					WM_USER+370

#define UWM_SET_THREAD_PRIORITY						WM_USER+375

#define UWM_DLG_SETTINGS_TASKS_SORTING_STATUS		WM_USER+376

#define UWM_DLG_SEARCH_COMPUTER_START				WM_USER+378
#define UWM_DLG_SEARCH_COMPUTER_READY				WM_USER+379

#define UWM_MSG_TEXT_NEW_SIZE						WM_USER+380
#define UWM_MSG_TEXT_NEW_COLOR						WM_USER+381

#define UWM_MSG_RULES_ADD							WM_USER+382
#define UWM_MSG_RULES_GET_COPY						WM_USER+383
#define UWM_MSG_RULES_INTERVAL						WM_USER+384
#define UWM_MSG_SHOW_RULES_DIALOG					WM_USER+385
#define UWM_MSG_BUTTON								WM_USER+386

#define UWM_MSG_SHOW_THREAD_GRAPH					WM_USER+387

#define UWM_MSG_SELECTION_HAS_CHANGED				WM_USER+390

#define UWM_MSG_NOTICES_SORT						WM_USER+391

#define UWM_MSG_FORCE_A_RESTART						WM_USER+392

#define UWM_MSG_WND									WM_USER+393
#define UWM_MSG_NEW_SIZE							WM_USER+394
#define UWM_MSG_CLICKED_ON_ICON						WM_USER+395

#define UWM_MSG_SELECTION_MODE						WM_USER+396

#define UWM_MSG_DOUBLE_CLICK						WM_USER+398

#define UWM_MSG_KEY									WM_USER+399

#define UWM_MSG_THREAD_GET_CONFIG					WM_USER+400
#define UWM_MSG_THREAD_GET_CONFIG_READY				WM_USER+401
#define UWM_MSG_THREAD_SET_CONFIG					WM_USER+402
#define UWM_MSG_THREAD_SET_CONFIG_READY				WM_USER+403

#define UWM_MSG_THREAD_GET_APP_CONFIG				WM_USER+405
#define UWM_MSG_THREAD_GET_APP_CONFIG_READY			WM_USER+406
#define UWM_MSG_THREAD_SET_APP_CONFIG				WM_USER+407
#define UWM_MSG_THREAD_SET_APP_CONFIG_READY			WM_USER+408

#define UWM_MSG_EXIT								WM_USER+499

#define UWM_MSG_MESSAGE_COMPUTER_SELECTED			WM_USER+500 // warning !!!!! need 100 position free after this number 
#define UWM_MSG_MESSAGE_PROJECT_FILTER				WM_USER+600 // warning !!!!! need 100 position free after this number 




// thread must be the last as it expands at least 20 -> 30

#define UTIMER_THREAD_REMOTE			WM_USER+700
#define UTIMER_DLG						WM_USER+701
#define UTIMER_REMOTE_VIEW				WM_USER+702
#define UTIMER_DLG_ABOUT				WM_USER+703
#define UTIMER_DLG_GRAPHIC				WM_USER+704
#define UTIMER_DLG_GRAPHIC_WND			WM_USER+705
#define UTIMER_DLG_GRAPHIC_THREAD		WM_USER+706
#define UTIMER_DLG_GRAPHIC_TRANSFER		WM_USER+707
#define UTIMER_DLG_GRAPHIC_TASKS		WM_USER+708
#define UTIMER_DLG_GRAPHIC_DEADLINE		WM_USER+709
#define UTIMER_DLG_COLOR_GENERAL		WM_USER+710
#define UTIMER_DLG_COLOR_GRAPHIC		WM_USER+711
#define UTIMER_DLG_COLOR_TASKS			WM_USER+712
#define UTIMER_DLG_COLOR_PROJECTS		WM_USER+713
#define UTIMER_DLG_COLOR_WARNINGS		WM_USER+714
#define UTIMER_DLG_COLOR_MESSAGES		WM_USER+715
#define UTIMER_DLG_COLOR_HISTORY		WM_USER+716
#define UTIMER_DLG_BAR_VIEW				WM_USER+717
#define UTIMER_DLG_FLOATING				WM_USER+718
#define UTIMER_DLG_MAINFRM				WM_USER+719
#define UTIMER_DLG_SEARCH_COMPUTERS		WM_USER+720
#define UTIMER_DLG_SETTING_GENERAL		WM_USER+721
#define UTIMER_DLG_RULES				WM_USER+722
#define UTIMER_DLG_SETTING_RULES		WM_USER+723
#define UTIMER_DLG_GRAPHIC_TEMPERATURE	WM_USER+724
#define UTIMER_DLG_COLOR_TEMPERATURE	WM_USER+725
#define UTIMER_DLG_LOGGING				WM_USER+726
#define UTIMER_DLG_UPDATE				WM_USER+727
#define UTIMER_DLG_TTHROTTLE			WM_USER+728
#define UTIMER_DLG_HISTORY				WM_USER+729
#define UTIMER_DLG_BAR_COMPUTER			WM_USER+730
#define UTIMER_DLG_BAR_PROJECT			WM_USER+731
#define UTIMER_DLG_SETTING_EXPERT		WM_USER+732
#define UTIMER_DLG_SCHEDULER			WM_USER+733
#define UTIMER_WND_SCHEDULER			WM_USER+734
#define UTIMER_DLG_SELECT_COMPUTER		WM_USER+735

#define UWM_MSG_SET_DEBT_OPEN_DIALOG					WM_USER+790

#define UWM_MSG_ADD_ACCOUNTMANAGER_OPEN_DIALOG			WM_USER+792
//#define UWM_MSG_ADD_ACCOUNTMANAGER_SET_STRING_ID		WM_USER+693
//#define UWM_MSG_ADD_ACCOUNTMANAGER_SET_THREAD_ID		WM_USER+694
#define UWM_MSG_ADD_ACCOUNTMANAGER_CHANGED				WM_USER+795

#define UWM_MSG_ADDPROJECT_OPEN_DIALOG					WM_USER+800
#define UWM_MSG_ADDPROJECT_SET_THREAD_ID				WM_USER+801
#define UWM_MSG_THREAD_GET_HOST_INFO					WM_USER+802		
#define UWM_MSG_THREAD_GET_HOST_INFO_READY				WM_USER+803
#define UWM_MSG_THREAD_GET_TIME_STATS					WM_USER+804
#define UWM_MSG_THREAD_GET_TIME_STATS_READY				WM_USER+805
#define UWM_MSG_THREAD_GET_ALL_PROJECTS_LIST			WM_USER+806
#define UWM_MSG_THREAD_GET_ALL_PROJECTS_LIST_READY		WM_USER+807
#define UWM_MSG_THREAD_GETPLATFORM						WM_USER+808
#define UWM_MSG_THREAD_GETPLATFORM_READY				WM_USER+809
#define UWM_MSG_THREAD_PROJECT_CONFIG					WM_USER+810
#define UWM_MSG_THREAD_PROJECT_CONFIG_READY				WM_USER+811
#define UWM_MSG_THREAD_PROJECT_CONFIG_POLL				WM_USER+812
#define UWM_MSG_THREAD_PROJECT_CONFIG_POLL_READY		WM_USER+813
#define UWM_MSG_THREAD_ACCOUNT_LOOKUP					WM_USER+814
#define UWM_MSG_THREAD_ACCOUNT_LOOKUP_READY				WM_USER+815
#define UWM_MSG_THREAD_ACCOUNT_LOOKUP_POLL				WM_USER+816
#define UWM_MSG_THREAD_ACCOUNT_LOOKUP_POLL_READY		WM_USER+817
#define UWM_MSG_THREAD_PROJECT_ATTACH					WM_USER+818
#define UWM_MSG_THREAD_PROJECT_ATTACH_READY				WM_USER+819
#define UWM_MSG_THREAD_CREATE_ACCOUNT					WM_USER+820
#define UWM_MSG_THREAD_CREATE_ACCOUNT_READY				WM_USER+821
#define UWM_MSG_THREAD_CREATE_ACCOUNT_POLL				WM_USER+822
#define UWM_MSG_THREAD_CREATE_ACCOUNT_POLL_READY		WM_USER+823

#define UWM_MSG_THREAD_GET_ACCOUNTMANAGER_INFO			WM_USER+830
#define UWM_MSG_THREAD_GET_ACCOUNTMANAGER_INFO_READY	WM_USER+831
#define UWM_MSG_THREAD_ACCOUNTMANAGER_ATTACH			WM_USER+832
#define UWM_MSG_THREAD_ACCOUNTMANAGER_ATTACH_READY		WM_USER+833
#define UWM_MSG_THREAD_ACCOUNTMANAGER_POLL				WM_USER+834
#define UWM_MSG_THREAD_ACCOUNTMANAGER_POLL_READY		WM_USER+835

#define UWM_MSG_THREAD_ACCOUNTMANAGER_SYNCHRONIZE		WM_USER+836
#define UWM_MSG_THREAD_ACCOUNTMANAGER_SYNCHRONIZE_READY WM_USER+837

#define INITIAL_FILE_OPEN_NAME							"BoincTasks"
//#define INITIAL_FILE_OPEN_NAME						"!<>Combined<>!"

extern CString GetStartupLogFileName(CString sTxt);

extern void StringReplace(std::string *psSource, char *pcToBeReplaced, char *psToBeReplacedWith);
extern void StringReplace(std::string *psSource, std::string *psToBeReplaced, std::string *psToBeReplacedWith);
extern void GenerateRandomString(char *s, int len);
extern void GenerateRandomStringHex(char *s, int len);
extern bool ReadFromFile(CString *psText, CString *psFileName);

#define COMPARE_EQUAL	0
#define COMPARE_LARGER	1
#define COMPARE_SMALLER	2
extern int CompareDouble(double d1, double d2, int imul);

#include "resource.h"       // main symbols

// CBoincTasksApp:
// See BoincTasks.cpp for the implementation of this class
//
class CMainFrame;
class CDlgUpdate;
class CAboutDlg;
class CDlgLogging;
class CDlgRulesLogging;
class CDlgProperties;
class CDlgScheduler;
class CDlgComputerLocation;
class CDlgColor;
class CDlgAddProject;
class CDlgAddAccountManager;
class CDlgSetDebt;

class CDlgFloating;

//class CDgPrefExclusive;
//class CDlgPrefDisk;
//class CDlgPrefNetwork;
//class CDlgPrefProcessor;
//class CDlgPrefMain;

class CDlgProxyMain;
class CDlgProxyHttp;
class CDlgProxySocks;

class CDlgSettingsMain;
class CDlgSettingsGeneral;
class CDlgSettingsView;
class CDlgSettingsProject;
class CDlgSettingsWarning;
class CDlgSettingsNotices;
class CDlgSettingsHistory;
class CDlgSettingsTasks;
class CDlgSettingsGadget;
class CDlgSettingsWWW;
class CDlgSettingsMobile;
class CDlgSettingsExpert;
class CDlgSettingsRules;
class CDlgSettingsMessage;
class CDlgSettingsTransfer;

class CDlgColorMain;
class CDlgColorGeneral;
class CDlgColorProjects;
class CDlgColorTasks;
class CDlgColorWarnings;
class CDlgColorMessages;
class CDlgColorHistory;
class CDlgColorGraphic;
class CDlgColorTemperature;
class CDlgFont;

class CMultiLanguage;
class CTranslation;

class CThreadIpSearch;
class CThreadGarbageCollectorData;

class CComputer;
class CBoincTasksApp : public CWinApp
{
public:
	CBoincTasksApp();
	CMainFrame			*m_pMainFrame;
//	CMultiDocTemplate	*m_pDocTemplateTasks;
//	CMultiDocTemplate	*m_pDocTemplateComputer;
//	CMultiDocTemplate	*m_pDocTemplateRemote;
	CMultiDocTemplate	*m_pDocTemplateComputerCombinedRemote;

	void	EmptyComputerList(std::deque<CComputer*> *plComputers);
//	CList	<CComputer*,CComputer*> m_lComputers;
	std::deque<CComputer*> m_lComputers;
	std::deque<std::string> m_lLocalhost;
	bool	m_blComputersChanged;

	CString GetBoincDirectory(char *pKey);

	void	UnCheckComputer(CString sName);
	bool	SetPriority(DWORD dwOwnerPID) ;
	void	Connect();
	bool	ValidScreenPosition(int *pLeft, int *pTop, int *pWidth, int *pHeight, int iLrMargin = 100, int iUdMargin = 100);

// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

	void	WriteXml(void);

	bool	m_bBoincClientStartedByBoincTasks;

	DWORD	m_dwThreadPriority;

	HMENU					m_hMenuComputer;
	HMENU					m_hMenuRemote;
	//HMENU					m_hMenuNetwork;
	//HMENU					m_hMenuAllow;
	//HMENU					m_hMenuAllowGpu;
	HMENU					m_hMenuSynchronize;
	HMENU					m_hMenuBenchmark;
	HMENU					m_hMenuReadConfig;

	CString					GetPassword(CString *pDir = NULL);

	CString					m_sLocation;

	CDlgUpdate				*m_pDlgUpdate;
	CAboutDlg				*m_pDlgAbout;

	CDlgLogging				*m_pDlgLogging;

	CDlgRulesLogging		*m_pDlgRulesLogging;

	CDlgAddProject			*m_pDlgAddProject;
	CDlgAddAccountManager	*m_pDlgAddAccountManager;
	CDlgSetDebt				*m_pDlgSetDebt;

	CDlgProperties			*m_pDlgProperties;

	CDlgScheduler			*m_pDlgScheduler;

	CDlgComputerLocation	*m_pDlgComputerLocation;

	CDlgFloating			*m_pDlgFloating;

	CDlgProxyMain			*m_pDlgProxyMain;
	CDlgProxyHttp			*m_pDlgProxyHttp;
	CDlgProxySocks			*m_pDlgProxySocks;

	CDlgSettingsMain		*m_pDlgSettingsMainDummy; 
	CDlgSettingsMessage		*m_pDlgSettingsMessage;
	CDlgSettingsTransfer	*m_pDlgSettingsTransfer;

	CDlgSettingsMain		*m_pDlgSettingsMain; 
	CDlgSettingsGeneral		*m_pDlgSettingsGeneral;
	CDlgSettingsView		*m_pDlgSettingsView;
	CDlgSettingsProject		*m_pDlgSettingsProjects;
	CDlgSettingsTasks		*m_pDlgSettingsTasks;
	CDlgSettingsHistory		*m_pDlgSettingsHistory;
	CDlgSettingsNotices		*m_pDlgSettingsNotices;
	CDlgSettingsWarning		*m_pDlgSettingsWarning;
	CDlgSettingsGadget		*m_pDlgSettingsGadget;
	CDlgSettingsWWW			*m_pDlgSettingsWWW;
	CDlgSettingsMobile		*m_pDlgSettingsMobile;
	CDlgSettingsExpert		*m_pDlgSettingsExpert;
	CDlgSettingsRules		*m_pDlgSettingsRules;

	
	CDlgColorMain			*m_pDlgColorMain;
	CDlgColorGeneral		*m_pDlgColorGeneral;
	CDlgColorProjects		*m_pDlgColorProjects;
	CDlgColorTasks			*m_pDlgColorTasks;
	CDlgColorWarnings		*m_pDlgColorWarnings;
	CDlgColorMessages		*m_pDlgColorMessages;
	CDlgColorHistory		*m_pDlgColorHistory;
	CDlgColorGraphic		*m_pDlgColorGraphic;
	CDlgColorTemperature	*m_pDlgColorTemperature;
	CDlgFont				*m_pDlgFont;

	CMultiLanguage			*m_pMultiLanguage;
	CTranslation			*m_pTranslation;

	bool	m_bShowAtStartup;

	bool	m_bExit;

//	bool	m_bRebootRemote;
//	bool	m_bRebootRemoteCombined;

	int		m_iThreadTimeout;

	bool	m_bDocumentOpen;
	bool	m_bDocumentFirstTimeOpen;

	int		m_iDownloadRefreshTime;	
	int		m_iUploadRefreshTime;
	bool	m_bAutoRefreshCheat;
	int		m_iAutoRefreshTime;
	int		m_iRefreshRefreshTime;
	int		m_iRefreshRefreshASAP;

	bool	m_bNoCleanup;

	CString m_sVlar;

	bool	m_bRegulatorActive;
	int		m_iRegulatorMaxTasks;
	double	m_dRegulatorDaysWorkRequested;
	double	m_dRegulatorMinDaysGpu;
	double	m_dRegulatorMinDaysCpu;
	double	m_dRegulatorDaysWorkRequestedMin;

	std::deque<std::string> *m_plLocalhostIp;

	CString	m_sCPID;

	CThreadGarbageCollectorData	*m_pThreadGarbageHistory;
	CThreadGarbageCollectorData	*m_pThreadGarbageLog;

	CThreadIpSearch *m_pThreadIpSearch;

	bool GetBoincTasksLanguage(int iLangIdSelected);

private:
	int		DeleteFiles(CString sDirectory, int iTimeToRemove);
	void	SetLocale(CString sLanguage);
	void	Reboot();
	std::deque<std::string>	*GetLocalhostIp();

	bool	m_bWSAStartup;

	void	FirstTimeLocalHost();

	int		ReadXml(void);
	void	ReadConfigXml(void);

	void	InitEncryption(void);
	int		Encrypt(char *msg,long orginalSize);
	int		Decrypt(char *dest,DWORD actualSize);

	// encryption
	CStringA	m_sEncryptKey;
	int			m_iInitialisationSuccess;
	HCRYPTPROV	m_cspContext;
	HCRYPTHASH	m_hashData;
	HCRYPTKEY	m_keyEncryption;
	// encryption


	CString TranslateToXml(char *pBuffer, int iActualSize);
	int		TranslateFromXml(char *pBufferIn, char *pBufferOut);

	HANDLE	m_hMutex;
	bool	m_bRestart;

	bool	m_bDebug;

	CWnd	wndInvisible;



// Implementation
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnAutostartup();
//	afx_msg void OnUpdateAutostartup(CCmdUI *pCmdUI);
//	afx_msg void OnAutostartupCombined();
//	afx_msg void OnUpdateAutostartupCombined(CCmdUI *pCmdUI);
	afx_msg void OnHelpManual();
	afx_msg void OnHelpForum();
	afx_msg void OnHelpBoincWebsite();
	afx_msg void OnFileCloseboincclientandallscienceapplications();
	afx_msg void OnUpdateFileCloseboincclientandallscienceapplications(CCmdUI *pCmdUI);
	afx_msg void OnFileStartboincclient();
	afx_msg void OnUpdateFileOnFileStartboincclient(CCmdUI *pCmdUI);
	afx_msg void OnFileReadColorSettings();
	afx_msg void OnFileWriteColorSettings();
};

extern CBoincTasksApp theApp;