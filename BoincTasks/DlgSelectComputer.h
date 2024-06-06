#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ListCtrlSelectComputer.h"

#define SELECT_COMPUTER_NETWORK		0
#define SELECT_COMPUTER_ALLOW		1
#define SELECT_COMPUTER_ALLOW_GPU	2
#define SELECT_COMPUTER_BENCHMARK	3
#define SELECT_COMPUTER_READCONFIG	4
#define SELECT_COMPUTER_SYNCHRONIZE	5

#define NUM_SELECT_COMPUTERS_COLUMNS 2

#define	SELECT_COLUMN_COMPUTER			0
#define	SELECT_COLUMN_STATUS			1


#define SELECT_COMPUTERS

class CRpcConnectionItem;
class CListCtrlSelectComputer;
// CDlgSelectComputer dialog

class CDlgSelectComputer : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSelectComputer)

public:
	CDlgSelectComputer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSelectComputer();

	void	InitialUpdate(std::deque<CRpcConnectionItem*> *list);
	void	ShowType(int iType);
	void	SafeWindowPlacement();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECT_COMPUTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnNetworkOpen(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnAllowOpen(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnAllowGpuOpen(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnBenchmarkOpen(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnReadConfigOpen(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnSynchronizeOpen(WPARAM parm1, LPARAM parm2);

	afx_msg LRESULT OnToolbarAllowNetwork(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnToolbarAllowCpu(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnToolbarAllowGpu(WPARAM parm1, LPARAM parm2);
	
	afx_msg LRESULT OnThreadReadStatusReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnThreadNetworkModeReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnThreadReadSnoozeCpuGpuReady(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnThreadReadHostInfoReady(WPARAM parm1, LPARAM parm2);
	afx_msg	LRESULT OnThreadSynchronizeReady(WPARAM wParam, LPARAM lParam);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void	AddComputers();
	void	UpdateStatusAll();
	void	UpdateStatusNetworkItem(int iPos);
	void	UpdateStatusAllowItem(int iPos, CRpcConnectionItem *pItem, bool bGPU);
	void	UpdateStatusConnected(int iPos, CRpcConnectionItem *pItem);
	void	UpdateStatusBenchmark(int iPos, CRpcConnectionItem *pItem);
	void	UpdateStatusSynchronize(int iPos, CRpcConnectionItem *pItem, int iFound);

	void	InvalidateAllow();
	void	RemoveStatusList();

	void	AddColumns();
	void	ActivateSelected(int iAction);
	void	SendCommand(int iType, int iItem, int iCommand);
	void	SendCommandNetwork(int iItem, int iCommand);
	void	SendCommandAllow(int iItem, int iCommand);
	void	SendCommandAllowGpu(int iItem, int iCommand);
	void	SendCommandReadConfig(int iItem, int iCommand);
	void	SendCommandBenchmark(int iItem, int iCommand);
	void	SendCommandSynchronize(int iItem, int iCommand);

	void	BenchMarkReady();

	HICON	m_hIcon;
	std::deque<CRpcConnectionItem*> *m_pList;
//	std::deque<int> m_iListNetworkMode;
	int		m_iType;
	int		m_iTimerInterval;
	int		m_iTimerIntervalNext;
public:
//	CListBox m_lComputerStatus;
	 CListCtrlSelectComputer m_lComputer;
	 CButton m_button1;
	 CButton m_button2;
	 CButton m_button3;
	 CButton m_button4;

	 afx_msg void OnBnClickedOk();
	 CButton m_buttonOK;
	 afx_msg void OnBnClickedButton1();
	 afx_msg void OnBnClickedButton2();
	 afx_msg void OnBnClickedButton3();
	 afx_msg void OnBnClickedButton4();
};
