#pragma once
#include "afxdialogex.h"


// CDlgAccept dialog

class CDlgAccept : public CDialog
{
	DECLARE_DYNAMIC(CDlgAccept)

public:
	CDlgAccept(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgAccept();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlgAccept };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	CString mText;

	DECLARE_MESSAGE_MAP()
public:
	void AddText(CString);
};
