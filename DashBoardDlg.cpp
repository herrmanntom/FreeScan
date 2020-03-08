// DashBoardDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "Supervisor.h"

#include "DashBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDashBoardDlg property page

IMPLEMENT_DYNCREATE(CDashBoardDlg, CTTPropertyPage)

CDashBoardDlg::CDashBoardDlg() : CTTPropertyPage(CDashBoardDlg::IDD)
{
	//{{AFX_DATA_INIT(CDashBoardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDlg = NULL;
	
	m_fOilTemp = 0.0;
	m_fWaterTemp = 0.0;
	m_fMATTemp = 0.0;
	m_fBatteryVolts = 0.0;
}

CDashBoardDlg::~CDashBoardDlg()
{
}

void CDashBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CTTPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDashBoardDlg)
	DDX_Control(pDX, IDC_THROT, m_Throttle);
	DDX_Control(pDX, IDC_ENGINELOAD, m_EngineLoad);	
	DDX_Control(pDX, IDC_BOOST, m_Boost);
	DDX_Control(pDX, IDC_MAT, m_MAT);
	DDX_Control(pDX, IDC_SPEEDO, m_Speedo);
	DDX_Control(pDX, IDC_TACHO, m_Tacho);
	DDX_Control(pDX, IDC_OILTEMP, m_OilTemp);
	DDX_Control(pDX, IDC_WATER, m_Water);
	DDX_Control(pDX, IDC_VOLT, m_Volt);
	DDX_Control(pDX, IDC_SPARK, m_Spark);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	Refresh();
}

// Returns a pointer to the Supervisor
CSupervisor* CDashBoardDlg::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns a pointer to the Supervisor
CSupervisor* CDashBoardDlg::GetData(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns if the ECU is interactive
BOOL CDashBoardDlg::GetInteract(void)
{
	return GetSupervisor()->GetInteract();
}

// Returns the current ECU Mode
DWORD CDashBoardDlg::GetCurrentMode(void)
{
	return GetSupervisor()->GetCurrentMode();
}

// Updates all of our controls
void CDashBoardDlg::Refresh(void)
{
	CString buf;
	DWORD	dwCurrentMode = GetCurrentMode();

	float fValue = GetData()->m_fOilTemp;
	if (fValue != m_fOilTemp)
	{ // check for a different value
		buf.Format("Oil Temp: %3.1f", fValue);
		//m_OilTemp.SetCaption(buf);
		if (fValue < 49.5f) {
			fValue = 49.5f;
		}
		else if (fValue > 110.0f) {
			fValue = 110.0f;
		}
		m_OilTemp.SetPos((int) (fValue * 10));
		m_fOilTemp = fValue; // store the new value
	}

	fValue = GetData()->m_fWaterTemp;
	if (fValue != m_fWaterTemp)
	{ // check for a different value
		buf.Format("Water Temp: %3.1f", fValue);
		//m_Water.SetCaption(buf);
		if (fValue < 49.5f) {
			fValue = 49.5f;
		}
		else if (fValue > 110.0f) {
			fValue = 110.0f;
		}
		m_Water.SetPos((int) (fValue * 10));
		m_fWaterTemp = fValue; // store the new value
	}

	fValue = GetData()->m_fMATTemp;
	if (fValue != m_fMATTemp)
	{ // check for a different value
		buf.Format("Mass Air Temp: %3.1f", fValue);
		//m_MAT.SetCaption(buf);
		if (fValue < 19.5f) {
			fValue = 19.5f;
		}
		else if (fValue > 80.0f) {
			fValue = 80.0f;
		}
		m_MAT.SetPos((int) (fValue * 10));
		m_fMATTemp = fValue; // store the new value
	}

	fValue = GetData()->m_fBatteryVolts;
	if (fValue != m_fBatteryVolts)
	{ // check for a different value
		buf.Format("Battery Volts: %3.1f", fValue);
		//m_Volt.SetCaption(buf);
		if (fValue < 7.9f) {
			fValue = 7.9f;
		}
		else if (fValue > 16.0f) {
			fValue = 16.9f;
		}
		m_Volt.SetPos((int) (fValue * 10));
		m_fBatteryVolts = fValue; // store the new value
	}

	fValue = GetData()->m_fMAP;
	buf.Format("MAP: %3.2f", fValue);
	//m_Boost.SetCaption(buf);
	if (fValue < 0.0f) {
		fValue = 0.0f;
	}
	else if (fValue > 2.0f) {
		fValue = 2.0f;
	}
	m_Boost.SetPos((int) (fValue * 100));

	fValue = GetData()->m_fSparkAdvance;
	buf.Format("Spark Adv: % 3.1f", fValue);
	//m_Spark.SetCaption(buf);
	if (fValue < 0.0f) {
		fValue = 0.0f;
	}
	else if (fValue > 60.0f) {
		fValue = 60.0f;
	}
	m_Spark.SetPos((int) (fValue * 10));

	int iValue = GetData()->m_iRPM;
	buf.Format("RPM: %4d", iValue);
	//m_Tacho.SetCaption(buf);
	if (iValue < 0) {
		iValue = 0;
	}
	else if (iValue > 8000) {
		iValue = 8000;
	}
	m_Tacho.SetPos(iValue);

	iValue = GetData()->m_iMPH;
	buf.Format("MPH: %3d", iValue);
	//m_Speedo.SetCaption(buf);
	if (iValue < 0) {
		iValue = 0;
	}
	else if (iValue > 120) {
		iValue = 120;
	}
	m_Speedo.SetPos(iValue);

	m_Throttle.SetPos(GetData()->m_iThrottlePos);

	m_EngineLoad.SetPos(GetData()->m_iEngineLoad);
}

BEGIN_MESSAGE_MAP(CDashBoardDlg, CTTPropertyPage)
	//{{AFX_MSG_MAP(CDashBoardDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_THROT, &CDashBoardDlg::OnNMCustomdrawThrot)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDashBoardDlg message handlers

BOOL CDashBoardDlg::OnInitDialog() 
{
	// Create the different background colour for this page
	m_brush.CreateSolidBrush(RGB(0, 0, 0)); // Black

	CTTPropertyPage::OnInitDialog();

	m_Throttle.SetRange(0, 100);
	m_Throttle.SetStep(1);

	m_EngineLoad.SetRange(0, 100);
	m_EngineLoad.SetStep(1);

	m_Boost.SetRange32(0, 200);
	m_Boost.SetStep(5);

	m_MAT.SetRange32(195, 800);
	m_MAT.SetStep(5);

	m_Speedo.SetRange32(0, 120);
	m_Speedo.SetStep(1);

	m_Tacho.SetRange32(0, 8000);
	m_Tacho.SetStep(5);

	m_OilTemp.SetRange32(495, 1100);
	m_OilTemp.SetStep(5);

	m_Water.SetRange32(495, 1100);
	m_Water.SetStep(5);

	m_Volt.SetRange32(79, 160);
	m_Volt.SetStep(1);

	m_Spark.SetRange32(0, 600);
	m_Spark.SetStep(5);

	// Add dialog items that want ToolTip text
//	m_toolTip.AddTool( GetDlgItem(IDC_EPROMID), IDC_EPROMID);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDashBoardDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CTTPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
		switch (nCtlColor)
	{
	//Edit controls need white background and black text
	//Note the 'return hbr' which is needed to draw the Edit
	//control's internal background (as opposed to text background)
/*	case CTLCOLOR_EDIT:
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(255,255,255));
		return hbr; */
	//Static controls need white text and same background as m_brush
	case CTLCOLOR_STATIC:
		LOGBRUSH logbrush;
		m_brush.GetLogBrush( &logbrush );
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkColor(logbrush.lbColor);
		return m_brush;
/*	//For listboxes, scrollbars, buttons, messageboxes and dialogs,
	//use the new brush (m_brush)
	case CTLCOLOR_LISTBOX:
	case CTLCOLOR_SCROLLBAR:
	case CTLCOLOR_BTN:
	case CTLCOLOR_MSGBOX: */
	case CTLCOLOR_DLG:
		return m_brush;
	//This shouldn't occurr since we took all the cases, but
	//JUST IN CASE, return the new brush
	default:
		return m_brush;
	} 

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDashBoardDlg::OnNMCustomdrawThrot(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
