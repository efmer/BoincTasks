// DlgColorTemperature.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgColorMain.h"
#include "DlgColorTemperature.h"
#include "Translation.h"


// CDlgColorTemperature dialog

IMPLEMENT_DYNAMIC(CDlgColorTemperature, CPropertyPage)

CDlgColorTemperature::CDlgColorTemperature()
	: CPropertyPage(CDlgColorTemperature::IDD)
{

}

CDlgColorTemperature::~CDlgColorTemperature()
{
}

BOOL CDlgColorTemperature::OnInitDialog()
{
	CPropertySheet *pSheet;
	CString			sTxt;

	CPropertyPage::OnInitDialog();

	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_1]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu1, COLOR_CORE0);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_2]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu2, COLOR_CORE1);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_3]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu3, COLOR_CORE2);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_4]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu4, COLOR_CORE3);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_5]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu5, COLOR_CORE4);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_6]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu6, COLOR_CORE5);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_7]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu7, COLOR_CORE6);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_8]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu8, COLOR_CORE7);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_9]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu9, COLOR_CORE8);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_10]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu10, COLOR_CORE9);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_11]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu11, COLOR_CORE10);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_12]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu12, COLOR_CORE11);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_13]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu13, COLOR_CORE12);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_14]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu14, COLOR_CORE13);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_15]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu15, COLOR_CORE14);
	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_16]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureCpu16, COLOR_CORE15);

	m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_1]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureGpu1, COLOR_GPU0);
	m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_2]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureGpu2, COLOR_GPU1);
	m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_3]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureGpu3, COLOR_GPU2);
	m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_4]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureGpu4, COLOR_GPU3);
	m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_5]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureGpu5, COLOR_GPU4);
	m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_6]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureGpu6, COLOR_GPU5);
	m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_7]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureGpu7, COLOR_GPU6);
	m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_8]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureGpu8, COLOR_GPU7);

	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_THROTTLE]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureThrottleCpu, COLOR_CPUP);
	m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_THROTTLE]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureThrottleGpu, COLOR_GPUP);

	m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_MAX]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureMax, COLOR_CPU_MAX);
	m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_MAX]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureMaxGpu, COLOR_GPU_MAX);

	m_colorTemperature[COLOR_TEMPERATURE_ARRAY_BACK]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureBack, COLOR_TEMPERATURE_BACK);
	m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID_FINE]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureGridFine, COLOR_TEMPERATURE_GRID_FINE);
	m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureGrid, COLOR_TEMPERATURE_GRID);
	m_colorTemperature[COLOR_TEMPERATURE_ARRAY_TEXT]	= theApp.GetProfileInt(registrySectionColor, registryColorTemperatureText, COLOR_TEMPERATURE_TEXT);

	m_bColorWindowUpdate= true;

// Translation
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogColorTitleTemperature];
    pTabCtrl->SetItem(iSel,&tcItem);

	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "0";m_buttonCore0.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "1";m_buttonCore1.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "2";m_buttonCore2.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "3";m_buttonCore3.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "4";m_buttonCore4.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "5";m_buttonCore5.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "6";m_buttonCore6.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "7";m_buttonCore7.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "8";m_buttonCore8.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "9";m_buttonCore9.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "10";m_buttonCore10.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "11";m_buttonCore11.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "12";m_buttonCore12.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "13";m_buttonCore13.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "14";m_buttonCore14.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureCore];sTxt += "15";m_buttonCore15.SetWindowText(sTxt);

	sTxt = gszTranslation[PosDialogColorTemperatureGpu];sTxt += "0";m_buttonGpu0.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureGpu];sTxt += "1";m_buttonGpu1.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureGpu];sTxt += "2";m_buttonGpu2.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureGpu];sTxt += "3";m_buttonGpu3.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureGpu];sTxt += "4";m_buttonGpu4.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureGpu];sTxt += "5";m_buttonGpu5.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureGpu];sTxt += "6";m_buttonGpu6.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureGpu];sTxt += "7";m_buttonGpu7.SetWindowText(sTxt);

	m_buttonTThrottleCpu.SetWindowText(gszTranslation[PosDialogColorTemperatureTCpu]);
	m_buttonTThrottleGpu.SetWindowText(gszTranslation[PosDialogColorTemperatureTGpu]);

	sTxt = gszTranslation[PosDialogColorTemperatureMax];sTxt += " Cpu";m_buttonMaxCpu.SetWindowText(sTxt);
	sTxt = gszTranslation[PosDialogColorTemperatureMax];sTxt += " Gpu";m_buttonMaxGpu.SetWindowText(sTxt);

	m_buttonBackground.SetWindowText(gszTranslation[PosDialogColorTemperatureBackground]);;
	m_buttonGridFine.SetWindowText(gszTranslation[PosDialogColorTemperatureGridFine]);;
	m_buttonGrid.SetWindowText(gszTranslation[PosDialogColorTemperatureGrid]);;
	m_buttonText.SetWindowText(gszTranslation[PosDialogColorTemperatureText]);;


	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
// Translation

	m_hWndGraphicTemperature = NULL;

	SetTimer(UTIMER_DLG_COLOR_TEMPERATURE, 1000, 0);	// interval 1 seconds

	return TRUE;  // return
}

COLORREF *CDlgColorTemperature::GetColorArray()
{
	return &m_colorTemperature[0];
}

void CDlgColorTemperature::SaveRegistry()
{
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu1 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_1]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu2 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_2]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu3 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_3]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu4 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_4]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu5 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_5]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu6 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_6]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu7 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_7]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu8 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_8]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu9 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_9]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu10 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_10]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu11 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_11]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu12 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_12]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu13 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_13]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu14 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_14]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu15 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_15]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu16 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_16]);

	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu1 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_1]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu2 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_2]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu3 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_3]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu4 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_4]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu5 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_5]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu6 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_6]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu7 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_7]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu8 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_8]);

	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureThrottleCpu , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_THROTTLE]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureThrottleGpu , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_THROTTLE]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureMax , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_MAX]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureMaxGpu , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_MAX]);

	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureBack	, m_colorTemperature[COLOR_TEMPERATURE_ARRAY_BACK]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGridFine, m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID_FINE]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGrid	, m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID]);
	theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureText	, m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID_FINE]);

	m_bColorWindowUpdate= true;
}

void CDlgColorTemperature::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE0, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_1]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE1, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_2]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE2, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_3]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE3, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_4]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE4, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_5]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE5, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_6]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE6, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_7]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE7, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_8]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE8, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_9]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE9, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_10]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE10, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_11]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE11, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_12]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE12, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_13]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE13, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_14]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE14, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_15]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_CORE15, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_16]);

	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_GPU0, m_wndTemperature[COLOR_TEMPERATURE_GPU_ARRAY_1]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_GPU1, m_wndTemperature[COLOR_TEMPERATURE_GPU_ARRAY_2]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_GPU2, m_wndTemperature[COLOR_TEMPERATURE_GPU_ARRAY_3]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_GPU3, m_wndTemperature[COLOR_TEMPERATURE_GPU_ARRAY_4]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_GPU4, m_wndTemperature[COLOR_TEMPERATURE_GPU_ARRAY_5]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_GPU5, m_wndTemperature[COLOR_TEMPERATURE_GPU_ARRAY_6]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_GPU6, m_wndTemperature[COLOR_TEMPERATURE_GPU_ARRAY_7]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_GPU7, m_wndTemperature[COLOR_TEMPERATURE_GPU_ARRAY_8]);

	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_TT_CPU, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_THROTTLE]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_TT_GPU, m_wndTemperature[COLOR_TEMPERATURE_GPU_ARRAY_THROTTLE]);

	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_MAX, m_wndTemperature[COLOR_TEMPERATURE_CPU_ARRAY_MAX]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_MAX_GPU, m_wndTemperature[COLOR_TEMPERATURE_GPU_ARRAY_MAX]);

	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_BACK, m_wndTemperature[COLOR_TEMPERATURE_ARRAY_BACK]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_GRID_FINE, m_wndTemperature[COLOR_TEMPERATURE_ARRAY_GRID_FINE]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_GRID, m_wndTemperature[COLOR_TEMPERATURE_ARRAY_GRID]);
	DDX_Control(pDX, IDC_WND_COLOR_TEMPERATURE_TEXT, m_wndTemperature[COLOR_TEMPERATURE_ARRAY_TEXT]);

	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE0, m_buttonCore0);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE1, m_buttonCore1);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE2, m_buttonCore2);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE3, m_buttonCore3);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE4, m_buttonCore4);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE5, m_buttonCore5);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE6, m_buttonCore6);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE7, m_buttonCore7);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE8, m_buttonCore8);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE9, m_buttonCore9);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE10, m_buttonCore10);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE11, m_buttonCore11);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE12, m_buttonCore12);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE13, m_buttonCore13);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE14, m_buttonCore14);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_CORE15, m_buttonCore15);

	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_GPU0, m_buttonGpu0);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_GPU1, m_buttonGpu1);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_GPU2, m_buttonGpu2);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_GPU3, m_buttonGpu3);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_GPU4, m_buttonGpu4);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_GPU5, m_buttonGpu5);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_GPU6, m_buttonGpu6);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_GPU7, m_buttonGpu7);

	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_TT_CPU, m_buttonTThrottleCpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_TT_GPU, m_buttonTThrottleGpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_MAX, m_buttonMaxCpu);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_MAX_GPU, m_buttonMaxGpu);
	
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_BACK, m_buttonBackground);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_GRID_FINE, m_buttonGridFine);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_GRID, m_buttonGrid);
	DDX_Control(pDX, IDC_BUTTON_COLOR_TEMPERATURE_TEXT, m_buttonText);

	DDX_Control(pDX, IDOK, m_buttonOk);
}


BEGIN_MESSAGE_MAP(CDlgColorTemperature, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE0, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore0)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE1, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore1)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE2, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore2)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE3, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore3)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE4, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore4)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE5, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore5)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE6, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore6)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE7, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore7)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE8, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore8)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE9, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore9)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE10, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore10)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE11, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore11)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_GPU0, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu0)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_GPU1, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu1)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_GPU2, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu2)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_GPU3, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu3)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_GPU4, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu4)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_GPU5, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu5)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_GPU6, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu6)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_GPU7, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu7)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_TT_CPU, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureTtCpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_TT_GPU, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureTtGpu)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_MAX, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureMax)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_MAX_GPU, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureMaxGpu)

	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_BACK, OnBnClickedButtonColorTemperatureBack)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_GRID_FINE, OnBnClickedButtonColorTemperatureGridFine)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_GRID, OnBnClickedButtonColorTemperatureGrid)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_TEXT, OnBnClickedButtonColorTemperatureText)

	ON_BN_CLICKED(IDOK, &CDlgColorTemperature::OnBnClickedOk)

	ON_MESSAGE(UWM_GRAPHIC_COLOR_CHANGED, OnColorChanged)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE12, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore12)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE13, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore13)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE14, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore14)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_TEMPERATURE_CORE15, &CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore15)
END_MESSAGE_MAP()


// CDlgColorTemperature message handlers

void CDlgColorTemperature::OnTimer(UINT_PTR nIDEvent)
{
	int iCount;
	if (m_bColorWindowUpdate)
	{
		::PostMessage(m_hWndGraphicTemperature,UWM_GRAPHIC_COLOR_CHANGED,0,0);
		m_bColorWindowUpdate = false;
		for (iCount = 0; iCount < COLOR_ARRAY_NUMBER_TEMPERATURE; iCount++)
		{
			m_wndTemperature[iCount].SetColor(m_colorTemperature[iCount]);
			m_wndTemperature[iCount].Invalidate(TRUE);
		}
	}
}

void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore0()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_1],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_1] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu1 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_1]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore1()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_2],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_2] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu2 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_2]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore2()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_3],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_3] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu3 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_3]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore3()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_4],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_4] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu4 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_4]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore4()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_5],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_5] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu5 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_5]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore5()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_6],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_6] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu6 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_6]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore6()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_7],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_7] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu7 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_7]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore7()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_8],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_8] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu8 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_8]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore8()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_9],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_9] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu9 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_9]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore9()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_10],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_10] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu10 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_10]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore10()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_11],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_11] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu11 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_11]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore11()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_12],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_12] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu12 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_12]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

//xxx

void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore12()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_13],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_13] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu13 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_13]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore13()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_14],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_14] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu14 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_14]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore14()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_15],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_15] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu15 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_15]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureCore15()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_16],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_16] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureCpu16 , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_16]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}
//xxx

void CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu0()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_1],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_1] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu1 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_1]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu1()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_2],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_2] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu2 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_2]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu2()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_3],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_3] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu3 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_3]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu3()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_4],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_4] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu4 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_4]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu4()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_5],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_5] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu5 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_5]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu5()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_6],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_6] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu6 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_6]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu6()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_7],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_7] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu7 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_7]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureGpu7()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_8],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_8] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGpu8 , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_8]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTemperature::OnBnClickedButtonColorTemperatureTtCpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_THROTTLE],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_THROTTLE] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureThrottleCpu , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_THROTTLE]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureTtGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_THROTTLE],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_THROTTLE] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureThrottleGpu , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_THROTTLE]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTemperature::OnBnClickedButtonColorTemperatureMax()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_MAX],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_MAX] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureMax , m_colorTemperature[COLOR_TEMPERATURE_CPU_ARRAY_MAX]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTemperature::OnBnClickedButtonColorTemperatureMaxGpu()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_MAX],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_MAX] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureMaxGpu , m_colorTemperature[COLOR_TEMPERATURE_GPU_ARRAY_MAX]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}


void CDlgColorTemperature::OnBnClickedButtonColorTemperatureBack()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_ARRAY_BACK],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_ARRAY_BACK] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureBack , m_colorTemperature[COLOR_TEMPERATURE_ARRAY_BACK]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTemperature::OnBnClickedButtonColorTemperatureGridFine()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID_FINE],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID_FINE] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGridFine , m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID_FINE]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}
void CDlgColorTemperature::OnBnClickedButtonColorTemperatureGrid()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureGrid , m_colorTemperature[COLOR_TEMPERATURE_ARRAY_GRID]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

void CDlgColorTemperature::OnBnClickedButtonColorTemperatureText()
{
	this->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	CColorDialog dlg(m_colorTemperature[COLOR_TEMPERATURE_ARRAY_TEXT],CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_colorTemperature[COLOR_TEMPERATURE_ARRAY_TEXT] = Correct(dlg.GetColor());
		m_bColorWindowUpdate= true;
		theApp.WriteProfileInt(registrySectionColor,registryColorTemperatureText , m_colorTemperature[COLOR_TEMPERATURE_ARRAY_TEXT]);
	}
	this->SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
}

COLORREF CDlgColorTemperature::Correct(COLORREF color)
{
	int r,g,b;

	r = color >> 16; 
	g = (color >> 8) & 0xff;
	b = color & 0xff;

	if ((r) < 20) r = 20;
	if ((g) < 20) g = 20;
	if ((b) < 20) b = 20;

	color = (r << 16) | (g << 8) | b;
	return color;

}
void CDlgColorTemperature::OnBnClickedOk()
{
	theApp.m_pDlgColorMain->ShowWindow(SW_HIDE);
	CPropertyPage::OnOK();
}

LRESULT CDlgColorTemperature::OnColorChanged(WPARAM wparm, LPARAM lparm)
{
//	HWND hWnd;

	m_hWndGraphicTemperature = (HWND) wparm;					// now we know the hWnd of the graphic dialog.
	::PostMessage(m_hWndGraphicTemperature,UWM_GRAPHIC_COLOR_CHANGED,0,0);

	return 0;
}



