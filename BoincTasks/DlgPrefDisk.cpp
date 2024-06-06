// DlgPrefDisk.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgPrefDisk.h"
#include "DlgPrefMain.h"
#include "ThreadRpc.h"
#include "NumberFormat.h"
#include "Translation.h"

// CDlgPrefDisk dialog

IMPLEMENT_DYNAMIC(CDlgPrefDisk, CPropertyPage)

CDlgPrefDisk::CDlgPrefDisk()
	: CPropertyPage(CDlgPrefDisk::IDD)
{

}

CDlgPrefDisk::~CDlgPrefDisk()
{
}

BOOL CDlgPrefDisk::OnInitDialog()
{
	CPropertySheet *pSheet;
	CPropertyPage::OnInitDialog();

	// translations
	pSheet = (CPropertySheet*) CWnd::GetParent();
	CTabCtrl* pTabCtrl = pSheet->GetTabControl();
	int iSel = pTabCtrl->GetCurSel();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = gszTranslation[PosDialogBoincSettingsTitleDisk];
    pTabCtrl->SetItem(iSel,&tcItem);

	m_textDiskUsage.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskTitle]);
	m_textUseAtMost1.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskAtMost]);
	m_textLeaveAtLeast.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskAtLeast]);
	m_textUseAtMost2.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskAtMost]);
	m_textWriteToDiskAtMostEvery.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskWriteEvery]);
	m_textUseAtMost3.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskAtMost]);
	m_textGigabyteDiskSpace.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskGigaDisk]);
	m_textGigabyteDiskSpaceFree.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskGigaDiskFree]);
	m_textPercOfTotalDiskSpace.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskGigaDiskTotal]);
	m_textSeconds.SetWindowText(gszTranslation[PosDialogBoincSettingsGeneralSeconds]);
	m_textPercOfPageFile.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskPage]);

	m_textMemoryUsage.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskMemoryTitle]);
	m_textUseAtMost4.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskAtMost]);
	m_textUseAtMost5.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskDiskAtMost]);
	m_textPercWhenComputerIsInUse.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskMemoryInUse]);
	m_textPercWhenComputerIsIdle.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskMemoryIdle]);
	m_checkMemoryLeaveApplicationInMemory.SetWindowText(gszTranslation[PosDialogBoincSettingsDiskMemoryLeaveIn]);

	m_buttonClear.SetWindowText(gszTranslation[PosDialogCommonButtonsClear]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);
	// translations

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgPrefDisk::FillDialog(CRpcPreferenceInfo *pPreferenceInfo)
{
	//C:\programma\BoincTasks\BoincTasks\boinc\boinc_source\boinc\clientgui\DlgAdvPreferences.cpp
	CString sFormat;
	CNumberFormat numberFormat;
	
	GLOBAL_PREFS *pPrefs;

	pPrefs = &pPreferenceInfo->prefs;

	sFormat = numberFormat.FormatNumberNT(pPrefs->disk_max_used_gb,2);
	ValueDiskUseAtMost.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->disk_min_free_gb,2);
	ValueDiskLeaveAtLeast.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->disk_max_used_pct,2);
	ValueDiskUseAtMostPerc.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->disk_interval,0);
	ValueDiskWriteToDiskEvery.SetWindowText(sFormat);
	
	sFormat = numberFormat.FormatNumberNT(pPrefs->vm_max_used_frac*100.0,2);
	ValueDiskUseAtMostPageFile.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->ram_max_used_busy_frac*100.0,2);
	ValueMemoryUseWhenInUse.SetWindowText(sFormat);

	sFormat = numberFormat.FormatNumberNT(pPrefs->ram_max_used_idle_frac*100.0,2);
	ValueMemoryUseWhenIdle.SetWindowText(sFormat);
	
	m_checkMemoryLeaveApplicationInMemory.SetCheck(pPrefs->leave_apps_in_memory);

}
extern double clamp_pct(double x);

void CDlgPrefDisk::ReadDialog(CRpcPreferenceInfo *pPreferenceInfo)
{
	CString sTxt;
	double	td;
	CNumberFormat numberFormat;

	GLOBAL_PREFS *pPrefs;
	GLOBAL_PREFS_MASK *pMask;

	pPrefs = &pPreferenceInfo->prefs;
	pMask  = &pPreferenceInfo->prefsMask;

	ValueDiskUseAtMost.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
	pPrefs->disk_max_used_gb = td;
	pMask->disk_max_used_gb = true;

	ValueDiskLeaveAtLeast.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
	pPrefs->disk_min_free_gb = td;
	pMask->disk_min_free_gb = true;

	ValueDiskUseAtMostPerc.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
    clamp_pct(td);
	pPrefs->disk_max_used_pct = td;
	pMask->disk_max_used_pct = true;

	ValueDiskWriteToDiskEvery.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
	pPrefs->disk_interval = td;
	pMask->disk_interval = true;

	ValueDiskUseAtMostPageFile.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
    clamp_pct(td);
	td = td / 100.0 ;
	pPrefs->vm_max_used_frac = td;
	pMask->vm_max_used_frac = true;

	ValueMemoryUseWhenInUse.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
    clamp_pct(td);
	td = td / 100.0;
	pPrefs->ram_max_used_busy_frac = td;
	pMask->ram_max_used_busy_frac = true;

	ValueMemoryUseWhenIdle.GetWindowText(sTxt);
	td = numberFormat.atofLocale(sTxt);
    clamp_pct(td);
	td = td / 100.0;
	pPrefs->ram_max_used_idle_frac = td;
	pMask->ram_max_used_idle_frac = true;
	
	pPrefs->leave_apps_in_memory = (m_checkMemoryLeaveApplicationInMemory.GetCheck() == TRUE);
	pMask->leave_apps_in_memory = true;
}

void CDlgPrefDisk::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VALUE_DISK_USE_AT_MOST, ValueDiskUseAtMost);
	DDX_Control(pDX, IDC_VALUE_DISK_LEAVE_AT_LEAST, ValueDiskLeaveAtLeast);
	DDX_Control(pDX, IDC_VALUE_DISK_USE_AT_MOST_PERC, ValueDiskUseAtMostPerc);
	DDX_Control(pDX, IDC_VALUE_DISK_WRITE_TO_DISK_EVERY, ValueDiskWriteToDiskEvery);
	DDX_Control(pDX, IDC_VALUE_DISK_USE_AT_MOST_PAGE_FILE, ValueDiskUseAtMostPageFile);
	DDX_Control(pDX, IDC_VALUE_MEMORY_USE_WHEN_IN_USE, ValueMemoryUseWhenInUse);
	DDX_Control(pDX, IDC_VALUE_MEMORY_USE_WHEN_IDLE, ValueMemoryUseWhenIdle);
	DDX_Control(pDX, IDC_CHECK_MEMORY_LEAVE_APPLICATION_IN_MEMORY, m_checkMemoryLeaveApplicationInMemory);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_TITLE, m_textDiskUsage);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_MEMORY_TITLE, m_textMemoryUsage);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_USE_AT_MOST1, m_textUseAtMost1);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_USE_AT_MOST2, m_textUseAtMost2);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_USE_AT_MOST3, m_textUseAtMost3);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_MEMORY_USE_AT_MOST1, m_textUseAtMost4);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_MEMORY_USE_AT_MOST2, m_textUseAtMost5);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_LEAVE_AT_LEAST, m_textLeaveAtLeast);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_WRITE_AT_MOST, m_textWriteToDiskAtMostEvery);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_GIGA_SPACE, m_textGigabyteDiskSpace);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_GIGA_SPACE_FREE, m_textGigabyteDiskSpaceFree);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_PERC_TOTAL, m_textPercOfTotalDiskSpace);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_SECONDS, m_textSeconds);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_DISK_PERC_SWAP, m_textPercOfPageFile);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_MEMORY_PERC_IN_USE, m_textPercWhenComputerIsInUse);
	DDX_Control(pDX, IDC_TEXT_BOINC_DISK_MEMORY_PERC_IDLE, m_textPercWhenComputerIsIdle);
	DDX_Control(pDX, IDCLEAR, m_buttonClear);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}


BEGIN_MESSAGE_MAP(CDlgPrefDisk, CPropertyPage)
	ON_BN_CLICKED(IDOK, &CDlgPrefDisk::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgPrefDisk::OnBnClickedCancel)
	ON_BN_CLICKED(IDCLEAR, &CDlgPrefDisk::OnBnClickedClear)
END_MESSAGE_MAP()


// CDlgPrefDisk message handlers

void CDlgPrefDisk::OnBnClickedOk()
{
	m_pDlgPrefMain->Ok();
}

void CDlgPrefDisk::OnBnClickedCancel()
{
	m_pDlgPrefMain->Cancel();
}

void CDlgPrefDisk::OnBnClickedClear()
{
	m_pDlgPrefMain->Clear();
}
