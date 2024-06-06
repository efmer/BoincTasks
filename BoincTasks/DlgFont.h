#pragma once
#include "afxwin.h"

#define FONT_TYPE_GENERAL	0
#define FONT_TYPE_GADGET	1

// CDlgFont dialog

class CDlgFont : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgFont)

public:
	CDlgFont();
	virtual ~CDlgFont();
	virtual BOOL OnInitDialog();

//	void	SetFont();
//	void	SetFontGadget();
//	void	SetFontSize();
//	void	SetFontSizeGadget();

	void	SetFont(bool bSystemFont, BYTE *pcBuffer, BYTE *pcBufferGadget);

	void	SendFont();
	void	SendFontGadget();

	bool	m_bSystemFont;

	LOGFONT		m_logFont;
	CFont		*m_pFont;

	LOGFONT		m_logFontGadget;
	CFont		*m_pFontGadget;

// Dialog Data
	enum { IDD = IDD_DIALOG_FONT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

//	void	SetLogFont(LOGFONT *pLf, CString *psFontName, int iFontSize, int iFontCharSet);
	void	SetEditFont(CEdit *pEdit,LOGFONT *pLf, CFont **pFont);
	void	ShowHideSystem();

	void	ProcessFont();
	void	ProcessFontGadget();
	static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam);

	CEdit	m_editFont;
	CEdit	m_editFontGadget;

	CStatic m_textGroupGeneral;
	CStatic m_textGroupGadget;

	CButton m_buttonGeneral;
	CButton m_buttonGadget;
	CButton m_checkUsesSystem;
	CButton m_buttonOk;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnCbnSelchangeComboFont();
	afx_msg void OnCbnSelchangeComboFontSize();
	afx_msg void OnCbnSelchangeComboFontGadget();
	afx_msg void OnCbnSelchangeComboFontGadgetSize();
	afx_msg LRESULT OnChangedFontInitial(WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedFontGeneral();
	afx_msg void OnBnClickedFontGadget();
	afx_msg void OnBnClickedCheckFontSystem();
};
