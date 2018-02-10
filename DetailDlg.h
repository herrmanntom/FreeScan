#if !defined(AFX_DETAILDLG_H__09BF9084_1326_11D2_983E_00E018900F2A__INCLUDED_)
#define AFX_DETAILDLG_H__09BF9084_1326_11D2_983E_00E018900F2A__INCLUDED_

#include "MABString.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DetailDlg.h : header file
//

#include "TTPropertyPage.h" // Our Tooltip Class
#include "EnumSer.h"
class CSupervisor;

/////////////////////////////////////////////////////////////////////////////
// CDetailDlg dialog
class CFreeScanDlg;

class CDetailDlg : public CTTPropertyPage
{
	DECLARE_DYNCREATE(CDetailDlg)

// Construction
public:
	CDetailDlg();
	~CDetailDlg();

// Dialog Data
	//{{AFX_DATA(CDetailDlg)
	enum { IDD = IDD_DETAIL };
	CComboBox	m_WriteDelay;
	CButton	m_mph;
	CButton	m_kph;
	CButton	m_DegF;
	CButton	m_DegC;
	CButton	m_Force;
	CListBox	m_Comments;
	CComboBox	m_Model;
	CButton	m_StartLog;
	CButton	m_CSV;
	CEdit	m_Received;
	CEdit	m_Sent;
	CButton	m_Stop;
	CButton	m_Start;
	CComboBox	m_ComSelect;
	CButton	m_Hide;
	CButton	m_Listen;
	CButton	m_Interact;
	//}}AFX_DATA

	CFreeScanDlg*	m_pMainDlg; // Base Dialog Pointer.
protected:
	CUIntArray m_cuPorts; // Stores the enumerated COM Port numbers
	CEnumerateSerial m_EnumSerial;//ARW 15/01/2005 New EnumSerial Class
	BOOL	m_bCSVFirstTime;
	BOOL	m_bLogFirstTime;
	int		m_iModel; // Model Number

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDetailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void WriteStatus(CString csText);
	void WriteASCII(unsigned char * buffer, int ilength);
public:
	void Refresh(void);
	void Init(void);

protected:
	CSupervisor* GetSupervisor(void); // returns a pointer to the Supervisor
	CSupervisor* GetData(void); // return a pointer to the Data
	BOOL GetInteract(void);// Returns if the ECU is interactive
	BOOL GetCentigrade(void); // Returns if FreeScan is in Metric
	BOOL GetMiles(void); // Returns if FreeScan is in Metric
	void Enumerate(void); //Finds this computer's serial ports
	// Generated message map functions
	//{{AFX_MSG(CDetailDlg)
	afx_msg void OnStartlog();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnHide();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokComselect();
	afx_msg void OnListen();
	afx_msg void OnInteract();
	afx_msg void OnCsv();
	afx_msg void OnCsvoptions();
	afx_msg void OnSelendokModel();
	afx_msg void OnForce();
	afx_msg void OnKillfocusComselect();
	afx_msg void OnDegC();
	afx_msg void OnDegF();
	afx_msg void OnKph();
	afx_msg void OnMph();
	afx_msg void OnSelendokDelay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETAILDLG_H__09BF9084_1326_11D2_983E_00E018900F2A__INCLUDED_)
