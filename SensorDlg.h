#if !defined(AFX_SENSORDLG_H__CB945664_6752_11D3_9867_0080C83832F8__INCLUDED_)
#define AFX_SENSORDLG_H__CB945664_6752_11D3_9867_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SensorDlg.h : header file
//

#include "TTPropertyPage.h" // Our Tooltip Class
#include "Protocols/EcuData.h"
#include "MainDlg.h"
#include "Supervisor.h"

/////////////////////////////////////////////////////////////////////////////
// CSensorDlg dialog

class CSensorDlg : public CTTPropertyPage
{
	DECLARE_DYNCREATE(CSensorDlg)

// Construction
public:
	CSensorDlg();
	~CSensorDlg();

// Dialog Data
private:
	//{{AFX_DATA(CSensorDlg)
	enum { IDD = IDD_SENSORS };
	CEdit	m_ThrottleADC;
	CEdit	m_MATADC;
	CEdit	m_BaroADC;
	CEdit	m_MapADC;
	CEdit	m_CoolantADC;
	CEdit	m_BaroVolts;
	CEdit	m_Baro;
	CEdit	m_MapVolts;
	CEdit	m_Boost;
	CEdit	m_CoolantVolts;
	CEdit	m_ThrottleVolts;
	CEdit	m_ThrottlePos;
	CEdit	m_MATVolts;
	CEdit	m_MAT;
	CEdit	m_CoolantTemp;
	//}}AFX_DATA

	CFreeScanDlg*	m_pMainDlg; // Base Dialog Pointer.

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSensorDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh(const CEcuData* const ecuData); // Updates all of our controls
	void RegisterMainDialog(CFreeScanDlg* const mainDialog);

private:
	CSupervisor* GetSupervisor(void); // returns a pointer to the Supervisor
	BOOL GetInteract(void);
	DWORD GetCurrentMode(void); // Returns the current ECU Mode

	// Generated message map functions
	//{{AFX_MSG(CSensorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENSORDLG_H__CB945664_6752_11D3_9867_0080C83832F8__INCLUDED_)
