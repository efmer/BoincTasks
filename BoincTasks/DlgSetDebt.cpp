// DlgSetDebt.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "DlgSetDebt.h"
#include "Translation.h"
#include "ThreadRpc.h"
#include "afxdialogex.h"


// CDlgSetDebt dialog

IMPLEMENT_DYNAMIC(CDlgSetDebt, CDialogEx)

CDlgSetDebt::CDlgSetDebt(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetDebt::IDD, pParent)
{
	m_pProject = NULL;
	m_pProjectsDiskUsage = new CRpcProjectsDiskUsage;
}

CDlgSetDebt::~CDlgSetDebt()
{
	if (m_pProject != NULL) delete m_pProject;
	if (m_pProjectsDiskUsage != NULL) delete m_pProjectsDiskUsage;
}

BOOL CDlgSetDebt::OnInitDialog()
{
	CDialog::OnInitDialog();

// Translation
	
	m_textSetDebtCpuLong.SetWindowText(gszTranslation[PosDialogSetDebtCpuLong]);
	m_textSetDebtNVidiaLong.SetWindowText(gszTranslation[PosDialogSetDebtNVidiaLong]);
	m_textSetDebtAtiLong.SetWindowText(gszTranslation[PosDialogSetDebtAtiLong]);
	m_textSetDebtCpuShort.SetWindowText(gszTranslation[PosDialogSetDebtCpuShort]);
	m_textSetDebtNVidiaShort.SetWindowText(gszTranslation[PosDialogSetDebtNVidiaShort]);
	m_textSetDebtAtiShort.SetWindowText(gszTranslation[PosDialogSetDebtAtiShort]);

	m_buttonApply.SetWindowText(gszTranslation[PosDialogCommonButtonsApply]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgSetDebt::GetCurrentSelection()
{
	int	iSel, iItems;
	CString sProject, sFormat;
	PROJECT *pProject;

	iSel = m_listProjects.GetCurSel();
	if (iSel >=0) m_listProjects.GetText(iSel, sProject);

	if (sProject.GetLength() > 0 )
	{
		iItems = (int) m_pProject->projects.size();
		for (int iCount=0; iCount < iItems; iCount++)
		{
			pProject = m_pProject->projects[iCount];
			if (pProject->project_name.c_str() == sProject)
			{
				sFormat.Format("%f",pProject->cpu_long_term_debt);
				m_editSetDebtCpuLong.SetWindowText(sFormat);
				sFormat.Format("%f",pProject->cuda_debt);
				m_editSetDebtNVidiaLong.SetWindowText(sFormat);
				sFormat.Format("%f",pProject->ati_debt);
				m_editSetDebtAtiLong.SetWindowText(sFormat);
				sFormat.Format("%f",pProject->cpu_short_term_debt);
				m_editSetDebtCpuShort.SetWindowText(sFormat);
				sFormat.Format("%f",pProject->cuda_short_term_debt);
				m_editSetDebtNVidiaShort.SetWindowText(sFormat);
				sFormat.Format("%f",pProject->ati_short_term_debt);
				m_editSetDebtAtiShort.SetWindowText(sFormat);
			}
		}
	}
}

void CDlgSetDebt::ApplyButton()
{
	CString sTxt;
	BOOL	bEnable;

	bEnable = FALSE;

	m_editSetDebtCpuShortSet.GetWindowText(sTxt);

	if (sTxt.GetLength() > 0) bEnable = TRUE;
	
	if (bEnable)
	{
		bEnable = FALSE;
		m_editSetDebtCpuLongSet.GetWindowText(sTxt);
		if (sTxt.GetLength() > 0) bEnable = TRUE;
	}

	m_buttonApply.EnableWindow(bEnable);
}

void CDlgSetDebt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_SET_DEBT_CPU_LONG, m_textSetDebtCpuLong);
	DDX_Control(pDX, IDC_TEXT_SET_DEBT_NVIDIA_LONG, m_textSetDebtNVidiaLong);
	DDX_Control(pDX, IDC_TEXT_SET_DEBT_ATI_LONG, m_textSetDebtAtiLong);
	DDX_Control(pDX, IDC_TEXT_SET_DEBT_CPU_SHORT, m_textSetDebtCpuShort);
	DDX_Control(pDX, IDC_TEXT_SET_DEBT_NVIDIA_SHORT, m_textSetDebtNVidiaShort);
	DDX_Control(pDX, IDC_TEXT_SET_DEBT_ATI_SHORT, m_textSetDebtAtiShort);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_CPU_LONG, m_editSetDebtCpuLong);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_CPU_LONG_SET, m_editSetDebtCpuLongSet);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_NVIDIA_LONG, m_editSetDebtNVidiaLong);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_NVIDIA_LONG_SET, m_editSetDebtNVidiaLongSet);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_ATI_LONG, m_editSetDebtAtiLong);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_ATI_LONG_SET, m_editSetDebtAtiLongSet);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_CPU_SHORT, m_editSetDebtCpuShort);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_CPU_SHORT_SET, m_editSetDebtCpuShortSet);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_NVIDIA_SHORT, m_editSetDebtNVidiaShort);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_NVIDIA_SHORT_SET, m_editSetDebtNVidiaShortSet);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_ATI_SHORT, m_editSetDebtAtiShort);
	DDX_Control(pDX, IDC_EDIT_SET_DEBT_ATI_SHORT_SET, m_editSetDebtAtiShortSet);
	DDX_Control(pDX, IDOK, m_buttonApply);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_LIST_SET_DEBT_PROJECTS, m_listProjects);
}


BEGIN_MESSAGE_MAP(CDlgSetDebt, CDialogEx)
	ON_MESSAGE(UWM_MSG_SET_DEBT_OPEN_DIALOG, OnOpenDialog)
	ON_MESSAGE(UWM_MSG_SET_DEBT_SET_THREAD_ID, OnSetThreadId)
	ON_MESSAGE(UWM_MSG_THREAD_READY_RPC_READ_PROJECT, OnReadProject)
	ON_MESSAGE(UWM_MSG_THREAD_RPC_SET_PROJECT_DEBT_READY, OnSetProjectDebtReady)

	ON_BN_CLICKED(IDOK, &CDlgSetDebt::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSetDebt::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST_SET_DEBT_PROJECTS, &CDlgSetDebt::OnLbnSelchangeListSetDebtProjects)
	ON_EN_CHANGE(IDC_EDIT_SET_DEBT_CPU_LONG_SET, &CDlgSetDebt::OnEnChangeEditSetDebtCpuLongSet)
	ON_EN_CHANGE(IDC_EDIT_SET_DEBT_CPU_SHORT_SET, &CDlgSetDebt::OnEnChangeEditSetDebtCpuShortSet)
END_MESSAGE_MAP()


// CDlgSetDebt message handlers


void CDlgSetDebt::OnBnClickedOk()
{
	PROJECT *pProject;
	int		iItems, iSel;
	CString sProject, sTxt;

	iSel = m_listProjects.GetCurSel();
	m_listProjects.GetText(iSel, sProject);

	iItems = (int) m_pProject->projects.size();

	for (int iCount=0; iCount < iItems; iCount++)
	{
		pProject = m_pProject->projects[iCount];
		if (pProject->project_name.c_str() == sProject)
		{
			m_sProject = sProject;
			m_editSetDebtCpuLongSet.GetWindowText(sTxt);
			pProject->cpu_long_term_debt = atof(sTxt);
			m_editSetDebtNVidiaLongSet.GetWindowText(sTxt);
			pProject->cuda_debt = atof(sTxt);
			m_editSetDebtAtiLongSet.GetWindowText(sTxt);
			pProject->ati_debt = atof(sTxt);
			m_editSetDebtCpuShortSet.GetWindowText(sTxt);
			pProject->cpu_short_term_debt = atof(sTxt);
			m_editSetDebtNVidiaShortSet.GetWindowText(sTxt);
			pProject->cuda_short_term_debt = atof(sTxt);
			m_editSetDebtAtiShortSet.GetWindowText(sTxt);
			pProject->ati_short_term_debt = atof(sTxt);
			::PostThreadMessage(m_iThreadId, UWM_MSG_THREAD_RPC_SET_PROJECT_DEBT, (WPARAM) this->m_hWnd, (LPARAM) pProject);
			return;
		}
	}
}

void CDlgSetDebt::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}


LRESULT CDlgSetDebt::OnOpenDialog(WPARAM parm1, LPARAM parm2)
{
	CString *psComputer, *psProject;
	CString sTxt;

	psProject = (CString *) parm1;
	psComputer = (CString *) parm2;

	sTxt = gszTranslation[PosGroupDialogSetDebt];
	sTxt+= " ";
	sTxt+= *psComputer;

	this->SetWindowText(sTxt);
	
	m_sProject = *psProject;
	m_sComputer = *psComputer;

	m_editSetDebtCpuShortSet.SetWindowText("");
	m_editSetDebtCpuLongSet.SetWindowText("");

	return 0;
}

LRESULT CDlgSetDebt::OnSetThreadId(WPARAM parm1, LPARAM parm2)
{
	m_iThreadId = (int) parm1;

	if (m_pProject == NULL)
	{
		m_pProject = new PROJECTS;
	}

	m_pProjectsDiskUsage->pDiskUsage = NULL;
	m_pProjectsDiskUsage->pProjects = m_pProject;

	::PostThreadMessage(m_iThreadId, UWM_MSG_THREAD_START_RPC_READ_PROJECT, (WPARAM) this->m_hWnd, (LPARAM) m_pProjectsDiskUsage);
	return 0;
}
	
LRESULT CDlgSetDebt::OnReadProject(WPARAM parm1, LPARAM parm2)
{
	int		iStatus, iItems;
	bool	bSelected;
	CRpcThreadReturn *pThreadReturn;
	VERSION_INFO	*pVersion;
	bool	bDebtActive;

	iStatus = (int) parm1;


	pThreadReturn = (CRpcThreadReturn *) parm2;
	pVersion = pThreadReturn->m_pBoincVersion;

	bDebtActive = true;
	if (pVersion->major == 6)
	{
		if (pVersion->minor >= 13)
		{
			bDebtActive = false;
		}
	}
	if (pVersion->major > 6)
	{
		bDebtActive = false;
	}

	delete pThreadReturn;

	if (iStatus)
	{
		m_pProject->clear();
	}

	m_listProjects.ResetContent();
	bSelected = false;


	if (!bDebtActive)
	{
		AfxMessageBox("Debt is no longer used in this BOINC version.");
		return 0;
	}

	iItems = (int) m_pProject->projects.size();

	for (int iCount=0; iCount < iItems; iCount++)
	{
		m_listProjects.AddString(m_pProject->projects[iCount]->project_name.c_str());
		if (m_pProject->projects[iCount]->project_name.c_str() == m_sProject)
		{
			m_listProjects.SetCurSel(iCount);
			bSelected = true; 
			// no break here pleas
		}
	}

	m_editSetDebtCpuLong.SetWindowText("");
	m_editSetDebtNVidiaLong.SetWindowText("");
	m_editSetDebtAtiLong.SetWindowText("");
	m_editSetDebtCpuShort.SetWindowText("");
	m_editSetDebtNVidiaShort.SetWindowText("");
	m_editSetDebtAtiShort.SetWindowText("");

	GetCurrentSelection();

	this->ShowWindow(SW_SHOW);

	return 0;
}

LRESULT CDlgSetDebt::OnSetProjectDebtReady(WPARAM parm1, LPARAM parm2)
{
	m_pProjectsDiskUsage->pDiskUsage = NULL;
	m_pProjectsDiskUsage->pProjects = m_pProject;

	::PostThreadMessage(m_iThreadId, UWM_MSG_THREAD_START_RPC_READ_PROJECT, (WPARAM) this->m_hWnd, (LPARAM) m_pProjectsDiskUsage);

	return 0;
}

void CDlgSetDebt::OnLbnSelchangeListSetDebtProjects()
{
	GetCurrentSelection();
}


void CDlgSetDebt::OnEnChangeEditSetDebtCpuLongSet()
{
	ApplyButton();
}


void CDlgSetDebt::OnEnChangeEditSetDebtCpuShortSet()
{
	ApplyButton();
}
