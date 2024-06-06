#pragma once

#include "afxcmn.h"
#include "WndColor.h"
#include "afxwin.h"

#define COLOR_MESSAGES_1				RGB(255,255,255)
#define COLOR_MESSAGES_2				RGB(251,183,185)
#define COLOR_MESSAGES_3				RGB(251,183,185)

#define COLOR_MESSAGES_H1				RGB(  20,240, 20)
#define COLOR_MESSAGES_H2				RGB(240,123, 59)
#define COLOR_MESSAGES_H3				RGB(240,240,  20)
#define COLOR_MESSAGES_H4				RGB(240,240,  20)
#define COLOR_MESSAGES_H5				RGB(240,240,  20)
#define COLOR_MESSAGES_H6				RGB(240,240,  20)
#define COLOR_MESSAGES_H7				RGB(240,240,  20)
#define COLOR_MESSAGES_H8				RGB(240,240,  20)
#define COLOR_MESSAGES_H9				RGB(240,240,  20)
#define COLOR_MESSAGES_H10				RGB(240,240,  20)

#define COLOR_MESSAGES_ARRAY_1			0
#define COLOR_MESSAGES_ARRAY_2			1
#define COLOR_MESSAGES_ARRAY_3			2
#define COLOR_MESSAGES_ARRAY_HL_1		3
#define COLOR_MESSAGES_ARRAY_HL_2		4
#define COLOR_MESSAGES_ARRAY_HL_3		5
#define COLOR_MESSAGES_ARRAY_HL_4		6
#define COLOR_MESSAGES_ARRAY_HL_5		7
#define COLOR_MESSAGES_ARRAY_HL_6		8
#define COLOR_MESSAGES_ARRAY_HL_7		9
#define COLOR_MESSAGES_ARRAY_HL_8		10
#define COLOR_MESSAGES_ARRAY_HL_9		11
#define COLOR_MESSAGES_ARRAY_HL_10		12

#define COLOR_ARRAY_NUMBER_MESSAGES		13
#define COLOR_ARRAY_HIGHLIGHT_MAX		(COLOR_MESSAGES_ARRAY_HL_10 - COLOR_MESSAGES_ARRAY_HL_1)

// CDlgColorMessages dialog

class CDlgColorMessages : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgColorMessages)

public:
	CDlgColorMessages();
	virtual ~CDlgColorMessages();
	virtual BOOL OnInitDialog();
	COLORREF *GetColorArray();
	void	SaveRegistry();

// Dialog Data
	enum { IDD = IDD_DIALOG_COLOR_MESSAGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	COLORREF	Correct(COLORREF color);
	bool		m_bColorWindowUpdate;
	CWndColor	m_wndMessages[COLOR_ARRAY_NUMBER_MESSAGES+1];
	COLORREF	m_colorMessages[COLOR_ARRAY_NUMBER_MESSAGES+1];


	DECLARE_MESSAGE_MAP()
    afx_msg void OnTimer(UINT_PTR nIDEvent);
public:


	afx_msg void OnBnClickedButtonColorMessages1();
	afx_msg void OnBnClickedButtonColorMessages2();
	afx_msg void OnBnClickedButtonColorMessages3();
	afx_msg void OnBnClickedOk();
	CButton m_buttonNormal;
	CButton m_buttonHigh;
	CButton m_buttonHighest;
	CButton m_buttonHighlight1;
	CButton m_buttonHighlight2;
	CButton m_buttonHighlight3;
	CButton m_buttonHighlight4;
	CButton m_buttonHighlight5;
	CButton m_buttonHighlight6;
	CButton m_buttonHighlight7;
	CButton m_buttonHighlight8;
	CButton m_buttonHighlight9;
	CButton m_buttonHighlight10;

	CButton m_buttonOk;
	afx_msg void OnBnClickedButtonColorMessagesHl1();
	afx_msg void OnBnClickedButtonColorMessagesHl2();
	afx_msg void OnBnClickedButtonColorMessagesHl3();
	afx_msg void OnBnClickedButtonColorMessagesHl4();
	afx_msg void OnBnClickedButtonColorMessagesHl5();
	afx_msg void OnBnClickedButtonColorMessagesHl6();
	afx_msg void OnBnClickedButtonColorMessagesHl7();
	afx_msg void OnBnClickedButtonColorMessagesHl8();
	afx_msg void OnBnClickedButtonColorMessagesHl9();
	afx_msg void OnBnClickedButtonColorMessagesHl10();
};
