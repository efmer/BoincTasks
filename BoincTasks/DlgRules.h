#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "WndColor.h"
#include "WndScheduler.h"
#include "Rules.h"



// CDlgRules dialog

class CRuleItem;
class CDlgRules : public CDialog
{
	DECLARE_DYNAMIC(CDlgRules)

public:
	CDlgRules(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRules();
	virtual BOOL OnInitDialog();
	void	SetItems(int iPosition, CRuleItem *pRuleItem);

	int		m_iType[RULE_ITEMS+1];
	int		m_iTypeClass[RULE_ITEMS+1];
	int		m_iOperator[RULE_ITEMS+1];
	CString m_sValue[RULE_ITEMS+1];
	_int64	m_iValue[RULE_ITEMS+1];
	double	m_dValue[RULE_ITEMS+1];

	_int64  m_iTime;

	_int64	m_iSnooze;

	int		m_iEventInternal;
	int		m_iEventExternal;
	CString	m_sEvent;
	int		m_iEventShow;

	CString	m_sName;
	CString	m_sComputer;
	CString m_sProject;
	CString m_sApplication;

	CString m_sRule;

	COLORREF	m_color;

	bool		m_bSetup;
	bool		m_bEdit;

	CRuleItem	*m_pRuleItem;						// don't use is only a copy
	std::deque<CRuleItem*> *m_plRules;
	std::deque<CRuleIntervalItem *> *m_plInterval;


// Dialog Data
	enum { IDD = IDD_DIALOG_RULES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void	SetInterval(CWndScheduler *pWndScheduler, std::deque<CRuleIntervalItem *> *plInterval);

	bool	CheckDouble(CString *psTxt);
	bool	CheckEmpty(CString *psTxt);
	bool	CheckEvent(CString *psTxt);
	void	AddRuleTypes(int iNr, CListBox *pListBox);
	void	CheckAndFillOperator(int iPosition, CListBox *pListRulesType,CListBox *pListRulesOperator);
	void	CheckOperator(CListBox *pListRulesOperator, int *piOperator);
	void	CheckForIncompatible();
	bool	CheckParameters(CString *psError);
	void	ClassifyType(CString *psText, int *piType, int *piTypeClass);

	bool	CheckDigits(CString *psTxt);
	bool	GetTime(CString *psValue, _int64 *pi64Time);
	bool	GetUse(CString *psUse, double *pdValue, __int64 *piValue);
	bool	GetStatus(CString *psStatus, double *pdStatus,__int64 *piCount);

	bool	GetRule(int iPosition, CString *psError);
	void	ShowRule(int iPosition, CString *psRule);
	void	ShowRuleRest(CString *psRule);
	bool	ValidRules();

	void	SelectListboxType(CListBox *pListBox, CString *pTxt);
	void	SelectListboxOperator(CListBox *pListBox, CString *pTxt);
	void	SelectListBoxEvent(int iEvent);

	void	ShowScheduler(bool bShow);

	HICON	m_hIcon;

    afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg LRESULT OnButton(WPARAM parm1, LPARAM parm2);
	afx_msg LRESULT OnNewInterval(WPARAM parm1, LPARAM parm2);

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listRulesType1;
	CListBox m_listRulesType2;
	CListBox m_listRulesType3;
	CListBox m_listRulesOperator1;
	CListBox m_listRulesOperator2;
	CListBox m_listRulesOperator3;
	CStatic m_textName;
	CStatic m_textComputer;
	CStatic m_textProject;
	CStatic m_textApplication;
	afx_msg void OnLbnSelchangeListRulesType1();
	afx_msg void OnLbnSelchangeListRulesType2();
	afx_msg void OnLbnSelchangeListRulesType3();
	afx_msg void OnLbnSelchangeListRulesOperator1();
	afx_msg void OnLbnSelchangeListRulesOperator2();
	afx_msg void OnLbnSelchangeListRulesOperator3();
	CEdit m_editValue1;
	CEdit m_editValue2;
	CEdit m_editValue3;
	CEdit m_editRulesStatus;
	afx_msg void OnBnClickedButtonRulesCheck();
	afx_msg void OnBnClickedButtonRulesAdd();
	afx_msg void OnBnClickedCancel();

	CEdit m_editName;
	CEdit m_editComputer;
	CEdit m_editProject;
	CEdit m_editApplication;
	CEdit m_editTime1;
	CButton m_buttonCheck;
	CButton m_buttonAdd;
	CButton m_buttonCancel;

	afx_msg void OnBnClickedButtonRulesColor();
	CButton m_buttonColor;
	CWndColor m_wndColor;
	CListBox m_listEvent;
	afx_msg void OnLbnSelchangeListRulesEvent();
	CEdit m_editEvent;
	CButton m_checkShowMessage;
	CButton m_checkShowMessageNotice;
	CButton m_checkShowMessageNone;

	afx_msg void OnBnClickedCheckRulesShow();
	CStatic m_textGroupType;
	CStatic m_textGroupOperator;
	CStatic m_textGroupValue;
	CStatic m_textGroupTime;
	CStatic m_textGroupEvent;

	CWndScheduler	m_windowScheduler;

	afx_msg void OnBnClickedCheckRulesShowNotice();

	afx_msg void OnBnClickedCheckRulesShowNone();
};
