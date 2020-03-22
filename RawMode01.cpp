// RawMode01.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "Supervisor.h"

#include "RawMode01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRawMode01 property page

IMPLEMENT_DYNCREATE(CRawMode01, CPropertyPage)

CRawMode01::CRawMode01() : CPropertyPage(CRawMode01::IDD)
{
	//{{AFX_DATA_INIT(CRawMode01)
	//}}AFX_DATA_INIT

	m_pMainDlg = NULL;
}

CRawMode01::~CRawMode01()
{
}

void CRawMode01::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRawMode01)
	DDX_Control(pDX, IDC_DB07, m_db07);
	DDX_Control(pDX, IDC_DB06, m_db06);
	DDX_Control(pDX, IDC_DB05, m_db05);
	DDX_Control(pDX, IDC_DB04, m_db04);
	DDX_Control(pDX, IDC_DB03, m_db03);
	DDX_Control(pDX, IDC_DB02, m_db02);
	DDX_Control(pDX, IDC_DB01, m_db01);
	DDX_Control(pDX, IDC_DB00, m_db00);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	Refresh(GetSupervisor()->GetEcuData());
}


BEGIN_MESSAGE_MAP(CRawMode01, CPropertyPage)
	//{{AFX_MSG_MAP(CRawMode01)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Returns a pointer to the Supervisor
CSupervisor* CRawMode01::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

/////////////////////////////////////////////////////////////////////////////
// CRawMode01 message handlers
// Updates all of our controls
void CRawMode01::Refresh(const CEcuData* const ecuData)
{
	CString buf;

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ecuData->m_ucF001[1],
		ecuData->m_ucF001[2],
		ecuData->m_ucF001[3],
		ecuData->m_ucF001[4],
		ecuData->m_ucF001[5],
		ecuData->m_ucF001[6],
		ecuData->m_ucF001[7],
		ecuData->m_ucF001[8]);
	m_db00.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ecuData->m_ucF001[9],
		ecuData->m_ucF001[10],
		ecuData->m_ucF001[11],
		ecuData->m_ucF001[12],
		ecuData->m_ucF001[13],
		ecuData->m_ucF001[14],
		ecuData->m_ucF001[15],
		ecuData->m_ucF001[16]);
	m_db01.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ecuData->m_ucF001[17],
		ecuData->m_ucF001[18],
		ecuData->m_ucF001[19],
		ecuData->m_ucF001[20],
		ecuData->m_ucF001[21],
		ecuData->m_ucF001[22],
		ecuData->m_ucF001[23],
		ecuData->m_ucF001[24]);
	m_db02.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ecuData->m_ucF001[25],
		ecuData->m_ucF001[26],
		ecuData->m_ucF001[27],
		ecuData->m_ucF001[28],
		ecuData->m_ucF001[29],
		ecuData->m_ucF001[30],
		ecuData->m_ucF001[31],
		ecuData->m_ucF001[32]);
	m_db03.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ecuData->m_ucF001[33],
		ecuData->m_ucF001[34],
		ecuData->m_ucF001[35],
		ecuData->m_ucF001[36],
		ecuData->m_ucF001[37],
		ecuData->m_ucF001[38],
		ecuData->m_ucF001[39],
		ecuData->m_ucF001[40]);
	m_db04.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ecuData->m_ucF001[41],
		ecuData->m_ucF001[42],
		ecuData->m_ucF001[43],
		ecuData->m_ucF001[44],
		ecuData->m_ucF001[45],
		ecuData->m_ucF001[46],
		ecuData->m_ucF001[47],
		ecuData->m_ucF001[48]);
	m_db05.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ecuData->m_ucF001[49],
		ecuData->m_ucF001[50],
		ecuData->m_ucF001[51],
		ecuData->m_ucF001[52],
		ecuData->m_ucF001[53],
		ecuData->m_ucF001[54],
		ecuData->m_ucF001[55],
		ecuData->m_ucF001[56]);
	m_db06.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		ecuData->m_ucF001[57],
		ecuData->m_ucF001[58],
		ecuData->m_ucF001[59],
		ecuData->m_ucF001[60],
		ecuData->m_ucF001[61],
		ecuData->m_ucF001[62],
		ecuData->m_ucF001[63],
		ecuData->m_ucF001[64]);
	m_db07.SetWindowText(buf);
}
