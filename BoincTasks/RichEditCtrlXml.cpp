// RichEditCtrlXml.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "XmlParser.h"
#include "RichEditCtrlXml.h"


// CRichEditCtrlXml

IMPLEMENT_DYNAMIC(CRichEditCtrlXml, CRichEditCtrl)

CRichEditCtrlXml::CRichEditCtrlXml()
{
	m_pXmlParser = NULL;
	bPaint = true;
}

CRichEditCtrlXml::~CRichEditCtrlXml()
{
	delete m_pXmlParser;
}

//void CRichEditCtrlXml::SetBoincVersion(bool bBoincClientV6)
//{
//	if (bBoincClientV6 != m_bBoincClientV6)
//	{
//		delete m_pXmlParser;
//		m_pXmlParser = NULL;
//		m_bBoincClientV6 = bBoincClientV6;
//	}
//}

bool CRichEditCtrlXml::Check(CString sFile)
{
	CXmlParserInfo parserInfo;
	int		iPos;
	bool	bBufferMore;
	char cBuffer[65000];
	int iLineIndex, iLines, iLen;
	CString sTxt;
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = CFM_COLOR | CFM_BOLD;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.dwEffects &= ~CFM_BOLD;

	bPaint = false;
	HideSelection(TRUE, TRUE);

	SetSel(0,-1);
	cf.crTextColor = RGB(0, 0, 0);
	SetWordCharFormat(cf);

	if (m_pXmlParser == NULL)
	{
		m_pXmlParser = new CXmlParser(sFile);
	}

	m_pXmlParser->Start(&parserInfo);

	iLines = GetLineCount();

	for (int iLine = 0; iLine < iLines; iLine++)
	{
		iPos = 0;
		iLen = LineLength(iLine);
//		char cBuffer[iLen+1];
		int iLen = GetLine(iLine, cBuffer,10000);
		cBuffer[iLen] = 0;
		iLineIndex = LineIndex(iLine);

		bBufferMore = true;
		while (bBufferMore)
		{
			bBufferMore = m_pXmlParser->Parse(&iPos, cBuffer, &parserInfo);
			SetSel(parserInfo.m_iPosBegin + iLineIndex, parserInfo.m_iPosEnd + iLineIndex);
			if (bBufferMore)
			{
				if (parserInfo.m_iStatus != PARSER_VALID_TAG)
				{
					if (parserInfo.m_iStatus != PARSER_VALID_ENDTAG)
					{
						cf.crTextColor = RGB(255, 0, 0);
						SetWordCharFormat(cf);
						bPaint = true;
						HideSelection(FALSE, TRUE);
						SetFocus();
						SetSel(parserInfo.m_iPosEnd + iLineIndex, parserInfo.m_iPosEnd + iLineIndex);
						return false;
					}
				}
				if (parserInfo.m_iStatus == PARSER_VALID_ENDTAG)		// no need to check a valid endtag
				{
					cf.crTextColor = RGB(18, 92, 184);
					SetWordCharFormat(cf);

					if (m_pXmlParser->CheckDefaultValue(&parserInfo))
					{
						SetSel(parserInfo.m_iPosBeginValue + iLineIndex, parserInfo.m_iPosBegin + iLineIndex);
						cf.crTextColor = RGB(0, 81, 40);
						SetWordCharFormat(cf);
					}
				}
				else
				{
					if (!m_pXmlParser->CheckSyntax(&parserInfo))
					{
						cf.crTextColor = RGB(255, 0, 0);
						SetWordCharFormat(cf);
						bPaint = true;
						HideSelection(FALSE, TRUE);
						SetFocus();
						SetSel(parserInfo.m_iPosEnd + iLineIndex, parserInfo.m_iPosEnd + iLineIndex);
						return false;
					} 
					else
					{
						cf.crTextColor = RGB(18, 92, 184);
						SetWordCharFormat(cf);
					}
				}
			}
		}
	}
	bPaint = true;
//	SetSel(-1,-1);
	HideSelection(FALSE, TRUE);
	SetFocus();

	CString sOpen = m_pXmlParser->NotClosed();
	if (sOpen.GetLength() > 0)
	{
		MessageBox("Missing closing tag: "+sOpen, "Tag not closed", MB_ICONERROR | MB_OK);
	}

	return true;
}

BEGIN_MESSAGE_MAP(CRichEditCtrlXml, CRichEditCtrl)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CRichEditCtrlXml message handlers

void CRichEditCtrlXml::OnPaint()
{
//    CClientDC cdc (this); 
//   CRect rcClient; 
//    GetClientRect (rcClient);

//	if (bPaint)
	{
		CRichEditCtrl::OnPaint();
//		CPaintDC dc(this); // device context for painting
	}
}

void CRichEditCtrlXml::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (bPaint)
	{
		CRichEditCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}