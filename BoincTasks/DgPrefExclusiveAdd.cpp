// DgPrefExclusiveAdd.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "Tlhelp32.h"
#include "Psapi.h"
#include "DgPrefExclusiveAdd.h"
#include "afxdialogex.h"

#include "Translation.h"

// CDgPrefExclusiveAdd dialog

IMPLEMENT_DYNAMIC(CDgPrefExclusiveAdd, CDialogEx)

CDgPrefExclusiveAdd::CDgPrefExclusiveAdd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDgPrefExclusiveAdd::IDD, pParent)
{

}

CDgPrefExclusiveAdd::~CDgPrefExclusiveAdd()
{
}

BOOL CDgPrefExclusiveAdd::OnInitDialog()
{
	CDialog::OnInitDialog();

	// translations
	SetWindowText(gszTranslation[PosDialogBoincSettingsExclusiveTitleAdd]);
	
	m_checkGpu.SetWindowText(gszTranslation[PosDialogBoincSettingsExclusiveGpu]);
	m_buttonOk.SetWindowText(gszTranslation[PosDialogCommonButtonsOk]);
	m_buttonCancel.SetWindowText(gszTranslation[PosDialogCommonButtonsCancel]);

	if (m_bSet)
	{
		m_editProgram.SetWindowText(m_sExe);
		if (m_sCpuGpu == EXCLUSIVE_TEXT_GPU)
		{
			m_checkGpu.SetCheck(TRUE);
		}
	}

	GetProcess();

	return TRUE;  // return TRUE  unless you set the focus to a control
}


static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// If the BFFM_INITIALIZED message is received
	// set the path to the start path.
	switch (uMsg)
	{
		case BFFM_INITIALIZED:
		{
			if (NULL != lpData)
			{
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
			}
		}
	}

	return 0; // The function should always return 0.
}

// HWND is the parent window.
// szCurrent is an optional start folder. Can be NULL.
// szPath receives the selected path on success. Must be MAX_PATH characters in length.
BOOL CDgPrefExclusiveAdd::BrowseForFolder(HWND hwnd, LPCTSTR szCurrent, LPTSTR szPath)
{
	BROWSEINFO   bi = { 0 };
	LPITEMIDLIST pidl;
	TCHAR        szDisplay[MAX_PATH];
	BOOL         retval;

//	CoInitialize();

	bi.hwndOwner      = hwnd;
	bi.pszDisplayName = szDisplay;
	bi.lpszTitle      = gszTranslation[PosDialogBoincSettingsExclusiveSelect];
	bi.ulFlags        = BIF_BROWSEINCLUDEFILES | BIF_NEWDIALOGSTYLE;
	bi.lpfn           = BrowseCallbackProc;
	bi.lParam         = (LPARAM) szCurrent;

	pidl = SHBrowseForFolder(&bi);

	if (NULL != pidl)
	{
		retval = SHGetPathFromIDList(pidl, szPath);
		if (strchr(szDisplay,'.') != NULL)
		{
			strcpy_s(szPath, MAX_PATH, szDisplay);
		}
		else 
		{
			szPath[0] = 0;
		}
		CoTaskMemFree(pidl);
	}
	else
	{
		retval = FALSE;
	}

	if (!retval)
	{
		szPath[0] = TEXT('\0');
	}

//	CoUninitialize();
	return retval;
}

bool CDgPrefExclusiveAdd::GetProcess()
{
	CString sFileName;
	HANDLE hProcessSnap, hProcess;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		CloseHandle( hProcessSnap );			// clean the snapshot object
		return false;
	}

	// Now walk the snapshot of processes
	do
	{
		CString sProcessName;
		sProcessName = pe32.szExeFile;
		sProcessName.MakeLower();

		hProcess = OpenProcess( MAXIMUM_ALLOWED, FALSE, pe32.th32ProcessID );
		if (hProcess != NULL)
		{
			char cBufferExe[1024];
			cBufferExe[0] = 0;

			if (GetModuleFileNameEx(hProcess,  NULL,  cBufferExe, 1023) > 0)
			{
				sFileName = cBufferExe;
				sFileName.MakeLower();
				if (sFileName.GetLength() > 0)
				{
					int iPos = sFileName.Find("system32");
					if (iPos < 0)
					{
						m_combo_process.AddString(sProcessName);
					}
				}
			}
			CloseHandle( hProcess );
		}

	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );

	return true;
}

CString CDgPrefExclusiveAdd::GetProcessFileName(CString sProcessFind)
{
	CString sFileName;

	HANDLE hProcessSnap, hProcess;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return "";
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		CloseHandle( hProcessSnap );			// clean the snapshot object
		return "";
	}

	// Now walk the snapshot of processes
	do
	{
		CString sProcessName;
		sProcessName = pe32.szExeFile;
		sProcessName.MakeLower();

		if (sProcessName == sProcessFind)
		{
			hProcess = OpenProcess( MAXIMUM_ALLOWED, FALSE, pe32.th32ProcessID );
			if (hProcess != NULL)
			{
				char cBufferExe[1024];
				cBufferExe[0] = 0;
				if (GetModuleFileNameEx(hProcess,  NULL,  cBufferExe, 1023) > 0)
				{
					sFileName = cBufferExe;
				}
				CloseHandle( hProcess );
			}
			return sFileName;
		}

	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );

	return "";
}



void CDgPrefExclusiveAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_GPU, m_checkGpu);
	DDX_Control(pDX, IDC_EDIT_PROGRAM, m_editProgram);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_COMBO_PROCESS, m_combo_process);
	DDX_Control(pDX, IDC_EDIT_GENERAL_PROCESS, m_edit_process);
}


BEGIN_MESSAGE_MAP(CDgPrefExclusiveAdd, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDgPrefExclusiveAdd::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDgPrefExclusiveAdd::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_GPU, &CDgPrefExclusiveAdd::OnBnClickedCheckGpu)
	ON_EN_CHANGE(IDC_EDIT_PROGRAM, &CDgPrefExclusiveAdd::OnEnChangeEditProgram)
	ON_BN_CLICKED(ID_OPEN_FILE, &CDgPrefExclusiveAdd::OnBnClickedOpenFile)
	ON_CBN_SELCHANGE(IDC_COMBO_PROCESS, &CDgPrefExclusiveAdd::OnCbnSelchangeComboProcess)
END_MESSAGE_MAP()


// CDgPrefExclusiveAdd message handlers


void CDgPrefExclusiveAdd::OnBnClickedOk()
{
	if (m_checkGpu.GetCheck())
	{
		m_sCpuGpu = EXCLUSIVE_TEXT_GPU;
	}
	else
	{
		m_sCpuGpu = EXCLUSIVE_TEXT_CPU_GPU;
	}
	m_editProgram.GetWindowText(m_sExe);
	if (m_sExe.GetLength() == 0)
	{
		m_edit_process.GetWindowText(m_sExe);
	}

	CDialogEx::OnOK();
}

void CDgPrefExclusiveAdd::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CDgPrefExclusiveAdd::OnBnClickedCheckGpu()
{
	// TODO: Add your control notification handler code here
}


void CDgPrefExclusiveAdd::OnEnChangeEditProgram()
{
}


void CDgPrefExclusiveAdd::OnBnClickedOpenFile()
{
	BOOL bResult;
	char bufferCurrent[MAX_PATH ];
	char bufferPath[MAX_PATH ];
	bufferPath[0] = 0;
	bufferPath[0] = 0;
	CString	sExe;

	SHGetSpecialFolderPath(0, bufferCurrent, CSIDL_PROGRAM_FILES, FALSE ); 

	bResult = BrowseForFolder(this->m_hWnd, bufferCurrent, bufferPath);
	if (bResult == IDOK)
	{
		sExe = bufferPath;
		m_editProgram.SetWindowText(sExe);
	}
}


void CDgPrefExclusiveAdd::OnCbnSelchangeComboProcess()
{
	int iSelect;
	CString sProcess, sFile;

	iSelect = m_combo_process.GetCurSel();
	m_combo_process.GetLBText(iSelect, sProcess);

	sFile = GetProcessFileName(sProcess);

	m_edit_process.SetWindowText(sFile);

}
