// EspritParser.cpp: implementation of the CEspritParser class.
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//
// This contains all low-level parsing functions.

#include "stdafx.h"
#include "..\FreeScan.h"
#include "..\MainDlg.h"
#include "EspritParser.h"

#include "..\Supervisor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEspritParser::CEspritParser()
{
	m_pSupervisor = NULL;

	memset(m_ucDTC, 0, 3);// Reset DTC buffer

	CFreeScanApp* pApp = (CFreeScanApp*) AfxGetApp();
	m_csCSVLogFile = pApp->GetProfileString(_T("EspritParser"), _T("CSV Log Filename"), _T(""));

	m_dwCSVRecord = 0; // Initialise the CSV record number
}

CEspritParser::~CEspritParser()
{
	if (m_file.m_hFile != CFile::hFileNull)
	{ // close our log file if it's open
		m_file.Flush();
		m_file.Close(); // close the logging file when we exit.
	}

	CFreeScanApp* pApp = (CFreeScanApp*) AfxGetApp();
	pApp->WriteProfileString("EspritParser", "CSV Log Filename", m_csCSVLogFile);
}

//WriteCSV(..) Writes CSV data to our log file.
//If bTitle is true, the first title line is written, otherwise the data is written.
void CEspritParser::WriteCSV(BOOL bTitle) 
{
	if (m_file.m_pStream == NULL) return;// i.e. no file open

	CString	csBuf;
	if (bTitle)
	{
		m_dwCSVRecord = 0;
		csBuf = _T("PC-Timestamp,Esprit Sample,Coolant Sensor V,Start Water Temp,TPS V,Des Idle,RPM,Road Speed,Crank Sensors,O2,Rich/Lean,Integrator,BLM,BLM Cell,Injector Base PW,IAC,Baro,MAP,A:F,TPS,MAT V,Knock Retard,Knock Count,BatV,Load,Spark,Coolant Temp,MAT,Wastegate DC,Secondary Injectors DC,Engine Running Time,A/C Demand,A/C Clutch,Closed Loop,Charcoal Canister Purge DC");
	}
	else
	{
		SYSTEMTIME localTime;

		const CEcuData *const ecuData = m_pSupervisor->GetEcuData();
		
		GetLocalTime(&localTime);

		csBuf.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d,%ld,%4.2f,%3.1f,%4.2f,%d,%d,%d,%d,%5.3f,%d,%d,%d,%d,%d,%d,%4.2f,%4.2f,%3.1f,%d,%4.2f,%3.1f,%d,%3.1f,%d,%3.1f,%3.1f,%3.1f,%d,%d,%d,%d,%d,%d,%d"),
			localTime.wYear, localTime.wMonth, localTime.wDay, localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
			m_dwCSVRecord, ecuData->m_fWaterVolts, ecuData->m_fStartWaterTemp, ecuData->m_fThrottleVolts,
			ecuData->m_iDesiredIdle, ecuData->m_iRPM, ecuData->m_iMPH, ecuData->m_iCrankSensors, ecuData->m_fO2VoltsLeft, ecuData->m_iRichLeanCounterL,
			ecuData->m_iIntegratorL, ecuData->m_iBLM, ecuData->m_iBLMCell, ecuData->m_iInjectorBasePWMsL,
			ecuData->m_iIACPosition, ecuData->m_fBaro, ecuData->m_fMAP, ecuData->m_fAFRatio, ecuData->m_iThrottlePos,
			ecuData->m_fMATVolts, ecuData->m_fKnockRetard, ecuData->m_iKnockCount, ecuData->m_fBatteryVolts,
			ecuData->m_iEngineLoad, ecuData->m_fSparkAdvance ,ecuData->m_fWaterTemp, ecuData->m_fMATTemp, ecuData->m_iBoostPW, ecuData->m_iSecondaryInjPW,
			ecuData->m_iRunTime, ecuData->m_bACRequest, ecuData->m_bACClutch, ecuData->m_bEngineClosedLoop, ecuData->m_iCanisterDC);
		m_dwCSVRecord++;
	}
	csBuf = csBuf + _T("\n"); // Line Feed because we're logging to disk
	m_file.WriteString(csBuf);
}

// Starts or stops csv logging to file
BOOL CEspritParser::StartCSVLog(BOOL bStart)
{
	CString csBuf = _T("");

	if (!bStart)
	{ // we want to close the logging file
		if (m_file.m_hFile != CFile::hFileNull)
		{
			WriteStatusLogged(_T("CSV Log has been stopped"));
			m_file.Close(); // close the logging file when we exit.
		}
		else
			WriteStatusLogged(_T("CSV Log is already closed"));

		return FALSE;
	}

	// We now must want to log to a file

	// Construct our File Dialog
	CFileDialog		Dialog(FALSE, _T("csv"), 
						m_csCSVLogFile, 
						OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
						_T("log Files (*.csv)|*.csv|All Files (*.*)|*.*||"), NULL);

	// Change the title
	Dialog.m_ofn.lpstrTitle = _T("Create/Open CSV Log");

	// Display the dialog box
	if (Dialog.DoModal() == IDOK)
	{
		m_csCSVLogFile = Dialog.GetPathName();

		if (!m_file.Open(m_csCSVLogFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText))
		{
			csBuf.Format(_T("Cannot open %s"), m_csCSVLogFile.GetString());
			WriteStatus(csBuf);
			AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP );
			return FALSE;
		}

		WriteStatusLogged(_T("CSV Log has been opened"));
		WriteCSV(TRUE); // log our data to a csv file
	}
	else // User pressed cancel
		WriteStatus(_T("User cancelled CSV log"));

	if (m_file.m_hFile != NULL)
		return TRUE;
	else
		return FALSE;
}

// Tells the Supervisor that there's been a data change
void CEspritParser::UpdateDialog(void)
{
	m_pSupervisor->UpdateDialog();
	WriteCSV(FALSE); // log our data to a csv file
}

// Parses the buffer into real data
int CEspritParser::Parse(unsigned char* buffer, int iLength)
{
	WriteASCII(buffer, iLength); // Log the activity

	int	iIndex=0, iPacketLength;
	unsigned char* pPacketStart=NULL;

	BOOL dataWasUpdated = FALSE;

	// Scan the whole packet for its header.
	for(iIndex=0;iIndex<iLength;iIndex++)
	{
		pPacketStart = buffer + iIndex; // Marks the start of Packet for the CRC.
		switch(buffer[iIndex])
		{ // Find valid headers. Length parameter is always the 2nd byte.
		case ECU_HEADER_ESPRIT: // Packet Header Start
			{
				iIndex++; // now find the length
				iPacketLength = GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				if (iPacketLength != 0)
				{// Data in Header
					if(buffer[iIndex]==1) // length 65 including mode byte
						dataWasUpdated |= ParseMode1(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==2) // length 65 including mode byte
						dataWasUpdated |= ParseMode2(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==3) // length max 7 including mode byte
						dataWasUpdated |= ParseMode3(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==4) // length max 11 including mode byte
						dataWasUpdated |= ParseMode4(&buffer[iIndex], iPacketLength);
					else
					{
						CString	temp; // write to the spy window
						temp.Format(_T("Unrecognised Mode: %02x"), buffer[iIndex]);
						WriteStatus(temp);
					}
					iIndex += iPacketLength;
				}
				// Check CRC
				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus(_T("Checksum Error"));
				break;
			}
		case 0x0a: // Computed Values
			{
				iIndex++; // now find the length
				iPacketLength = GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseAnalogues(&buffer[iIndex], iPacketLength);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus(_T("Checksum Error"));
				break;
			}
		case 0x05: // ADC Values
			{
				iIndex++; // now find the length
				iPacketLength = GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseADC(&buffer[iIndex], iPacketLength);
				iIndex += iPacketLength; // should be 10 or 58 from ECU
				// Check CRC
				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus(_T("Checksum Error"));
				break;
			}
		default:
			{
			CString buf;
			buf.Format(_T("%02x <- Unrecognised Header"), buffer[iIndex]);
			WriteStatus(buf);
			}
		}// Switch
	}// for()

	// Force the main application to update itself
	if (dataWasUpdated) {
		UpdateDialog();
	}

	return iLength; // Successfully parsed.
}

// Translates the incoming data stream as ADC Values
BOOL CEspritParser::ParseADC(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len>10)
	{
		WriteStatus(_T("Warning: F005 larger than expected, packet truncated."));
		len = 10;
	}

	ecuData->copyToF005(buffer, len);

	// Work out real world data from the packet.

	if (buffer[63] & 0x80)
		ecuData->m_bEngineClosedLoop = TRUE;  // bit 7
	else
		ecuData->m_bEngineClosedLoop = FALSE; // bit 7

	if (buffer[0] & 0x40)
		ecuData->m_bEngineStalled = TRUE;  // bit 6
	else
		ecuData->m_bEngineStalled = FALSE; // bit 6

	ecuData->m_iMPH = (int)buffer[2]; // Count is in MPH
	ecuData->m_fBatteryVolts = (float)buffer[4] / (float)10.0;
	ecuData->m_fWaterTemp = ((float)buffer[9] * (float)0.75) - (float)40.0; // in °C

	return TRUE;
}

// Translates the incoming data stream as Analogue Values
BOOL CEspritParser::ParseAnalogues(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len>3)
	{
		WriteStatus(_T("Warning: F00A larger than expected, packet truncated."));
		len = 3;
	}

	ecuData->copyToF00A(buffer, len);

	// Work out real world data from the packet.

	ecuData->m_iRPM = ((int)buffer[1] * 256) + (int)buffer[2];

	return TRUE;
}

// Translates the incoming data stream as Mode 1
BOOL CEspritParser::ParseMode1(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len<10) // remember half duplex. We read our commands as well
	{
		WriteStatus(_T("Received our TX command echo for mode 1."));
		return FALSE;
	}
	else if (len>65)
	{
		WriteStatus(_T("Warning: F001 larger than expected, packet truncated."));
		len = 65;
	}

	ecuData->copyToF001(buffer, len);

	// Work out real-world data from the packet.
	// Mode number is in index 0
	if (buffer[63] & 0x80)
		ecuData->m_bEngineClosedLoop = TRUE;  // bit 7
	else
		ecuData->m_bEngineClosedLoop = FALSE; // bit 7

	if (buffer[64] & 0x40)
		ecuData->m_bEngineStalled = TRUE;  // bit 6
	else
		ecuData->m_bEngineStalled = FALSE; // bit 6

	// Status Word 2
	if (buffer[6] & 0x08)
		ecuData->m_bACRequest = TRUE;  // byte 6, bit 3
	else
		ecuData->m_bACRequest = FALSE; // byte 6, bit 3

	if (buffer[7] & 0x01)
		ecuData->m_bACClutch = TRUE;  // byte 7, bit 0
	else
		ecuData->m_bACClutch = FALSE; // byte 7, bit 0

	// Analogues
	ecuData->m_iEpromID = (int)buffer[2] + ((int)buffer[1] * 256);
	m_ucDTC[0] = buffer[3]; // Fault code byte 1
	m_ucDTC[1] = buffer[4]; // Fault code byte 2
	m_ucDTC[2] = buffer[5]; // Fault code byte 3
	ecuData->m_fWaterVolts = (float)(((float)buffer[8] / (float)255.0) * (float) 5.0);
	ecuData->m_iWaterTempADC = buffer[8]; // in Counts
	ecuData->m_fStartWaterTemp = ((float)buffer[9] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fThrottleVolts = (float)(((float)buffer[10] / (float)255.0) * (float) 5.0);
	ecuData->m_iThrottleADC = buffer[10]; // in Counts
	ecuData->m_iDesiredIdle = (int)((float)buffer[11] * (float) 12.5);
	ecuData->m_iRPM = ((int)buffer[12] * 256) + (int)buffer[13];
	ecuData->m_iMPH = (int)buffer[14]; // Count is in MPH
	ecuData->m_iCrankSensors = buffer[15];
	ecuData->m_fO2VoltsLeft = (float) buffer[17] * (float) 0.00444;
	ecuData->m_iRichLeanCounterL = (int)buffer[18];
	ecuData->m_iIntegratorL = (int)buffer[19];
	ecuData->m_iBLM = (int)buffer[20];
	ecuData->m_iBLMCell = (int)buffer[21];
	ecuData->m_iIACPosition = (int)buffer[23];
	ecuData->m_fBaro = (((float)buffer[24] - (float)130.0)/ (float)100) + (float) 1.0; // in Bar Absolute
	ecuData->m_fBaroVolts = ((float)buffer[24] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_iBaroADC = buffer[24]; // in Counts
	ecuData->m_fMAP = (((float)buffer[25] - (float)130.0)/ (float)100) + (float) 1.0; // in Bar Absolute
	ecuData->m_fMAPVolts = ((float)buffer[25] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_iMAPADC = buffer[25]; // in Counts
	ecuData->m_iThrottlePos = (int)((float)buffer[27] / (float)2.55);
	ecuData->m_fMATVolts = ((float)buffer[29] / (float)255.0) * (float)5.0; // in Volts
	ecuData->m_iMATADC = buffer[29]; // in Counts
	ecuData->m_iBoostPW = (int) ((float)buffer[31] / (float)2.55); // Boost Solenoid
	ecuData->m_fBatteryVolts = (float)buffer[34] / (float)10.0;
	ecuData->m_iEngineLoad = (int)((float)buffer[36] / (float) 2.55);
	ecuData->m_iSecondaryInjPW = (int) ((float)buffer[37] / (float)2.55); // Secondary Injectors
	ecuData->m_fSparkAdvance = ((float)((buffer[39] * 256) + buffer[40]) * (float)90.0) / (float)256.0; // in °
	ecuData->m_fWaterTemp = ((float)buffer[41] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fMATTemp = ((float)buffer[42] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_iKnockCount = (int)buffer[43];
	ecuData->m_fKnockRetard = ((float)buffer[44] * (float)22.5) / (float)256.0; // in °
	ecuData->m_iInjectorBasePWMsL = (int) ( (float)((buffer[45] * 256) + buffer[46]) / (float)65.536);
	ecuData->m_fAFRatio = (float)buffer[47] / (float)10.0; // Air Fuel Ratio
	ecuData->m_iRunTime = (buffer[52] * 256) + buffer[53]; // Total running time
	ecuData->m_iCanisterDC = (int)(((float)buffer[30]) / 2.56f); // Canister Purge
	
	ParseDTCs(buffer); // Process the DTCs into text

	return TRUE;
}

// Translates the incoming data stream as Mode 2
BOOL CEspritParser::ParseMode2(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 2.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 2.");
		return FALSE;
	}
	else if (len>65)
	{
		WriteStatus("Warning: F002 larger than expected, packet truncated.");
		len = 65;
	}

	ecuData->copyToF002(buffer, len);

	// Mode number is in index 0
	// Work out real-world data from the packet.

	return TRUE;
}

// Translates the incoming data stream as Mode 3
BOOL CEspritParser::ParseMode3(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 3.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 3.");
		return FALSE;
	}
	else if (len>20)
	{
		WriteStatus("Warning: F003 larger than expected, packet truncated.");
		len = 20;
	}

	ecuData->copyToF003(buffer, len);

	return TRUE;
}

// Translates the incoming data stream as Mode 4
BOOL CEspritParser::ParseMode4(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 4.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 4.");
		return FALSE;
	}
	else if (len>11)
	{
		WriteStatus("Warning: F004 larger than expected, packet truncated.");
		len = 11;
	}

	ecuData->copyToF004(buffer, len);

	// Mode number is in index 0
	// Work out real-world data from the packet.

	return TRUE;
}

// Translates the DTC Codes
void CEspritParser::ParseDTCs(unsigned char* buffer)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	ecuData->m_csDTC.Empty();

	if ((m_ucDTC[0] == 0) && (m_ucDTC[1] == 0) && (m_ucDTC[2] == 0))
		ecuData->m_csDTC = "No reported faults.";
	else
	{
		ecuData->m_csDTC = "The following faults are reported:\n";
		
		// Now print the fault-codes
		if (m_ucDTC[0] & 0x80) // 
		{ // 41 - Engine Speed Signal Missing
			ecuData->m_csDTC += "41 - Engine Speed Signal Missing";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x40)
		{ // 13 - Oxygen Sensor; Open Circuit
			ecuData->m_csDTC += "13 - Oxygen Sensor; Open Circuit";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x20)
		{ // 14 - Coolant Temperature Sensor Circuit; High Temperature Indicated
			ecuData->m_csDTC += "14 - Coolant Temperature Sensor Circuit; High Temperature Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x10)
		{ // 15 - Coolant Temperature Sensor Circuit; Low Temperature Indicated
			ecuData->m_csDTC += "15 - Coolant Temperature Sensor Circuit; Low Temperature Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x08)
		{ // 21 - Throttle Position Sensor (TPS) Circuit; Signal Voltage High
			ecuData->m_csDTC += "21 - Throttle Position Sensor (TPS) Circuit; Signal Voltage High";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x04)
		{ // 22 - Throttle Position Sensor (TPS) Circuit; Signal Voltage Low
			ecuData->m_csDTC += "22 - Throttle Position Sensor (TPS) Circuit; Signal Voltage Low";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "22 - TIP: Is the sensor plugged in?";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x02)
		{ // 23 - Mass Air Temperature (MAT) Sensor Circuit; Low Temperature Indicated
			ecuData->m_csDTC += "23 - Mass Air Temperature (MAT) Sensor Circuit; Low Temperature Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x01)
		{ // 24 - Vehicle Speed Sensor (VSS) Circuit
			ecuData->m_csDTC += "24 - Vehicle Speed Sensor (VSS) Circuit";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "24 - TIP: A weak idle mixture or bad misfire can cause this.";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[1] & 0x80)
		{ // 25 - Mass Air Temperature (MAT) Sensor Circuit; High Temperature Indicated
			ecuData->m_csDTC += "25 - Mass Air Temperature (MAT) Sensor Circuit; High Temperature Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x40)
		{ // 66 - Air conditioner Pressure Transducer Open or Short Circuited
			ecuData->m_csDTC += "66 - Air conditioner Pressure Transducer Open or Short Circuited";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x20)
		{ // 32 - EGR Diagnostic
			ecuData->m_csDTC += "32 - EGR Diagnostic";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x10)
		{ // 33 - Manifold Absolute Pressure (MAP) Sensor Circuit; Signal Voltage High
			ecuData->m_csDTC += "33 - Manifold Absolute Pressure (MAP) Sensor Circuit; Signal Voltage High";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "33 - TIP: Is the pipe between the manifold and MAP sensor disconnected?";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x08)
		{ // 34 - Manifold Absolute Pressure (MAP) Sensor Circuit; Signal Voltage Low
			ecuData->m_csDTC += "34 - Manifold Absolute Pressure (MAP) Sensor Circuit; Signal Voltage Low";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x04)
		{ // 35 - Idle Speed Error
			ecuData->m_csDTC += "35 - Idle Speed Error";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "35 - TIP: Maybe IAC Valve or bad misfire.";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x02)
		{ // 26 - Quad-Driver (QDM) Circuit
			ecuData->m_csDTC += "26 - Quad-Driver (QDM) Circuits";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "26 - TIP - Some relay (probably) or secondary injector is open-circuit";
			ecuData->m_csDTC += "\n";
			if (buffer[54] & 0x80)
			{
				ecuData->m_csDTC += "26 -B A/C Clutch, EGR, Chk Light, Fan, Wastegate or Canister Relay";
				ecuData->m_csDTC += "\n";
			}
			if (buffer[54] & 0x01)
			{
				ecuData->m_csDTC += "26 -A  Coolant or RPM Relay, Secondary Injectors";
				ecuData->m_csDTC += "\n";
			}
		}
		if (m_ucDTC[1] & 0x01)
		{ // 42 - Electronic Spark Timing (EST) Circuit
			ecuData->m_csDTC += "42 - Electronic Spark Timing (EST) Circuit";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[2] & 0x80)
		{ // 43 - Electronic Spark Control (ESC) Circuit
			ecuData->m_csDTC += "43 - Electronic Spark Control (ESC) Circuit - Knock Sensor";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x40)
		{ // 44 - Oxygen Sensor Circuit; Lean Exhaust Indicated
			ecuData->m_csDTC += "44 - Oxygen Sensor Circuit; Lean Exhaust Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x20)
		{ // 45 - Oxygen Sensor Circuit; Rich Exhaust Indicated
			ecuData->m_csDTC += "45 - Oxygen Sensor Circuit; Rich Exhaust Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x10)
		{ // 51 - Mem-Cal Error
			ecuData->m_csDTC += "51 - Mem-Cal Error";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x08)
		{ // 31 - Baro Sensor Circuit; Signal Voltage Low or High
			ecuData->m_csDTC += "31 - Baro Sensor Circuit; Signal Voltage Low or High";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x04)
		{ // 53 - Battery Voltage Too High
			ecuData->m_csDTC += "53 - Battery Voltage Too High";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "53 - TIP: Check alternator and, if faulty, also check battery for damage.";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x02)
		{ // 80 - Oil Temperature Sensor Too High
			ecuData->m_csDTC += "80 - Oil Temperature Sensor Too High";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x01)
		{ // 65 - Fuel Injector Circuit, Low Current
			ecuData->m_csDTC += "65 - Fuel Injector Circuit, Low Current";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "65 - TIP: Check battery volts, is the alternator OK?";
			ecuData->m_csDTC += "\n";
		}
	}
}
