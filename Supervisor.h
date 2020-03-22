#if !defined(AFX_SUPERVISOR_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_SUPERVISOR_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Supervisor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSupervisor window

#include "SerialPort.h"
#include "Protocols/EcuData.h"

class CFreeScanDlg;
class CStatusDlg;

// Protocols installed:
// Add what extra protocols you've included here:
class CELM327Protocol;
class CEspritProtocol;
class CElanProtocol;
class CLotusCarltonProtocol;
class CGM1994CamaroZ28Protocol;
class CGM1993CamaroZ28Protocol;
class CGM1992PontiacProtocol;
class CGM1989CorvetteProtocol;
class CGMA140Protocol;
class CGMA143Protocol;
class CGMA160Protocol;
// You'll need to do a specific include in the .cpp file as well.

// For every protocol, these must be unique IDs
// These are the messages the user sends to the protocol class
#define WM_PROT_CMD_RESETSTATE		WM_USER+110	// Resets the protocol state machine.
#define WM_PROT_CMD_SETINTERACT		WM_USER+111	// Set the interact mode.
#define WM_PROT_CMD_GETINTERACT		WM_USER+112	// Gets the interact mode.
#define WM_PROT_CMD_ECUMODE			WM_USER+113	// Set the next ECU mode.
#define WM_PROT_CMD_GETECUMODE		WM_USER+114	// Get the current ECU mode
#define WM_PROT_CMD_STARTCSV		WM_USER+115	// Starts csv logging
#define WM_PROT_CMD_FORCESHUTUP		WM_USER+116	// Forces the FreeScan to transmit

// These are defines for the ECU message requests.
// For function ECUMode(...)
#define	ECU_GET_DTCS				100
#define ECU_CLEAR_DTCS				101
#define ECU_CLEAR_BLM				102
#define ECU_RESET_IAC				103
#define ECU_SET_DES_IDLE			104
// : public CObject
class CSupervisor
{
// Construction
public:
	CSupervisor(CFreeScanDlg* MainDlg = NULL, CStatusDlg* pStatusDlg = NULL);

// Attributes
public:
	BOOL		m_bCentigrade; // Is FreeScan in Metric or Imperial?
	BOOL		m_bMiles; // Is FreeScan in Metric or Imperial?

	CFreeScanDlg*		m_pMainDlg;
	CStatusDlg*			m_pStatusDlg;

private:
//	Our protocols go here:
	CELM327Protocol*			m_pELM327Protocol;
	CEspritProtocol*			m_pEspritProtocol;
	CElanProtocol*				m_pElanProtocol;
	CLotusCarltonProtocol*		m_pLotusCarltonProtocol;
	CGM1994CamaroZ28Protocol*	m_pGM1994CamaroZ28Protocol;
	CGM1993CamaroZ28Protocol*	m_pGM1993CamaroZ28Protocol;
	CGM1992PontiacProtocol*		m_pGM1992PontiacProtocol;
	CGM1989CorvetteProtocol*	m_pGM1989CorvetteProtocol;
	CGMA140Protocol*			m_pGMA140Protocol;
	CGMA143Protocol*			m_pGMA143Protocol;
	CGMA160Protocol*			m_pGMA160Protocol;
	CSerialPort*				m_pCom;// Our com port object pointer

	HWND	m_ProtocolWnd;// Handle of the protocol window
	CWnd*	m_pParentWnd; // for SetCurrentPort()
	int		m_iModel; // for SetCurrentPort()

	CEcuData*					m_pEcuData;

// Implementation
public:
	virtual ~CSupervisor();
	void WriteStatus(CString csText);
	void Init(CWnd* pParentWnd, int iModel);// Initialises the Supervisor and Protocol
	void UpdateDialog(void);// Updates the dialogs because of a data change
	void ConvertDegrees(void); // converts all temps to degF
	void ConvertMiles(void); // converts speeds to kph
	void SetCurrentPort(UINT nPort);// Sets current com port
	UINT GetCurrentPort(void);// Returns the current com port
	DWORD GetWriteDelay(void);//Gets the Write Delay from the serial port
	void SetWriteDelay(DWORD nDelay);//Sets the Write Delay to the serial port
	BOOL Start(void);// Starts the ECU monitoring
	BOOL Restart(void);
	BOOL Stop(void);
	BOOL ShutDown(void);
	BOOL StartCSVLog(BOOL bStart);// Starts or stops csv logging to file
	void Deallocate(void); // Deallocates the models we don't want monitor.
	void Interact(BOOL bInteract);// Requests whether FreeScan talks to the ECU or not
	BOOL GetInteract(void);// Gets the interact status
	void Centigrade(BOOL bUnit);
	void Miles(BOOL bUnit);
	BOOL GetCentigrade(void);
	BOOL GetMiles(void);
	DWORD GetCurrentMode(void);// Returns the current ECU mode
	void ECUMode(DWORD dwMode, unsigned char Data = 0);// Changes the ECU mode number
	void ForceShutUp(void); // Forces the protocol to send a shut-up command to the ECU.
	const CEcuData *const GetEcuData(void);
	CEcuData *const GetModifiableEcuData(void);

	void Test(void); // Sends a packet to the current parser for testing

private:
	void PumpMessages(void);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERVISOR_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
