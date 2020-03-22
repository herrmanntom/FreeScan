#if !defined(AFX_StatusBitsDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_)
#define AFX_StatusBitsDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusBitsDlg.h : header file
//

#include "Protocols/EcuData.h"
#include "MainDlg.h"
#include "Supervisor.h"

/////////////////////////////////////////////////////////////////////////////
// CStatusBitsDlg dialog

#include "Led.h"

class CStatusBitsDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatusBitsDlg)

// Construction
public:
	CStatusBitsDlg();
	~CStatusBitsDlg();

// Dialog Data
private:
	//{{AFX_DATA(CStatusBitsDlg)
	enum { IDD = IDD_STATUSBITS };
	CLed	m_led3;
	CLed	m_led2;
	CLed	m_led1;
	//}}AFX_DATA

	CFreeScanDlg*	m_pMainDlg; // Base Dialog Pointer.

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatusBitsDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void Refresh(const CEcuData* const ecuData);
	void RegisterMainDialog(CFreeScanDlg* const mainDialog);

// Implementation
private:
	BOOL GetInteract(void);// Returns if the ECU is interactive

	CSupervisor* GetSupervisor(void); // returns a pointer to the Supervisor
	// Generated message map functions
	//{{AFX_MSG(CStatusBitsDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_StatusBitsDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_)
