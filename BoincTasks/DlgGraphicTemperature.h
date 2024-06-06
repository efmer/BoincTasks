#pragma once

#include "resource.h"
#include "RemoteCombinedDoc.h"
#include "wndgraphictemperature.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "WndColor.h"
#include "ThreadRpcClient.h"

#define STOP_TEMPERATURE	-1


#define SLIDER_VALUE		MAX_SAMPLE_BUFFER_GRAPHIC_SCALABLE/30
#define SLIDER_VALUE_FINE	1000


#define GRAPHIC_TEMPERATURE_CPU_ARRAY_1 CORE00
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_2 CORE01
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_3 CORE02
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_4 CORE03
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_5 CORE04
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_6 CORE05
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_7 CORE06
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_8 CORE07
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_9 CORE08
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_10 CORE09
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_11 CORE10
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_12 CORE11
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_13 CORE12
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_14 CORE13
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_15 CORE14
#define GRAPHIC_TEMPERATURE_CPU_ARRAY_16 CORE15

#define GRAPHIC_TEMPERATURE_GPU_ARRAY_1 GPU0
#define	GRAPHIC_TEMPERATURE_GPU_ARRAY_2 GPU1
#define GRAPHIC_TEMPERATURE_GPU_ARRAY_3 GPU2
#define GRAPHIC_TEMPERATURE_GPU_ARRAY_4 GPU3
#define GRAPHIC_TEMPERATURE_GPU_ARRAY_5 GPU4
#define GRAPHIC_TEMPERATURE_GPU_ARRAY_6 GPU5
#define GRAPHIC_TEMPERATURE_GPU_ARRAY_7 GPU6
#define GRAPHIC_TEMPERATURE_GPU_ARRAY_8 GPU7

#define GRAPHIC_TEMPERATURE_CPU_ARRAY_THROTTLE	THROTTLE_CPU
#define GRAPHIC_TEMPERATURE_GPU_ARRAY_THROTTLE	THROTTLE_GPU

#define GRAPHIC_TEMPERATURE_CPU_ARRAY_LIMIT		LIMIT_CPU
#define GRAPHIC_TEMPERATURE_GPU_ARRAY_LIMIT		LIMIT_GPU


#define GRAPHIC_ARRAY_NUMBER_TEMPERATURE	TEMPERATURE_ARRAY


// CDlgGraphicTemperature dialog

class CDlgGraphicTemperature : public CDialog
{
	DECLARE_DYNAMIC(CDlgGraphicTemperature)

public:
	CDlgGraphicTemperature(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGraphicTemperature();
	void	InitialUpdate(CRemoteCombinedDoc *pDoc);
	void	Init();
	void	SafeWindowPlacement();
	void	ComputerSelectionChanged();
	void	ShowControls(bool bShow);

	void	AddTemperatureToList(int iComputer, int iCore, float fTemperature);
	void	AddTemperature(int iComputer, int iCore, float fTemperature);

// Dialog Data
	enum { IDD = IDD_DIALOG_GRAPHIC_TEMPERATURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
			BOOL OnHelpInfo( HELPINFO* HelpInfo);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Check(int iCheckItem);

	HICON	m_hIcon;
	CBitmap m_bmp;

	CRemoteCombinedDoc *m_pDoc;

	bool				m_bInvalidate;
	int					m_iInvalidateEvery;

	int					m_iSelectedComputers;
	std::deque<CRpcConnectionItem*>	m_lListConnectionsItems;
	std::deque<CTemperatureGraphItem*>	m_lListTemperature;
	CTemperatureGraphBuffer	m_temperatureGraphBuffer;
	int					m_iSelectedTemperature;

	CWndGraphicTemperature	m_window;

	int					m_iWindowInitialLeftMargin;
	int					m_iWindowInitialTopMargin;

	bool				m_bAddCoresList[TEMPERATURE_ARRAY];
	bool				m_bAddStringDone;
	bool				m_bInit;

//	UINT				*m_piTemperature;
	int					m_iTemperatureCount;
	bool				m_bThreadLocked;
	int					m_iThreadLocked;
	bool				m_bTimerFastMode;

//	time_t				m_tLastAddTime;

	CString				m_sSelectedComputer;
	int					m_iSelectedThreadId;

	bool				m_bActive[TEMPERATURE_ARRAY];
	bool				m_bColorWindowUpdate;

	bool				m_bColorTemperatureDialogKnowsUs;
	bool				m_bComputerSelectionChanged;

	bool				m_bFullScreenGraph;
	LONG				m_lLastWidth;
	LONG				m_lLastHeight;

	int					m_iInvalideWait;
	bool				m_bTimerActive;


	DECLARE_MESSAGE_MAP()



	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	void SetWindowPosition(LONG lWidth, LONG lHeight, UINT nType);
	afx_msg LRESULT OnColorChanged(WPARAM wparm, LPARAM lparm);
public:
	afx_msg void OnBnClickedHide();

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeGraphicTemperatureComboTime();
	afx_msg LRESULT OnRpcConnection(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadTemperature(WPARAM wParam, LPARAM lParam);

	CSliderCtrl m_slider;
	CSliderCtrl m_sliderFine;
	CListBox m_listComputers;
	CComboBox m_setTime;

	afx_msg void OnLbnSelchangeListGraphicTemperatureComputers();

	CButton m_check[GRAPHIC_ARRAY_NUMBER_TEMPERATURE];
	CWndColor m_wndColor[GRAPHIC_ARRAY_NUMBER_TEMPERATURE];

	afx_msg void OnBnClickedGraphicTemperatureCheckCpu0();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu1();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu2();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu3();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu4();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu5();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu6();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu7();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu8();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu9();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu10();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu11();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu12();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu13();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu14();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpu15();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpuP();
	afx_msg void OnBnClickedGraphicTemperatureCheckCpuMax();
	afx_msg void OnBnClickedGraphicTemperatureCheckGpu0();
	afx_msg void OnBnClickedGraphicTemperatureCheckGpu1();
	afx_msg void OnBnClickedGraphicTemperatureCheckGpu2();
	afx_msg void OnBnClickedGraphicTemperatureCheckGpu3();
	afx_msg void OnBnClickedGraphicTemperatureCheckGpu4();
	afx_msg void OnBnClickedGraphicTemperatureCheckGpu5();
	afx_msg void OnBnClickedGraphicTemperatureCheckGpu6();
	afx_msg void OnBnClickedGraphicTemperatureCheckGpu7();
	afx_msg void OnBnClickedGraphicTemperatureCheckGpuP2();
	afx_msg void OnBnClickedGraphicTemperatureCheckGpuMax();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	CButton m_buttonHelp;
	afx_msg void OnBnClickedHelp();
};
