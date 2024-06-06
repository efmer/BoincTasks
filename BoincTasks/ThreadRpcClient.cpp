// ThreadRpcClient.cpp : implementation file
//

#include "stdafx.h"
#ifdef BoincTasks
#include "BoincTasks.h"
#else
#include "client.h"
#endif
#include "RpcClient.h"
#include "ThreadRpcClient.h"
#include "ThreadRpcSearch.h"
#include "DlgLogging.h"
#include "DlgSettingsGeneral.h"
#include "\programma\common\bf\Blowfish.h"

CRpcInfoClientSearch::CRpcInfoClientSearch()
{
	Clear();

}

CRpcInfoClientSearch::~CRpcInfoClientSearch()
{
}

void CRpcInfoClientSearch::Clear()
{
	m_iThreadID = 0;
}


// CThreadRpcClient

IMPLEMENT_DYNCREATE(CThreadRpcClient, CWinThread)

CThreadRpcClient::CThreadRpcClient()
{
	m_pRpcClient = new CRpcClient;
	m_pSearchBoinc = NULL;

	m_pcBuffer = NULL;
}

CThreadRpcClient::~CThreadRpcClient()
{
	delete m_pRpcClient;
	if (m_pcBuffer)
	{
		delete m_pcBuffer;
	}

//	Delete them here even as they are created in RemoteCombinedDoc
//	delete m_pcPassword;
//	delete m_pcComputerId;
}

BOOL CThreadRpcClient::InitInstance()
{
	bool bConnected = false, bReceived;
	CString sMessage, sALower;
	HWND	hWnd;
//	int		iThread;
	int		iStatus, iPortTThrottle, iPortToolbox;
	CBlowFish bf;
	int		iMsg;

	sMessage = "BT";
	hWnd = NULL;
	m_bExit = false;

	while (!m_bExit)
	{
		MSG msg;
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			iMsg = msg.message;
			if (iMsg > 0)
			{
				if (iMsg == UWM_MSG_THREAD_QUIT)
				{
					hWnd = (HWND) msg.wParam;
					m_bExit = true;
				}

				// read temperature
				if (iMsg == UWM_MSG_THREAD_START_RPC_CLIENT)
				{
					hWnd = (HWND) msg.wParam;
					CRpcInfoClient *pInfoClient = (CRpcInfoClient *) msg.lParam;
					m_sKey = pInfoClient->m_sKey;
					iStatus = SendReceive(&hWnd, pInfoClient, false);
					pInfoClient->m_iStatus = iStatus;
					pInfoClient->m_sHostName = m_sHostName;
					pInfoClient->m_sComputerId = m_sComputerId;
					pInfoClient->m_sPort = m_sPort;
					::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_CLIENT, (WPARAM) pInfoClient, 0);
//					break;		nono
				}

				if (iMsg == UWM_MSG_THREAD_START_RPC_CLIENT_SET)
				{
					hWnd = (HWND) msg.wParam;
					CRpcInfoClient *pInfoClient = (CRpcInfoClient *) msg.lParam;
					m_sKey = pInfoClient->m_sKey;
					iStatus = SendReceive(&hWnd, pInfoClient, true);
					pInfoClient->m_iStatus = iStatus;
					pInfoClient->m_sHostName = m_sHostName;
					pInfoClient->m_sComputerId = m_sComputerId;
					pInfoClient->m_sPort = m_sPort;
					::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_CLIENT, (WPARAM) pInfoClient, 0);
//					break;		nono
				}
			
				// read temperature graphic
				if (iMsg == UWM_MSG_THREAD_START_RPC_CLIENT_TEMPERATURE)
				{
					char cBufferG[32];
					char cBufferError[128];
					char cBufferMessage[255];
					char cBufferFrom[128];
//					char cBufferLog[1024];


					char *pBufferError;
					bool bDebugGraphic;
					CTemperatureGraphBuffer *pTemperatureGraphBuffer;

					bDebugGraphic = theApp.m_pDlgLogging->m_bLogDebugTThrottleGraphic;

					hWnd = (HWND) msg.wParam;
					UINT *piBuffer;
					pTemperatureGraphBuffer = (CTemperatureGraphBuffer *) msg.lParam;
					piBuffer = pTemperatureGraphBuffer->m_piTemperatureBuffer;

					if (m_iPort < 2) m_iPort = 31416 + 1;

					_snprintf_s(cBufferG, 31,_TRUNCATE,"%d", m_iPort);
					m_sPort = cBufferG;
					bReceived = false;

//					char *pBufferMessage;
//					pBufferMessage = new char [255];
					_snprintf_s(cBufferMessage, 254,_TRUNCATE,"BT_From_To%lld", pTemperatureGraphBuffer->m_tLastAddTime);
					bConnected = m_pRpcClient->Connect((char *) m_sHostName.c_str(), (char *) m_sPort.c_str(), 0, false);

					if (m_pcBuffer == NULL)
					{
						m_pcBuffer = new char[THREAD_RPC_BUFFER+64];
					}

//					char	cOutput[1024*32];
					m_sKey = theApp.m_pDlgSettingsGeneral->m_sPassword;
					if (m_sKey.GetLength() > 0)
					{
						bf.Encrypt(cBufferMessage, m_pcBuffer,  m_sKey.GetBuffer(), THREAD_RPC_BUFFER); m_sKey.ReleaseBuffer();
					}
					else
					{
						strcpy_s((char *) m_pcBuffer, THREAD_RPC_BUFFER,cBufferMessage);
					}

					if (bConnected)	bReceived = m_pRpcClient->SendReceive((char *) m_pcBuffer);
					else bReceived = false;
					m_pRpcClient->Disconnect();	
					m_pRpcClient->ClearAddress();
//					m_pRpcClient->Cleanup();
//					delete m_pcBuffer;
//					m_pcBuffer = NULL;
				
					int iStatus, iItemCount;
					iItemCount = 0;
					if (bReceived)
					{
						if (bDebugGraphic)
						{
							char cBufferFormat[128];
							char cBufferLog[1024];
							BYTE *pcBuffer;
//							pcBufferFrom = new char [128];
//							pcBufferLog = new char [1024];
							strcpy_s(cBufferFrom,127, "Receive TThrottle Graphic: ");
							strcat_s(cBufferFrom,127, m_sHostName.c_str());
							strcat_s(cBufferFrom,127, ", ");
							strcat_s(cBufferFrom,127, m_sPort.c_str());
							strcpy_s(cBufferLog,1023, "Raw: ");
							pcBuffer = (BYTE *) m_pRpcClient->m_pcReceiveBuffer;
							for (int iCount = 0; iCount < 100; iCount++)
							{
								if (*pcBuffer == 0) break;
								_snprintf_s(cBufferFormat,127,_TRUNCATE,"%x ",*pcBuffer);
								pcBuffer++;
								strcat_s(cBufferLog,1023,cBufferFormat);
							}
							SendLogging(cBufferFrom, cBufferLog, LOGGING_DEBUG);
//							delete pcBufferFrom;
//							delete pcBufferLog;
							pBufferError = &cBufferError[0];
						}
						else
						{
							pBufferError = NULL;
						}

						iStatus = ReadGraphic((BYTE *) m_pRpcClient->m_pcReceiveBuffer, piBuffer, &iItemCount, pBufferError, &pTemperatureGraphBuffer->m_tLastAddTime);
						if (bDebugGraphic)
						{
							char cBufferFrom[128];
							char cBufferLog[128];
//							char *pcBufferFrom, *pcBufferLog;
//							pcBufferFrom = new char [128];
//							pcBufferLog = new char [128];
							strcpy_s(cBufferFrom,127, "Receive TThrottle Graphic: ");
							strcat_s(cBufferFrom,127, m_sHostName.c_str());
							strcat_s(cBufferFrom,127, ", ");
							strcat_s(cBufferFrom,127, m_sPort.c_str());
							strcpy_s(cBufferLog,127, pBufferError);
							SendLogging(cBufferFrom, cBufferLog, LOGGING_DEBUG);
//							delete pcBufferFrom;
//							delete pcBufferLog;
						}

					}
					else
					{
//						m_pRpcClient->Disconnect();									// no receive disconnect and connect next time
						iStatus = -1;
					}
	
					while (::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))			// flush the message buffer
					{
						if (msg.message == UWM_MSG_THREAD_QUIT)
						{
							hWnd = (HWND) msg.wParam;
							m_bExit = true;	
						}
						Sleep(1);
					}

//					pTemperatureGraphItem->m_tLastAddTime = m_tLastAddTime;

					::PostMessage(hWnd,UWM_MSG_THREAD_READY_RPC_CLIENT_TEMPERATURE, iItemCount, iStatus);
				}
//					break;		nono


				if (iMsg == UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT)
				{
					int		iPos;
					char	*pcFound;
					bool	bBoinc;
					char	*pcMessage = "dummy";
	
					hWnd = (HWND) msg.wParam;		// windows instead of a thread

					m_pInfo = (CRpcInfoClientSearch *) msg.lParam;
					m_iPort = m_pInfo->m_iPort;

					bool	bReceived;
					CString	sReceive;

					char	cPort[64];
//					pcPort = new char[64];

					m_iBoincTThrottle = 0;
					m_pRpcClient->Disconnect();
					m_pRpcClient->ClearAddress();

					iPortTThrottle = m_iPort + 1;
					iPortToolbox = m_iPort + 2;

					bBoinc = false;
					_snprintf_s(cPort, 63,_TRUNCATE,"%d", m_iPort);
					if (m_pRpcClient->Connect((char *) m_pInfo->m_sIp.c_str(), cPort, m_pInfo->m_iTimeOut, false))	// time out used to be 30
					{
						bReceived = m_pRpcClient->SendReceive(pcMessage);
						if (bReceived)
						{
							char cBufferFrom[128];
//							pcBufferFrom = new char [128];
						
							strcpy_s(cBufferFrom,127, "Scan BOINC client: ");
							strcat_s(cBufferFrom,127, m_pInfo->m_sIp.c_str());
							strcat_s(cBufferFrom,127, ", ");
							strcat_s(cBufferFrom,127,cPort);

							SendLogging(cBufferFrom, m_pRpcClient->m_pcReceiveBuffer, LOGGING_DEBUG);
//							delete pcBufferFrom;

							pcFound = strstr(m_pRpcClient->m_pcReceiveBuffer, "boinc");
							if (pcFound != 0)
							{
								bBoinc = true;
								m_iBoincTThrottle += BIT_TRUE_BOINC;
							}
							else
							{
								if (strlen(m_pRpcClient->m_pcReceiveBuffer) == 0)	// later versions of BOINC only return a blank string
								{
									bBoinc = true;
									m_iBoincTThrottle += BIT_TRUE_BOINC;
								}
							}
						}
					}
					m_pRpcClient->Disconnect();
					m_pRpcClient->ClearAddress();

					// Toolbox
					_snprintf_s(cPort, 63,_TRUNCATE,"%d", iPortToolbox);
					if (m_pRpcClient->Connect((char *) m_pInfo->m_sIp.c_str(), cPort, m_pInfo->m_iTimeOut, false))	/// Timeout used to be 30
					{
						struct	hostent *host;
						struct	in_addr *ptr;	// To retrieve the IP Address
						char	szHostName[MAX_PATH], szIp[32];
						gethostname(szHostName, MAX_PATH );
						szIp[0] = 0;
						host = gethostbyname(szHostName);
						if(host != NULL)
						{
							ptr = (struct in_addr *) host->h_addr_list[0];
							_snprintf_s(szIp,32,_TRUNCATE,"%d.%d.%d.%d",ptr->S_un.S_un_b.s_b1,ptr->S_un.S_un_b.s_b2,ptr->S_un.S_un_b.s_b3,ptr->S_un.S_un_b.s_b4);
						}

						char	cOutput[1024*32];
						strcpy_s((char *) cOutput,1024*32,"<toolbox>");
						strcat_s((char *) cOutput,1024*32,"<hn:");
						strcat_s((char *) cOutput,1024*32,szHostName);
						strcat_s((char *) cOutput,1024*32,">");
						strcat_s((char *) cOutput,1024*32,"<ip:");
						strcat_s((char *) cOutput,1024*32,szIp);
						strcat_s((char *) cOutput,1024*32,">");
						bReceived = m_pRpcClient->SendReceive(cOutput);
						if (bReceived)
						{
							CStringA sAKey, sATemp;
							sAKey = "9@^";
							sAKey+= "A()Z";
							sAKey+= "xy35!~?";
							sAKey.SetAt(2,'>');
							sAKey.SetAt(7,'<');

//							<TOOLBOX>
							bf.Decrypt(m_pRpcClient->m_pcReceiveBuffer, cOutput, sAKey.GetBuffer(), 1024*32); sAKey.ReleaseBuffer();
							sReceive = cOutput;
//							else sReceive = m_pRpcClient->m_pcReceiveBuffer;
							sALower = sReceive;
							sALower.MakeLower();
							iPos = sALower.Find("<toolbox>",0);
							if (iPos >=0)
							{
								iPos = sALower.Find("<hn:");
								if (iPos >= 0)
								{
									sATemp = sReceive.Mid(iPos+4);
									iPos = sATemp.Find(">");
									if (iPos >= 0)
									{
										sATemp = sATemp.Left(iPos);
										if (sATemp.GetLength() > 0)
										{
											m_pInfo->m_sFoundHostName = sATemp;
										}
									}
								}
								iPos = sALower.Find("<pw:");
								if (iPos >= 0)
								{
									sATemp = sReceive.Mid(iPos+4);
									iPos = sATemp.Find(">");
									if (iPos >= 0)
									{
										sATemp = sATemp.Left(iPos);
										if (sATemp.GetLength() > 0)
										{
											m_pInfo->m_sPassWord = sATemp;
										}
									}
								}
								m_iBoincTThrottle += BIT_TRUE_TOOLBOX;
							}
						}
					}
					m_pRpcClient->Disconnect();
					m_pRpcClient->ClearAddress();

					m_pInfo->m_sFoundHostName = "";

					// TThrottle
					_snprintf_s(cPort, 63,_TRUNCATE,"%d", iPortTThrottle);
					if (m_pRpcClient->Connect((char *) m_pInfo->m_sIp.c_str(), cPort, m_pInfo->m_iTimeOut, false))	/// Timeout used to be 30
					{
						char	cOutput[1024*32];
						m_sKey = theApp.m_pDlgSettingsGeneral->m_sPassword;
						if (m_sKey.GetLength() > 0)
						{
							bf.Encrypt(pcMessage, cOutput,  m_sKey.GetBuffer(), 1024*32); m_sKey.ReleaseBuffer();
						}
						else
						{
							strcpy_s((char *) cOutput,1024*32,pcMessage);
						}
						bReceived = m_pRpcClient->SendReceive(cOutput);
						if (bReceived)
						{
							if (m_sKey.GetLength() > 0)
							{
								bf.Decrypt(m_pRpcClient->m_pcReceiveBuffer, cOutput, m_sKey.GetBuffer(), 1024*32); m_sKey.ReleaseBuffer();
								sReceive = cOutput;
							}
							else sReceive = m_pRpcClient->m_pcReceiveBuffer;
							sALower = sReceive;
							sALower.MakeLower();
							iPos = sALower.Find("tthrottle",0);
							if (iPos >=0)
							{
								iPos = sALower.Find("<hn:");
								if (iPos >= 0)
								{
									sReceive = sReceive.Mid(iPos+4);
									iPos = sReceive.Find(">");
									if (iPos >= 0)
									{
										sReceive = sReceive.Left(iPos);
										if (sReceive.GetLength() > 0)
										{
											m_pInfo->m_sFoundHostName = sReceive;
										}
									}
								}
								m_iBoincTThrottle += BIT_TRUE_TTHROTTLE;
							}
						}
					}
					m_pRpcClient->Disconnect();
					m_pRpcClient->ClearAddress();


					if (bBoinc)
					{
						// now try to connect to the BOINC client
						m_pSearchBoinc = (CThreadRpcSearch *) AfxBeginThread(RUNTIME_CLASS(CThreadRpcSearch));
						m_pInfo->m_iPort = m_iPort;
						m_pSearchBoinc->PostThreadMessageA(UWM_MSG_THREAD_CONNECT_RPC,this->m_nThreadID, (LPARAM) m_pInfo);
					}
					else
					{
						::PostMessage(hWnd, UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT_READY, (WPARAM) m_pInfo, (LPARAM) m_iBoincTThrottle);
						if (m_pInfo->m_bAutoExit)
						{
							m_bExit = true;
						}
					}
//					delete pcPort;
//					pcPort = NULL;
				}
				if (iMsg == UWM_MSG_THREAD_CONNECT_RPC_READY)
				{
					int iConnectionStatus;
					iConnectionStatus = (int) msg.wParam;
					m_pInfo->m_iConnectionStatus = iConnectionStatus;
					if (iConnectionStatus == RPC_STATUS_CONNECTED)
					{
						if (m_pInfo->m_sFoundHostName.length() == 0)
						{
							m_pInfo->m_sFoundHostName = m_pInfo->m_sHostID;
						}
					}
					::PostMessage(hWnd,UWM_MSG_THREAD_CHECK_TB_RPC_CLIENT_READY, (WPARAM) m_pInfo, (LPARAM) m_iBoincTThrottle);
					if (m_pInfo->m_bAutoExit)
					{
						m_bExit = true;
					}
				}
				if (iMsg == UWM_MSG_THREAD_IP_WITH_MAC)
				{
					m_sHostName = *(std::string*) msg.lParam;
				}
			}

		}
		Sleep(1);
	}

	m_pRpcClient->Disconnect();
	m_pRpcClient->ClearAddress();

	m_pRpcClient->Cleanup();

	CString *psId;
	psId = new CString;
	*psId = "TThrottle, ";
	*psId+= m_sComputerId.c_str();
	::PostMessage(hWnd,UWM_MSG_THREAD_QUIT_READY,0, (LPARAM) psId);

	Sleep(500);		// time needed for the threads to handle the post messages. Otherwise we get memory leaks
	return FALSE;
}

int CThreadRpcClient::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

int CThreadRpcClient::SendReceive(HWND *phWnd, CRpcInfoClient *pInfoClient, bool bSet)
{
	MSG		msg;
	bool	bReceived;
	bool	bConnected;
	CString sMessage, sFormat;
	char	cOutput[1024*32];
	char	cBuffer[32];
	CStringA sAKey;
	CBlowFish bf;

	if (bSet)
	{
		sMessage = "<BT_SET>";
//		char cOutput[8];
		int iCheck;
		if (pInfoClient->m_iActivive) iCheck = 1;
		else iCheck = 0;
		sFormat.Format("<AA%d>",iCheck);
		sMessage += sFormat;
		sFormat.Format("<SC%d>",pInfoClient->m_iSetCore);
		sMessage += sFormat;
		sFormat.Format("<SG%d>",pInfoClient->m_iSetGpu);
		sMessage += sFormat;
		sFormat.Format("<XC%d>",pInfoClient->m_iMaxCpu);
		sMessage += sFormat;
		sFormat.Format("<MC%d>",pInfoClient->m_iMinCpu);
		sMessage += sFormat;
		GenerateRandomString((char *) cOutput, 7);
		sMessage += "<RS";
		sMessage += (char *) cOutput;
		sMessage += ">";
	}
	else
	{
		sMessage = "<BT>";
	}

	if (m_sKey.GetLength() > 0)
	{
		bf.Encrypt(sMessage.GetBuffer(), cOutput, m_sKey.GetBuffer(), 1024*32); sMessage.ReleaseBuffer();m_sKey.ReleaseBuffer();
		sMessage = cOutput;
	}

	m_pRpcClient->m_sComputerId = m_sComputerId;
	bReceived = false;
	if (m_iPort < 2) m_iPort = 31416 + 1;

	_snprintf_s(cBuffer, 31,_TRUNCATE,"%d", m_iPort);
	m_sPort = cBuffer;
	bConnected = m_pRpcClient->Connect((char *) m_sHostName.c_str(), (char *) m_sPort.c_str(), 0, false);
	if (bConnected)	bReceived = m_pRpcClient->SendReceive(sMessage.GetBuffer());
	else bReceived = false;
	m_pRpcClient->Disconnect();		//$#$#$#$#$#$#$#$#$
	m_pRpcClient->ClearAddress();
//	m_pRpcClient->Cleanup();
	int iStatus;
	if (bReceived)
	{
		char *pcReceiveBuffer;
		if (m_sKey.GetLength() > 0)
		{
			bf.Decrypt(m_pRpcClient->m_pcReceiveBuffer, cOutput, m_sKey.GetBuffer(), 1024*32); m_sKey.ReleaseBuffer();
			pcReceiveBuffer = (char *) cOutput;
		}
		else pcReceiveBuffer = m_pRpcClient->m_pcReceiveBuffer;

		iStatus = ReadMessage(pcReceiveBuffer, pInfoClient);

		if (theApp.m_pDlgLogging->m_bLogDebugTThrottleData)
		{
			char cBufferFrom[255];
//			pcBufferFrom = new char [255];
			strcpy_s(cBufferFrom,254,"Receive TThrottle: ");
			strcat_s(cBufferFrom, 254,m_sHostName.c_str());
			strcat_s(cBufferFrom, 254,", ");
			strcat_s(cBufferFrom, 254,m_sPort.c_str());
			SendLogging(cBufferFrom, pcReceiveBuffer, LOGGING_DEBUG);
//			delete pcBufferFrom;
//			pcBufferFrom = NULL;
		}
	}
	else
	{
//		m_pRpcClient->Disconnect();									// no receive disconnect and connect next time
		iStatus = -1;
	}

	while (::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))			// flush the message buffer
	{
		if (msg.message == UWM_MSG_THREAD_QUIT)
		{
			*phWnd = (HWND) msg.wParam;
			m_bExit = true;		// only capture quit
		}
		Sleep(1);
	}

	return iStatus;
}
	
int CThreadRpcClient::ReadMessage(char *pcBuffer, CRpcInfoClient *pInfoClient)
{
	int		iCount, iNumber;
	char	*pcFound;
	double	fNumber, fActive;
	char	cSearch[4];

	pInfoClient->m_fCpuThrottle = -1;
	pInfoClient->m_fGpuThrottle = -1;
	pInfoClient->m_fTopCpuTemperature = -1;
	pInfoClient->m_fTopGpuTemperature = -1;
	pInfoClient->m_fVersion = -1;
	pInfoClient->m_iNrNVIDIA = 0;
	pInfoClient->m_iNrATI = 0;

	pInfoClient->m_sTxt		= "??";
	pInfoClient->m_iSetCore = -1;
	pInfoClient->m_iSetGpu = -1;
	pInfoClient->m_iMinCpu = -1;
	pInfoClient->m_iMaxCpu = -1;
	pInfoClient->m_iActivive = -1;

	if (strstr(pcBuffer, "<BT_SET>"))
	{
		pInfoClient->m_bReady = false;
		return 0;
	}
	pInfoClient->m_bReady = true;
	

	for (iCount = 0; iCount < MAX_GPU; iCount++)
	{
		pInfoClient->m_fGpuTemperature[iCount] = -1;
	}

	*(pcBuffer+DEFAULT_RECV_BUFFER_SIZE) = 0;	// extra end marker

	pcFound = strstr(pcBuffer, "<TThrottle>");
	if (pcFound == NULL) return -1;
	pcBuffer += 11;

	if (!FindNumber(&pcBuffer, "<PV", &pInfoClient->m_fVersion))  return -1;
	if (!FindNumber(&pcBuffer, "<AC", &fActive))  return -1;
	if (!FindNumber(&pcBuffer, "<TC", &fNumber))  return -1;
	if (!FindNumber(&pcBuffer, "<TG", &fNumber))  return -1;

	// Number of GPU's
	fNumber = 0;
	FindNumber(&pcBuffer, "<NV", &fNumber);
	pInfoClient->m_iNrNVIDIA = (int) fNumber;
	fNumber = 0;
	FindNumber(&pcBuffer, "<NA", &fNumber);
	pInfoClient->m_iNrATI = (int) fNumber;

	if (fActive == 0) // off
	{
		pInfoClient->m_fCpuThrottle = 100;
		pInfoClient->m_fGpuThrottle = 100;
	}
	else
	{
		if (!FindNumber(&pcBuffer, "<DC", &pInfoClient->m_fCpuThrottle))  return -1;
		if (!FindNumber(&pcBuffer, "<DG", &pInfoClient->m_fGpuThrottle))  return -1;
	}

	// find highest number CPU
	cSearch[0] = 'C';
	cSearch[1] = 'T';	// cpu core
	cSearch[3] = 0;
	for (iCount = 0; iCount < MAX_CPU; iCount++)
	{
		cSearch[2] = '0' + iCount;
		if (!FindNumber(&pcBuffer, &cSearch[0], &fNumber)) break;	// no more CPU's
		if (fNumber > pInfoClient->m_fTopCpuTemperature) pInfoClient->m_fTopCpuTemperature = fNumber;
	}
	// find highest number GPU
	cSearch[0] = 'G';
	cSearch[1] = 'T';	// cpu core
	cSearch[3] = 0;
	for (iCount = 0; iCount < MAX_GPU; iCount++)
	{
		cSearch[2] = '0' + iCount;
		if (!FindNumber(&pcBuffer, &cSearch[0], &fNumber)) break;	// no more GPU's
		if (fNumber > pInfoClient->m_fTopGpuTemperature) pInfoClient->m_fTopGpuTemperature = fNumber;
		pInfoClient->m_fGpuTemperature[iCount] = fNumber;
	}

	iNumber = -1;
	FindNumber(&pcBuffer, "<AA", &iNumber);
	pInfoClient->m_iActivive = iNumber;

	iNumber = -1;
	FindNumber(&pcBuffer, "<SC", &iNumber);
	pInfoClient->m_iSetCore = iNumber;

	iNumber = -1;
	FindNumber(&pcBuffer, "<SG", &iNumber);
	pInfoClient->m_iSetGpu = iNumber;

	iNumber = -1;
	FindNumber(&pcBuffer, "<XC", &iNumber);
	pInfoClient->m_iMaxCpu = iNumber;

	iNumber = -1;
	FindNumber(&pcBuffer, "<MC", &iNumber);
	pInfoClient->m_iMinCpu = iNumber;

	FindString(&pcBuffer, "<TX", &pInfoClient->m_sTxt);

	if (theApp.m_pDlgLogging->m_bLogDebugTThrottleRead)
	{
		char *pcBufferFrom, *pcBufferLog, cBufferFormat[128];
		pcBufferFrom = new char [128];
		pcBufferLog = new char [1024];
		strcpy_s(pcBufferFrom,127, "processed TThrottle: ");
		strcat_s(pcBufferFrom,127, m_sHostName.c_str());
		strcat_s(pcBufferFrom,127, ", ");
		strcat_s(pcBufferFrom,127,m_sPort.c_str());

		strcpy_s(pcBufferLog,1023, "");

		_snprintf_s(cBufferFormat,127,_TRUNCATE,"NV: %d, NA: %d, CpuMax: %.1f, GpuMax: %.1f ", pInfoClient->m_iNrNVIDIA, pInfoClient->m_iNrATI, pInfoClient->m_fTopCpuTemperature, pInfoClient->m_fTopGpuTemperature);
		strcat_s(pcBufferLog,1023,cBufferFormat);

		for (iCount = 0; iCount < MAX_GPU; iCount++)
		{
			_snprintf_s(cBufferFormat,127,_TRUNCATE,"Gpu%d: %.1f ", iCount, pInfoClient->m_fGpuTemperature[iCount]);
			strcat_s(pcBufferLog,1023,cBufferFormat);
		}
		SendLogging(pcBufferFrom, pcBufferLog, LOGGING_DEBUG);
		delete pcBufferFrom;
		delete pcBufferLog;
	}
	
// <TThrottle><HN: hostname>	header
// <CT%d %.1f>" Cpu count, temperature
// <GT%d %.1f>" Cpu count  temperature
// <AA%d>		Auto active
// <SC%d>		Set Core
// <SG%d>		Set Gpu
// <XC%d>		Max Cpu
// <MC%d>		Min Cpu
// <TX%s>		Program list
// <RS%s>		Random string

	return 0;
}

bool CThreadRpcClient::FindNumber(char **pcBuffer, char *pcCompare, double *pfNumber) 
{
	int		iLen;
	char	*pFound;

	pFound = strstr(*pcBuffer, pcCompare);
	if (pFound != NULL)
	{
		*pcBuffer = pFound;
		iLen = (int) strlen(pcCompare);
		*pcBuffer += iLen;
		*pfNumber = (float) atof(*pcBuffer);
		return true;
	}
	else return false;
}

bool CThreadRpcClient::FindNumber(char **pcBuffer, char *pcCompare, int *piNumber) 
{
	int		iLen;
	char	*pFound;

	pFound = strstr(*pcBuffer, pcCompare);
	if (pFound != NULL)
	{
		*pcBuffer = pFound;
		iLen = (int) strlen(pcCompare);
		*pcBuffer += iLen;
		*piNumber =  atoi(*pcBuffer);
		return true;
	}
	else return false;
}

bool CThreadRpcClient::FindString(char **pcBuffer, char *pcCompare, CString *psTxt) 
{
	int		iLen;
	char	*pFound, *pFoundEnd;

	pFound = strstr(*pcBuffer, pcCompare);
	if (pFound != NULL)
	{
		*pcBuffer = pFound;
		iLen = (int) strlen(pcCompare);
		*pcBuffer += iLen;
		pFoundEnd = strstr(*pcBuffer, ">");
		if (pFound != NULL)
		{
			*psTxt = *pcBuffer;
			iLen = (int) (pFoundEnd - pFound);
			*psTxt = psTxt->Mid(0, iLen-3);
			return true;
		}
	}

	return false;
}

#define ERROR_READ_GRAPHIC_INCORRECT_ID	-1
#define ERROR_READ_GRAPHIC_VERSION		-2
#define ERROR_READ_GRAPHIC_STOP1		-3
#define ERROR_READ_GRAPHIC_CPU_OUTRANGE	-4
#define ERROR_READ_GRAPHIC_GPU_OUTRANGE	-5
#define ERROR_READ_GRAPHIC_NO_TEMP		-6
#define ERROR_READ_GRAPHIC_IMPOSSIBLE	-7


int CThreadRpcClient::ReadGraphic(BYTE *pcBuffer, UINT *piBufferReturn, int *piItemCount, char *pcError, time_t *ptLastAddTime)
{
	char	*pIdString = "TThrottle_From_To";
	char	*pFound;
	char	cBufferStop[3],cBufferTime[64];
	time_t	tTThrottle;
	struct	tm   tmTThrottle;
	int		iCount;
	int		iCpuCount, iGpuCount, iCpuMaxTemperature, iGpuMaxTemperature, iItems, iItemsRead, iVersion, iVersionTotal, iVersionBufferLen;
	UINT	iTemperature[V3_TEMPERATURE_NR_OF_TOTAL][V2_TEMPERATURE_BUFFER_LEN];	// longest buffer
	UINT	*piBufferReturnTemp;
	bool	bDebug;


	bDebug = false;
	if (pcError != NULL)
	{
		bDebug = true;
		*pcError = 0;
	}

	*piItemCount = 0;

	cBufferStop[0] = 1;
	cBufferStop[1] = 0;	

	pFound = strstr((char *) pcBuffer, pIdString);
	if (pFound == NULL)
	{
		if (bDebug)
		{
			_snprintf_s(pcError,127,_TRUNCATE,"ERROR: ID, %d", ERROR_READ_GRAPHIC_INCORRECT_ID);
		}
		return ERROR_READ_GRAPHIC_INCORRECT_ID;
	}

	pcBuffer = (BYTE *) (pFound + strlen(pIdString));
	iVersion = (*pcBuffer) - '0';

	if (iVersion < 2 || iVersion > 3)					//version 2 or 3 (5.8)
	{
		if (bDebug)
		{
			_snprintf_s(pcError,127,_TRUNCATE,"ERROR: Version: %X, E: %d", *pcBuffer, ERROR_READ_GRAPHIC_VERSION);
		}
		return ERROR_READ_GRAPHIC_VERSION;
	}
	pcBuffer++;

	tTThrottle = atoi((char *) pcBuffer);

	if (bDebug)
	{
		localtime_s(&tmTThrottle, &tTThrottle);
		strftime(cBufferTime, 40, "%d-%m-%Y %H:%M:%S", &tmTThrottle);
	}


	pFound = strstr((char *) pcBuffer, cBufferStop);
	if (pFound == NULL)
	{
		if (bDebug)
		{
			_snprintf_s(pcError,127,_TRUNCATE,"ERROR: Stop, E: %d, T: ", ERROR_READ_GRAPHIC_STOP1);
			strcat_s(pcError, 127, cBufferTime);
		}
		return ERROR_READ_GRAPHIC_STOP1;
	}
	pcBuffer = (BYTE *) (pFound + 1);

	iCpuCount = (*pcBuffer) -1;
	pcBuffer++;
	iGpuCount = (*pcBuffer) -1;
	pcBuffer++;

	iCpuMaxTemperature = (*pcBuffer) -1;
	pcBuffer++;
	iGpuMaxTemperature = (*pcBuffer) -1;
	pcBuffer++;

	if (iVersion == 3)
	{
		iVersionTotal = V3_TEMPERATURE_NR_OF_TOTAL;
		iVersionBufferLen = V3_TEMPERATURE_BUFFER_LEN;
	}
	else
	{
		iVersionTotal = V3_TEMPERATURE_NR_OF_TOTAL;
		iVersionBufferLen = V2_TEMPERATURE_BUFFER_LEN;
	}

	for (iCount = 0; iCount < V3_TEMPERATURE_NR_OF_TOTAL; iCount++)
	{
		iTemperature[iCount][0] = STOP_TEMPERATURE;
	}

	iItems = 0;
	for (iCount = 0; iCount < iVersionTotal; iCount++)
	{
//		if (iVersion == 1)
//		{
//			if (iCount == CORE08)			// V 1 only 8 cores
//			{
//				iCount = GPU0;
//			}
//			if (iCount == GPU6)				// V1 only 6 Gpu's
//			{
//				iCount = THROTTLE_CPU;
//			}
//			if (iCount == LIMIT_CPU)		// V1 no limit Cpu
//			{
//				for (int iLimitCount = 0; iLimitCount < V1_TEMPERATURE_BUFFER_LEN; iLimitCount++)
//				{
//					iTemperature[LIMIT_CPU][iLimitCount] = iCpuMaxTemperature*10;
//					iTemperature[LIMIT_GPU][iLimitCount] = iGpuMaxTemperature*10;
//				}
//				break;
//			}
//		}
		if (iVersion == 2)
		{
			if (iCount == CORE12)			// V 2 only 12 cores
			{
				iCount = GPU0;
			}
		}

		if (!FindTemperature((BYTE **) &pcBuffer, iTemperature[iCount], &iItemsRead, iVersionBufferLen))
		{
			if (bDebug)
			{
				_snprintf_s(pcError,127,_TRUNCATE,"ERROR: Pos: %d, Items: %d, Read: %d, E: %d, T: ", iCount, iItems, iItemsRead, ERROR_READ_GRAPHIC_NO_TEMP);
				strcat_s(pcError, 127, cBufferTime);
			}
			return ERROR_READ_GRAPHIC_NO_TEMP;
		}
		if (iItems != iItemsRead)
		{
			if (iItems != 0 && iItemsRead != 0)
			{
				if (bDebug)
				{
					_snprintf_s(pcError,127,_TRUNCATE,"ERROR: Pos: %d, Items: %d, Read: %d, E: %d, T: ", iCount, iItems, iItemsRead, ERROR_READ_GRAPHIC_IMPOSSIBLE);
					strcat_s(pcError, 127, cBufferTime);
				}
				return ERROR_READ_GRAPHIC_IMPOSSIBLE;	// impossible unequal number of temperatures.
			}
			if (iItemsRead != 0)
			{
				iItems = iItemsRead;
			}
		}
	}

//	*piTemperature = STOP_TEMPERATURE


	*ptLastAddTime = tTThrottle + iItems;

	*piBufferReturn = (int) iVersion;
	piBufferReturn++;
	*piBufferReturn = (int) ptLastAddTime;
	piBufferReturn++;
	*piBufferReturn = iCpuMaxTemperature;
	piBufferReturn++;
	*piBufferReturn = iGpuMaxTemperature;
	piBufferReturn++;

	piBufferReturnTemp = piBufferReturn;
	for (iCount = 0; iCount < iVersionTotal; iCount++)
	{
		piBufferReturnTemp = piBufferReturn;
		for (int iItemCount = 0; iItemCount < iItems; iItemCount++)
		{
			*piBufferReturnTemp = iTemperature[iCount][iItemCount];
			piBufferReturnTemp++;
		}
		piBufferReturn += iVersionBufferLen;
	}

	*piItemCount = iItems;

	if (bDebug)
	{
		_snprintf_s(pcError,127,_TRUNCATE,"OK: Items: %d, T: ", iItems);
		strcat_s(pcError, 127, cBufferTime);
	}

	return 0;

}

bool CThreadRpcClient::FindTemperature(BYTE **pcBuffer, UINT *piTemperature, int *piItems, int iVersionBufferLen) 
{
	BYTE	cHigh, cLow;
	UINT	iItems, iTemperature;

	iItems = 0;

	*piTemperature = STOP_TEMPERATURE;

	while (iItems <= (UINT) iVersionBufferLen)
	{
		cLow = **pcBuffer;
		if (cLow == 0)
		{
			return false;
		}
		(*pcBuffer) += 1;
		if (cLow == 1)
		{
			*piItems = iItems;
			return true;		// marker
		}
		cHigh = **pcBuffer;
		if (cHigh == 0)
		{
			return false;
		}
		(*pcBuffer) += 1;
		cHigh -= 2;
		iTemperature = cLow;
		iTemperature += (cHigh << 8);
		iTemperature -= 2;
		iTemperature = iTemperature >> 2;
		*piTemperature = iTemperature;
//		(*pfTemperature) /= 10;
		piTemperature++;
		iItems++;
	}
	return false;
}

void CThreadRpcClient::SendLogging(char *pcFrom, char *pcMessage, int iDebug)
{
	CLoggingMessage log;
//	psLog = new CLoggingMessage;

	log.m_pFrom =  pcFrom; log.m_pText = pcMessage;
	theApp.m_pDlgLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) &log, iDebug);

//	delete psLog;
}

BEGIN_MESSAGE_MAP(CThreadRpcClient, CWinThread)
END_MESSAGE_MAP()


// CThreadRpcClient message handlers
