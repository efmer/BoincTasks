// AnsiToUtf8.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "AnsiToUtf8.h"
#include "Translation.h"

// CAnsiToUtf8

IMPLEMENT_DYNAMIC(CAnsiToUtf8, CWnd)

CAnsiToUtf8::CAnsiToUtf8()
{

}

CAnsiToUtf8::~CAnsiToUtf8()
{
}


BEGIN_MESSAGE_MAP(CAnsiToUtf8, CWnd)
END_MESSAGE_MAP()

CStringA CAnsiToUtf8::ToUtf8(CStringA *pAscii)
{
	CString sAscii;

	sAscii = ToUtf8(pAscii->GetBuffer());
	pAscii->ReleaseBuffer();
	
	return sAscii;
}

CStringA CAnsiToUtf8::ToUtf8(char *pAscii)
{
	CStringA sUTF8Out, sAscii;
	CStringW sUnicode;
	CStringW utf16;
//	int iStatus;

	sAscii = pAscii;

	sUnicode = AsciiToUtf16(sAscii);

	sUTF8Out = Utf16toUtf8(sUnicode);


   int len = MultiByteToWideChar(CP_UTF8, 0, sUTF8Out, -1, NULL, 0);
   if (len>1)
   { 
      wchar_t *ptr = utf16.GetBuffer(len-1);
      if (ptr) MultiByteToWideChar(CP_UTF8, 0, sUTF8Out, -1, ptr, len);
      utf16.ReleaseBuffer();
   }
   	return sUTF8Out;

//	iStatus = GetLastError();
//
//	sUTF8Out.Format("Utf-8 conversion error: %d",iStatus);
//	return sUTF8Out;
}


CStringW CAnsiToUtf8::AsciiToUtf16(CStringA sAscii)
{
	CStringW utf16;
	
	int len = MultiByteToWideChar(CP_ACP, 0, sAscii, -1, NULL, 0);
	if (len > 1) 
	{
		 wchar_t *ptr = utf16.GetBuffer(len-1);
		MultiByteToWideChar(CP_ACP, 0, sAscii, -1, ptr, len);
		utf16.ReleaseBuffer();
	}
	return utf16;
}

CStringA CAnsiToUtf8::Utf16toUtf8(CStringW sUtf16)
{
   CStringA sUtf8;
   int len = WideCharToMultiByte(CP_UTF8, 0, sUtf16, -1, NULL, 0, 0, 0);
   if (len>1)
   { 
      char *ptr = sUtf8.GetBuffer(len-1);
      if (ptr) WideCharToMultiByte(CP_UTF8, 0, sUtf16, -1, ptr, len, 0, 0);
      sUtf8.ReleaseBuffer();
   }
   return sUtf8;
}

// CAnsiToUtf8 message handlers


