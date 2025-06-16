// DlgAccept.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "afxdialogex.h"
#include "DlgAccept.h"

#include <iostream>
#include <string>
#include <codecvt>
#include <locale>


// CDlgAccept dialog

IMPLEMENT_DYNAMIC(CDlgAccept, CDialog)

CDlgAccept::CDlgAccept(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CDlgAccept, pParent)
	, mText(_T(""))
{

}

CDlgAccept::~CDlgAccept()
{
}

BOOL CDlgAccept::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}




void CDlgAccept::AddText(CString txt)
{	
	txt.Replace("’", "'");
    txt.Replace("\r", "\r\n");
	txt.Replace("\n", "\r\n");
	txt.Replace(". ", "\r\n");	

    txt.Replace("&lt;", "<");
    txt.Replace("&gt;", ">");
    txt.Replace("&#xD;","\r");
    txt.Replace("<ul>", "");
    txt.Replace("</ul>", "");
    txt.Replace("<li>", "");
    txt.Replace("</li>", "");
    txt.Replace("<a href=", "");
    txt.Replace("</a>", "");
    txt.Replace("<b>", "");
    txt.Replace("</b>", "");
    txt.Replace("<i>", "");
    txt.Replace("</i>", "");
    txt.Replace("<![CDATA[", "");
    txt.Replace("]]>", "");
    txt.Replace("<p>", "");
    txt.Replace("</p>", "");
    txt.Replace("<ol>", "");
    txt.Replace("</ol>", "");
   
    txt.Replace("<ol type=\"a\">", "");

    // any UTF-8 to wide string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_str = converter.from_bytes(txt);
    txt = wide_str.c_str();
    mText.Append(txt);
   //	mText = txt;
}

void CDlgAccept::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ACCEPT_TEXT, mText);
}


BEGIN_MESSAGE_MAP(CDlgAccept, CDialog)
END_MESSAGE_MAP()


// CDlgAccept message handlers
