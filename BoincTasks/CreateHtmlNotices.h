#pragma once



// CCreateHtmlNotices command target


class CNoticesInfoItem;

//class NOTICES;
class CCreateHtmlNotices : public CObject
{
public:
	CCreateHtmlNotices();
	virtual ~CCreateHtmlNotices();

	bool	Create(std::deque<CNoticesInfoItem*> *pNotices, CString *psFileHtml, CString *psStringHtml, char *pcComputer, double  dTimeShowFrom);
	bool	Create(CString *sTxt,  CString *psHtml);
	void	ReplaceNotice(CNoticesInfoItem* pNoticesInfoItem, CString *psInsertOneNotice, CString *psInsertMoreLink, CString *psInsertImageStyle);
	void	AddLineBreak(CString *psCheck);


};


