// InPlaceEdit.cpp : implementation file
//

#include "stdafx.h"
#include "InPlaceEdit.h"
#include "listviewex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit

CInPlaceEdit::CInPlaceEdit(CWnd *pWnd, int iItem, int iSubItem, CString sInitText)
:m_sInitText( sInitText )
{
	m_pWnd = pWnd;
	m_iItem = iItem;
	m_iSubItem = iSubItem;
	m_bESC = FALSE;

	m_bPassword = false;
	if (m_sInitText.GetAt(0) == PASS_CHAR)
	{
		m_sInitText = m_sInitText.Mid(1);
		m_bPassword = true;
	}
	m_bSecondHidden = false;
	if (m_sInitText.GetAt(0) == HIDE_SECOND)
	{
		m_sInitText = m_sInitText.Mid(1);

		int iHide = m_sInitText.Find(HIDE_CHAR,1);
		if (iHide > 0)
		{
			m_sHiddenText = m_sInitText.Mid(iHide+1);
			m_sInitText = m_sInitText.Left(iHide);
		}

		m_bSecondHidden = true;
	}

}

CInPlaceEdit::~CInPlaceEdit()
{
}


BEGIN_MESSAGE_MAP(CInPlaceEdit, CEdit)
	//{{AFX_MSG_MAP(CInPlaceEdit)
//	ON_WM_KILLFOCUS()
//	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit message handlers

BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
				|| pMsg->wParam == VK_DELETE
				|| pMsg->wParam == VK_ESCAPE
				|| GetKeyState( VK_CONTROL)
				)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;		    	// DO NOT process further
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

/*
void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
/*
	CString str;
	GetWindowText(str);

	// Send Notification to parent of ListView ctrl
//	LV_DISPINFO dispinfo;
//	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
//	dispinfo.hdr.idFrom = GetDlgCtrlID();
//	dispinfo.hdr.code = LVN_ENDLABELEDIT;
//
//	dispinfo.item.mask = LVIF_TEXT;
//	dispinfo.item.iItem = m_iItem;
//	dispinfo.item.iSubItem = m_iSubItem;
//	dispinfo.item.pszText = m_bESC ? NULL : LPTSTR((LPCTSTR)str);
//	dispinfo.item.cchTextMax = str.GetLength();

	m_pWnd->SendMessage(WM_INPLACE_READY, (WPARAM)0, (LPARAM) &str);

	DestroyWindow();
}
*/

/*
void CInPlaceEdit::OnNcDestroy()
{
	CEdit::OnNcDestroy();

//	delete this;
}
*/

void CInPlaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( nChar == VK_ESCAPE || nChar == VK_RETURN || nChar == VK_TAB)
	{
		if( nChar == VK_ESCAPE ) m_bESC = TRUE;
//		GetParent()->SetFocus();
		m_pView->OnChar(nChar, nRepCnt, nFlags);		// send the keyboard data to the view
		return;
	}


	CEdit::OnChar(nChar, nRepCnt, nFlags);

	// Resize edit control if needed

	// Get text extent
	CString str;

	GetWindowText( str );
	CWindowDC dc(this);
	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = dc.SelectObject( pFont );
	CSize size = dc.GetTextExtent( str );

	dc.SelectObject( pFontDC );
	size.cx += 5;			   	// add some extra buffer

	// Get client rect
	CRect rect, parentrect;
	GetClientRect( &rect );
	GetParent()->GetClientRect( &parentrect );

	// Transform rect to parent coordinates
	ClientToScreen( &rect );
	GetParent()->ScreenToClient( &rect );

	// Check whether control needs to be resized
	// and whether there is space to grow
	if( size.cx > rect.Width() )
	{
		if( size.cx + rect.left < parentrect.right )
			rect.right = rect.left + size.cx;
		else
			rect.right = parentrect.right;
		MoveWindow( &rect );
	}
}

int CInPlaceEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1) return -1;

	// Set the proper font
	CFont* font = GetParent()->GetFont();
	SetFont(font);

	SetWindowText( m_sInitText );
	SetSel( 0, -1 );
	SetFocus();

	return 0;
}

CString CInPlaceEdit::GetText()
{
	CString sTxt, sPass;

	GetWindowText(sTxt);

	if (m_bPassword)
	{
		sPass = PASS_CHAR;
		sTxt = sPass + sTxt;
	}
	if (m_bSecondHidden)
	{
		if (sTxt.GetLength() > 0)
		{
			sPass = HIDE_SECOND;
			sTxt = sPass + sTxt;
			sPass = HIDE_CHAR;
			sTxt = sTxt + sPass + m_sHiddenText;
		}
		else sTxt = "";
	}

	return sTxt;
}