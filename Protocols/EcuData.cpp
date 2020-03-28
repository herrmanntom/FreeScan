#include "stdafx.h"
#include "EcuData.h"

const int   CEcuData::c_iUNSUPPORTED = -4240;
const float CEcuData::c_fUNSUPPORTED = -424.2f;

CEcuData::CEcuData()
{
	m_dwBytesSent = 0;
	m_dwBytesReceived = 0;

	m_ucF005 = new unsigned char[100]; // never should get this big
	m_ucF00A = new unsigned char[100]; // never should get this big
	m_ucF001 = new unsigned char[100]; // Mode 1 data buffer
	m_ucF002 = new unsigned char[100]; // Mode 2 data buffer
	m_ucF003 = new unsigned char[100]; // Mode 3 data buffer
	m_ucF004 = new unsigned char[100]; // Mode 4 data buffer

	ResetVariables();
}

CEcuData::CEcuData(const CEcuData& other): m_csProtocolComment(other.m_csProtocolComment), m_csDTC(other.m_csDTC)
{
	m_dwBytesSent = other.m_dwBytesSent;
	m_dwBytesReceived = other.m_dwBytesReceived;
	m_bEngineClosedLoop = other.m_bEngineClosedLoop;
	m_bEngineStalled = other.m_bEngineStalled;
	m_bACRequest = other.m_bACRequest;
	m_bACClutch = other.m_bACClutch;

	m_ucF005 = new unsigned char[100];
	memcpy(m_ucF005, other.m_ucF005, sizeof(m_ucF005));
	m_ucF00A = new unsigned char[100];
	memcpy(m_ucF00A, other.m_ucF00A, sizeof(m_ucF00A));
	m_ucF001 = new unsigned char[100];
	memcpy(m_ucF001, other.m_ucF001, sizeof(m_ucF001));
	m_ucF002 = new unsigned char[100];
	memcpy(m_ucF002, other.m_ucF002, sizeof(m_ucF002));
	m_ucF003 = new unsigned char[100];
	memcpy(m_ucF003, other.m_ucF003, sizeof(m_ucF003));
	m_ucF004 = new unsigned char[100];
	memcpy(m_ucF004, other.m_ucF004, sizeof(m_ucF004));
	
	m_fBatteryVolts = other.m_fBatteryVolts;
	m_iRPM = other.m_iRPM;
	m_iIACPosition = other.m_iIACPosition;
	m_iDesiredIdle = other.m_iDesiredIdle;
	m_iMPH = other.m_iMPH;
	m_iMPH_inKPH = other.m_iMPH_inKPH;
	m_fStartWaterTemp = other.m_fStartWaterTemp;
	m_fStartWaterTemp_inF = other.m_fStartWaterTemp_inF;
	m_fWaterTemp = other.m_fWaterTemp;
	m_fWaterTemp_inF = other.m_fWaterTemp_inF;
	m_fWaterVolts = other.m_fWaterVolts;
	m_iWaterTempADC = other.m_iWaterTempADC;
	m_fOilTemp = other.m_fOilTemp;
	m_fOilTemp_inF = other.m_fOilTemp_inF;
	m_fMATTemp = other.m_fMATTemp;
	m_fMATTemp_inF = other.m_fMATTemp_inF;
	m_fMATVolts = other.m_fMATVolts;
	m_iMATADC = other.m_iMATADC;
	m_iEpromID = other.m_iEpromID;
	m_iRunTime = other.m_iRunTime;
	m_iCrankSensors = other.m_iCrankSensors;
	m_iThrottlePos = other.m_iThrottlePos;
	m_fThrottleVolts = other.m_fThrottleVolts;
	m_iThrottleADC = other.m_iThrottleADC;
	m_iEngineLoad = other.m_iEngineLoad;
	m_fBaro = other.m_fBaro;
	m_fBaroVolts = other.m_fBaroVolts;
	m_iBaroADC = other.m_iBaroADC;
	m_fMAP = other.m_fMAP;
	m_fMAPVolts = other.m_fMAPVolts;
	m_iMAPADC = other.m_iMAPADC;
	m_iBoostPW = other.m_iBoostPW;
	m_iCanisterDC = other.m_iCanisterDC;
	m_iSecondaryInjPW = other.m_iSecondaryInjPW;
	m_iInjectorBasePWMsL = other.m_iInjectorBasePWMsL;
	m_iInjectorBasePWMsR = other.m_iInjectorBasePWMsR;
	m_fAFRatio = other.m_fAFRatio;
	m_fAirFlow = other.m_fAirFlow;
	m_fO2VoltsLeft = other.m_fO2VoltsLeft;
	m_fO2VoltsRight = other.m_fO2VoltsRight;
	m_iIntegratorL = other.m_iIntegratorL;
	m_iIntegratorR = other.m_iIntegratorR;
	m_iBLM = other.m_iBLM;
	m_iBLMRight = other.m_iBLMRight;
	m_iBLMCell = other.m_iBLMCell;
	m_iRichLeanCounterL = other.m_iRichLeanCounterL;
	m_iRichLeanCounterR = other.m_iRichLeanCounterR;
	m_fSparkAdvance = other.m_fSparkAdvance;
	m_fKnockRetard = other.m_fKnockRetard;
	m_iKnockCount = other.m_iKnockCount;
}

CEcuData::~CEcuData()
{
	delete m_ucF005;
	delete m_ucF00A;
	delete m_ucF001; // Delete Mode 1 data buffer
	delete m_ucF002; // Delete Mode 2 data buffer
	delete m_ucF003; // Delete Mode 3 data buffer
	delete m_ucF004; // Delete Mode 4 data buffer
}

// Reset variables.
void CEcuData::ResetVariables(void)
{
	m_csProtocolComment = "";

	memset(m_ucF005, 0, 100);
	memset(m_ucF00A, 0, 100);
	memset(m_ucF001, 0, 90);
	memset(m_ucF002, 0, 90);
	memset(m_ucF003, 0, 25);
	memset(m_ucF004, 0, 25);

	m_csDTC = "No reported faults."; // Reset Fault Codes

	// Reset normal engine parameters
	m_bEngineClosedLoop = FALSE; //
	m_bEngineStalled = TRUE;  // bit 6
	m_bACRequest = FALSE; // mode 1,  bit 0
	m_bACClutch = FALSE; // mode 1, bit 2
	m_fBatteryVolts = c_fUNSUPPORTED;
	m_iRPM = c_iUNSUPPORTED;
	m_iIACPosition = c_iUNSUPPORTED;
	m_iDesiredIdle = c_iUNSUPPORTED;
	m_iMPH = c_iUNSUPPORTED;
	m_iMPH_inKPH = c_iUNSUPPORTED;
	m_fStartWaterTemp = c_fUNSUPPORTED;
	m_fStartWaterTemp_inF = c_fUNSUPPORTED;
	m_fWaterTemp = c_fUNSUPPORTED;
	m_fWaterTemp_inF = c_fUNSUPPORTED;
	m_iWaterTempADC = c_iUNSUPPORTED;
	m_fOilTemp = c_fUNSUPPORTED;
	m_fOilTemp_inF = c_fUNSUPPORTED;
	m_fWaterVolts = c_fUNSUPPORTED;
	m_fMATTemp = c_fUNSUPPORTED;
	m_fMATTemp_inF = c_fUNSUPPORTED;
	m_fMATVolts = c_fUNSUPPORTED;
	m_iMATADC = c_iUNSUPPORTED;
	m_iEpromID = c_iUNSUPPORTED;
	m_iRunTime = c_iUNSUPPORTED;
	m_iCrankSensors = c_iUNSUPPORTED;
	m_iThrottlePos = c_iUNSUPPORTED;
	m_fThrottleVolts = c_fUNSUPPORTED;
	m_iThrottleADC = c_iUNSUPPORTED;
	m_iEngineLoad = c_iUNSUPPORTED;
	m_fBaro = c_fUNSUPPORTED;
	m_fBaroVolts = c_fUNSUPPORTED;
	m_iBaroADC = c_iUNSUPPORTED;
	m_fMAP = c_fUNSUPPORTED;
	m_fMAPVolts = c_fUNSUPPORTED;
	m_iMAPADC = c_iUNSUPPORTED;
	m_iBoostPW = c_iUNSUPPORTED; // Pulse-width of the turbo boost controller
	m_iCanisterDC = c_iUNSUPPORTED; // Duty Cycle of Charcoal Cansister controller
	m_iSecondaryInjPW = c_iUNSUPPORTED; // Pulse-width of secondary injectors
	m_iInjectorBasePWMsL = c_iUNSUPPORTED; // Injector Opening Time in Ms Left
	m_iInjectorBasePWMsR = c_iUNSUPPORTED; // Injector Opening Time in Ms Right
	m_fAFRatio = c_fUNSUPPORTED; // Air Fuel Ratio
	m_fAirFlow = c_fUNSUPPORTED; // Air Flow
	m_fSparkAdvance = c_fUNSUPPORTED;
	m_fKnockRetard = c_fUNSUPPORTED;
	m_iKnockCount = c_iUNSUPPORTED;
	m_fO2VoltsLeft = c_fUNSUPPORTED;
	m_fO2VoltsRight = c_fUNSUPPORTED;
	m_iIntegratorL = c_iUNSUPPORTED; // Integrator Value Left
	m_iIntegratorR = c_iUNSUPPORTED; // Integrator Value Right
	m_iRichLeanCounterL = c_iUNSUPPORTED; // Rich/Lean Counter Left
	m_iRichLeanCounterR = c_iUNSUPPORTED; // Rich/Lean Counter Right
	m_iBLM = c_iUNSUPPORTED;	// Contents of the current BLM Cell
	m_iBLMRight = c_iUNSUPPORTED;	// Contents of the current BLM Cell
	m_iBLMCell = c_iUNSUPPORTED; // Current BLM Cell
}
