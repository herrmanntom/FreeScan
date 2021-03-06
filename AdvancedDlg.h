#if !defined(AFX_ADVANCEDDLG_H__CB945664_6752_11D3_1234_0080C83832F8__INCLUDED_)
#define AFX_ADVANCEDDLG_H__CB945664_6752_11D3_1234_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "TTPropertyPage.h" // Our Tooltip Class
#include "EcuData.h"
#include "SupervisorInterface.h"

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDlg dialog

class CAdvancedDlg : public CTTPropertyPage
{
	DECLARE_DYNCREATE(CAdvancedDlg)

// Construction
public:
	CAdvancedDlg();
	~CAdvancedDlg();

private:
// Dialog Data
	//{{AFX_DATA(CAdvancedDlg)
	enum { IDD = IDD_ADVANCED };
	CButton	m_SetDesIdle;
	CButton	m_ResetIAC;
	CButton	m_ResetBLM;
	CEdit	m_DesiredIdle;
	//}}AFX_DATA

	CSupervisorInterface*	m_pSupervisor;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh(const CEcuData* const ecuData); // Updates all of our controls
	void RegisterSupervisor(CSupervisorInterface* const pSupervisor);

private:

	// Generated message map functions
	//{{AFX_MSG(CAdvancedDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetdesidle();
	afx_msg void OnResetBLM();
	afx_msg void OnResetIAC();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDDLG_H__CB945664_6752_11D3_9867_0080C83832F8__INCLUDED_)
