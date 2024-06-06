// Translate.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSettingsView.h"
#include "Translation.h"
#include "Translate.h"
#include "AnsiToUtf8.h"


// CTranslate

CTranslate::CTranslate()
{
}

CTranslate::~CTranslate()
{
}

void CTranslate::TranslateBody(CString *psBody, bool bUtf8)
{
	int iPosBegin, iPosEnd, iPos;
	CString sTranslate;
	CString	sLeft, sRight;

	iPos = 0;

	iPosBegin = psBody->Find("_(\"", iPos);
	while (iPosBegin >= 0)
	{
		iPosEnd = psBody->Find("\")",iPosBegin);
		if (iPosEnd > 0)
		{
			sLeft = psBody->Left(iPosBegin);
			sRight= psBody->Mid(iPosEnd+2);
			sTranslate = psBody->Mid(iPosBegin+3,(iPosEnd-iPosBegin)-3);

			if (theApp.m_pDlgSettingsView->m_bBoincTranslatable)
			{
				if (Translate(&sTranslate, bUtf8))
				{
					*psBody = sLeft + "^" + sTranslate + "^" + sRight;
				}
				else
				{
					*psBody = sLeft + sTranslate + sRight;
				}
			}
			else
			{
				*psBody = sLeft + sTranslate + sRight;
			}
			iPos = sLeft.GetLength();
			iPosBegin = psBody->Find("_(\"", iPos);
		}
		else 
		{
			return;
		}
	}
}

bool CTranslate::Translate(CString *psTranslate, bool bUtf8)
{
	CString sFind, sFindEnglish, sTranslated;
	int iPos, iPosSep;
	bool	bShow;

	bShow = false;
	for (int iCount = PosTranslatableBoincText0; iCount <= PosTranslatableBoincText19; iCount++)
	{
		sFind = gszTranslation[iCount];
		if (sFind == "??")
		{
			bShow = true;
		}
		iPos = sFind.Find(*psTranslate);
		if (iPos == 0)
		{
			iPosSep = sFind.Find("^");
			if (iPosSep > 0)
			{
				sFindEnglish = sFind.Left(iPosSep);
				if (sFindEnglish == *psTranslate)
				{
					sTranslated = sFind.Mid(iPosSep+1);
					if (sTranslated.GetLength() > 0)
					{
						if (bUtf8)
						{
							CAnsiToUtf8 utf8;
							*psTranslate = utf8.ToUtf8(&sTranslated);
						}
						else
						{
							*psTranslate = sTranslated;
						}
						return false;
					}
					else
					{
						return false;		// English
					}
				}
			}
		}
	}

	return bShow;
}

bool CTranslate::TranslateWWW(CString *psTranslate)
{
	CString sFind, sEnglish, sTranslated;
	int iPosSep;

	if (theApp.m_pDlgSettingsView->m_bBoincTranslatable)
	{
		for (int iCount = PosTranslatableWWWText0; iCount <= PosTranslatableWWWText19; iCount++)
		{
			sFind = gszTranslation[iCount];
			iPosSep = sFind.Find("^");
			if (iPosSep > 0)
			{
				sEnglish = sFind.Left(iPosSep);
				sTranslated = sFind.Mid(iPosSep+1);
				psTranslate->Replace(sEnglish,sTranslated);
			}
		}
	}
	return false;
}

// CTranslate member functions
