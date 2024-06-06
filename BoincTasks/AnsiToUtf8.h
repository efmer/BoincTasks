#pragma once


// CAnsiToUtf8

class CAnsiToUtf8 : public CWnd
{
	DECLARE_DYNAMIC(CAnsiToUtf8)

public:
	CAnsiToUtf8();
	virtual ~CAnsiToUtf8();

	CStringA ToUtf8(CStringA *pAscii);
	CStringA ToUtf8(char *pAscii);

	CStringW AsciiToUtf16(CStringA sAscii);
	CStringA Utf16toUtf8(CStringW sUtf16);

protected:
	DECLARE_MESSAGE_MAP()
};


