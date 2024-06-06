// InPlaceEdit.h : header file
//

#define WM_INPLACE_READY WM_USER+1

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit window

class CListViewEx;
class CInPlaceEdit : public CEdit
{
// Construction
public:
	CInPlaceEdit(CWnd *pWnd,int iItem, int iSubItem, CString sInitText);

	CString GetText();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInPlaceEdit();
	CString m_sInitText;
	BOOL    m_bESC;	 	// To indicate whether ESC key was pressed
	int		m_iItem;
	int		m_iSubItem;
	bool	m_bPassword;
	bool	m_bSecondHidden;
	CString m_sHiddenText;

	CListViewEx *m_pView;

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceEdit)
//	afx_msg void OnKillFocus(CWnd* pNewWnd);
//	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CWnd *m_pWnd;



};
