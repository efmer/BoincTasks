#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "UpdateCheck.h"

// CDlgUpdate dialog

class CUpdateCallback : public IBindStatusCallback 
{ 
public: 
CUpdateCallback(); 
~CUpdateCallback(); 

// Pointer to the main dialog. 
HWND hwnd;

STDMETHOD(OnStartBinding)( 
/* [in] */ DWORD dwReserved, 
/* [in] */ IBinding __RPC_FAR *pib) 
{ return E_NOTIMPL; } 

STDMETHOD(GetPriority)( 
/* [out] */ LONG __RPC_FAR *pnPriority) 
{ return E_NOTIMPL; } 

STDMETHOD(OnLowResource)( 
/* [in] */ DWORD reserved) 
{ return E_NOTIMPL; } 

STDMETHOD(OnProgress)( 
/* [in] */ ULONG ulProgress, 
/* [in] */ ULONG ulProgressMax, 
/* [in] */ ULONG ulStatusCode, 
/* [in] */ LPCWSTR wszStatusText); 

STDMETHOD(OnStopBinding)( 
/* [in] */ HRESULT hresult, 
/* [unique][in] */ LPCWSTR szError) 
{ return E_NOTIMPL; } 

STDMETHOD(GetBindInfo)( 
/* [out] */ DWORD __RPC_FAR *grfBINDF, 
/* [unique][out][in] */ BINDINFO __RPC_FAR *pbindinfo) 
{ return E_NOTIMPL; } 

STDMETHOD(OnDataAvailable)( 
/* [in] */ DWORD grfBSCF, 
/* [in] */ DWORD dwSize, 
/* [in] */ FORMATETC __RPC_FAR *pformatetc, 
/* [in] */ STGMEDIUM __RPC_FAR *pstgmed) 
{ return E_NOTIMPL; } 

STDMETHOD(OnObjectAvailable)( 
/* [in] */ REFIID riid, 
/* [iid_is][in] */ IUnknown __RPC_FAR *punk) 
{ return E_NOTIMPL; } 

STDMETHOD_(ULONG,AddRef)() 
{ return 0; } 

STDMETHOD_(ULONG,Release)() 
{ return 0; } 

STDMETHOD(QueryInterface)( 
/* [in] */ REFIID riid, 
/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject) 
{ return E_NOTIMPL; } 
}; 


class CNoticeBoincTasksUpdate;
class CDlgUpdate : public CDialog
{
	DECLARE_DYNAMIC(CDlgUpdate)

public:
	CDlgUpdate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUpdate();
	virtual BOOL OnInitDialog();
	int		CheckForNewVersion(bool bForceCheck);
	void	SendUpdateNotice(CNoticeBoincTasksUpdate *pNoticeBoincTasksUpdate);
	void	CheckUpdate();
	int		GetVersion();
	void	ShowVersion();
	BOOL	BrowseForFolder(HWND hwnd, LPCTSTR szCurrent, LPTSTR szPath);

	CEdit		m_textBox;

	int			m_iCheckAuto;
	int			m_iCheckBeta;

// Dialog Data
	enum { IDD = IDD_DLGUPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
			void Update(bool bDownload);

	HICON		m_hIcon;

	CUpdateVersion	m_versionBoincTasks;
	CUpdateVersion	m_versionTThrottle;

	bool		m_bUseBrowser;
	bool		m_bStartUpdate;

	UINT_PTR	m_pTimer;

	CButton	m_buttonUpdate;
	CButton m_buttonCheck;
	CButton m_buttonOk;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedUpdatecheck();

	CButton m_checkAuto;
	CButton m_checkBeta;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckUpdateAuto();
	afx_msg void OnBnClickedCheckUpdateBeta();
	CRichEditCtrl m_richEdit;
	CProgressCtrl m_progress;
	afx_msg void OnBnClickedFolder();
	CButton m_updateFolder;
	CEdit m_editUpdateFolder;
	CStatic m_textUpdateTemp;
	CButton m_checkUseBrowser;
	afx_msg void OnBnClickedCheckUpdateWeb();

	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);
};
