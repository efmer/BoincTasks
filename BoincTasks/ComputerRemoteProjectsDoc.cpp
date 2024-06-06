// ComputerRemoteProjectsDoc.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "ComputerRemoteProjectsDoc.h"


// CComputerRemoteProjectsDoc

IMPLEMENT_DYNCREATE(CComputerRemoteProjectsDoc, CDocument)

CComputerRemoteProjectsDoc::CComputerRemoteProjectsDoc()
{
}

BOOL CComputerRemoteProjectsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CComputerRemoteProjectsDoc::~CComputerRemoteProjectsDoc()
{
}


BEGIN_MESSAGE_MAP(CComputerRemoteProjectsDoc, CDocument)
END_MESSAGE_MAP()


// CComputerRemoteProjectsDoc diagnostics

#ifdef _DEBUG
void CComputerRemoteProjectsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CComputerRemoteProjectsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CComputerRemoteProjectsDoc serialization

void CComputerRemoteProjectsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CComputerRemoteProjectsDoc commands
