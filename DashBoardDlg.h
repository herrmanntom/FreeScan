
#if !defined(AFX_DashBoardDlg_H__CB945454_6752_11D3_1234_0080C83832F8__INCLUDED_)
#define AFX_DashBoardDlg_H__CB945664_6752_11D3_1234_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DashBoardDlg.h : header file
//

#include "TTPropertyPage.h" // Our Tooltip Class
#include "Protocols/EcuData.h"
#include "MainDlg.h"
#include "Supervisor.h"

/////////////////////////////////////////////////////////////////////////////
// CDashBoardDlg dialog
class CFreeScanDlg;

class CDashBoardDlg : public CTTPropertyPage
{
	DECLARE_DYNCREATE(CDashBoardDlg)

// Construction
public:
	CDashBoardDlg();
	~CDashBoardDlg();

// Dialog Data
private:
	//{{AFX_DATA(CDashBoardDlg)
	enum { IDD = IDD_DASHBOARD };
	CProgressCtrl	m_Throttle;
	CProgressCtrl	m_EngineLoad;
	CProgressCtrl	m_Boost;
	CEdit			m_BoostText;
	CProgressCtrl	m_MAT;
	CEdit			m_MATText;
	CProgressCtrl	m_Speedo;
	CEdit			m_SpeedoMphText;
	CEdit			m_SpeedoKphText;
	CProgressCtrl	m_Tacho;
	CEdit			m_TachoText;
	CProgressCtrl	m_OilTemp;
	CEdit			m_OilTempText;
	CProgressCtrl	m_Water;
	CEdit			m_WaterText;
	CProgressCtrl	m_Volt;
	CEdit			m_VoltText;
	CProgressCtrl	m_Spark;
	CEdit			m_SparkText;
	//}}AFX_DATA

	CBrush m_brush; // For our background colour

	CFreeScanDlg*	m_pMainDlg; // Base Dialog Pointer.

	// Variables store to reduce unnecessary updates
	float	m_fOilTemp;
	float	m_fWaterTemp;
	float	m_fMATTemp;
	float	m_fBatteryVolts;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDashBoardDlg)
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
	//{{AFX_MSG(CDashBoardDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DashBoardDlg_H__CB945454_6752_11D3_9867_0080C83832F8__INCLUDED_)
