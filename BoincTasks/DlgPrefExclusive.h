#pragma once
#include "afxcmn.h"
#include "ListCtrlRules.h"
#include "afxwin.h"

#define NUM_EXCLUSIVE_COLUMNS 3

// CDgPrefExclusive dialog

class CRpcConfig;
class CDlgPrefMain;
class CDgPrefExclusive : public CPropertyPage
{
	DECLARE_DYNAMIC(CDgPrefExclusive)

public:
	CDgPrefExclusive();
	virtual ~CDgPrefExclusive();
	virtual BOOL OnInitDialog();

//			void FillDialog(CRpcConfig *pConfig);
			bool ReadDialog(CRpcConfig *pConfig);

			void AddColumns();
// Dialog Data
	enum { IDD = IDD_DIALOG_PREFERENCE_EXCLUSIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

			void EnableButtons(BOOL bEnable);

			bool	m_bChanged;


//	CImageList m_StateImageList;

	DECLARE_MESSAGE_MAP()
public:
	CDlgPrefMain *m_pDlgPrefMain;

	CListCtrlRules m_list;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAddExclusive();
	afx_msg void OnBnClickedButtonDeleteExclusive();
	afx_msg void OnBnClickedCancel();
	CButton m_buttonAdd;
	CButton m_buttonDelete;
	CButton m_buttonOk;
	CButton m_buttonCancel;
	CButton m_buttonEdit;
	afx_msg void OnBnClickedButtonEditExclusive();
	afx_msg void OnLvnItemchangedListExclude(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnFillDialog(WPARAM parm1, LPARAM parm2);
};
