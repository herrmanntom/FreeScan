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

static const int THROTTLE_MIN = 0;
static const int THROTTLE_MAX = 100;

static const int LOAD_MIN = 0;
static const int LOAD_MAX = 100;

static const int BOOST_MIN = 0;
static const int BOOST_MAX = 200;

static const int MAT_MIN = 200;
static const int MAT_MAX = 800;

static const int SPEEDO_MIN = 0;
static const int SPEEDO_MAX = 170;

static const int TACHO_MIN = 0;
static const int TACHO_MAX = 8000;

static const int AIR_FUEL_RATIO_MIN = 110;
static const int AIR_FUEL_RATIO_MAX = 170;

static const int WATER_MIN = 500;
static const int WATER_MAX = 1100;

static const int BAT_VOLT_MIN = 80;
static const int BAT_VOLT_MAX = 160;

static const int SPARK_MIN = 0;
static const int SPARK_MAX = 600;

/////////////////////////////////////////////////////////////////////////////
// CDashBoardDlg property page

IMPLEMENT_DYNCREATE(CDashBoardDlg, CTTPropertyPage)

CDashBoardDlg::CDashBoardDlg() : CTTPropertyPage(CDashBoardDlg::IDD)
{
	//{{AFX_DATA_INIT(CDashBoardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDlg = NULL;
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
	DDX_Control(pDX, IDC_BOOST_TEXT, m_BoostText);
	DDX_Control(pDX, IDC_MAT, m_MAT);
	DDX_Control(pDX, IDC_MAT_TEXT, m_MATText);
	DDX_Control(pDX, IDC_SPEEDO, m_Speedo);
	DDX_Control(pDX, IDC_SPEEDO_MPH_TEXT, m_SpeedoMphText);
	DDX_Control(pDX, IDC_SPEEDO_KPH_TEXT, m_SpeedoKphText);
	DDX_Control(pDX, IDC_TACHO, m_Tacho);
	DDX_Control(pDX, IDC_TACHO_TEXT, m_TachoText);	
	DDX_Control(pDX, IDC_AIRFUEL, m_AirFuelRatio);
	DDX_Control(pDX, IDC_AIRFUEL_TEXT, m_AirFuelRatioText);
	DDX_Control(pDX, IDC_WATER, m_Water);
	DDX_Control(pDX, IDC_WATER_TEXT, m_WaterText);	
	DDX_Control(pDX, IDC_VOLT, m_Volt);
	DDX_Control(pDX, IDC_VOLT_TEXT, m_VoltText);
	DDX_Control(pDX, IDC_SPARK, m_Spark);
	DDX_Control(pDX, IDC_SPARK_TEXT, m_SparkText);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	Refresh(GetSupervisor()->GetEcuData());
}

// Returns a pointer to the Supervisor
CSupervisor* CDashBoardDlg::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns if the ECU is interactive
BOOL CDashBoardDlg::GetInteract(void)
{
	return GetSupervisor()->GetInteract();
}

static inline void updateField(CProgressCtrl *const progressMeter, CEdit * const textBox, const char *const textFormat, const float fValue, const int progressMeterScaleFactor, const int progressMeterMin, const int progressMeterMax) {
	if (fValue != CEcuData::c_fUNSUPPORTED) {
		if (textBox != NULL && textFormat != NULL) {
			CString buf;
			buf.Format(textFormat, fValue);
			textBox->SetWindowText(buf);
		}

		if (progressMeter != NULL) {
			int progressMeterValue = (int)(fValue * progressMeterScaleFactor);
			if (progressMeterValue < progressMeterMin) {
				progressMeterValue = progressMeterMin;
			}
			else if (progressMeterValue > progressMeterMax) {
				progressMeterValue = progressMeterMax;
			}
			progressMeter->SetPos(progressMeterValue);
		}
	}
	else if (textBox != NULL) {
		textBox->SetWindowText("N/A ");
	}
}

static inline void updateField(CProgressCtrl *const progressMeter, CEdit * const textBox, const char *const textFormat, const int iValue, const int progressMeterScaleFactor, const int progressMeterMin, const int progressMeterMax) {
	if (iValue != CEcuData::c_iUNSUPPORTED) {
		if (textBox != NULL) {
			CString buf;
			buf.Format(textFormat, iValue);
			textBox->SetWindowText(buf);
		}

		int progressMeterValue = iValue * progressMeterScaleFactor;
		if (progressMeterValue < progressMeterMin) {
			progressMeterValue = progressMeterMin;
		}
		else if (progressMeterValue > progressMeterMax) {
			progressMeterValue = progressMeterMax;
		}
		progressMeter->SetPos(progressMeterValue);
	}
	else if (textBox != NULL) {
		textBox->SetWindowText("N/A ");
	}
}

// Updates all of our controls
void CDashBoardDlg::Refresh(const CEcuData* const ecuData)
{
	updateField(&m_AirFuelRatio, &m_AirFuelRatioText, "%3.1f ", ecuData->m_fAFRatio, AIR_FUEL_RATIO_MIN, AIR_FUEL_RATIO_MAX, 10);

	updateField(&m_Water, &m_WaterText, "%3.1f ", ecuData->m_fWaterTemp, WATER_MIN, WATER_MAX, 10);
	
	updateField(&m_MAT, &m_MATText, "%3.1f ", ecuData->m_fMATTemp, MAT_MIN, MAT_MAX, 10);
	
	updateField(&m_Volt, &m_VoltText, "%3.1f ", ecuData->m_fBatteryVolts, BAT_VOLT_MIN, BAT_VOLT_MAX, 10);
	
	updateField(&m_Boost, &m_BoostText, "%3.2f ", ecuData->m_fMAP, BOOST_MIN, BOOST_MAX, 100);
	
	updateField(&m_Spark, &m_SparkText, "%3.1f ", ecuData->m_fSparkAdvance, SPARK_MIN, SPARK_MAX, 10);

	updateField(&m_Tacho, &m_TachoText, "%4d ", ecuData->m_iRPM, TACHO_MIN, TACHO_MAX, 1);

	updateField(&m_Speedo, &m_SpeedoMphText, "%3d ", ecuData->m_iMPH,       SPEEDO_MIN, SPEEDO_MAX, 1);
	updateField(NULL,      &m_SpeedoKphText, "%3d ", ecuData->m_iMPH_inKPH, 0,   0, 1);

	updateField(&m_Throttle, NULL, NULL, ecuData->m_iThrottlePos, THROTTLE_MIN, THROTTLE_MAX, 1);

	updateField(&m_EngineLoad, NULL, NULL, ecuData->m_iEngineLoad, LOAD_MIN, LOAD_MAX, 1);
}

void CDashBoardDlg::RegisterMainDialog(CFreeScanDlg* const mainDialog) {
	m_pMainDlg = mainDialog;
}

BEGIN_MESSAGE_MAP(CDashBoardDlg, CTTPropertyPage)
	//{{AFX_MSG_MAP(CDashBoardDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static inline void setProgressMeterBounds(CProgressCtrl *const progressMeter, const int progressMeterMin, const int progressMeterMax) {
	progressMeter->SetRange(progressMeterMin, progressMeterMax);
	progressMeter->SetStep(min(1, (progressMeterMax - progressMeterMin) / 100));
}

/////////////////////////////////////////////////////////////////////////////
// CDashBoardDlg message handlers

BOOL CDashBoardDlg::OnInitDialog() 
{
	// Create the different background colour for this page
	m_brush.CreateSolidBrush(RGB(0, 0, 0)); // Black

	CTTPropertyPage::OnInitDialog();

	setProgressMeterBounds(&m_Throttle, THROTTLE_MIN, THROTTLE_MAX);

	setProgressMeterBounds(&m_EngineLoad, LOAD_MIN, LOAD_MAX);

	setProgressMeterBounds(&m_Boost, BOOST_MIN, BOOST_MAX);

	setProgressMeterBounds(&m_MAT, MAT_MIN, MAT_MAX);

	setProgressMeterBounds(&m_Speedo, SPEEDO_MIN, SPEEDO_MAX);

	setProgressMeterBounds(&m_Tacho, TACHO_MIN, TACHO_MAX);

	setProgressMeterBounds(&m_AirFuelRatio, AIR_FUEL_RATIO_MIN, AIR_FUEL_RATIO_MAX);

	setProgressMeterBounds(&m_Water, WATER_MIN, WATER_MAX);

	setProgressMeterBounds(&m_Volt, BAT_VOLT_MIN, BAT_VOLT_MAX);

	setProgressMeterBounds(&m_Spark, SPARK_MIN, SPARK_MAX);

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
