#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "WndGraphicTransfer.h"
#include "ThreadRpc.h"
#include "DlgLogging.h"
#include "ColorListBox.h"

// CDlgGraphicTransfer dialog

class CComputerGraphicTransferListItem
{
public:
	CThreadRpc	*m_pThreadRpc;
	int			m_ThreadRpcId;
	char		*m_pcThreadRpcId;

	DAILY_XFER_HISTORY *pDailyXferHistory;
	
//	std::deque<CGraphicXfer *> m_lXfer;
};

class CRemoteCombinedDoc;
class CThreadRpc;
//class PROJECT;
class CDlgGraphicColor;
class CDlgGraphicTransfer : public CDialog
{
	DECLARE_DYNAMIC(CDlgGraphicTransfer)

public:
	CDlgGraphicTransfer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGraphicTransfer();
	void	SafeWindowPlacement();
	void	InitialUpdate(CRemoteCombinedDoc *pDoc, bool bShowWindow = false);

//	int		m_iExpand;

// Dialog Data
	enum { IDD = IDD_DIALOG_GRAPHIC_TRANSFER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CBitmap m_bmp;

	void		ShowControls(bool bShow);
	void		DisplayStatus(int iStatus);
	void		UpdateGraphic();
	void		AddComputer(CString sComputer, COLORREF color, int iRepeat);
	void		ResetTransfers();

	bool		m_bStartUpdate;
	bool		m_bShowWindow;
	int			m_iShowWindowDelay;

	std::deque<CComputerGraphicTransferListItem*>	m_lComputer;

	int			m_iAddComputer;
	int			m_iNrThreadReturn;

	CRemoteCombinedDoc *m_pDoc;

	bool		m_bInvalidate;
	int			m_iTimer;
	int			m_iTimerEnd;
	bool		m_bThreadBusy;

	bool		m_bUpdate;


	HICON		m_hIcon;
	CWndGraphicTransfer m_window;
	int			m_iWindowInitialLeftMargin;
	int			m_iWindowInitialTopMargin;

	CButton		m_buttonUpdate;
	CButton		m_checkSingleMultiple;


	CEdit		m_editLineThickness;

	CStatic		m_textLineThickness;
	CStatic		m_textComputers;

	CColorListBox m_listComputers;

	int			m_iLineThickness;
	bool		m_bShowUp;
	bool		m_bShowDown;
	bool		m_bBlockMode;

	bool		m_bFullScreenGraph;
	LONG		m_lLastWidth;
	LONG		m_lLastHeight;

	CString		m_sMagic;
	bool		m_bDebugMode;

	bool		m_bTimerActive;

	CLoggingMessage m_sLog;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void  OnMouseMove(UINT, CPoint);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);

	afx_msg LRESULT OnInitialGraphic(WPARAM parm1, LPARAM parm2);
	afx_msg	LRESULT OnCloseGraphic(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnGraphicsReady(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnColorChanged(WPARAM parm1, LPARAM parm2);
public:
	afx_msg void OnLbnSelchangeListGraphicComputers();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonColors();

	afx_msg void OnEnChangeEditGraphicLine();

	afx_msg LRESULT OnDoubleClick(WPARAM parm1, LPARAM parm2);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckGraphicSingleMultiple();

	afx_msg void OnBnClickedCheckGraphicTransferDown();
	afx_msg void OnBnClickedCheckGraphicTransferUp();
	CButton m_checkDown;
	CButton m_checkUp;
	CButton m_checkBar;
	afx_msg void OnBnClickedCheckGraphicBlock();

	CButton m_buttonHelp;
	afx_msg void OnBnClickedHelp();
};
