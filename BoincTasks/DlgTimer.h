#pragma once



// CDlgTimer dialog

class CDlgTimer : public CDialog
{
	DECLARE_DYNAMIC(CDlgTimer)

public:
	CDlgTimer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTimer();

	void	AttachWnd(CWnd *pWnd);

	CWnd	*m_pAttachedWnd;

// Dialog Data
	enum { IDD = IDD_DIALOG_TIMER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();



	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};
