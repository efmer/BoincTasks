#pragma once

// CComputers command target

class CComputer : public CObject
{
public:
	CComputer();
	virtual ~CComputer();

	int		m_iComputerChecked;
	CString m_sComputerGroup;
	CString m_sComputerId;
	CString m_sComputerMAC;
	int		m_iValidMac;
	CString	m_sComputerIp;
	CString m_sComputerPassword;
	int		m_iEncryptedConnection;
	int		m_iPort;

// RPC Only in combined 


};


