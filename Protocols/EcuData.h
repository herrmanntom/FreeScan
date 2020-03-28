#pragma once
class CEcuData
{
public:
	
	static const int   c_iUNSUPPORTED;
	static const float c_fUNSUPPORTED;

public:
	CEcuData();
	CEcuData(const CEcuData& other);
	~CEcuData();

	DWORD		m_dwBytesSent;
	DWORD		m_dwBytesReceived;
	CString		m_csProtocolComment; // public copy of the current protocol's comments

    // Data that has been read-in and parsed from the ECU.
	BOOL	m_bEngineClosedLoop; //
	BOOL	m_bEngineStalled; //

	BOOL	m_bACRequest; // bit 0
	BOOL	m_bACClutch; // bit 2

	// Raw Data : just a store so that we can debug
	unsigned char*	m_ucF005;
	unsigned char*	m_ucF00A;
	unsigned char*	m_ucF001; // Mode 1 data buffer
	unsigned char*	m_ucF002; // Mode 2 data buffer
	unsigned char*	m_ucF003; // Mode 3 data buffer
	unsigned char*	m_ucF004; // Mode 4 data buffer

	// Parsed Data Store
	CString			m_csDTC; // Fault Descriptions

	float	m_fBatteryVolts; // in V
	int		m_iRPM;
	int		m_iIACPosition; // Idle Stepper Motor Position 1 - 170
	int		m_iDesiredIdle;
	int		m_iMPH;
	int		m_iMPH_inKPH;
	float	m_fStartWaterTemp; // Coolant Temperature when engine was started Deg C
	float	m_fStartWaterTemp_inF;
	float	m_fWaterTemp; // Coolant Temperature Deg C
	float	m_fWaterTemp_inF;
	float	m_fWaterVolts; // Coolant Temperature Sensor Voltage
	int		m_iWaterTempADC; // Coolant Temperature A/D Converter
	float	m_fOilTemp; // Oil Temperature Deg C
	float	m_fOilTemp_inF;
	float	m_fMATTemp; // Manifold Air Temperature Deg C
	float	m_fMATTemp_inF;
	float	m_fMATVolts; // Manifold Air Temperature Sensor Voltage
	int 	m_iMATADC; // Manifold Air Temperature A/D Converter
	int		m_iEpromID;
	int		m_iRunTime; // in seconds
	int		m_iCrankSensors; // 0, 1 or 2
	int		m_iThrottlePos;
	float	m_fThrottleVolts; // Sensor Voltage
	int		m_iThrottleADC; // A/D Converter
	int		m_iEngineLoad;
	float	m_fBaro; // Air pressure in Bar absolute
	float	m_fBaroVolts; // Air pressure in Volts
	int		m_iBaroADC; // Air pressure in A/D Converter
	float	m_fMAP; // in Bar absolute
	float	m_fMAPVolts; // in Volts
	int		m_iMAPADC; // A/D Converter
	int		m_iBoostPW; // Pulse-width (duty cycle) of the turbo boost controller
	int		m_iCanisterDC; // Duty Cycle of Charcoal Cansister controller
	int		m_iSecondaryInjPW; // Pulse-width of the secondary injectors
	int		m_iInjectorBasePWMsL; // Injector Opening Time in Ms Left
	int		m_iInjectorBasePWMsR; // Injector Opening Time in Ms Right
	float	m_fAFRatio; // Air Fuel Ratio
	float	m_fAirFlow; // Air Flow
	float	m_fO2VoltsLeft; //O2 sensor volts left
	float	m_fO2VoltsRight; //O2 sensor volts right (if fitted)
	int		m_iIntegratorL; // Integrator Value Left
	int		m_iIntegratorR; // Integrator Value Right
	int		m_iBLM;	// Contents of the current BLM Cell
	int		m_iBLMRight;	// Contents of the current BLM Cell
	int		m_iBLMCell; // Current BLM Cell
	int		m_iRichLeanCounterL; // Rich/Lean Counter Left
	int		m_iRichLeanCounterR; // Rich/Lean Counter Right
	float	m_fSparkAdvance;
	float	m_fKnockRetard;
	int		m_iKnockCount;

private:
	void ResetVariables(void);
};

