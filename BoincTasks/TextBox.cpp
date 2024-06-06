// TextBox.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "TextBox.h"


// CTextBox

IMPLEMENT_DYNAMIC(CTextBox, CEdit)

CTextBox::CTextBox()
{
	m_iHeight = 0;
	m_iWidth = 0;
	m_crText = RGB(255,255,255);
}

CTextBox::~CTextBox()
{
}


BEGIN_MESSAGE_MAP(CTextBox, CEdit)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()

	ON_MESSAGE (UWM_MSG_TEXT_NEW_SIZE, OnNewSize)	
	ON_MESSAGE (UWM_MSG_TEXT_NEW_COLOR, OnNewColor)	

END_MESSAGE_MAP()



// CTextBox message handlers


LRESULT CTextBox::OnNewSize(WPARAM parm1, LPARAM parm2)
{
	m_iWidth = (int) parm1;
	m_iHeight = (int) parm2;
	this->Invalidate();
	return 0;
}

LRESULT CTextBox::OnNewColor(WPARAM parm1, LPARAM parm2)
{
	m_crText = (COLORREF) parm1;
	return 0;
}

void CTextBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	DWORD dwFlags = 0;

	CRect rc;
	GetClientRect(rc);
	CString strText;
	GetWindowText(strText);
	CBitmap bmp;


	///////////////////////////////////////////////////////
	//
	// Set up for double buffering...
	//
	CDC* pDCMem;
	CBitmap*	pOldBitmap = NULL;
	bool	m_bTransparent = false;

	CFont	*pFont = GetFont( );

	pDCMem = new CDC;
	pDCMem->CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());
	pOldBitmap = pDCMem->SelectObject(&bmp);

	// Fill in backgound if not transparent
	if (!m_bTransparent)
	{
//		if (m_fillmode == Normal)
//		{
//			CBrush br;
//			
//			if (m_hBackBrush != NULL) br.Attach(m_hBackBrush);
//			else br.Attach(m_hwndBrush);
//					
//			pDCMem->FillRect(rc,&br);
//
//			br.Detach();
//		}
//		else // Gradient Fill
//		{
//			DrawGradientFill(pDCMem, &rc, m_crLoColor, m_crHiColor, 100);
//		}

		CBrush br;
		br.CreateSolidBrush(RGB(255,255,255));
//		br.Attach(m_hwndBrush);
		pDCMem->FillRect(rc,&br);

	}
	

	// If the text is flashing turn the text color on
	// then to the color of the window background.

	LOGBRUSH lb;
	ZeroMemory(&lb,sizeof(lb));

	// Stop Checking complaining
//	if (m_hBackBrush)
//		::GetObject(m_hBackBrush,sizeof(lb),&lb);


	// Something to do with flashing
//	if (!m_bState && m_Type == Text)
//		pDCMem->SetTextColor(lb.lbColor);

	DWORD style = GetStyle();
	
	switch (style & SS_TYPEMASK)
	{
		case SS_RIGHT: 
			dwFlags = DT_RIGHT | DT_WORDBREAK; 
			break; 
		
		case SS_CENTER: 
			dwFlags = SS_CENTER | DT_WORDBREAK;
			break;

		case SS_LEFTNOWORDWRAP: 
			dwFlags = DT_LEFT; 
			break;

		default: // treat other types as left
			case SS_LEFT: 
				dwFlags = DT_LEFT | DT_WORDBREAK; 
				break;
	}	

		
	// Added to expand tabs...
	if(strText.Find(_T('\t')) != -1)
		dwFlags |= DT_EXPANDTABS;

	// If the text centered make an assumtion that
	// the will want to center verticly as well
	if (style & SS_CENTERIMAGE)
	{
		dwFlags = DT_CENTER;

		// Apply 
		if (strText.Find(_T("\r\n")) == -1)
		{
			dwFlags |= DT_VCENTER;

			// And because DT_VCENTER only works with single lines
			dwFlags |= DT_SINGLELINE; 
		}

	}

	CRect rectText;
	rectText = rc;
	rectText.DeflateRect(m_iWidth/2,m_iHeight/2,m_iWidth/2,m_iHeight/2);

	CFont *pOldFont = pDCMem->SelectObject(pFont);
	COLORREF oldColor = pDCMem->SetTextColor(m_crText);
	pDCMem->DrawText(strText,rectText,dwFlags);

	// Restore DC's State
//	pDCMem->SetBkMode(nMode);
	pDCMem->SelectObject(pOldFont);
	pDCMem->SetTextColor(oldColor);

	dc.BitBlt(0,0,rc.Width(),rc.Height(),pDCMem,0,0,SRCCOPY);
	// continue DC restore 
	pDCMem->SelectObject ( pOldBitmap ) ;
	delete pDCMem;
	pDCMem = NULL;
}



BOOL CTextBox::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}