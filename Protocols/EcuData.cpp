
#include "EcuData.h"

const float CEcuData::c_fUNSUPPORTED = -4204.2f;
const int   CEcuData::c_iUNSUPPORTED = (int) c_fUNSUPPORTED;
const BOOL  CEcuData::c_bUNSUPPORTED = (BOOL) c_fUNSUPPORTED;
const float CEcuData::c_fSUPPORTED_BY_PROTOCOL = c_fUNSUPPORTED + 10.01f;
const int   CEcuData::c_iSUPPORTED_BY_PROTOCOL = (int) c_fSUPPORTED_BY_PROTOCOL;
const BOOL  CEcuData::c_bSUPPORTED_BY_PROTOCOL = (BOOL) c_fSUPPORTED_BY_PROTOCOL;

BOOL CEcuData::isValid(const float value) {
	if (value != c_fUNSUPPORTED && value != c_fSUPPORTED_BY_PROTOCOL) {
		return TRUE;
	}
	return FALSE;
}
BOOL CEcuData::isValid(const int value) {
	if (value != c_iUNSUPPORTED && value != c_iSUPPORTED_BY_PROTOCOL) {
		return TRUE;
	}
	return FALSE;
}
BOOL CEcuData::isSupported(const float value) {
	if (value != c_fUNSUPPORTED) {
		return TRUE;
	}
	return FALSE;
}
BOOL CEcuData::isSupported(const int value) {
	if (value != c_iUNSUPPORTED) {
		return TRUE;
	}
	return FALSE;
}

const int MAX_RAW_DATA_SIZE = 100;

CEcuData::CEcuData() {
	m_iCsvColumnCount = c_iUNSUPPORTED;

	m_ucF005 = new unsigned char[MAX_RAW_DATA_SIZE]; // never should get this big
	m_ucF00A = new unsigned char[MAX_RAW_DATA_SIZE]; // never should get this big
	m_ucF001 = new unsigned char[MAX_RAW_DATA_SIZE]; // Mode 1 data buffer
	m_ucF002 = new unsigned char[MAX_RAW_DATA_SIZE]; // Mode 2 data buffer
	m_ucF003 = new unsigned char[MAX_RAW_DATA_SIZE]; // Mode 3 data buffer
	m_ucF004 = new unsigned char[MAX_RAW_DATA_SIZE]; // Mode 4 data buffer

	memset(m_ucF005, 0, MAX_RAW_DATA_SIZE);
	memset(m_ucF00A, 0, MAX_RAW_DATA_SIZE);
	memset(m_ucF001, 0, MAX_RAW_DATA_SIZE);
	memset(m_ucF002, 0, MAX_RAW_DATA_SIZE);
	memset(m_ucF003, 0, MAX_RAW_DATA_SIZE);
	memset(m_ucF004, 0, MAX_RAW_DATA_SIZE);
	
	m_csProtocolComment = "";
	
	m_csDTC = ""; // Reset Fault Codes

	// Reset normal engine parameters
	m_bEngineClosedLoop = c_bUNSUPPORTED;
	m_bEngineStalled = c_bUNSUPPORTED;
	m_bACRequest = c_bUNSUPPORTED;
	m_bACClutch = c_bUNSUPPORTED;
	m_fBatteryVolts = c_fUNSUPPORTED;
	m_iRPM = c_iUNSUPPORTED;
	m_iIACPosition = c_iUNSUPPORTED;
	m_iDesiredIdle = c_iUNSUPPORTED;
	m_iRoadSpeed_MPH = c_iUNSUPPORTED;
	m_fStartCoolantTemp = c_fUNSUPPORTED;
	m_fCoolantTemp = c_fUNSUPPORTED;
	m_iCoolantTempADC = c_iUNSUPPORTED;
	m_fOilTemp = c_fUNSUPPORTED;
	m_fCoolantVolts = c_fUNSUPPORTED;
	m_fMATemp = c_fUNSUPPORTED;
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

	//ResetVariablesForGuiTest();
}

CEcuData::CEcuData(const CEcuData& other) {
	m_ucF005 = new unsigned char[MAX_RAW_DATA_SIZE];
	m_ucF00A = new unsigned char[MAX_RAW_DATA_SIZE];
	m_ucF001 = new unsigned char[MAX_RAW_DATA_SIZE];
	m_ucF002 = new unsigned char[MAX_RAW_DATA_SIZE];
	m_ucF003 = new unsigned char[MAX_RAW_DATA_SIZE];
	m_ucF004 = new unsigned char[MAX_RAW_DATA_SIZE];

	copyFields(&other);
}

void CEcuData::copyFields(const CEcuData* const other) {
	if (other == NULL) {
		ASSERT(false);
		return;
	}

	m_iCsvColumnCount = other->m_iCsvColumnCount;

	memcpy(m_ucF005, other->m_ucF005, sizeof(m_ucF005));
	memcpy(m_ucF00A, other->m_ucF00A, sizeof(m_ucF00A));
	memcpy(m_ucF001, other->m_ucF001, sizeof(m_ucF001));
	memcpy(m_ucF002, other->m_ucF002, sizeof(m_ucF002));
	memcpy(m_ucF003, other->m_ucF003, sizeof(m_ucF003));
	memcpy(m_ucF004, other->m_ucF004, sizeof(m_ucF004));

	m_csProtocolComment = other->m_csProtocolComment;

	m_csDTC = other->m_csDTC;

	m_bEngineClosedLoop = other->m_bEngineClosedLoop;
	m_bEngineStalled = other->m_bEngineStalled;
	m_bACRequest = other->m_bACRequest;
	m_bACClutch = other->m_bACClutch;
	m_fBatteryVolts = other->m_fBatteryVolts;
	m_iRPM = other->m_iRPM;
	m_iIACPosition = other->m_iIACPosition;
	m_iDesiredIdle = other->m_iDesiredIdle;
	m_iRoadSpeed_MPH = other->m_iRoadSpeed_MPH;
	m_fStartCoolantTemp = other->m_fStartCoolantTemp;
	m_fCoolantTemp = other->m_fCoolantTemp;
	m_fCoolantVolts = other->m_fCoolantVolts;
	m_iCoolantTempADC = other->m_iCoolantTempADC;
	m_fOilTemp = other->m_fOilTemp;
	m_fMATemp = other->m_fMATemp;
	m_fMATVolts = other->m_fMATVolts;
	m_iMATADC = other->m_iMATADC;
	m_iEpromID = other->m_iEpromID;
	m_iRunTime = other->m_iRunTime;
	m_iCrankSensors = other->m_iCrankSensors;
	m_iThrottlePos = other->m_iThrottlePos;
	m_fThrottleVolts = other->m_fThrottleVolts;
	m_iThrottleADC = other->m_iThrottleADC;
	m_iEngineLoad = other->m_iEngineLoad;
	m_fBaro = other->m_fBaro;
	m_fBaroVolts = other->m_fBaroVolts;
	m_iBaroADC = other->m_iBaroADC;
	m_fMAP = other->m_fMAP;
	m_fMAPVolts = other->m_fMAPVolts;
	m_iMAPADC = other->m_iMAPADC;
	m_iBoostPW = other->m_iBoostPW;
	m_iCanisterDC = other->m_iCanisterDC;
	m_iSecondaryInjPW = other->m_iSecondaryInjPW;
	m_iInjectorBasePWMsL = other->m_iInjectorBasePWMsL;
	m_iInjectorBasePWMsR = other->m_iInjectorBasePWMsR;
	m_fAFRatio = other->m_fAFRatio;
	m_fAirFlow = other->m_fAirFlow;
	m_fO2VoltsLeft = other->m_fO2VoltsLeft;
	m_fO2VoltsRight = other->m_fO2VoltsRight;
	m_iIntegratorL = other->m_iIntegratorL;
	m_iIntegratorR = other->m_iIntegratorR;
	m_iBLM = other->m_iBLM;
	m_iBLMRight = other->m_iBLMRight;
	m_iBLMCell = other->m_iBLMCell;
	m_iRichLeanCounterL = other->m_iRichLeanCounterL;
	m_iRichLeanCounterR = other->m_iRichLeanCounterR;
	m_fSparkAdvance = other->m_fSparkAdvance;
	m_fKnockRetard = other->m_fKnockRetard;
	m_iKnockCount = other->m_iKnockCount;
}

CEcuData::~CEcuData() {
	delete m_ucF005;
	delete m_ucF00A;
	delete m_ucF001; // Delete Mode 1 data buffer
	delete m_ucF002; // Delete Mode 2 data buffer
	delete m_ucF003; // Delete Mode 3 data buffer
	delete m_ucF004; // Delete Mode 4 data buffer
}

static inline void copyAndSet(unsigned char* const target, const int targetLength, const unsigned char* const source, const int sourceLength) {
	if (targetLength >= 0 && sourceLength >= 0) {
		const int copyLength = min(sourceLength, targetLength);
		memcpy(target, source, copyLength);
		if ((targetLength - copyLength) > 0) {
			memset(target + copyLength, 0, targetLength - copyLength);
		}
	}
}

inline int CEcuData::convertSpeed(const BOOL inMph, const int valueInMph) {
	if (inMph == TRUE) {
		return valueInMph;
	}
	else {
		if (CEcuData::isValid(valueInMph)) {
			return (int)((float)valueInMph * 1.609344f);
		}
		else if (CEcuData::isSupported(valueInMph)) {
			return CEcuData::c_iSUPPORTED_BY_PROTOCOL;
		}
		else {
			return CEcuData::c_iUNSUPPORTED;
		}
	}
}

inline float CEcuData::convertTemperature(const BOOL inDgC, const float valueInDgC) {
	if (inDgC == TRUE) {
		return valueInDgC;
	}
	else {
		if (CEcuData::isValid(valueInDgC)) {
			return (valueInDgC * 1.8f) + 32.0f;
		}
		else if (CEcuData::isSupported(valueInDgC)) {
			return CEcuData::c_fSUPPORTED_BY_PROTOCOL;
		}
		else {
			return CEcuData::c_fUNSUPPORTED;
		}
	}
}

void CEcuData::copyToF005(const unsigned char* const sourceBuffer, const int sourceLength) {
	copyAndSet(m_ucF005, MAX_RAW_DATA_SIZE, sourceBuffer, sourceLength);
}
void CEcuData::copyToF00A(const unsigned char* const sourceBuffer, const int sourceLength) {
	copyAndSet(m_ucF00A, MAX_RAW_DATA_SIZE, sourceBuffer, sourceLength);
}
void CEcuData::copyToF001(const unsigned char* const sourceBuffer, const int sourceLength) {
	copyAndSet(m_ucF001, MAX_RAW_DATA_SIZE, sourceBuffer, sourceLength);
}
void CEcuData::copyToF002(const unsigned char* const sourceBuffer, const int sourceLength) {
	copyAndSet(m_ucF002, MAX_RAW_DATA_SIZE, sourceBuffer, sourceLength);
}
void CEcuData::copyToF003(const unsigned char* const sourceBuffer, const int sourceLength) {
	copyAndSet(m_ucF003, MAX_RAW_DATA_SIZE, sourceBuffer, sourceLength);
}
void CEcuData::copyToF004(const unsigned char* const sourceBuffer, const int sourceLength) {
	copyAndSet(m_ucF004, MAX_RAW_DATA_SIZE, sourceBuffer, sourceLength);
}

void CEcuData::setRoadSpeed_MPH(const int value) {
	m_iRoadSpeed_MPH = value;
}
void CEcuData::setStartCoolantTemp_dgC(const float value) {
	m_fStartCoolantTemp = value;
}
void CEcuData::setCoolantTemp_dgC(const float value) {
	m_fCoolantTemp = value;
}
void CEcuData::setOilTemp_dgC(const float value) {
	m_fOilTemp = value;
}
void CEcuData::setMATemp_dgC(const float value) {
	m_fMATemp = value;
}

void CEcuData::copyFromF005(unsigned char* const targetBuffer, int targetBufferLen) const {
	copyAndSet(targetBuffer, targetBufferLen, m_ucF005, MAX_RAW_DATA_SIZE);
}
void CEcuData::copyFromF00A(unsigned char* const targetBuffer, int targetBufferLen) const {
	copyAndSet(targetBuffer, targetBufferLen, m_ucF00A, MAX_RAW_DATA_SIZE);
}
void CEcuData::copyFromF001(unsigned char* const targetBuffer, int targetBufferLen) const {
	copyAndSet(targetBuffer, targetBufferLen, m_ucF001, MAX_RAW_DATA_SIZE);
}
void CEcuData::copyFromF002(unsigned char* const targetBuffer, int targetBufferLen) const {
	copyAndSet(targetBuffer, targetBufferLen, m_ucF002, MAX_RAW_DATA_SIZE);
}
void CEcuData::copyFromF003(unsigned char* const targetBuffer, int targetBufferLen) const {
	copyAndSet(targetBuffer, targetBufferLen, m_ucF003, MAX_RAW_DATA_SIZE);
}
void CEcuData::copyFromF004(unsigned char* const targetBuffer, int targetBufferLen) const {
	copyAndSet(targetBuffer, targetBufferLen, m_ucF004, MAX_RAW_DATA_SIZE);
}

int CEcuData::getRoadSpeed(const BOOL inMph) const {
	return convertSpeed(inMph, m_iRoadSpeed_MPH);
}
float CEcuData::getStartCoolantTemp(const BOOL inDgC) const {
	return convertTemperature(inDgC, m_fStartCoolantTemp);
}
float CEcuData::getCoolantTemp(const BOOL inDgC) const {
	return convertTemperature(inDgC, m_fCoolantTemp);
}
float CEcuData::getOilTemp(const BOOL inDgC) const {
	return convertTemperature(inDgC, m_fOilTemp);
}
float CEcuData::getMATemp(const BOOL inDgC) const {
	return convertTemperature(inDgC, m_fMATemp);
}

static CString generateCsvColumn(const BOOL header, int* const columnCount, const float fValue, const char* const title, const char* const format) {
	CString	csBuf = _T("");
	if (CEcuData::isSupported(fValue)) {
		if (header == TRUE) {
			csBuf += _T(",");
			csBuf += _T(title);
		}
		else if (CEcuData::isValid(fValue)) {
			csBuf.Format(_T(format), fValue);
			csBuf = _T(",") + csBuf;
		}
		else {
			csBuf = _T(", ");
		}
		(*columnCount)++;
	}
	return csBuf;
}

static CString generateCsvColumn(const BOOL header, int* const columnCount, const int iValue, const char* const title, const char* const format) {
	CString	csBuf = _T("");
	if (CEcuData::isSupported(iValue)) {
		if (header == TRUE) {
			csBuf += _T(",");
			csBuf += _T(title);
		}
		else if (CEcuData::isValid(iValue)) {
			csBuf.Format(_T(format), iValue);
			csBuf = _T(",") + csBuf;
		}
		else {
			csBuf = _T(", ");
		}
		(*columnCount)++;
	}
	return csBuf;
}

CString CEcuData::generateCsvLine(const BOOL header) {
	int columnCount = 0;
	CString	csBuf = _T("");
	// main engine / vehicle fields
	csBuf += generateCsvColumn(header, &columnCount, m_iRPM, "Engine Speed (RPM)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iDesiredIdle, "Desired Idle (RPM)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iRoadSpeed_MPH, "Road Speed (MPH)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, convertSpeed(FALSE, m_iRoadSpeed_MPH), "Road Speed (KPH)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_fOilTemp, "Oil Temperature (°C)", "%3.1f");
	csBuf += generateCsvColumn(header, &columnCount, m_fCoolantTemp, "Coolant Temperature (°C)", "%3.1f");
	csBuf += generateCsvColumn(header, &columnCount, m_fStartCoolantTemp, "Start Coolant Temperature (°C)", "%3.1f");
	csBuf += generateCsvColumn(header, &columnCount, m_iThrottlePos, "Throttle Position (%)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iEngineLoad, "Engine Load (%)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_fBaro, "Atmospheric Pressure (bar)", "%4.2f");
	csBuf += generateCsvColumn(header, &columnCount, m_fMAP, "Manifold Air Pressure (bar)", "%4.2f");
	csBuf += generateCsvColumn(header, &columnCount, m_fMATemp, "Manifold Air Temperature (°C)", "%3.1f");
	csBuf += generateCsvColumn(header, &columnCount, m_fAirFlow, "Air Flow", "%3.1f");
	csBuf += generateCsvColumn(header, &columnCount, m_fBatteryVolts, "Battery (V)", "%3.1f");
	csBuf += generateCsvColumn(header, &columnCount, m_bACRequest, "Air Conditioner Demand", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_bACClutch, "Air Conditioner Clutch Engaged", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_bEngineStalled, "Engine Stalled", "%d");

	// parameters controlled by engine management 
	csBuf += generateCsvColumn(header, &columnCount, m_fAFRatio, "Air:Fuel (ratio)", "%3.1f");
	csBuf += generateCsvColumn(header, &columnCount, m_fSparkAdvance, "Spark Advance (°)", "%3.1f");
	csBuf += generateCsvColumn(header, &columnCount, m_fKnockRetard, "Knock Retard (°)", "%3.1f");
	csBuf += generateCsvColumn(header, &columnCount, m_iInjectorBasePWMsL, "Injector Base Pule Width (ms)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iInjectorBasePWMsR, "Injector Base Pule Width [Right Bank] (ms)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iSecondaryInjPW, "Secondary Injectors Pule Width (ms)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iBoostPW, "Wastegate Pule Width (ms)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iIACPosition, "IAC Position", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iCanisterDC, "Charcoal Canister Purge Duty Cycle", "%d");

	// parameters used by engine management to analyze and track engine state
	csBuf += generateCsvColumn(header, &columnCount, m_iCrankSensors, "Crank Sensor (Counter)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iKnockCount, "Knock (Counter)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_fO2VoltsLeft, "O2 Sensor (V)", "%5.3f");
	csBuf += generateCsvColumn(header, &columnCount, m_fO2VoltsRight, "O2 Sensor [Right Bank] (V)", "%5.3f");
	csBuf += generateCsvColumn(header, &columnCount, m_iRichLeanCounterL, "Rich / Lean (Counter)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iRichLeanCounterR, "Rich / Lean [Right Bank] (Counter)", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iIntegratorL, "Integrator", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iIntegratorR, "Integrator [Right Bank]", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iBLM, "BLM value", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iBLMRight, "BLM value [Right Bank]", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iBLMCell, "BLM Cell", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_bEngineClosedLoop, "Closed Loop", "%d");
	csBuf += generateCsvColumn(header, &columnCount, m_iRunTime, "Engine Running Time (s)", "%d");

	// redundant "raw voltages" from sensor that are represented with interpreted values above
	csBuf += generateCsvColumn(header, &columnCount, m_fCoolantVolts, "Coolant Sensor (V)", "%4.2f");
	csBuf += generateCsvColumn(header, &columnCount, m_fThrottleVolts, "Throttle Position Sensor (V)", "%4.2f");
	csBuf += generateCsvColumn(header, &columnCount, m_fBaroVolts, "Atmospheric Pressure Sensor (V)", "%4.2f");
	csBuf += generateCsvColumn(header, &columnCount, m_fMAPVolts, "Manifold Air Pressure Sensor (V)", "%4.2f");
	csBuf += generateCsvColumn(header, &columnCount, m_fMATVolts, "Manifold Air Temperature Sensor (V)", "%4.2f");

	if (m_iCsvColumnCount < 0) {
		m_iCsvColumnCount = columnCount;
	}
	else if (m_iCsvColumnCount != columnCount) {
		return "[CSV Implemenation ERROR!]" + csBuf;
	}
	return csBuf;
}

// Reset variables for GUI test
void CEcuData::SetVariablesForGuiTest(void)
{
	m_csProtocolComment = "";

	memset(m_ucF005, 0, MAX_RAW_DATA_SIZE);
	memset(m_ucF00A, 0, MAX_RAW_DATA_SIZE);
	memset(m_ucF001, 0, MAX_RAW_DATA_SIZE);
	memset(m_ucF002, 0, MAX_RAW_DATA_SIZE);
	memset(m_ucF003, 0, MAX_RAW_DATA_SIZE);
	memset(m_ucF004, 0, MAX_RAW_DATA_SIZE);

	m_csDTC = "23 - Mass Air Temperature (MAT) Sensor Circuit; Low Temperature Indicated\n"; // Reset Fault Codes

	// Reset normal engine parameters
	m_bEngineClosedLoop = TRUE; //
	m_bEngineStalled = FALSE;  // bit 6
	m_bACRequest = FALSE; // mode 1,  bit 0
	m_bACClutch = FALSE; // mode 1, bit 2
	m_fBatteryVolts = 13.1f;
	m_iRPM = 4321;
	m_iIACPosition = 48;
	m_iDesiredIdle = 987;
	m_iRoadSpeed_MPH = 133;
	m_fStartCoolantTemp = 16.3f;
	m_fCoolantTemp = 89.8f;
	m_iCoolantTempADC = 115;
	m_fOilTemp = c_fUNSUPPORTED;
	m_fCoolantVolts = 2.25f;
	m_fMATemp = -38.0f;
	m_fMATVolts = 5.0f;
	m_iMATADC = 0;
	m_iEpromID = 2475;
	m_iRunTime = 758;
	m_iCrankSensors = 3;
	m_iThrottlePos = 72;
	m_fThrottleVolts = 3.08f;
	m_iThrottleADC = 157;
	m_iEngineLoad = 92;
	m_fBaro = 0.79f;
	m_fBaroVolts = 2.14f;
	m_iBaroADC = 109;
	m_fMAP = 1.98f;
	m_fMAPVolts = 4.47f;
	m_iMAPADC = 228;
	m_iBoostPW = 58; // Pulse-width of the turbo boost controller
	m_iCanisterDC = c_iUNSUPPORTED; // Duty Cycle of Charcoal Cansister controller
	m_iSecondaryInjPW = 0; // Pulse-width of secondary injectors
	m_iInjectorBasePWMsL = 10; // Injector Opening Time in Ms Left
	m_iInjectorBasePWMsR = c_iUNSUPPORTED; // Injector Opening Time in Ms Right
	m_fAFRatio = 12.7f; // Air Fuel Ratio
	m_fAirFlow = c_fUNSUPPORTED; // Air Flow
	m_fSparkAdvance = 15.1f;
	m_fKnockRetard = 0;
	m_iKnockCount = 3;
	m_fO2VoltsLeft = 0.897f;
	m_fO2VoltsRight = c_fUNSUPPORTED;
	m_iIntegratorL = 128; // Integrator Value Left
	m_iIntegratorR = c_iUNSUPPORTED; // Integrator Value Right
	m_iRichLeanCounterL = 99; // Rich/Lean Counter Left
	m_iRichLeanCounterR = c_iUNSUPPORTED; // Rich/Lean Counter Right
	m_iBLM = 150;	// Contents of the current BLM Cell
	m_iBLMRight = c_iUNSUPPORTED;	// Contents of the current BLM Cell
	m_iBLMCell = 15; // Current BLM Cell
}
