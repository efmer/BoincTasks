#pragma once
#include "afxcmn.h"
#include "WndTooltip.h"


// CDlgTooltip dialog

class CDlgTooltip : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTooltip)

public:
	CDlgTooltip(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTooltip();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DLG_TOOLTIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CWndTooltip m_window;
};
