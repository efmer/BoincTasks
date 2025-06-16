#pragma once

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif

// CDlgHtmlAccept dialog

class CDlgHtmlAccept : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDlgHtmlAccept)

public:
	CDlgHtmlAccept(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgHtmlAccept();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlgHtmlAccept, IDH = 119 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
