// RemoteTransferView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "threadrpc.h"
//#include "ComputerDoc.h"
#include "ListViewEx.h"					// base class
#include "TemplateRemoteTransferView.h" // base class
#include "RemoteCombinedDoc.h"
#include "DlgSettingsView.h"
#include "DlgSettingsTransfer.h"
#include "DlgPrefMain.h"
#include "SortColumn.h"
#include "Selection.h"
#include "Translation.h"

static int _gnRemoteTransferColumnWidth[NUM_REMOTE_TRANSFER_COLUMNS] =
{
	100,			200,			100,			120,			160,				120,			200,	100
};

// CTemplateRemoteTransferView

IMPLEMENT_DYNCREATE(CTemplateRemoteTransferView, CListViewEx)

CTemplateRemoteTransferView::CTemplateRemoteTransferView()
{
	this->m_bSelection = FALSE;	// disable default selection

	m_dataDummy.m_color = RGB(255,255,255);
	m_dataDummy.m_cSortPrimary = "";
	m_dataDummy.m_cSortSecondary = "";
	m_dataDummy.m_cSortTertiary = "";

	m_pSortColumn = new CSortColumn;
	m_pSelection = new CSelection;

	m_iWaitingTurner = 0;

	m_bDraged = false;
}

CTemplateRemoteTransferView::~CTemplateRemoteTransferView()
{
	for (int iCount = 0; iCount < (int) m_lListData.size(); iCount++)
	{
		delete m_lListData.at(iCount);
	}
//	m_lListData.clear();

	delete m_pSortColumn;
	delete m_pSelection;
}

void CTemplateRemoteTransferView::OnInitialUpdate()
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetExtendedStyle (listCtrl.GetExtendedStyle()|LVS_EX_HEADERDRAGDROP);
	AddAllColumns();

	CListViewEx::OnInitialUpdate();
}

void CTemplateRemoteTransferView::AddAllColumns(bool bAdd)
{
	int			iCount, iColumn, iWidth, iFormat, iNr;
	int			iColumnHeader[NUM_REMOTE_TRANSFER_COLUMNS+1];
	LV_COLUMN	lvc;
//	CListData	**pData;

	// read back the column sequence

	if (bAdd)
	{
		m_iNrColomns = (int) ::SendMessage(theApp.m_pDlgSettingsTransfer->m_hWnd, UWM_MSG_DLG_SETTINGS_TRANSFERS_GET, (WPARAM) &m_iColumnOrder[0],(LPARAM) &iColumnHeader[0]);
	}
	else
	{	
		for (iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
		{
			iColumnHeader[iCount] = -1;
		}

		for (iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
		{
			iNr = m_iColumnOrder[iCount];
			if (iNr > NUM_REMOTE_TRANSFER_COLUMNS) break;
			if (iNr >=0) iColumnHeader[iNr] = iCount;
		}
		::SendMessage(theApp.m_pDlgSettingsTransfer->m_hWnd, UWM_MSG_DLG_SETTINGS_TRANSFERS_SET, (WPARAM) &m_iColumnOrder[0],(LPARAM) &iColumnHeader[0]);
	}

	CListCtrl& listCtrl = GetListCtrl();
	// build columns
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(iCount = 0; iCount<m_iNrColomns; iCount++)
	{
		iColumn = iColumnHeader[iCount];
		if (iColumn >= 0)
		{
			lvc.iSubItem = iCount;
			lvc.pszText = gszTranslation[PosGroupViewTasksTransfer+iColumn];
			iWidth = ReadColumnWidth(iColumn, _gnRemoteTransferColumnWidth[iColumn]);
			m_iColumnWidth[iColumn] = iWidth;
			lvc.cx = m_iColumnWidth[iColumn];

			switch(iColumn)
			{
				case COLUMN_TRANSFER_SIZE:
				case COLUMN_TRANSFER_ELAPSED:
				case COLUMN_TRANSFER_SPEED:
					iFormat = LVCFMT_RIGHT;
				break;
				case COLUMN_TRANSFER_PROGRESS:
					iFormat = LVCFMT_CENTER;
				break;
				default:
					iFormat = LVCFMT_LEFT;	//COLUMN_TRANSFER_PROJECT COLUMN_TRANSFER_FILE COLUMN_TRANSFER_STATUS COLUMN_TRANSFER_COMPUTER
			}

			lvc.fmt = iFormat;
			if (bAdd)
			{
				listCtrl.InsertColumn(iCount,&lvc);
			}
			else
			{
				listCtrl.SetColumn(iCount, &lvc);
			}

		}
	}
	LPINT pnOrder = (LPINT) malloc(m_iNrColomns*sizeof(int));
	if (pnOrder != NULL)
	{
		for (iCount = 0; iCount < m_iNrColomns; iCount++)
		{
			pnOrder[iCount] = iCount; 
		}
		listCtrl.SetColumnOrderArray(m_iNrColomns,pnOrder);
		free(pnOrder);
	}

	m_pSortColumn->SetColumnOrder(m_iNrColomns, &iColumnHeader[0]);
	m_pSortColumn->Setup(&listCtrl,this->m_hWnd, m_iNrColomns, registrySortTransferPrimary, registrySortTransferSecondary, registrySortTransferTertiary ,262,263, 263);
	m_pSelection->Setup(&listCtrl, &m_lListData, COLUMN_TRANSFER_FILE, COLUMN_TRANSFER_COMPUTER, NUM_REMOTE_TRANSFER_COLUMNS, &m_iColumnOrder[0]);
}

void CTemplateRemoteTransferView::ReadAllColumnWidth()
{
	CListCtrl& listCtrl = GetListCtrl();
	int iWidth;

	for(int iCount = 0; iCount< NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
	{
		iWidth = ReadColumnWidth(iCount, _gnRemoteTransferColumnWidth[iCount]);
		listCtrl.SetColumnWidth(m_iColumnOrder[iCount], iWidth);
		m_iColumnWidth[m_iColumnOrder[iCount]] = iWidth;
	}
}
int CTemplateRemoteTransferView::ReadColumnWidth(int iColumn, int iDefault)
{
	CString	sRegistry;
	char	cChar;
	int		iWidth;

	sRegistry = registryColumnTransfers;
	cChar = iColumn + 'a';
	sRegistry = sRegistry + "_" + cChar; 

	iWidth =  theApp.GetProfileInt(registrySectionColumn, sRegistry, iDefault);
//	if (iWidth < 20) return iDefault;
	return iWidth;
}

void CTemplateRemoteTransferView::CheckAndSaveColumnsWidth()
{
	int		iWidth, iColumn;
	bool	bChanged = false;
	char	cChar;
	CString sTxt;

	CListCtrl& listCtrl = GetListCtrl();

	for (int iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
	{
		iColumn = m_iColumnOrder[iCount];
		if (iColumn >= 0)
		{
			iWidth = listCtrl.GetColumnWidth(iColumn);
			if (m_iColumnWidth[iColumn] != listCtrl.GetColumnWidth(iColumn))
			{
				m_iColumnWidth[iColumn] = iWidth;
				bChanged = true;
				sTxt = registryColumnTransfers;
				cChar = iCount + 'a';
				sTxt = sTxt + "_" + cChar; 
				// update new width in registry
				theApp.WriteProfileInt(registrySectionColumn, sTxt, iWidth);
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CTemplateRemoteTransferView, CListViewEx)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()

	ON_NOTIFY(HDN_ITEMCLICK, 0, OnItemclickList)
    ON_NOTIFY_EX(HDN_ENDDRAG, 0, OnHeaderEndDrag)
	ON_NOTIFY_EX(HDN_ITEMCHANGED, 0, OnHeaderChanged) 
//	ON_NOTIFY_EX(HDN_ITEMCLICK, 0, OnHeaderClick) 
END_MESSAGE_MAP()

// CTemplateRemoteTransferView diagnostics

#ifdef _DEBUG
void CTemplateRemoteTransferView::AssertValid() const
{
	CListViewEx::AssertValid();
}

#ifndef _WIN32_WCE
void CTemplateRemoteTransferView::Dump(CDumpContext& dc) const
{
	CListViewEx::Dump(dc);
}
#endif
#endif //_DEBUG


// CTemplateRemoteTransferView message handlers

void CTemplateRemoteTransferView::DisplayRpc(CThreadRpc *pThreadRpc, int iNrTransfer, int *piRow, char *pcComputerId, int iComputerId)
{
	char				cBuffer[128],cBuffer2[128];
	CRpcTransferInfo	info;
	bool				bOverride = false;//, bSelectedFound = false;
	int					iCount, iOrder;
	UINT				iSortingColumnPrimary, iSortingOptionPrimary, iSortingColumnSecondary, iSortingOptionSecondary, iSortingColumnTertiary, iSortingOptionTertiary;
	float				fPerc, fBytes;
	LV_ITEM lvi;
	CString				sTxt;
	CListData			*pData;
	char				cComma;
	FILE_TRANSFER		*pFileTransfer;

	CListCtrl& listCtrl = GetListCtrl();
	cComma = theApp.m_pDlgSettingsView->m_cComma;


	m_pSortColumn->GetColumnOption(&iSortingColumnPrimary, &iSortingOptionPrimary, &iSortingColumnSecondary, &iSortingOptionSecondary, &iSortingColumnTertiary, &iSortingOptionTertiary);

	for(iCount = 0; iCount < iNrTransfer; iCount++)
	{
		while (*piRow >= (int) m_lListData.size())
		{
			pData = new CListData;
			m_lListData.push_back(pData);
		}
		pData = m_lListData.at(*piRow);
	
		pThreadRpc->GetRpcTransfer(iCount,&info);
		pFileTransfer = info.m_pFileTransfer;

		pData->m_cSortPrimary = "";		// init strings, because is one of them is invalid the sorting crashes.
		pData->m_cSortSecondary = "";
		pData->m_cSortTertiary = "";

		// colors

		pData->m_color = RGB(255,255,255);

		// follow the name
		pData->m_bSelected = m_pSelection->Follow((char *) pFileTransfer->name.c_str(), pcComputerId);

		lvi.mask = LVIF_PARAM;
		lvi.iItem = *piRow;
		lvi.iSubItem = 0;
//		lvi.pszText = "";
		lvi.lParam = (LPARAM) pData;
		listCtrl.SetItem(&lvi);

		iOrder = m_iColumnOrder[COLUMN_TRANSFER_PROJECT];
		if (iOrder >= 0) listCtrl.SetItemText(*piRow,iOrder,pFileTransfer->project_name.c_str());

		if (iSortingColumnPrimary == COLUMN_TRANSFER_PROJECT) pData->m_cSortPrimary = (char *) pFileTransfer->project_name.c_str();
		if (iSortingColumnSecondary == COLUMN_TRANSFER_PROJECT) pData->m_cSortSecondary = (char *) pFileTransfer->project_name.c_str();
		if (iSortingColumnTertiary == COLUMN_TRANSFER_PROJECT) pData->m_cSortTertiary = (char *) pFileTransfer->project_name.c_str();

		iOrder = m_iColumnOrder[COLUMN_TRANSFER_FILE];
		if (iOrder >= 0) listCtrl.SetItemText(*piRow, iOrder,pFileTransfer->name.c_str());
		if (iSortingColumnPrimary == COLUMN_TRANSFER_FILE) pData->m_cSortPrimary = (char *) pFileTransfer->name.c_str();
		if (iSortingColumnSecondary == COLUMN_TRANSFER_FILE) pData->m_cSortSecondary = (char *) pFileTransfer->name.c_str();
		if (iSortingColumnTertiary == COLUMN_TRANSFER_FILE) pData->m_cSortTertiary = (char *) pFileTransfer->name.c_str();

		if (pFileTransfer->bytes_xferred > 0)
		{
			fPerc = (float) pFileTransfer->bytes_xferred * 100;
			fPerc /= (float) pFileTransfer->nbytes;
			if (fPerc > 100) fPerc = 100;
			cBuffer[1] = 2;
		}
		else
		{
			fPerc = 0;
			cBuffer[1] = 1;
		}
		_snprintf_s(&cBuffer[3],50,_TRUNCATE,"%.3f",fPerc);

		if (cComma != '.')
		{
			char *cFind = strchr(cBuffer, '.'); 
			if (cFind) *cFind = cComma;
		}

		cBuffer[0] = PERC_CHAR; 
		cBuffer[2] = (char) fPerc;
		cBuffer[2]+= 1;

		iOrder = m_iColumnOrder[COLUMN_TRANSFER_PROGRESS];
		if (iOrder >= 0) listCtrl.SetItemText(*piRow, iOrder,cBuffer);

		if (iSortingColumnPrimary == COLUMN_TRANSFER_PROGRESS) pData->m_fSortPrimary = fPerc;
		if (iSortingColumnSecondary == COLUMN_TRANSFER_PROGRESS) pData->m_fSortSecondary = fPerc;
		if (iSortingColumnTertiary == COLUMN_TRANSFER_PROGRESS) pData->m_fSortTertiary = fPerc;

		fBytes = (float) pFileTransfer->nbytes;
		fBytes/=KILO_BYTE;
		_snprintf_s(&cBuffer[0],50,_TRUNCATE,"%.2f K", fBytes);
		if (cComma != '.')
		{
			char *cFind = strchr(cBuffer, '.'); 
			if (cFind) *cFind = cComma;
		}
		iOrder = m_iColumnOrder[COLUMN_TRANSFER_SIZE];
		if (iOrder >= 0) listCtrl.SetItemText(*piRow,iOrder,cBuffer);

		if (iSortingColumnPrimary == COLUMN_TRANSFER_SIZE) pData->m_iSortPrimary = (int) pFileTransfer->nbytes;
		if (iSortingColumnSecondary == COLUMN_TRANSFER_SIZE) pData->m_iSortSecondary = (int) pFileTransfer->nbytes;
		if (iSortingColumnTertiary == COLUMN_TRANSFER_SIZE) pData->m_iSortTertiary = (int) pFileTransfer->nbytes;

		cBuffer[0] = 0;
//		if (info.m_iTimeSoFar >1)
		{
			int iHour, iMinute, iSecond;
			iMinute = (int) (pFileTransfer->time_so_far / 60);
			iSecond = (int) (pFileTransfer->time_so_far - iMinute*60);
			iHour	= iMinute / 60;
			iMinute = iMinute - iHour *60;
			_snprintf_s(cBuffer,50,_TRUNCATE,"%02d:%02d:%02d",iHour,iMinute,iSecond);
		}
		sTxt = cBuffer;

		int iElapsedTime;
		iElapsedTime = pFileTransfer->next_request_time - pFileTransfer->first_request_time;
		cBuffer[0] = 0;
		if (iElapsedTime >1)
		{
			int iHour, iMinute, iSecond;
			iMinute = iElapsedTime / 60;
			iSecond = iElapsedTime - iMinute*60 ;
			iHour	= iMinute / 60;
			iMinute = iMinute - iHour *60;
			_snprintf_s(cBuffer,50,_TRUNCATE,"%02d:%02d:%02d",iHour,iMinute,iSecond);
			sTxt += " - ";
			sTxt += cBuffer;
		}
		iOrder = m_iColumnOrder[COLUMN_TRANSFER_ELAPSED];
		if (iOrder >= 0) listCtrl.SetItemText(*piRow,iOrder,sTxt);

		if (iSortingColumnPrimary == COLUMN_TRANSFER_ELAPSED) pData->m_iSortPrimary = iElapsedTime;
		if (iSortingColumnSecondary == COLUMN_TRANSFER_ELAPSED) pData->m_iSortSecondary = iElapsedTime;
		if (iSortingColumnTertiary == COLUMN_TRANSFER_ELAPSED) pData->m_iSortTertiary = iElapsedTime;

		_snprintf_s(&cBuffer[0],50,_TRUNCATE,"%.2f KBps",pFileTransfer->xfer_speed/KILO_BYTE);
		if (cComma != '.')
		{
			char *cFind = strchr(cBuffer, '.'); 
			if (cFind) *cFind = cComma;
		}
		iOrder = m_iColumnOrder[COLUMN_TRANSFER_SPEED];
		if (iOrder >= 0) listCtrl.SetItemText(*piRow,iOrder,cBuffer);

		if (iSortingColumnPrimary == COLUMN_TRANSFER_SPEED) pData->m_fSortPrimary = (float) pFileTransfer->xfer_speed;
		if (iSortingColumnSecondary == COLUMN_TRANSFER_SPEED) pData->m_fSortSecondary = (float) pFileTransfer->xfer_speed;
		if (iSortingColumnTertiary == COLUMN_TRANSFER_SPEED) pData->m_fSortTertiary = (float) pFileTransfer->xfer_speed;

		int	iStatus = 0;
		CString sStatus;

		int	iUploadDownload;

		if (info.m_bBoincClientV6)
		{
			iUploadDownload = pFileTransfer->status;
		}
		else
		{
			if (pFileTransfer->is_upload)
			{
				iUploadDownload = STATUS_TRANSFER_UPLOAD;
			}
			else
			{
				iUploadDownload = STATUS_TRANSFER_DOWNLOAD;
			}

		}

		if (pFileTransfer->xfer_active)
		{
			if (iUploadDownload == STATUS_TRANSFER_UPLOAD) {sStatus = gszTranslation[PosViewTransfersStatusUploading] ; iStatus = 3;}			//"Uploading"
			else
			{
				sStatus = gszTranslation[PosViewTransfersStatusDownloading]; iStatus = 0;														//"Downloading"
			}
		}
		else
		{
			if (pFileTransfer->project_backoff <= 0)
			{
				if (iUploadDownload == STATUS_TRANSFER_UPLOAD) {sStatus = gszTranslation[PosViewTransfersStatusUploadP] ; iStatus = 3;}			//"Uploading"
				else
				{
					sStatus = gszTranslation[PosViewTransfersStatusDownloadP]; iStatus = 0;														//"Downloading"
				}

				CTime ctNext((time_t) pFileTransfer->next_request_time);
				CTime ctNow(CTime::GetCurrentTime());
				CTimeSpan toGo = ctNext - ctNow;
				if (toGo.GetTotalSeconds() >= 0)
				{
					sStatus += " (";
					sStatus += gszTranslation[PosViewTransfersStatusRetry];						//"Retry in:";
					sStatus+= " ";
					sStatus += toGo.Format("%H:%M:%S");
					sStatus += ")";
					iStatus = 2;
				}
				else
				{
					if (iUploadDownload == STATUS_TRANSFER_UPLOAD) {sStatus = gszTranslation[PosViewTransfersStatusUploadP]; iStatus = 4;}			//"Upload pending"
					else
					{
						sStatus = gszTranslation[PosViewTransfersStatusDownloadP] ; iStatus = 1;													//"Download pending"
					}

				}
				if (pFileTransfer->num_retries > 0) _snprintf_s(&cBuffer2[0],100,_TRUNCATE,gszTranslation[PosViewTransfersStatusRetryCount] , pFileTransfer->num_retries);						//",retry : %d"		
				else cBuffer2[0] = 0;
				sStatus += cBuffer2;
			}
			else
			{
				if (iUploadDownload == STATUS_TRANSFER_UPLOAD) {sStatus = gszTranslation[PosViewTransfersStatusUploadP]; iStatus = 4;}			//"Upload pending"
				else
				{
					{sStatus = gszTranslation[PosViewTransfersStatusDownloadP] ; iStatus = 1;}	//"Download pending"
				}

				CTimeSpan timeSpan;
				timeSpan = (__time64_t) (pFileTransfer->project_backoff);
				sStatus += " (";
				sStatus += gszTranslation[PosViewTransfersStatusProjectBack]; //"Project backoff";
				sStatus += ": ";
				sStatus += timeSpan.Format("%H:%M:%S");
				sStatus += ")";
			}
		}

		iOrder = m_iColumnOrder[COLUMN_TRANSFER_STATUS];
		if (iOrder >= 0) listCtrl.SetItemText(*piRow,iOrder,sStatus);

		if (iSortingColumnPrimary == COLUMN_TRANSFER_STATUS) pData->m_iSortPrimary = iStatus;
		if (iSortingColumnSecondary == COLUMN_TRANSFER_STATUS) pData->m_iSortSecondary = iStatus;
		if (iSortingColumnTertiary == COLUMN_TRANSFER_STATUS) pData->m_iSortTertiary = iStatus;

		iOrder = m_iColumnOrder[COLUMN_TRANSFER_COMPUTER];
		if (iOrder >= 0)
		{
			if (pcComputerId != NULL) listCtrl.SetItemText(*piRow,iOrder, pcComputerId);
		}
		pData->m_iComputerId = iComputerId;

		(*piRow)++;
	}
}

void CTemplateRemoteTransferView::DisplayMakeLineColors(bool bUseAlternatingLines, bool bUseSkin, int iUseSkin)
{
	int			iItemCount;
	bool		bMax;
	CListData	*pData;

	m_bUseSkin = bUseSkin;
	m_iUseSkin = iUseSkin;
	if (!bUseAlternatingLines) return;

	CListCtrl& listCtrl = GetListCtrl();
	iItemCount = listCtrl.GetItemCount();


	for (int iCount = 1; iCount < iItemCount; iCount+=2)
	{
		pData = (CListData*) listCtrl.GetItemData(iCount);

		if (bUseSkin)
		{
			bMax = false;
			if ((pData->m_color&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
			if (((pData->m_color>>8)&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
			if (((pData->m_color>>16)&0xff) < COLOR_OFFSET_TOTAL) bMax = true;
	
			if (bMax) 	pData->m_color -= RGB(COLOR_OFFSET_MIN,COLOR_OFFSET_MIN,COLOR_OFFSET_MIN);
			else pData->m_color -= RGB(COLOR_OFFSET_STRIPING,COLOR_OFFSET_STRIPING,COLOR_OFFSET_STRIPING);
		}
		else
		{
			pData->m_color -= RGB(20,20,20);
		}
	}
}

void CTemplateRemoteTransferView::OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pSortColumn->ClickItem(pNMHDR, pResult);
//	m_iTimerWu = 1000;				// 1000 to ensure direct timer message
}

void CTemplateRemoteTransferView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CListCtrl& listCtrl = GetListCtrl();

	if (nChar == VK_END)
	{
		m_pSelection->SelectionEnd(&listCtrl);		
	}
	if (nChar == VK_HOME)
	{
		m_pSelection->SelectionBegin(&listCtrl);		
	}

	if (m_pSelection->SelectionNumber() != 0)
	{
		if (nChar == VK_DOWN)
		{
			m_pSelection->SelectionMoveDown(&listCtrl,m_iColumnOrder[COLUMN_TRANSFER_FILE],m_iColumnOrder[COLUMN_TRANSFER_COMPUTER],1);
			return;
		}
		if (nChar == VK_UP)
		{
			m_pSelection->SelectionMoveUp(&listCtrl,m_iColumnOrder[COLUMN_TRANSFER_FILE],m_iColumnOrder[COLUMN_TRANSFER_COMPUTER],1);
			return;
		}
		if (nChar == VK_NEXT)
		{
			m_pSelection->SelectionMoveDown(&listCtrl,m_iColumnOrder[COLUMN_TRANSFER_FILE],m_iColumnOrder[COLUMN_TRANSFER_COMPUTER],listCtrl.GetCountPerPage());
			return;
		}
		if (nChar == VK_PRIOR)
		{
			m_pSelection->SelectionMoveUp(&listCtrl,m_iColumnOrder[COLUMN_TRANSFER_FILE],m_iColumnOrder[COLUMN_TRANSFER_COMPUTER],listCtrl.GetCountPerPage());
			return;
		}
	}
	CListViewEx::OnKeyDown(nChar, nRepCnt, nFlags );
}

void CTemplateRemoteTransferView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CListViewEx::OnKeyUp(nChar, nRepCnt, nFlags );
}

void CTemplateRemoteTransferView::OnRButtonDown(UINT nFlags, CPoint point)
{
//	if (m_pSelection->SelectionNumber() == 0)
	{
		CListCtrl& listCtrl = GetListCtrl();
		int row = listCtrl.HitTest( point, NULL );
		m_pSelection->SelectionSetRight(row);
	}
	
	CListViewEx::OnRButtonDown(nFlags, point);
}

void CTemplateRemoteTransferView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl& listCtrl = GetListCtrl();
	int row = listCtrl.HitTest( point, NULL );

	m_pSelection->SelectionSet(row);

	CListViewEx::OnLButtonDown(nFlags, point);

}

BOOL CTemplateRemoteTransferView::OnHeaderEndDrag(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER	*pNMH ;

	pNMH  = (NMHEADER *) pNMHDR;

	if (pNMH->pitem->mask & HDI_ORDER)
	{
		CListCtrl& listCtrl = GetListCtrl();
		int iw = listCtrl.GetColumnWidth(0); 
		listCtrl.PostMessage(LVM_SETCOLUMNWIDTH, 0, (LPARAM) iw); 
		m_bDraged = true;
	}

	return FALSE;
}

BOOL CTemplateRemoteTransferView::OnHeaderChanged(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
{
	int  iCountHeader;
	int  iCount, iDelta, iMove1;
	int  iOrder[50];
	int	 iNumber, iColumnHeader[NUM_REMOTE_TRANSFER_COLUMNS+1], iColumnHeaderAfter[NUM_REMOTE_TRANSFER_COLUMNS+1];;

	if (!m_bDraged) return 0;
	m_bDraged = false;
	iDelta = -1;

	CListCtrl& listCtrl = GetListCtrl();	
	CHeaderCtrl* pHeaderCtrl = listCtrl.GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
	{
		iCountHeader = pHeaderCtrl->GetItemCount();

		LPINT pnOrder = (LPINT) malloc(iCountHeader*sizeof(int));
		ASSERT(pnOrder != NULL);
		if (NULL != pnOrder)
		{
			pHeaderCtrl->GetOrderArray(pnOrder, iCountHeader);
			for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iOrder[iCount] = pnOrder[iCount]; 
			}

			m_pSortColumn->OrderChanged(iCountHeader, pnOrder);

			for (iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
			{
				iNumber = m_iColumnOrder[iCount];
				if (iNumber > NUM_REMOTE_TRANSFER_COLUMNS) break;
				if (iNumber >=0) iColumnHeader[iNumber] = iCount;
			}

	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iMove1 = pnOrder[iCount];
				iColumnHeaderAfter[iCount] = iColumnHeader[iMove1];
			}

			for (iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
			{
				m_iColumnOrder[iCount] = -1;
			}

	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iNumber = iColumnHeaderAfter[iCount];
				if (iNumber > NUM_REMOTE_TRANSFER_COLUMNS) break;
				if (iNumber >=0) m_iColumnOrder[iNumber] = iCount;
			}


	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				pnOrder[iCount] = iCount; 
			}
			pHeaderCtrl->SetOrderArray(iCountHeader, pnOrder);	// set to 0,1,2, again
			free(pnOrder);
         }
	}

	AddAllColumns(false);
	listCtrl.Invalidate(TRUE);
//	m_tNextHistoryUpdateTime = 0;				// 1000 to ensure direct timer message
//	m_iSortingChanged = 2;
	return FALSE;
}

void CTemplateRemoteTransferView::ClipBoard(bool bAll)
{
    HGLOBAL		hglbCopy; 
    LPTSTR		lptstrCopy; 
	CString		sTxt,sClipBoard;
	int			iItems, iNr;
	bool		bSelected;
	
	if ( !OpenClipboard() ) return;
	if( !EmptyClipboard() ) return;
 
	CListCtrl& listCtrl = GetListCtrl();
	
	iItems = listCtrl.GetItemCount();

	for (int iCountItems = 0; iCountItems < iItems; iCountItems++)
	{
		bSelected = m_pSelection->SelectionSelected(&listCtrl, iCountItems);

		if (bAll) bSelected = true;
		if (bSelected)
		{
			for (int iCountSearch = 0; iCountSearch < NUM_REMOTE_TRANSFER_COLUMNS; iCountSearch++)
			{
				for (int iCount = 0; iCount < NUM_REMOTE_TRANSFER_COLUMNS; iCount++)
				{
					iNr = m_iColumnOrder[iCount];
					if (iNr > NUM_REMOTE_TRANSFER_COLUMNS) break;
					if (iNr >=0)
					{
						if (iCountSearch == iNr)
						{
							sTxt = listCtrl.GetItemText(iCountItems,iNr);
							RemoveSpecialText(&sTxt);
							sClipBoard += sTxt;
							sClipBoard += "\t";
							break;
						}
					}
				}
			}
			sClipBoard += "\r\n";
		}
	}

	// Allocate a global memory object for the text. 
	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (sClipBoard.GetLength() + 1) * sizeof(char)); 
	if (hglbCopy == NULL) 
    { 
		CloseClipboard(); 
        return; 
	} 
 
	// Lock the handle and copy the text to the buffer. 
    lptstrCopy = (LPTSTR) GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, sClipBoard.GetBuffer(), sClipBoard.GetLength() * sizeof(char)); 
	lptstrCopy[sClipBoard.GetLength()] = (char) 0;    // null character 
	GlobalUnlock(hglbCopy); 
 
	// set the locale
	HANDLE hLC=GlobalAlloc(GMEM_MOVEABLE,sizeof(LCID));
	if (hLC)
	{
		PLCID pLc=(PLCID)GlobalLock(hLC);
		if (pLc)
		{
			*pLc = theApp.m_pTranslation->m_lcid;
			GlobalUnlock(hLC);
			if (!SetClipboardData(CF_LOCALE, pLc))	GlobalFree(hLC);
		}
		else
		{
			GlobalFree(hLC);
		}
	}

	// Place the handle on the clipboard. 
	SetClipboardData(CF_TEXT, hglbCopy); 

	CloseClipboard();
}
