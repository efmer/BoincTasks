#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "WndGraphic.h"
//#include "ComputerDoc.h"
#include "ThreadRpc.h"
#include "DlgLogging.h"
#include "ColorListBox.h"


//#define GRAPHIC_TEST_XML 1

// CDlgGraphic dialog

#define GRAPHIC_HOST	0
#define GRAPHIC_USER	1
#define GRAPHIC_TOTAL	0
#define GRAPHIC_AVERAGE	1


#define GRAPHIC_GET_LIST		0
#define GRAPHIC_GET_PROJECT		1
#define GRAPHIC_GET_PROJECT_MODE 2
#define GRAPHIC_SET_PROJECT_MODE 3
#define GRAPHIC_SELECT_PROJECT	4
#define GRAPHIC_GET_COMPUTER	5
#define GRAPHIC_SELECT_COMPUTER	6
#define GRAPHIC_GET_TYPE		7
#define GRAPHIC_SET_TYPE		8

#define	GRAPHIC_TYPE_USER		"__U__"
#define	GRAPHIC_TYPE_COMPUTER	"__C__"
#define	GRAPHIC_TYPE_TOTAL		"__T__"
#define	GRAPHIC_TYPE_AVERATE	"__A__"

class CComputerGraphicType
{
public:
	int m_iHostOrUser;
	int m_iTotalOrAverage;
};

class CSelectedItem
{
public:
	CString		m_sName;
	bool		m_bSelected;
};

class CComputerGraphicListItem
{
public:
	CThreadRpc	*m_pThreadRpc;
	int			m_ThreadRpcId;
	char		*m_pcThreadRpcId;

	PROJECT		*m_pProjects[MAX_PROJECTS+1];
	CString		m_sThreadRpcUrl[MAX_PROJECTS+1];
	CString		m_sThreadRpcName[MAX_PROJECTS+1];
};

class CRemoteCombinedDoc;
class CThreadRpc;
class PROJECT;
class CDlgGraphicColor;
class CDlgGraphic : public CDialog
{
	DECLARE_DYNAMIC(CDlgGraphic)

public:
	CDlgGraphic(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGraphic();
	void	SafeWindowPlacement();
	void	InitialUpdate(CRemoteCombinedDoc *pDoc, bool bShowWindow = false);

	int		m_iExpand;

// Dialog Data
	enum { IDD = IDD_DIALOG_GRAPHIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CBitmap m_bmp;

	void		ShowControls(bool bShow);
	void		AddTest(PROJECT *pProjectStatistics);
	void		PostCommandView(int iCommand);
	void		DisplayStatus(int iStatus);
	void		UpdateGraphic();
	void		ResetProjects();
	void		CheckOneGraph();
	void		SortWebItem(std::deque<CSelectedItem*> *plWebItem);
	void		AddProject(CString sComputer, CString sProject, COLORREF color, int iRepeat);

	bool		m_bStartUpdateStatistics;		// start triggers m_bRunUpdateStatistics
//	bool		m_bRunUpdateStatistics;			// this one does the actual update
	bool		m_bShowWindow;
	int			m_iShowWindowDelay;

	std::deque<CComputerGraphicListItem*>	m_lComputer;
	std::deque<CSelectedItem*>				m_lWebComputers;
	std::deque<CSelectedItem*>				m_lWebProjects;
	CComputerGraphicType					m_webType;
	bool		m_bWebProjectsSingle;

	int			m_iNrThreadRpc;
	int			m_iNrThreadReturn;

	CRemoteCombinedDoc *m_pDoc;

	bool		m_bInvalidate;
	int			m_iTimer;
	int			m_iTimerEnd;
	bool		m_bThreadBusy;

	bool		m_bProjectsOneGraph;
	bool		m_bCheckCombineProjects;


	HICON		m_hIcon;
//	CBitmap		m_bmp;
	CWndGraphic m_window;
	int			m_iWindowInitialLeftMargin;
	int			m_iWindowInitialTopMargin;

//	CButton		m_colors;
	CButton		m_radioUser;
	CButton		m_radioTotal;
	CButton		m_radioHost;
	CButton		m_radioAverage;
	CButton		m_buttonUpdate;
	CButton		m_checkCombineProject;
	CButton		m_checkSingleMultiple;
	CButton		m_checkExpanded;


	CEdit		m_editLineThickness;

	CStatic		m_textLineThickness;
	CStatic		m_textDebugMode;
	CStatic		m_textComputers;
	CStatic		m_textProjects;

	CColorListBox m_listProjects;
	CColorListBox m_listComputers;


	int			m_iUserHost;
	int			m_iTotalAverage;
	int			m_iLineThickness;

	bool		m_bFullScreenGraph;
	LONG		m_lLastWidth;
	LONG		m_lLastHeight;

//	CDlgGraphicColor	*m_pDlgColor;

	CString		m_sMagic;
	bool		m_bDebugMode;
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
	afx_msg LRESULT GetGraphic(WPARAM parm1, LPARAM parm2);
public:
	afx_msg void OnLbnSelchangeListGraphicComputers();
	afx_msg void OnLbnSelchangeListGraphicProjects();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedRadioGraphicUser();
	afx_msg void OnBnClickedRadioGraphicTotal();
	afx_msg void OnBnClickedRadioGraphicHost();
	afx_msg void OnBnClickedRadioGraphicAverage();
	afx_msg void OnBnClickedButtonColors();

	afx_msg void OnEnChangeEditGraphicLine();
	afx_msg void OnBnClickedCheckGraphicProjectsSame();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckGraphicSingleMultiple();

	afx_msg void OnBnClickedCheckGraphicExtended();

	CButton m_buttonHelp;
	afx_msg void OnBnClickedHelp();
};
