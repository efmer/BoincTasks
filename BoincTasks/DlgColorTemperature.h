#pragma once
#include "afxcmn.h"
#include "WndColor.h"
#include "afxwin.h"

//wit
#define COLOR_CORE0 RGB(255,255,255)
//l geel
#define COLOR_CORE1 RGB(255,255,128)
//l blauw
#define COLOR_CORE2 RGB(128,255,255)
//l paars
#define COLOR_CORE3 RGB(255,128,255)
//l grijs
#define COLOR_CORE4	RGB(192,192,192)
//oranje
#define COLOR_CORE5	RGB(255,128,0)
//d blauw
#define COLOR_CORE6 RGB(0,255,255)
//d paars
#define COLOR_CORE7 RGB(128,0,255)
#define COLOR_CORE8 RGB(138,0,255)
#define COLOR_CORE9 RGB(148,0,255)
#define COLOR_CORE10 RGB(158,0,255)
#define COLOR_CORE11 RGB(168,0,255)
#define COLOR_CORE12 RGB(178,0,255)
#define COLOR_CORE13 RGB(188,0,255)
#define COLOR_CORE14 RGB(198,0,255)
#define COLOR_CORE15 RGB(208,0,255)

#define COLOR_GPU0  RGB(255,198,140)
#define COLOR_GPU1	RGB(159,159,0)
#define COLOR_GPU2  RGB(255,198,140)
#define COLOR_GPU3	RGB(159,159,0)
#define COLOR_GPU4	RGB(159,159,0)
#define COLOR_GPU5	RGB(159,159,0)
#define COLOR_GPU6	RGB(159,159,0)
#define COLOR_GPU7	RGB(159,159,0)

#define COLOR_CPUP	RGB(205,205,205)
#define COLOR_GPUP	RGB(205,205,128)

#define	COLOR_CPU_MAX RGB(255,000,000)
#define	COLOR_GPU_MAX RGB(255,128,064)

#define COLOR_TEMPERATURE_BACK		RGB(30,30,30)
#define COLOR_TEMPERATURE_GRID_FINE	RGB(0,100,0)
#define COLOR_TEMPERATURE_GRID		RGB(0,200,0)
#define COLOR_TEMPERATURE_TEXT		RGB(255,255,255)

#define COLOR_TEMPERATURE_CPU_ARRAY_1 CORE00
#define COLOR_TEMPERATURE_CPU_ARRAY_2 CORE01
#define COLOR_TEMPERATURE_CPU_ARRAY_3 CORE02
#define COLOR_TEMPERATURE_CPU_ARRAY_4 CORE03
#define COLOR_TEMPERATURE_CPU_ARRAY_5 CORE04
#define COLOR_TEMPERATURE_CPU_ARRAY_6 CORE05
#define COLOR_TEMPERATURE_CPU_ARRAY_7 CORE06
#define COLOR_TEMPERATURE_CPU_ARRAY_8 CORE07
#define COLOR_TEMPERATURE_CPU_ARRAY_9 CORE08
#define COLOR_TEMPERATURE_CPU_ARRAY_10 CORE09
#define COLOR_TEMPERATURE_CPU_ARRAY_11 CORE10
#define COLOR_TEMPERATURE_CPU_ARRAY_12 CORE11
#define COLOR_TEMPERATURE_CPU_ARRAY_13 CORE12
#define COLOR_TEMPERATURE_CPU_ARRAY_14 CORE13
#define COLOR_TEMPERATURE_CPU_ARRAY_15 CORE14
#define COLOR_TEMPERATURE_CPU_ARRAY_16 CORE15

#define COLOR_TEMPERATURE_GPU_ARRAY_1 GPU0
#define COLOR_TEMPERATURE_GPU_ARRAY_2 GPU1
#define COLOR_TEMPERATURE_GPU_ARRAY_3 GPU2
#define COLOR_TEMPERATURE_GPU_ARRAY_4 GPU3
#define COLOR_TEMPERATURE_GPU_ARRAY_5 GPU4
#define COLOR_TEMPERATURE_GPU_ARRAY_6 GPU5
#define COLOR_TEMPERATURE_GPU_ARRAY_7 GPU6
#define COLOR_TEMPERATURE_GPU_ARRAY_8 GPU7

#define COLOR_TEMPERATURE_CPU_ARRAY_THROTTLE	THROTTLE_CPU 
#define COLOR_TEMPERATURE_GPU_ARRAY_THROTTLE	THROTTLE_GPU

#define COLOR_TEMPERATURE_CPU_ARRAY_MAX		LIMIT_CPU
#define COLOR_TEMPERATURE_GPU_ARRAY_MAX		LIMIT_GPU

#define COLOR_TEMPERATURE_ARRAY_BACK		(LIMIT_GPU+1)
#define COLOR_TEMPERATURE_ARRAY_GRID_FINE	(LIMIT_GPU+2)
#define COLOR_TEMPERATURE_ARRAY_GRID		(LIMIT_GPU+3)
#define COLOR_TEMPERATURE_ARRAY_TEXT		(LIMIT_GPU+4)



#define COLOR_ARRAY_NUMBER_TEMPERATURE	COLOR_TEMPERATURE_ARRAY_TEXT+1

// CDlgColorTemperature dialog

class CDlgColorTemperature : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgColorTemperature)

public:
	CDlgColorTemperature();
	virtual ~CDlgColorTemperature();
	virtual BOOL OnInitDialog();
	COLORREF *GetColorArray();
	void	SaveRegistry();

// Dialog Data
	enum { IDD = IDD_DIALOG_COLOR_TEMPERATURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	COLORREF	Correct(COLORREF color);
	bool		m_bColorWindowUpdate;
	CWndColor	m_wndTemperature[COLOR_ARRAY_NUMBER_TEMPERATURE+1];
	COLORREF	m_colorTemperature[COLOR_ARRAY_NUMBER_TEMPERATURE+1];

	HWND		m_hWndGraphicTemperature;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg LRESULT OnColorChanged(WPARAM wparm, LPARAM lparm);

	afx_msg void OnBnClickedButtonColorTemperatureCore0();
	afx_msg void OnBnClickedButtonColorTemperatureCore1();
	afx_msg void OnBnClickedButtonColorTemperatureCore2();
	afx_msg void OnBnClickedButtonColorTemperatureCore3();
	afx_msg void OnBnClickedButtonColorTemperatureCore4();
	afx_msg void OnBnClickedButtonColorTemperatureCore5();
	afx_msg void OnBnClickedButtonColorTemperatureCore6();
	afx_msg void OnBnClickedButtonColorTemperatureCore7();
	afx_msg void OnBnClickedButtonColorTemperatureGpu0();
	afx_msg void OnBnClickedButtonColorTemperatureGpu1();
	afx_msg void OnBnClickedButtonColorTemperatureGpu2();
	afx_msg void OnBnClickedButtonColorTemperatureGpu3();
	afx_msg void OnBnClickedButtonColorTemperatureGpu4();
	afx_msg void OnBnClickedButtonColorTemperatureGpu5();
	CButton m_buttonCore0;
	CButton m_buttonCore1;
	CButton m_buttonCore2;
	CButton m_buttonCore3;
	CButton m_buttonCore4;
	CButton m_buttonCore5;
	CButton m_buttonCore6;
	CButton m_buttonCore7;
	CButton m_buttonCore8;
	CButton m_buttonCore9;
	CButton m_buttonCore10;
	CButton m_buttonCore11;
	CButton m_buttonCore12;
	CButton m_buttonCore13;
	CButton m_buttonCore14;
	CButton m_buttonCore15;

	CButton m_buttonGpu0;
	CButton m_buttonGpu1;
	CButton m_buttonGpu2;
	CButton m_buttonGpu3;
	CButton m_buttonGpu4;
	CButton m_buttonGpu5;
	CButton m_buttonGpu6;
	CButton m_buttonGpu7;

	CButton m_buttonTThrottleCpu;
	CButton m_buttonTThrottleGpu;
	CButton m_buttonMaxCpu;
	CButton m_buttonMaxGpu;

	CButton m_buttonOk;

	CButton m_buttonBackground;
	CButton m_buttonGridFine;
	CButton m_buttonGrid;
	CButton m_buttonText;


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonColorTemperatureTtCpu();
	afx_msg void OnBnClickedButtonColorTemperatureTtGpu();
	afx_msg void OnBnClickedButtonColorTemperatureMax();
	afx_msg void OnBnClickedButtonColorTemperatureBack();
	afx_msg void OnBnClickedButtonColorTemperatureGridFine();
	afx_msg void OnBnClickedButtonColorTemperatureGrid();
	afx_msg void OnBnClickedButtonColorTemperatureText();

	afx_msg void OnBnClickedButtonColorTemperatureCore8();
	afx_msg void OnBnClickedButtonColorTemperatureCore9();
	afx_msg void OnBnClickedButtonColorTemperatureCore10();
	afx_msg void OnBnClickedButtonColorTemperatureCore11();
	afx_msg void OnBnClickedButtonColorTemperatureGpu6();
	afx_msg void OnBnClickedButtonColorTemperatureGpu7();
	afx_msg void OnBnClickedButtonColorTemperatureMaxGpu();
	afx_msg void OnBnClickedButtonColorTemperatureCore12();
	afx_msg void OnBnClickedButtonColorTemperatureCore13();
	afx_msg void OnBnClickedButtonColorTemperatureCore14();
	afx_msg void OnBnClickedButtonColorTemperatureCore15();
};
