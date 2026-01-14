#pragma once

#include "WebServer.h"

using namespace std;

#define BT_HTML_HEADER		"__InsertBoincTasks__";

#define HTML_MIME_JS		"text/javascript"
#define HTML_MIME_CSS		"text/css"
#define HTML_MIME_ICO		"image/x-icon"
#define HTML_MIME_GIF		"image/gif"
#define HTML_MIME_PNG		"image/png"

#define HTML_METHOD_CODE_UNDEFINED	0
#define HTML_METHOD_CODE_GET		1
#define HTML_METHOD_CODE_POST		2

#define HTML_METHOD_GET		"GET"
#define HTML_METHOD_POST	"POST"

#define HTML_HEADER_HOST		"Host:"
#define HTML_HEADER_USER_AGENT	"User-Agent:"
#define HTML_HEADER_ACCEPT		"Accept:"
#define HTML_HEADER_COOKIE		"Cookie:"


#define HTML_COOKIE_SESSION		"bt_session="

class CHtmlHeaderRecieve
{
public:
	int		m_iMethod;
	string	m_sUrl;
	string	m_sParameter;
	string	m_sHost;
	string	m_sUserAgent;
	string	m_sAccept;
	string	m_sCookieSession;
};

class CHtmlHeaderSend
{
public:
	struct	tm   m_tmtime;
	bool	m_bTimeValid;
	string	m_sMime;
	string	m_sContentLength;
	string	m_sContent;
	bool	m_bBinairy;
	int		m_iByteContentLength;
	BYTE	*m_pbContent;
};

class CWebServer2;

// CWebServerProcess

class CWebServerProcess : public CWnd
{
	DECLARE_DYNAMIC(CWebServerProcess)

public:
	CWebServerProcess();
	virtual ~CWebServerProcess();

//	void Start(CWebServer2 *pWebServer,  int iOs, string sPort, int iPort);
	BYTE *Process(string *psRecieved, int *piLen = NULL);
	void ResetPassword();

	CHtmlHeaderRecieve	m_htmlHeaderRecieve;
	CHtmlHeaderSend		m_htmlHeaderSend;
protected:
	DECLARE_MESSAGE_MAP()
	void LoginHtmlAuto();
	void LoginHtmlNormal();
	void Get();
	void Post();

	bool GetLine(string *psRecieved, string *psLine);
	void ProcessMethod(string *psLine);
	void ProcessLine(string *psLine);
	void StripSpace(string *psLine);
	void ReplaceCode(string *psText);
	string ExtractParameter(string *psUrl);

	void SetMimeType(string sMime, bool bBinairy);
	void SetMessageLength(int iLen);
	void SetMessage(string *psMessage);
	bool SetMessageFile(string sFileName);
	void ResetSend();
	BYTE *Finish(int *piLen);
	void AddCrLf(string *psSend);
	void GetComputerTree(string *psData);
	CString ExtractXmlTag(char **pcFind);
	CView *pViewGetView(CDocument *pDoc, CRuntimeClass *pClass);

	string m_sCrLf;

	BYTE	*m_pbFileDataBuffer;
	int		m_iFileDataBuferLen;

	char	m_cWebFolderBuffer[MAX_PATH];
	char	m_cRandomString[257];	
	string	m_sAWebServerPasswordClear;
	string	m_sAWebServerPasswordHex;

	int		m_iWebServerLogin;
	bool	m_bWebServerSelectMultiple;

	int		m_iWebServerHtmlCurrentTab;

//	CWebServer2 *m_pWebServer;


};


