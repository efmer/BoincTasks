#pragma once
#include "afxwin.h"

// CDlgSettingsTransfer dialog

class CDlgSettingsTransfer : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSettingsTransfer)

public:
	CDlgSettingsTransfer();
	virtual ~CDlgSettingsTransfer();
	virtual BOOL OnInitDialog();
	void	ReadColumsFromRegistry();
	void	WriteColumsToRegistry();

	void	TransferColumns();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS_TRANSFER};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg		LRESULT OnSettingsTransferGet(WPARAM parm1, LPARAM parm2);
	afx_msg		LRESULT OnSettingsTransferSet(WPARAM parm1, LPARAM parm2);
	afx_msg		void OnBnClickedOk();

	int			m_iActiveColumns;
	int			m_iTransferColumnIn[NUM_REMOTE_TRANSFER_COLUMNS+1];
	int			m_iTransferColumn[NUM_REMOTE_TRANSFER_COLUMNS+1];
	int			m_iTransferColumnLookup[NUM_REMOTE_TRANSFER_COLUMNS+1];

	CBitmap m_bmp;

	DECLARE_MESSAGE_MAP()
public:
	CButton m_buttonOk;
};
