#pragma once
#include "XmlParser.h"

// CRichEditCtrlXml

class CRichEditCtrlXml : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CRichEditCtrlXml)

public:
	CRichEditCtrlXml();
	virtual ~CRichEditCtrlXml();

//	void SetBoincVersion(bool bBoincClientV6);
	bool Check(CString sFile);

protected:
	DECLARE_MESSAGE_MAP()

	bool	bPaint;

	CXmlParser *m_pXmlParser;

//	bool	m_bBoincClientV6;

public:
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


