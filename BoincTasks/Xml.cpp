// Xml.cpp : implementation file
//

#include "stdafx.h"
#include "boinctasks.h"
#include <direct.h>
#include "Xml.h"
#include "DlgLogging.h"
#include "DlgSettingsExpert.h"

// CXml

CXml::CXml(CStringA sAFileName, bool bUserDirectory, bool bMakeLower,  bool bStripPercentage)
{
	m_sAFileName = sAFileName;

	if (theApp.m_pDlgLogging) m_pWndLogging = theApp.m_pDlgLogging;
	else m_pWndLogging = NULL;

	m_bUserDirectory = bUserDirectory;
	m_bMakeLower = bMakeLower;
	m_bStripPercentage =  bStripPercentage;
	m_bLogLocalDirectoryOnce = false;
}

CXml::CXml(bool bMakeLower,  bool bStripPercentage)
{
	m_bMakeLower = bMakeLower;
	m_bStripPercentage =  bStripPercentage;
}

CXml::~CXml()
{

}

bool CXml::WriteStart(CStringA sAMainTag)
{
	CStringA sALine;
	CStringA sADirectory, sAFile;
	CString  sFile;

	m_sAMainTag = sAMainTag;

	sADirectory = GetDirectory();
	sAFile = sADirectory + "\\";
	sAFile = sAFile + m_sAFileName;
	sFile = sAFile;

	CFileException e;
	if( !m_file.Open( sFile.GetBuffer(), CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		CString sTxt;

		sTxt = "Unable to write to xml file, ";
		sTxt+= sFile;
		sTxt+= "Look in Help->Show Log for more details.";

		AfxMessageBox(sTxt);
		return false;
	}

	sALine.Format("<%s>\r\n",m_sAMainTag);
	m_file.Write(sALine,sALine.GetLength());

	return true;
}

void CXml::WriteEnd()
{
	CStringA sALine;

	sALine.Format("</%s>",m_sAMainTag);
	m_file.Write(sALine,sALine.GetLength());

	m_file.Close();
}

void CXml::WriteBeginGroup(CStringA sAGroup)
{
	CStringA sLine;
	m_sAgroup = sAGroup;

	sLine.Format("<%s>\r\n",sAGroup);
	m_file.Write(sLine,sLine.GetLength());
}

void CXml::WriteEndGroup()
{
	CStringA sLine;

	sLine.Format("</%s>\r\n",m_sAgroup);
	m_file.Write(sLine,sLine.GetLength());
}

void CXml::WriteItem(CStringA sATag, CStringA sAValue, int iTab)
{
	CStringA sLine;

	sAValue.Replace(" ","%20");			// change space into %20

	sAValue.Replace("<","%3C");			// change <
	sAValue.Replace("/","%2F");			// change /
	sAValue.Replace(">","%3E");			// change >

	sLine.Format("<%s>%s</%s>\r\n",sATag,sAValue,sATag);

	if (iTab)
	{
		sLine = "    " + sLine;
	}

	m_file.Write(sLine,sLine.GetLength());

}

void CXml::WriteItemBin(CStringA sATag,BYTE *pBuffer, int iSize)
{
	CStringA sValue, sLine, sFormat;
	BYTE	 byte;

	for (int iCount = 0; iCount < iSize; iCount++)
	{
		byte = *(pBuffer+iCount);
		sFormat.Format("%x",byte);
		sValue += "%";
		sValue += sFormat;
	}
	sLine.Format("<%s>%s</%s>\r\n",sATag,sValue,sATag);
	m_file.Write(sLine,sLine.GetLength());
}

bool CXml::ReadItem(CStringA sAId, CString *psValue)
{
	for (int iCount = 0; iCount < m_iItems; iCount++)
	{
		if (sAId == m_sItem[iCount])
		{
			*psValue = m_sItemValue[iCount];
			return true;
		}

	}
	*psValue = L"";
	return false;

}

bool CXml::ReadItemBin(CStringA sAId, BYTE *pBuffer, int *piSize)
{
	CString sValue;
	int		iPos, iPosBuffer, iAdd, iValueLen;
	BYTE	cHigh, cLow, cValue;

	*piSize = 0;

	for (int iCount = 0; iCount < m_iItems; iCount++)
	{
		if (sAId == m_sItem[iCount])
		{
			sValue = m_sItemValue[iCount];
			iPos = 0;
			iPosBuffer = 0;
			iValueLen = sValue.GetLength();

			while(iPos < iValueLen)
			{
				if (sValue[iPos] == '%')
				{
					cHigh = sValue[iPos+1];
					cLow = sValue[iPos+2];
					if (cLow == '%' || cLow == 0)
					{
						cLow = cHigh;
						cHigh= '0';
						iAdd = 2;
					}
					else iAdd = 3;
					if (!ConvertHexDec(cHigh,cLow,&cValue))
					{
						return false;
					}
					*(pBuffer+iPosBuffer) = cValue;
					iPos += iAdd;
					iPosBuffer++;
					*piSize = (*piSize) + 1;
					if (iPosBuffer > 254) return false;
				}
				else iPos++;
			}
			return true;
		}

	}
	return false;
}

bool CXml::ConvertHexDec(BYTE cHigh, BYTE cLow, BYTE *pcValue)
{
	if (cLow >= 'a')
	{
		cLow -= 'a';
		cLow += 10;
	}
	else cLow -= '0';
	if (cLow > 15)
	{
		return false;
	}
	*pcValue = cLow;

	if (cHigh >= 'a')
	{
		cHigh -= 'a';
		cHigh += 10;
	}
	else cHigh -= '0';
	if (cHigh > 15)
	{
		return false;
	}
	*pcValue += (cHigh << 4);
	return true;
}

bool CXml::ReadStart(CStringA sMainTag)
{
	CStringA sTag;

	m_iPos = 0;

	if (m_sAXmlDataLower.GetLength() == 0)
	{
		ReadFile(&m_sAXmlDataLower);
	}

	if (m_sAXmlDataLower.GetLength() > 0)
	{
		sTag = "<" + sMainTag + ">" ;
		m_iPos = m_sAXmlDataLower.Find(sTag);
		if (m_iPos >= 0)
		{
			m_iPos += sTag.GetLength();
			return true;
		}
	}
	return false;
}

bool CXml::ReadStart(CStringA *psData, CStringA sMainTag)
{
	CStringA sTag;

	m_sAXmlData = *psData;
	m_sAXmlData.Replace(" ","");	// strip spaces
	m_sAXmlData.Replace("\t","");	// strip tab
	m_sAXmlData.Replace("\r","");	// strip cr
	m_sAXmlData.Replace("\n","");	// strip lf
	RemoveComment();

	m_sAXmlDataLower = m_sAXmlData;
	CharLower(m_sAXmlDataLower.GetBuffer());m_sAXmlDataLower.ReleaseBuffer();

//	m_sAXmlDataLower.MakeLower();	// all lower

	m_iPos = 0;

	sTag = "<" + sMainTag + ">" ;
	m_iPos = m_sAXmlDataLower.Find(sTag);
	if (m_iPos >= 0)
	{
		m_iPos += sTag.GetLength();
		return true;
	}
	return false;
}

//<computer>			= sAGrouptag
//<id>4955000</id>
//<id_name>fred-giga-i7-xp</id_name>
//<ip> </ip>
//</computer>

bool CXml::ReadGroup(CStringA sAGroupTag, bool bFind, bool bScan)
{
	CStringA sATag,sAGroupTagRead, sAGroupTagC, sAItem, sAValue;
	int	iResult, iIndexBegin;

	iResult = XML_ERROR;

	// first must be the group

	if (bScan)
	{
		CString sFind;
		sFind = "<";
		sFind+= sAGroupTag;
		sFind+= ">";
		iIndexBegin = m_sAXmlDataLower.Find(sFind, m_iPos);
		if (iIndexBegin < 0) return false;
		m_iPos = iIndexBegin;
	}
	ReadCommand(&sAGroupTagRead);

	if (bFind)
	{
		while (sAGroupTagRead != sAGroupTag)
		{
			while ((iResult = ReadCommandSequence(&sAGroupTagRead, &sAItem, &sAValue)) == XML_OK)
			{
			}
			if (iResult != XML_GROUP)
			{
				return false;
			}
			ReadCommand(&sAGroupTagRead);
			if (sAGroupTagRead.GetLength() == 0)
			{
				return false;
			}
		}
	}
	else
	{
		if (sAGroupTagRead != sAGroupTag) return false;
	}

//	sAGroupTagC = "\\" + sAGroupTag;  	
	m_iItems = 0;
	while ((iResult = ReadCommandSequence(&sAGroupTag, &sAItem, &sAValue)) == XML_OK)
	{
		m_sItem[m_iItems] = sAItem;

		if (m_bStripPercentage)
		{
			sAValue.Replace("%20"," ");			// change %20 back into space
			sAValue.Replace("%3C","<");			// change <	
			sAValue.Replace("%2F","/");			// change /
			sAValue.Replace("%3E",">");			// change >
		//	sAValue.Replace("%3c","<");			// change <	
		//	sAValue.Replace("%2f","/");			// change /
		//	sAValue.Replace("%3e",">");			// change >
		}

		m_sItemValue[m_iItems] = sAValue;
		if (m_iItems >= MAX_ITEMS) return false;
		m_iItems++;
	}

	if (iResult == XML_GROUP) return true;

	if (iResult == XML_ERROR)
	{
		m_sLog += "XML ERROR: ";
		m_sLog += sAGroupTag;
		m_sLog += ", ";
		m_sLog += sAItem;
		m_sLog += ", ";
		m_sLog += sAValue;
		m_sLog += "\r\n";
	}

	return false;
}

// get CXml <xxx>something</xxx>
int CXml::ReadCommandSequence(CStringA *psAGroup, CStringA *psAItem, CStringA *psAValue)
{
	int		iResult, iIndexBegin, iLen;
	CStringA sACommand1, sACommand2, sAGroup, sAId;

	iResult = ReadCommand(&sACommand1);
	if (iResult != XML_OK)
	{
		return XML_ERROR;
	}

	sAGroup = "/" + *psAGroup;
	if (sACommand1 == sAGroup)
	{
		return XML_GROUP;
	}

	*psAItem = sACommand1;

	// find beginning next command
	iIndexBegin = m_sAXmlDataLower.Find(L'<', m_iPos);
	if (iIndexBegin < 0)
	{
		return XML_ERROR;
	}
	// ok in between <> and </> is the value
	
	iLen = iIndexBegin - m_iPos;
	if (iLen == 0) 	*psAValue = "";
	else 
	{
		if (m_bMakeLower)	*psAValue = m_sAXmlDataLower.Mid(m_iPos, iLen );
		else				*psAValue = m_sAXmlData.Mid(m_iPos, iLen );
	}
	m_iPos += iLen;

	// ok got value now it has to be closed by /command 

	iResult = ReadCommand(&sACommand2);

	if (iResult == XML_OK)
	{
		sACommand1 = "/" + sACommand1;
		if (sACommand1 == sACommand2)
		{
			// right sequence
			return XML_OK;			
		}
	}
	return XML_ERROR;
}

int CXml::ReadCommand(CStringA *psACommand)
{
	int iIndexEnd;
	char cChar;
//	sId;

	// command is <xxx> or </xxx>

	cChar = m_sAXmlDataLower.GetAt(m_iPos);
	if (cChar != '<') 
	{
		return XML_ERROR;
	}
	m_iPos++; // past <
	iIndexEnd = m_sAXmlDataLower.Find(">", m_iPos);
	if (iIndexEnd >= 0)
	{
		int iLen = (iIndexEnd - m_iPos);
		if (iLen <= 0)
		{
			return XML_ERROR;
		}
		*psACommand = m_sAXmlDataLower.Mid(m_iPos, iLen );
		m_iPos += iLen;		// past command
		m_iPos ++;			// past >
		return XML_OK;
	}
	return XML_ERROR;
}

int CXml::ReadFile(CStringA *pAXml)
{
	CFile		file;
	int			iReadCount, iTotalReadCount;
	BYTE		bBuffer[50001];
	CString		sPathAndFileName, sPathOnly;
	CStringA	sALine, sBuffer;
	CStringA	sADirectory, sAFile;
	CString		sFile;

	m_sAXmlData = "";
	m_sAXmlDataLower = "";

	iTotalReadCount = 0;
	*pAXml = L"";

	sADirectory = GetDirectory();
	sAFile = sADirectory + "\\";
	sAFile = sAFile + m_sAFileName;
	sFile = sAFile;

	CFileException ex;

	if (file.Open(sFile, CFile::modeRead, &ex))
	{
		iReadCount = 1;
		while (iReadCount)
		{
			iReadCount = file.Read(bBuffer, 50000);
			bBuffer[iReadCount] = 0;
			sBuffer = bBuffer;
			sBuffer.Replace(" ","");	// strip spaces
			sBuffer.Replace("\t","");	// strip tab
			sBuffer.Replace("\r","");	// strip cr
			sBuffer.Replace("\n","");	// strip lf
			m_sAXmlData += sBuffer;
			iTotalReadCount += iReadCount;
		}

		RemoveComment();

		m_sAXmlDataLower = m_sAXmlData;
		m_sAXmlDataLower.MakeLower();	// all lower
		file.Close();
	}
	else
	{
	  TRACE(_T("File could not be opened %d\n"), ex.m_cause);
	}

	return iTotalReadCount;
}

void CXml::RemoveComment()
{
	int iBegin, iEnd;
	CString sPart1, sPart2;

	while (1)
	{
		iBegin = m_sAXmlData.Find("<!--",0);
		iEnd = m_sAXmlData.Find("-->",0);

		if (iBegin<0) break;
		if (iEnd<0) break;

		if (iBegin > iEnd) break;

		sPart1 = m_sAXmlData.Mid(0,iBegin);
		sPart2 = m_sAXmlData.Mid(iEnd+3);

		m_sAXmlData = sPart1 + sPart2;
	}
	return;
}

CStringA CXml::GetDirectory()
{
	if (m_bUserDirectory) return GetUserDirectory(theApp.m_pDlgSettingsExpert->m_sPath);

	return GetLocalDirectory();
}

CStringA CXml::GetUserDirectory(CString sPath, bool bLog)
{
	CStringA	sAUserDirectory, sAUserName, sATxt, sAFindTxt;
	DWORD		dwLen;
	char		cPath[MAX_PATH];
	char		cBuffer[256];
	bool		bFoundPath;
	dwLen = 255;
	cBuffer[0] = 0;

	GetUserNameA(cBuffer,&dwLen);
	sAUserName = cBuffer;


	sAUserDirectory = sPath;	//theApp.m_pDlgSettingsExpert->m_sPath;
	if (sAUserDirectory.GetLength() > 0)
	{
		int iLength;
		char cChar;

		iLength = sAUserDirectory.GetLength();
		cChar = sAUserDirectory.GetAt(iLength-1);
		if (cChar == '\\')
		{
			sAUserDirectory = sAUserDirectory.Left(iLength-1);
		}

		if (bLog)
		{
			if (theApp.m_pDlgLogging->m_bLogDebugDirectory)
			{
				CLoggingMessage *psLog;
				psLog = new CLoggingMessage;
				psLog->m_pFrom = "GetUserDirectory-Override"; psLog->m_pText = sAUserDirectory.GetBuffer();
				if (m_pWndLogging)
				{
					m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_DEBUG);
				}
				delete psLog;
			}
		}
		return sAUserDirectory;
	}

	if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, cPath))) 
	{
		sAUserDirectory = cPath;
		bFoundPath = true;
	}
	else 
	{
		sAUserDirectory = "c:\\documents and settings\\" + sAUserName + "\\Application Data";
		bFoundPath = false;
	}
	sAFindTxt = sAUserDirectory;

	// search on various locations.
	if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'D');	
	if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'E');	
	if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'F');		
	if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'G');	
	if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'H');	
	if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'I');		

	if(_chdir(sAUserDirectory))
	{
		// try it the Vista way
		sAUserDirectory = "c:\\users\\" + sAUserName;
		sAUserDirectory+= "\\appdata\\roaming";
		if (!bFoundPath) sAFindTxt = sAUserDirectory;
		if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'D');	
		if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'E');	
		if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'F');		
		if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'G');	
		if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'H');	
		if(_chdir(sAUserDirectory)) sAUserDirectory.SetAt(0,'I');

		if(_chdir(sAUserDirectory))
		{
			if (bLog)
			{
				sAFindTxt += "\r\nUnable to find directory, go to Computers->extra->settings and give a valid path for your computer.";
				CLoggingMessage *psLog;
				psLog = new CLoggingMessage;

				psLog->m_pFrom = "GetUserDirectory1"; psLog->m_pText = sAFindTxt.GetBuffer();
				if (m_pWndLogging)
				{
					m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
				}
				delete psLog;
			}
			return sAUserDirectory;
		}
	}

	sAUserDirectory += "\\eFMer";
	_mkdir(sAUserDirectory);
	sAUserDirectory += "\\BoincTasks";
	_mkdir(sAUserDirectory);

	if(_chdir(sAUserDirectory))
	{
		if (bLog)
		{
			sAFindTxt = sAUserDirectory;
			sAFindTxt += "\r\nUnable to find directory, go to Computers->extra->settings and give a valid path for your computer.";
			CLoggingMessage *psLog;
			psLog = new CLoggingMessage;

			psLog->m_pFrom = "GetUserDirectory2"; psLog->m_pText = sAFindTxt.GetBuffer();
			if (m_pWndLogging)
			{
				m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_NORMAL);
			}
			delete psLog;
		}
		return sAUserDirectory;
	}

	if (bLog)
	{
		if (theApp.m_pDlgLogging->m_bLogDebugDirectory)
		{
			CLoggingMessage *psLog;
			psLog = new CLoggingMessage;

			psLog->m_pFrom = "GetUserDirectory3"; psLog->m_pText = sAUserDirectory.GetBuffer();
			if (m_pWndLogging)
			{
				m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_DEBUG);
			}
			delete psLog;
		}
	}
	return sAUserDirectory;
}

CStringA CXml::GetLocalDirectory()
{
	char	strExePath [MAX_PATH];
	CString sDirectory;
	int		iFoundPos;
	CString	sPath;

	DWORD dwSize = GetModuleFileName (NULL, strExePath, MAX_PATH);	// not 0 terminated
	strExePath[dwSize] = 0;
	sDirectory = strExePath;
	iFoundPos = sDirectory.ReverseFind('\\');

	if (iFoundPos > 0)
	{
		sDirectory = sDirectory.Mid(0,iFoundPos);
	}

	if (theApp.m_pDlgLogging != NULL)
	{
		if (theApp.m_pDlgLogging->m_bDebugMode)
		{
			if (m_pWndLogging)
			{
				if (theApp.m_pDlgLogging->m_bLogDebugLocalDirectory)
				{
					CLoggingMessage *psLog;
					psLog = new CLoggingMessage;
					psLog->m_pFrom = "GetLocalDirectory"; psLog->m_pText = sDirectory.GetBuffer();m_pWndLogging->SendMessage(UWM_MSG_LOGGING_TEXT,(WPARAM) psLog, LOGGING_DEBUG);
					delete 	psLog;
				}
			}
		}
	}
	return sDirectory;
}