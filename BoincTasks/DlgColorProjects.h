#pragma once

#include "afxcmn.h"
#include "WndColor.h"
#include "afxwin.h"

#define COLOR_PROJECT_NORMAL		RGB(255,255,255)
#define COLOR_PROJECT_SUSPENDED		RGB(192,192,192)
#define COLOR_PROJECT_NONEWWORK		RGB(192,192,192)	

#define COLOR_ARRAY_PROJECT_NORMAL		0
#define COLOR_ARRAY_PROJECT_SUSPENDED	1
#define COLOR_ARRAY_PROJECT_NONEWWORK	2

#define COLOR_ARRAY_NUMBER_PROJECTS 3

// CDlgColorProjects dialog

class CDlgColorProjects : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgColorProjects)

public:
	CDlgColorProjects();
	virtual ~CDlgColorProjects();
	virtual BOOL OnInitDialog();
	COLORREF *GetColorArray();
	void	SaveRegistry();


// Dialog Data
	enum { IDD = IDD_DIALOG_COLOR_PROJECTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	COLORREF	Correct(COLORREF color);
	bool		m_bColorWindowUpdate;
	CWndColor	m_wndProjects[COLOR_ARRAY_NUMBER_PROJECTS+1];
	COLORREF	m_colorProjects[COLOR_ARRAY_NUMBER_PROJECTS+1];

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButtonColorProject();
	afx_msg void OnBnClickedButtonColorProjectSuspended();
	afx_msg void OnBnClickedButtonColorProjectNoNew();
public:

	afx_msg void OnBnClickedOk();
	CButton m_buttonRunning;
	CButton m_buttonSuspended;
	CButton m_buttonNoNewTasks;
	CButton m_buttonOk;
};
