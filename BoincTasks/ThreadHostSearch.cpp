// HostSearch.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ThreadHostSearch.h"

#include "winsock2.h"
#include <ws2tcpip.h>
#include "Wspiapi.h"
#include "Npapi.h"

CHostSearchInfo::CHostSearchInfo(void)
{
	Clear();
}


CHostSearchInfo::~CHostSearchInfo(void)
{
	if (m_pcHostName != NULL)		delete m_pcHostName;
	if (m_pcHostNameFull != NULL)	delete m_pcHostNameFull;
}

void CHostSearchInfo::Clear()
{
	m_pcHostName = NULL;
	m_pcHostNameFull = NULL;
}

void CHostSearchInfo::AddHostName(char *pcHostName)
{
	Add(&m_pcHostName, pcHostName);
}
void CHostSearchInfo::AddHostNameFull(char *pcHostNameFull)
{
	Add(&m_pcHostNameFull, pcHostNameFull);
}

void CHostSearchInfo::Add(char **pcDest, char *pcSource)
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




// CHostSearch

IMPLEMENT_DYNCREATE(CThreadHostSearch, CWinThread)

CThreadHostSearch::CThreadHostSearch()
{
}

CThreadHostSearch::~CThreadHostSearch()
{
}

BOOL CThreadHostSearch::InitInstance()
{
	HWND hWnd;
	CHostSearchInfo	 *pInfo;

	CString strTemp, sHostName;
	struct hostent *host;
	struct in_addr *ptr;	// To retrieve the IP Address
	char szHostName[MAX_PATH];

	CWaitCursor wc;
    LPNETRESOURCE lpnr = NULL;
    DWORD dwResult, dwResultEnum;
    HANDLE hEnum;
    DWORD cbBuffer = 16384;     // 16K is a good size
    DWORD cEntries = -1;        // enumerate all possible entries
    LPNETRESOURCE lpnrLocal;    // pointer to enumerated structures
    DWORD i;

	m_bExit = false;

	while (!m_bExit)
	{
		MSG msg;
		if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if (msg.message == UWM_MSG_THREAD_QUIT)
			{
				hWnd = (HWND) msg.wParam;
				m_bExit = true;
			}
			if (msg.message == UWM_MSG_THREAD_HOSTSEARCH)
			{
				hWnd = (HWND) msg.wParam;
				pInfo = (CHostSearchInfo *) msg.lParam;
/////////////////////////
			    //
				// Call the WNetOpenEnum function to begin the enumeration.
				//
				dwResult = WNetOpenEnum(RESOURCE_CONTEXT, 
						                RESOURCETYPE_ANY,   // all resources
								        0,  // enumerate all resources
										lpnr,       // NULL first time the function is called
						                &hEnum);    // handle to the resource

				if (dwResult != NO_ERROR)
				{
					::PostMessage(hWnd,UWM_MSG_THREAD_HOSTSEARCH_QUIT,0,0);
					return FALSE;
				}
				//
				// Call the GlobalAlloc function to allocate resources.
				//
				lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);
				if (lpnrLocal == NULL)
				{
					::PostMessage(hWnd,UWM_MSG_THREAD_HOSTSEARCH_QUIT,0,0);
					return FALSE;
				}
 
				do {
					//
				    // Initialize the buffer.
				    //
				    ZeroMemory(lpnrLocal, cbBuffer);
					//
					// Call the WNetEnumResource function to continue
					//  the enumeration.
					//
					dwResultEnum = WNetEnumResource(hEnum,  // resource handle
						                    &cEntries,      // defined locally as -1
							                lpnrLocal,      // LPNETRESOURCE
								            &cbBuffer);     // buffer size
					//
					// If the call succeeds, loop through the structures.
					//
					if (dwResultEnum == NO_ERROR)
					{
						for (i = 0; i < cEntries; i++)
						{
							// Call an application-defined function to
							//  display the contents of the NETRESOURCE structures.
							//
//							DisplayStruct(i, &lpnrLocal[i]);

							// If the NETRESOURCE structure represents a container resource, 
							//  call the EnumerateFunc function recursively.

							if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER))
							{
								if ( lpnrLocal[i].lpRemoteName )
								{
									CString strFullName = lpnrLocal[i].lpRemoteName;
									CString sFormat;
									if ( 0 == strFullName.Left(2).Compare("\\\\") )	strFullName = strFullName.Right(strFullName.GetLength()-2);
									gethostname( szHostName, MAX_PATH );
									sHostName = szHostName;
									host = gethostbyname(strFullName);
									if(host == NULL)
									{
										int iError = WSAGetLastError();

									    struct addrinfo *result = NULL;
										struct addrinfo *ptr = NULL;
										struct addrinfo hints;
										DWORD dwRetval;

										//--------------------------------
										// Setup the hints address info structure
										// which is passed to the getaddrinfo() function
										ZeroMemory( &hints, sizeof(hints) );
										hints.ai_family = AF_UNSPEC;
										hints.ai_socktype = SOCK_STREAM;
										hints.ai_protocol = IPPROTO_TCP;

										//--------------------------------
										// Call getaddrinfo(). If the call succeeds,
										// the result variable will hold a linked list
										// of addrinfo structures containing response
										// information
										dwRetval = getaddrinfo(strFullName, NULL, &hints, &result);
										if ( dwRetval != 0 )
										{
											sFormat = "";							
										}
										else
										{
											char *pString;
											// Eg. 211.40.35.76 split up like this.
											pString = (char*) result->ai_addr[0].sa_data;
											BYTE a = *(pString+2);
											BYTE b = *(pString+3);
											BYTE c = *(pString+4);
											BYTE d = *(pString+5);

											sFormat.Format("%d.%d.%d.%d",a,b,c,d);
										}
										freeaddrinfo(result);

									}
									else
									{
										ptr = (struct in_addr *) host->h_addr_list[0];

										// Eg. 211.40.35.76 split up like this.
										int a = ptr->S_un.S_un_b.s_b1;  // 211
										int b = ptr->S_un.S_un_b.s_b2;  // 40
										int c = ptr->S_un.S_un_b.s_b3;  // 35
										int d = ptr->S_un.S_un_b.s_b4;  // 76

										sFormat.Format("%d.%d.%d.%d",a,b,c,d);
									}

									bool bWait = true;
									strFullName.MakeLower();
									sHostName.MakeLower();
									if (sHostName == strFullName)
									{
										//strFullName += " (localhost)";
										bWait = false;		// skip localhost
									}
									else
									{
										pInfo->AddHostName(sFormat.GetBuffer());sFormat.ReleaseBuffer();
										pInfo->AddHostNameFull(strFullName.GetBuffer()); strFullName.ReleaseBuffer();
										::PostMessage(hWnd,UWM_MSG_THREAD_HOSTSEARCH_READY,(WPARAM) pInfo,0);
									}

									while (bWait)
									{
										if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
										{
											if (msg.message == UWM_MSG_THREAD_QUIT)
											{
												hWnd = (HWND) msg.wParam;
												m_bExit = true;
												bWait = false;
											}
											if (msg.message == UWM_MSG_THREAD_HOSTSEARCH)
											{
												hWnd = (HWND) msg.wParam;
												pInfo = (CHostSearchInfo *) msg.lParam;
												bWait = false;
											}
										}
										Sleep(100);
									}
								}
							}
							if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
							{
								if (msg.message == UWM_MSG_THREAD_QUIT)
								{
									hWnd = (HWND) msg.wParam;
									m_bExit = true;
								}
							}

							if (m_bExit) break;
						} // 	for (i = 0; i < cEntries; i++)
					 } // if (dwResultEnum == NO_ERROR)
					// Process errors.
					//
					else
					{
						if (dwResultEnum != ERROR_NO_MORE_ITEMS)
						{
							printf("WNetEnumResource failed with error %d\n", dwResultEnum);
						}
						break;
					}
					if (m_bExit) break;
				}	
				while (dwResultEnum != ERROR_NO_MORE_ITEMS); // Do
				//
				// Call the GlobalFree function to free the memory.
				//
				GlobalFree((HGLOBAL) lpnrLocal);
				//
				// Call WNetCloseEnum to end the enumeration.
				//
				dwResult = WNetCloseEnum(hEnum);
				m_bExit = true;
/////////////////////////
			} // if (msg.message == UWM_MSG_THREAD_HOSTSEARCH)
		} // if ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		Sleep(100);
	} // while (!m_bExit)	

	::PostMessage(hWnd,UWM_MSG_THREAD_HOSTSEARCH_QUIT,0,0);

	return FALSE;
}

int CThreadHostSearch::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadHostSearch, CWinThread)
END_MESSAGE_MAP()


// CHostSearch message handlers
