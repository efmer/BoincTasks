// RemoteMView.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
//#include "ComputerDoc.h"
#include "ListViewEx.h"   // base class for BoinCComputerView
#include "RemoteCombinedDoc.h"
#include "TemplateRemoteMessageView.h"
#include "DlgPrefMain.h"
#include "DlgSettingsView.h"
#include "DlgSettingsMessage.h"
#include "DlgColorMessages.h"
#include "ThreadRpc.h"
#include "DateConvert.h"
#include "Translate.h"
#include "Translation.h"
#include "MainFrm.h"
#include "SortColumn.h"

static int _gnRemoteMessageColumnWidth[NUM_REMOTE_MESSAGE_COLUMNS] =
{
	100			,200			,100			,500			,100
};

// CRemoteCombinedMessageView

IMPLEMENT_DYNCREATE(CTemplateRemoteMessageView, CListViewEx)

CTemplateRemoteMessageView::CTemplateRemoteMessageView()
{
	m_bMessagesCorrupt = false;
	m_bForceRefreshAll = false;
	m_bAlwaysShow = true;
	m_bFilterChanged = true;
	m_bFilterGeneral = true;
	m_bFilterAllProjects = true;
	m_bDraged = false;
}

CTemplateRemoteMessageView::~CTemplateRemoteMessageView()
{

}

void CTemplateRemoteMessageView::OnInitialUpdate()
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetExtendedStyle (listCtrl.GetExtendedStyle()|LVS_EX_HEADERDRAGDROP);
	AddAllColumns();

	CListViewEx::OnInitialUpdate();
}

void CTemplateRemoteMessageView::AddAllColumns(bool bAdd)
{
	int			iCount, iColumn, iWidth, iFormat, iNr;
	int			iColumnHeader[NUM_REMOTE_MESSAGE_COLUMNS+1];
	LV_COLUMN	lvc;

	// read back the column sequence

	if (bAdd)
	{
		m_iNrColomns = (int) ::SendMessage(theApp.m_pDlgSettingsMessage->m_hWnd, UWM_MSG_DLG_SETTINGS_MESSAGES_GET, (WPARAM) &m_iColumnOrder[0],(LPARAM) &iColumnHeader[0]);
	}
	else
	{	
		for (iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
		{
			iColumnHeader[iCount] = -1;
		}

		for (iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
		{
			iNr = m_iColumnOrder[iCount];
			if (iNr > NUM_REMOTE_MESSAGE_COLUMNS) break;
			if (iNr >=0) iColumnHeader[iNr] = iCount;
		}
		::SendMessage(theApp.m_pDlgSettingsMessage->m_hWnd, UWM_MSG_DLG_SETTINGS_MESSAGES_SET, (WPARAM) &m_iColumnOrder[0],(LPARAM) &iColumnHeader[0]);
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
			lvc.pszText = gszTranslation[PosMessagesRowNr+iColumn];
			iWidth = ReadColumnWidth(iColumn, _gnRemoteMessageColumnWidth[iColumn]);
			m_iColumnWidth[iColumn] = iWidth;
			lvc.cx = m_iColumnWidth[iColumn];

			switch(iColumn)
			{
			case COLUMN_MESSAGE_NR:
				case COLUMN_MESSAGE_TIME:
					iFormat = LVCFMT_RIGHT;
				break;
//					iFormat = LVCFMT_CENTER;
				default:
					iFormat = LVCFMT_LEFT;	// COLUMN_MESSAGE_PROJECT  COLUMN_MESSAGE_MESSAGE COLUMN_MESSAGE_COMPUTER
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

	int iItems;
	iItems = listCtrl.GetItemCount();
	if (iItems <= 0)
	{
		LV_ITEM		lvi;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;

		lvi.iItem = 0;
		lvi.iSubItem = 0;
		lvi.pszText = "";	
		lvi.iImage = 0;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK(1);

		lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_NORMAL];
		listCtrl.InsertItem(&lvi);

		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		listCtrl.SetItem(&lvi);

		CString sTxt;
		sTxt = gszTranslation[PosViewMessagesStatusSelectOne];		//needed from char -> CString to get the right codepage
		listCtrl.SetItemText(0,m_iColumnOrder[COLUMN_MESSAGE_MESSAGE],sTxt);
	}

	m_iSortingPrimary =  theApp.GetProfileInt(registrySectionSorting, registrySortMessagesPrimary, SORTING_AZ);
	SetSortIndicator(m_iColumnOrder[COLUMN_MESSAGE_NR], m_iSortingPrimary);
}

void CTemplateRemoteMessageView::ReadAllColumnWidth()
{
	CListCtrl& listCtrl = GetListCtrl();
	int iWidth;

	for(int iCount = 0; iCount< NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
	{
		iWidth = ReadColumnWidth(iCount, _gnRemoteMessageColumnWidth[iCount]);
		listCtrl.SetColumnWidth(m_iColumnOrder[iCount], iWidth);
		m_iColumnWidth[m_iColumnOrder[iCount]] = iWidth;
	}
}
int CTemplateRemoteMessageView::ReadColumnWidth(int iColumn, int iDefault)
{
	CString	sRegistry;
	char	cChar;
	int		iWidth;

	sRegistry = registryColumnMessages;
	cChar = iColumn + 'a';
	sRegistry = sRegistry + "_" + cChar; 

	iWidth =  theApp.GetProfileInt(registrySectionColumn, sRegistry, iDefault);
//	if (iWidth < 20) return iDefault;
	return iWidth;
}

void CTemplateRemoteMessageView::CheckAndSaveColumnsWidth()
{
	int		iWidth, iColumn;
	bool	bChanged = false;
	char	cChar;
	CString sTxt;

	CListCtrl& listCtrl = GetListCtrl();

	for (int iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
	{
		iColumn = m_iColumnOrder[iCount];
		if (iColumn >= 0)
		{
			iWidth = listCtrl.GetColumnWidth(iColumn);
			if (m_iColumnWidth[iColumn] != listCtrl.GetColumnWidth(iColumn))
			{
				m_iColumnWidth[iColumn] = iWidth;
				bChanged = true;
				sTxt = registryColumnMessages;
				cChar = iCount + 'a';
				sTxt = sTxt + "_" + cChar; 
				// update new width in registry
				theApp.WriteProfileInt(registrySectionColumn, sTxt, iWidth);
			}
		}
	}
}

void CTemplateRemoteMessageView::CheckIfLastIsShown()
{
	int	iNrItems, iN, iFirst, iLast;

	CListCtrl& listCtrl = GetListCtrl();
	iNrItems = listCtrl.GetItemCount();

	if (iNrItems > 0)
	{
		iN = listCtrl.GetTopIndex();
		if (m_iSortingPrimary == SORTING_AZ)
		{
			// Select all of the items that are completely visible.
			iLast = iN + listCtrl.GetCountPerPage();
			if (iLast == iNrItems)	m_bAlwaysShow = true;
			else					m_bAlwaysShow = false;
		}
		else
		{
			iFirst = iN - listCtrl.GetCountPerPage();
			if (iFirst <= 0)		m_bAlwaysShow = true;
			else					m_bAlwaysShow = false;
		}
	}
}


void CTemplateRemoteMessageView::StripingSkin(COLORREF *pCrColor)
{
//#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

	unsigned int offset, color;

	color = *pCrColor;
	if ((*pCrColor & 0xff) < COLOR_OFFSET_TOTAL)
	{
		offset = RGB(COLOR_OFFSET_STRIPING, 0, 0);
		color += offset;
	}
	else
	{
		offset = RGB(COLOR_OFFSET_STRIPING, 0, 0);
		color -= offset;
	}
	if (((*pCrColor >> 8) & 0xff) < COLOR_OFFSET_TOTAL)
	{
		offset = RGB(0, COLOR_OFFSET_STRIPING, 0);
		color += offset;
	}
	else
	{
		offset = RGB(0, COLOR_OFFSET_STRIPING, 0);
		color -= offset;
	}
	if (((*pCrColor >> 16) & 0xff) < COLOR_OFFSET_TOTAL)
	{
		offset = RGB(0, 0, COLOR_OFFSET_STRIPING);
		color += offset;
	}
	else
	{
		offset = RGB(0, 0, COLOR_OFFSET_STRIPING);
		color -= offset;
	}
	*pCrColor = color;
}

void CTemplateRemoteMessageView::SetSortIndicator(int iColumn, int iSorting)
{
	CString sTxt, sSortUp, sSortDown, sSort;
	char cBuffer[255];
	LV_COLUMN	lvc;

	if (iSorting == SORTING_AZ)	sSort = SORT_STRING_UP_PRIMARY;
	else						sSort = SORT_STRING_DOWN_PRIMARY;

	CListCtrl& listCtrl = GetListCtrl();	

	lvc.pszText = &cBuffer[0];
	lvc.cchTextMax = 254;
	lvc.mask = LVCF_TEXT;
	listCtrl.GetColumn(iColumn,&lvc);

	sTxt = lvc.pszText;

	lvc.pszText = &cBuffer[0];
	lvc.cchTextMax = 254;
	lvc.mask = LVCF_TEXT;
	listCtrl.GetColumn(m_iColumnOrder[COLUMN_MESSAGE_NR],&lvc);
	sTxt = lvc.pszText;

	sTxt.Replace(SORT_STRING_UP_PRIMARY,"");
	sTxt.Replace(SORT_STRING_DOWN_PRIMARY,"");

	sTxt = sSort + sTxt;	

	lvc.iSubItem = m_iColumnOrder[COLUMN_MESSAGE_NR];
	lvc.pszText = sTxt.GetBuffer();
	listCtrl.SetColumn(m_iColumnOrder[COLUMN_MESSAGE_NR],&lvc);
	sTxt.ReleaseBuffer();
}

BEGIN_MESSAGE_MAP(CTemplateRemoteMessageView, CListViewEx)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()

	ON_NOTIFY(HDN_ITEMCLICK, 0, OnItemclickList)
    ON_NOTIFY_EX(HDN_ENDDRAG, 0, OnHeaderEndDrag)
	ON_NOTIFY_EX(HDN_ITEMCHANGED, 0, OnHeaderChanged) 
END_MESSAGE_MAP()


bool CTemplateRemoteMessageView::DisplayRpc(CThreadRpc *pThreadRpc, int iNrMessages, int iNrFiltered, int iLastReadMessages, char *pcComputerId)
{
	char				cBuffer[256];
	CRpcMessageInfo		info;
	bool				bLineColor = false, bOverride = false, bSelectedFound = false, bChanged = false;;
	int					iCount, iRow, iColumn;
	CString				sItemTxt, sProject, sBody;
	LV_ITEM				lvi;
	COLORREF			*pColorsMessages;
	CTranslate			translate;
//	char				*pcHighlight1;
//
//	pcHighlight1 = 


	pColorsMessages = theApp.m_pDlgColorMessages->GetColorArray();
	
	m_data[MESSAGE_COLOR_1].m_color		= *(pColorsMessages + COLOR_MESSAGES_ARRAY_1);
	m_data[MESSAGE_COLOR_1_LINE].m_color= *(pColorsMessages + COLOR_MESSAGES_ARRAY_1);
	m_data[MESSAGE_COLOR_2].m_color		= *(pColorsMessages + COLOR_MESSAGES_ARRAY_2);
	m_data[MESSAGE_COLOR_2_LINE].m_color= *(pColorsMessages + COLOR_MESSAGES_ARRAY_2);
	m_data[MESSAGE_COLOR_3].m_color		= *(pColorsMessages + COLOR_MESSAGES_ARRAY_3);
	m_data[MESSAGE_COLOR_3_LINE].m_color= *(pColorsMessages + COLOR_MESSAGES_ARRAY_3);

	for (int iHighLight = 0; iHighLight <= COLOR_ARRAY_HIGHLIGHT_MAX; iHighLight++)
	{
		m_data[MESSAGE_COLOR_HL_1+iHighLight].m_color		= *(pColorsMessages + COLOR_MESSAGES_ARRAY_HL_1+iHighLight);
		m_data[MESSAGE_COLOR_HL_1_LINE+iHighLight].m_color	= *(pColorsMessages + COLOR_MESSAGES_ARRAY_HL_1+iHighLight);
	}

	m_bUseSkin = theApp.m_pDlgSettingsView->m_bUseSkin;
	m_iUseSkin = theApp.m_pDlgSettingsView->m_iUseSkin;
	if (theApp.m_pDlgSettingsView->m_bAlternatingStriped)
	{
		if (m_bUseSkin)
		{
			StripingSkin(&m_data[MESSAGE_COLOR_1_LINE].m_color);
			StripingSkin(&m_data[MESSAGE_COLOR_2_LINE].m_color);
			StripingSkin(&m_data[MESSAGE_COLOR_3_LINE].m_color);

			for (int iHighLight = 0; iHighLight <= COLOR_ARRAY_HIGHLIGHT_MAX; iHighLight++)
			{
				StripingSkin(&m_data[MESSAGE_COLOR_HL_1_LINE + iHighLight].m_color);
			}
		}
		else
		{
			m_data[MESSAGE_COLOR_1_LINE].m_color-= RGB(20,20,20);
			m_data[MESSAGE_COLOR_2_LINE].m_color-= RGB(20,20,20);
			m_data[MESSAGE_COLOR_3_LINE].m_color-= RGB(20,20,20);
			for (int iHighLight = 0; iHighLight <= COLOR_ARRAY_HIGHLIGHT_MAX; iHighLight++)
			{
				m_data[MESSAGE_COLOR_HL_1_LINE+iHighLight].m_color-= RGB(20,20,20);
			}
		}
	}

	CListCtrl& listCtrl = GetListCtrl();

	if (!m_bFilterChanged)
	{
		if (iLastReadMessages == iNrMessages)
		{
			// check if there are items that are still in the init state
			if (CheckIfMessagesCorrupt(&listCtrl))
			{
				m_bMessagesCorrupt = true;
			}
			return FALSE;
		}
	}
	else
	{
		iLastReadMessages = 1;
	}

	if (m_bFilterChanged)
	{
		iLastReadMessages = 1;
		m_bFilterChanged = false;
	}

	if (m_iSortingPrimary == SORTING_AZ)
	{
		iRow = iLastReadMessages-1;
	}
	else
	{
		iRow = iNrMessages - iLastReadMessages;
		iRow-= iNrFiltered;
	}

	for(iCount = iLastReadMessages-1; iCount < iNrMessages; iCount++)
	{
		pThreadRpc->GetRpcMessage(iCount,&info);

		// filter

		sProject = info.m_pszProject;

		if (IsFiltered(&sProject))
		{
			continue;
		}

		// filter


		// colors

		if (bLineColor && theApp.m_pDlgSettingsView->m_bAlternatingStriped)
		{
			if (info.m_iPriority == 1) lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_1_LINE];
			else
			{
				if (info.m_iPriority == 2) lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_2_LINE];
				else lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_3_LINE];
			}
			bLineColor = false;
		}
		else
		{
			if (info.m_iPriority == 1) lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_1];
			else
			{
				if (info.m_iPriority == 2) lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_2];
				else lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_3];
			}
			bLineColor = true;
		}

		// highlight body text

		CString sLower;
		sBody = info.m_pszBody;
		sBody.Remove(0xa);					// there is a lf 0xa at the end that shows up in XP systems.
		translate.TranslateBody(&sBody);

		for (int iHighlightCount = 0; iHighlightCount <= COLOR_ARRAY_HIGHLIGHT_MAX; iHighlightCount++)
		{
			if ((theApp.m_pDlgSettingsMessage->m_sHighlight[iHighlightCount].GetLength() > 0) || (theApp.m_pDlgSettingsMessage->m_sHighlightProject[iHighlightCount].GetLength() > 0))
			{
				sLower = sBody;
				sLower.MakeLower();
				if (sLower.Find(theApp.m_pDlgSettingsMessage->m_sHighlight[iHighlightCount]) >=0)
				{
					sLower = info.m_pszProject;
					sLower.MakeLower();
					if (sLower.Find(theApp.m_pDlgSettingsMessage->m_sHighlightProject[iHighlightCount]) >= 0)
					{
						if (bLineColor)	lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_HL_1 + iHighlightCount];
						else			lvi.lParam = (LPARAM) &m_data[MESSAGE_COLOR_HL_1_LINE + iHighlightCount];
					}
				}
			}
		}

		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iItem = iRow;
		lvi.iSubItem = 0;
		lvi.pszText = "";
		listCtrl.SetItem(&lvi);

		_snprintf_s(cBuffer,40,_TRUNCATE,"%d", info.m_iSegNo);
		iColumn = m_iColumnOrder[COLUMN_MESSAGE_NR];
		if (iColumn >= 0) listCtrl.SetItemText(iRow,iColumn,cBuffer);

		iColumn = m_iColumnOrder[COLUMN_MESSAGE_PROJECT];
		if (iColumn >= 0)	listCtrl.SetItemText(iRow,iColumn,info.m_pszProject);

		time_t time = info.m_iTimeStamp;

		CDateConvert dateConvert;
		dateConvert.Convert(time, &cBuffer[0]);

		iColumn = m_iColumnOrder[COLUMN_MESSAGE_TIME];
		if (iColumn >= 0) listCtrl.SetItemText(iRow,iColumn,cBuffer);

		iColumn = m_iColumnOrder[COLUMN_MESSAGE_MESSAGE];
		if (iColumn >= 0)
		{
			listCtrl.SetItemText(iRow, iColumn, sBody);	
		}

		if (pcComputerId != NULL)
		{
			iColumn = m_iColumnOrder[COLUMN_MESSAGE_COMPUTER];
			if (iColumn >= 0) listCtrl.SetItemText(iRow,iColumn, pcComputerId);
		}
		
		if (m_iSortingPrimary == SORTING_AZ) iRow++;
		else								 iRow--;
		bChanged = true;
	}	

	return bChanged;
}

bool CTemplateRemoteMessageView::IsFiltered(CString *psProject)
{
	bool bFoundFilter;

	if (*psProject != "")
	{
		if (!m_bFilterAllProjects)
		{
			bFoundFilter = false;
			if (m_lProjectFilter.size() > 0)
			{
				for (int iPos=0; iPos < (int) m_lProjectFilter.size(); iPos++)
				{
					if (*psProject == m_lProjectFilter.at(iPos)->m_sProject)
					{
						if (m_lProjectFilter.at(iPos)->m_bSelected)
						{
							bFoundFilter = true;
						}
					}
				}
			}
			else return false;
			return !bFoundFilter;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (m_bFilterGeneral)
		{
			return false;
		}
	}
	return true;
}

bool CTemplateRemoteMessageView::CheckIfMessagesCorrupt(CListCtrl *pListCtrl)
{
	CString sItemTxt;
	int iColumn, iNrItems;

	iColumn = 0;	// always 0, never the column order !!!!!
	if (iColumn >= 0)
	{
		iNrItems = pListCtrl->GetItemCount();
		if (iNrItems > 0)
		{
			for	(int iCount = 0; iCount < iNrItems-1; iCount++)
			{
				sItemTxt = pListCtrl->GetItemText(iCount,iColumn);
	
				if (sItemTxt == MESSAGE_INIT_TEXT)
				{
					return true;
				}
			}
		}
	}
	return false;
}

// CRemoteCombinedMessageView diagnostics

#ifdef _DEBUG
void CTemplateRemoteMessageView::AssertValid() const
{
	CListViewEx::AssertValid();
}

#ifndef _WIN32_WCE
void CTemplateRemoteMessageView::Dump(CDumpContext& dc) const
{
	CListViewEx::Dump(dc);
}
#endif
#endif //_DEBUG


// CRemoteCombinedMessageView message handlers


void CTemplateRemoteMessageView::ClipBoard(bool bAll)
{
    HGLOBAL		hglbCopy; 
    LPTSTR		lptstrCopy; 
	CString		sTxt,sClipBoard;
	int			iItems, iColumn;
	bool		bSelected;

	if ( !OpenClipboard() ) return;
	if( !EmptyClipboard() ) return;
 
	CListCtrl& listCtrl = GetListCtrl();
	
	iItems = listCtrl.GetItemCount();

	iColumn = m_iColumnOrder[COLUMN_MESSAGE_COMPUTER];

	if (iColumn >= 0) sClipBoard = listCtrl.GetItemText(0,iColumn);
	if (sClipBoard.GetLength() > 0) sClipBoard += "\r\n\r\n";

	for (int iCount = 0; iCount < iItems; iCount++)
	{
		bSelected = false;
		if (listCtrl.GetItemState(iCount,LVIS_SELECTED)) bSelected = true;
		if (bAll) bSelected = true;
		if (bSelected)
		{
			iColumn = m_iColumnOrder[COLUMN_MESSAGE_NR];
			if (iColumn >= 0) sClipBoard += listCtrl.GetItemText(iCount, iColumn);
			sClipBoard += "\t";
			iColumn = m_iColumnOrder[COLUMN_MESSAGE_PROJECT];
			if (iColumn >= 0) sTxt = listCtrl.GetItemText(iCount, iColumn);
			if (sTxt.GetLength() > 0) 	sClipBoard += sTxt + "\t";
			else						sClipBoard += "\t\t";
			iColumn = m_iColumnOrder[COLUMN_MESSAGE_TIME];
			if (iColumn >= 0) sClipBoard += listCtrl.GetItemText(iCount, iColumn);
			sClipBoard += "\t";
			iColumn = m_iColumnOrder[COLUMN_MESSAGE_MESSAGE];
			if (iColumn >= 0) sClipBoard += listCtrl.GetItemText(iCount, iColumn);
			sClipBoard += "\t";
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

void CTemplateRemoteMessageView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CListViewEx::OnVScroll(nSBCode, nPos, pScrollBar);
	CheckIfLastIsShown();
}

BOOL CTemplateRemoteMessageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL bReturn;

	bReturn = CListViewEx::OnMouseWheel(nFlags, zDelta, pt);
	CheckIfLastIsShown();
	return bReturn;
}

BOOL CTemplateRemoteMessageView::OnHeaderEndDrag(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
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

BOOL CTemplateRemoteMessageView::OnHeaderChanged(UINT iNr, NMHDR* pNMHDR, LRESULT* pResult)
{
	int  iCountHeader;
	int  iCount, iDelta, iMove1;
	int  iOrder[50];
	int	 iNumber, iColumnHeader[NUM_REMOTE_MESSAGE_COLUMNS+1], iColumnHeaderAfter[NUM_REMOTE_MESSAGE_COLUMNS+1];;

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

//			m_pSortColumn->OrderChanged(iCountHeader, pnOrder);

			for (iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
			{
				iNumber = m_iColumnOrder[iCount];
				if (iNumber > NUM_REMOTE_MESSAGE_COLUMNS) break;
				if (iNumber >=0) iColumnHeader[iNumber] = iCount;
			}

	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iMove1 = pnOrder[iCount];
				iColumnHeaderAfter[iCount] = iColumnHeader[iMove1];
			}

			for (iCount = 0; iCount < NUM_REMOTE_MESSAGE_COLUMNS; iCount++)
			{
				m_iColumnOrder[iCount] = -1;
			}

	        for (iCount = 0; iCount < iCountHeader; iCount++)
			{
				iNumber = iColumnHeaderAfter[iCount];
				if (iNumber > NUM_REMOTE_MESSAGE_COLUMNS) break;
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
	m_bForceRefreshAll = true;		// so everything is loaded from the beginnning
	return FALSE;
}

void CTemplateRemoteMessageView::OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString sTxt, sSortUp, sSortDown, sSort;
	int iColumn;

	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	iColumn = phdr->iItem;

	if (iColumn != m_iColumnOrder[COLUMN_MESSAGE_NR]) return;

	if (m_iSortingPrimary == SORTING_AZ)m_iSortingPrimary = SORTING_ZA;
	else								m_iSortingPrimary = SORTING_AZ;

	SetSortIndicator(iColumn, m_iSortingPrimary);

	theApp.WriteProfileInt(registrySectionSorting, registrySortMessagesPrimary, m_iSortingPrimary);

	m_bForceRefreshAll = true;
}
