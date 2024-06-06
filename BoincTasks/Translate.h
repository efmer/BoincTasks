#pragma once

// CTranslate command target

class CTranslate : public CObject
{
public:
	CTranslate();
	virtual ~CTranslate();

	void	TranslateBody(CString *psBody, bool bUtf8=false);
	bool	Translate(CString *psTranslate, bool bUtf8 = false);
	bool	TranslateWWW(CString *psTranslate);
};


