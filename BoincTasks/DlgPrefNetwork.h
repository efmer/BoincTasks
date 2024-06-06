#pragma once
#include "afxwin.h"
#include "TimeString.h"
#include "WndScheduler.h"

// CDlgPrefNetwork : Property page dialog

class CDlgPrefMain;
class CRpcPreferenceInfo;
class CDlgPrefNetwork : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgPrefNetwork)

// Constructors
public:
	CDlgPrefNetwork();
	virtual ~CDlgPrefNetwork();
	virtual BOOL OnInitDialog();
			void FillDialog(CRpcPreferenceInfo *pPreferenceInfo);
			bool ReadDialog(CRpcPreferenceInfo *pPreferenceInfo);
			void ShowGraphic(bool bEnable);
// Dialog Data
	enum { IDD = IDD_DIALOG_PREFERENCE_NETWORK };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
	void	ShowHideDay(int iDay);

	CTimeString	timeStringDouble;

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
public:
	CDlgPrefMain *m_pDlgPrefMain;

	CEdit ValueNetworkDownloadRate;
	CEdit ValueNetworkUploadRate;
	CEdit ValueNetworkConnectEvery;
	CEdit ValueNetworkAdditionalBuffer;

	CStatic m_textNetworkAdditionalBufferWarning;
	CButton m_checkSkipImageVerification;
	CButton m_checkConfirmBeforeConnecting;
	CButton m_checkDisconnectWhenDone;

	CButton m_checkDay[8];
	CEdit m_editDayOfWeek[8][2];

	CEdit m_editTimeFrom;
	CEdit m_editTimeTo;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckNetworkMonday();
	afx_msg void OnBnClickedCheckNetworkTuesday();
	afx_msg void OnBnClickedCheckNetworkWednesday();
	afx_msg void OnBnClickedCheckNetworkThursday();
	afx_msg void OnBnClickedCheckNetworkFriday();
	afx_msg void OnBnClickedCheckNetworkSaturday();
	afx_msg void OnBnClickedCheckNetworkSunday();
	afx_msg void OnBnClickedClear();
	CStatic m_textMaxDownloadRate;
	CStatic m_textMaxUploadRate;
	CStatic m_textConnectEvery;
	CStatic m_textAdditionalWorkBuffer;
	CStatic m_textGeneralOptions;
	CStatic m_textKbs1;
	CStatic m_textKbs2;
	CStatic m_textDays1;
	CStatic m_textDaysMax;
	CStatic m_textConnectionOptions;
	CStatic m_textNetworkUsageAllowed;
	CStatic m_textEveryDayBetween;
	CStatic m_textAnd;
	CStatic m_textNoRestrictionsIfEqual;
	CStatic m_textDayOfWeekOverride;
	CStatic m_textHourMinutes;
	CButton m_buttonClear;
	CButton m_buttonOk;
	CButton m_buttonCancel;
	CStatic m_textTransferAtMost;
	CStatic m_textDaysEvery;
	CEdit ValueNetworkTransfer;
	CStatic m_textMBytesEvery;
	CEdit ValueNetworkTransferDays;
	CButton m_checkGraphic;
	CWndScheduler m_window;
	afx_msg void OnBnClickedCheckGraphical();

	CStatic m_divider1;
	CStatic m_divider2;
	CStatic m_divider3;
	CStatic m_divider4;
	CStatic m_divider5;
	CStatic m_divider6;
	CStatic m_divider7;
};
