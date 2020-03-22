// ElanParser.cpp: implementation of the CElanParser class.
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//
// This contains all low-level parsing functions.

#include "stdafx.h"
#include "..\FreeScan.h"
#include "..\MainDlg.h"
#include "ElanParser.h"

#include "..\Supervisor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElanParser::CElanParser()
{
	m_pSupervisor = NULL;

	CFreeScanApp* pApp = (CFreeScanApp*) AfxGetApp();
	m_csCSVLogFile = pApp->GetProfileString("ElanParser", "CSV Log Filename", "");

	m_dwCSVRecord = 0; // Initialise the CSV record number
	memset(m_ucDTC, 0, 3);// Reset DTC buffer
}

CElanParser::~CElanParser()
{
	if (m_file.m_hFile != CFile::hFileNull)
	{ // close our log file if it's open
		m_file.Flush();
		m_file.Close(); // close the logging file when we exit.
	}

	CFreeScanApp* pApp = (CFreeScanApp*) AfxGetApp();
	pApp->WriteProfileString("ElanParser", "CSV Log Filename", m_csCSVLogFile);
}

// Starts or stops csv logging to file
BOOL CElanParser::StartCSVLog(BOOL bStart)
{
	CString csBuf = "";

	if (!bStart)
	{ // we want to close the logging file
		if (m_file.m_hFile != CFile::hFileNull)
		{
			WriteStatusLogged("CSV Log file has been stopped");
			m_file.Close(); // close the logging file when we exit.
		}
		else
			WriteStatusLogged("CSV Log file is already closed");

		return FALSE;
	}

	// We now must want to log to a file

	// Construct our File Dialog
	CFileDialog		Dialog(FALSE, "csv", 
						m_csCSVLogFile, 
						OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
						"log Files (*.csv)|*.csv|All Files (*.*)|*.*||", NULL);

	// Change the title
	Dialog.m_ofn.lpstrTitle = "Create/Open CSV Log File";

	// Display the dialog box
	if (Dialog.DoModal() == IDOK)
	{
		m_csCSVLogFile = Dialog.GetPathName();

		if (!m_file.Open(m_csCSVLogFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText))
		{
			csBuf.Format("Cannot open %s", m_csCSVLogFile);
			WriteStatus(csBuf);
			AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP );
			return FALSE;
		}

		WriteStatusLogged("CSV Log file has been opened");
		WriteCSV(TRUE);
	}
	else // User pressed cancel
		WriteStatus("User cancelled CSV log file");

	if (m_file.m_hFile != NULL)
		return TRUE;
	else
		return FALSE;
}

//WriteCSV(..) Writes CSV data to our log file.
//If bTitle is true, the first title line is written, otherwise the data is written.
void CElanParser::WriteCSV(BOOL bTitle) 
{
	if (m_file.m_pStream == NULL) return;// i.e. no file open

	CString	csBuf;
	if (bTitle)
	{
		m_dwCSVRecord = 0;
		csBuf.Format("Elan Sample,Coolant Sensor Volts,Start Water Temp,TPS Volts,Desired Idle,RPM,Road Speed,Crank Sensors,Oxygen Sensor,Idle Actuator Position,Barometric Air Pressure,MAP,Air:Fuel Ratio,TPS,MAT Sensor Volts,Knock Retard,Battery Volts,Engine Load,Spark Timing,Coolant Temp,MAT,Engine Running Time");
	}
	else
	{
		const CEcuData *const ecuData = m_pSupervisor->GetEcuData();
		csBuf.Format("%ld,%4.2f,%3.1f,%4.2f,%d,%d,%d,%d,%5.3f,%d,%4.2f,%4.2f,%3.1f,%d,%4.2f,%3.1f,%3.1f,%d,%3.1f,%3.1f,%3.1f,%d",
			m_dwCSVRecord, ecuData->m_fWaterVolts, ecuData->m_fStartWaterTemp, ecuData->m_fThrottleVolts,
			ecuData->m_iDesiredIdle, ecuData->m_iRPM, ecuData->m_iMPH, ecuData->m_iCrankSensors, ecuData->m_fO2VoltsLeft, ecuData->m_iIACPosition,
			ecuData->m_fBaro, ecuData->m_fMAP, ecuData->m_fAFRatio, ecuData->m_iThrottlePos,
			ecuData->m_fMATVolts, ecuData->m_fKnockRetard, ecuData->m_fBatteryVolts, ecuData->m_iEngineLoad, ecuData->m_fSparkAdvance,
			ecuData->m_fWaterTemp, ecuData->m_fMATTemp, ecuData->m_iRunTime);
		m_dwCSVRecord++;
	}
	csBuf = csBuf + "\n"; // Line Feed because we're logging to disk
	m_file.WriteString(csBuf);
}

// Tells the Supervisor that there's been a data change
void CElanParser::UpdateDialog(void)
{
	m_pSupervisor->UpdateDialog();
	WriteCSV(FALSE); // log our data to a csv file
}

// Parses the buffer into real data
int CElanParser::Parse(unsigned char* buffer, int iLength)
{
	WriteASCII(buffer, iLength); // Log the activity

	int	iIndex=0, iPacketLength;
	unsigned char* pPacketStart=NULL;

	// Scan the whole packet for its header.
	for(iIndex=0;iIndex<iLength;iIndex++)
	{
		pPacketStart = buffer + iIndex; // Marks the start of Packet for the CRC.
		switch(buffer[iIndex])
		{ // Find valid headers. Length parameter is always the 2nd byte.
		case ECU_HEADER_ELAN: // Packet Header Start
			{
				iIndex++; // now find the length
				unsigned char ucLenByte = 0;
				ucLenByte = buffer[iIndex];
				iPacketLength = GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				if (iPacketLength != 0)
				{// Data in Header
					if(buffer[iIndex]==1) // Main data-stream
					{
						if(ucLenByte==0x95) // length 64 including mode byte
							ParseMode1_0(&buffer[iIndex], iPacketLength);
					}
					else if(buffer[iIndex]==2) // length 65 including mode byte
						ParseMode2(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==3) // length max 7 including mode byte
						ParseMode3(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==4) // length max 11 including mode byte
						ParseMode4(&buffer[iIndex], iPacketLength);
					else
					{
						CString	temp; // write to the spy window
						temp.Format("Unrecognised Mode: %02x", buffer[iIndex]);
						WriteStatus(temp);
					}
					iIndex += iPacketLength;
				}
				// Check CRC
				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error");
				break;
			}
		case 0x0a: // Computed Values
			{
				iIndex++; // now find the length
				iPacketLength = GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				ParseAnalogues(&buffer[iIndex], iPacketLength);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error");
				break;
			}
		case 0x05: // ADC Values
			{
				iIndex++; // now find the length
				iPacketLength = GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				ParseADC(&buffer[iIndex], iPacketLength);
				iIndex += iPacketLength; // should be 10 or 58 from ECU
				// Check CRC
				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error");
				break;
			}
		default:
			{
			CString buf;
			buf.Format("%02x <- Unrecognised Header", buffer[iIndex]);
			WriteStatus(buf);
			}
		}// Switch
	}// for()

	// Force the main application to update itself
	UpdateDialog();

	return iLength; // Successfully parsed.
}

// Translates the incomming data stream as ADC Values
void CElanParser::ParseADC(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len==0) // remember half duplex. We read our commands as well
		WriteStatus("Received our TX command echo for mode 1.");
	else if (len>10)
	{
		WriteStatus("Warning: F005 larger than expected, packet truncated.");
		len = 10;
	}
	// we have data to process
	// copy buffer into raw data array
	memcpy(ecuData->m_ucF005, buffer, len);

	// Work out real world data from the packet.

//	if (buffer[0] & 0x80)
//		ecuData->m_bEngineOffIdle = TRUE;  // bit 7
//	else
//		ecuData->m_bEngineOffIdle = FALSE; // bit 7
//
//	if (buffer[0] & 0x40)
//		ecuData->m_bEngineStalled = TRUE;  // bit 6
//	else
//		ecuData->m_bEngineStalled = FALSE; // bit 6

//	ecuData->m_iMPH = (int)buffer[2]; // Count is in MPH
//	ecuData->m_fBatteryVolts = (float)buffer[4] / (float)10.0;
//	ecuData->m_fWaterTemp = ((float)buffer[9] * (float)0.75) - (float)40.0; // in �C
}

// Translates the incoming data stream as Analogue Values
void CElanParser::ParseAnalogues(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len>3)
	{
		WriteStatus("Warning: F00A larger than expected, packet truncated.");
		len = 3;
	}

	memcpy(ecuData->m_ucF005, buffer, len);

	// Work out real world data from the packet.

//	ecuData->m_iRPM = ((int)buffer[1] * 256) + (int)buffer[2];
}

// Translates the incoming data stream as Mode 1
void CElanParser::ParseMode1_0(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len<10) // remember half duplex. We read our commands as well
	{
		WriteStatus("Received our TX command echo for mode 1.");
		return;
	}
	else if (len>65)
	{
		WriteStatus("Warning: F001 larger than expected, packet truncated.");
		len = 65;
	}

	memcpy(ecuData->m_ucF001, buffer, len);

	// Work out real-world data from the packet.
	// Status Word 1
//	if (buffer[7] & 0x01)
//		ecuData->m_bACRequest = TRUE;  // mode 1, byte 10, bit 0
//	else
//		ecuData->m_bACRequest = FALSE; // mode 1, byte 10, bit 0
//
//	if (buffer[7] & 0x04)
//		ecuData->m_bACClutch = TRUE;  // mode 1, byte 10, bit 2
//	else
//		ecuData->m_bACClutch = FALSE; // mode 1, byte 10, bit 2

	// Analogues
	ecuData->m_iEpromID = (int)buffer[2] + ((int)buffer[1] * 256);
	m_ucDTC[0] = buffer[3]; // Fault code byte 1
	m_ucDTC[1] = buffer[4]; // Fault code byte 2
	m_ucDTC[2] = buffer[5]; // Fault code byte 3
	ecuData->m_fWaterTemp = ((float)buffer[6] * (float)0.75) - (float)40.0; // in �C
	ecuData->m_fStartWaterTemp = ((float)buffer[7] * (float)0.75) - (float)40.0; // in �C
	ecuData->m_fThrottleVolts = (float)(((float)buffer[9] / (float)255.0) * (float) 5.0);
	ecuData->m_iThrottlePos = (int)((float)buffer[10] / (float)2.55);
	ecuData->m_iRPM = ((int)buffer[11] * 25);
	ecuData->m_fSparkAdvance = (float)buffer[12]; // in �
	ecuData->m_iMPH = (int)buffer[17]; // Count is in MPH
	ecuData->m_iIACPosition = (int)buffer[21];
	ecuData->m_iDesiredIdle = (int)((float)buffer[22] * (float) 12.5);
	ecuData->m_fBatteryVolts = (float)buffer[26] / (float)10.0;
	ecuData->m_fMAP = (((float)buffer[28] - (float)128.0)/ (float)100) + (float) 1.0; // in Bar Absolute
	ecuData->m_fMAPVolts = ((float)buffer[28] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_fO2VoltsLeft = (float) buffer[32] / (float) 226.0;
	ecuData->m_fAFRatio = (float)buffer[33] / (float)10.0; // Air Fuel Ratio
	ecuData->m_iRunTime = (buffer[49] * 256) + buffer[50]; // Total running time

//	ecuData->m_fWaterVolts = (float)(((float)buffer[8] / (float)255.0) * (float) 5.0);
//	ecuData->m_iCrankSensors = buffer[15];
//	ecuData->m_fBaro = (((float)buffer[24] - (float)128.0)/ (float)100) + (float) 1.0; // in Bar Absolute
//	ecuData->m_fBaroVolts = ((float)buffer[24] / (float) 255.0) * (float) 5.0; // in Volts
//	ecuData->m_fMATVolts = ((float)buffer[29] / (float)255.0) * (float)5.0; // in Volts
//	ecuData->m_fKnockRetard = ((float)buffer[31] * (float)45.0) / (float)256.0; // in �
//	ecuData->m_iEngineLoad = (int)((float)buffer[36] / (float) 2.55);
//	ecuData->m_fMATTemp = ((float)buffer[42] * (float)0.75) - (float)40.0; // in �C
	
	ecuData->m_fAirFlow = 0.0;

	ParseDTCs(); // Process the DTCs into text
}

// Translates the incoming data stream as Mode 2
void CElanParser::ParseMode2(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 2.");
		return;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 2.");
		return;
	}
	else if (len>65)
	{
		WriteStatus("Warning: F002 larger than expected, packet truncated.");
		len = 65;
	}

	memcpy(ecuData->m_ucF002, buffer, len);

	// Mode number is in index 0
	// Work out real-world data from the packet.
}

// Translates the incoming data stream as Mode 3
void CElanParser::ParseMode3(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 3.");
		return;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 3.");
		return;
	}
	else if (len>11)
	{
		WriteStatus("Warning: F003 larger than expected, packet truncated.");
		len = 11;
	}

	memcpy(ecuData->m_ucF003, buffer, len);

	// Mode number is in index 0
	// Work out real-world data from the packet.
	m_ucDTC[0] = buffer[1]; // Fault code byte 1
	m_ucDTC[1] = buffer[2]; // Fault code byte 2
	m_ucDTC[2] = buffer[3]; // Fault code byte 3
	ParseDTCs(); // Process the DTCs into text
}

// Translates the incoming data stream as Mode 4
void CElanParser::ParseMode4(unsigned char* buffer, int len)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 4.");
		return;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 4.");
		return;
	}
	else if (len>11)
	{
		WriteStatus("Warning: F004 larger than expected, packet truncated.");
		len = 11;
	}

	memcpy(ecuData->m_ucF004, buffer, len);

	// Mode number is in index 0
	// Work out real-world data from the packet.
}

// Translates the DTC Codes
void CElanParser::ParseDTCs(void)
{
	CEcuData *const ecuData = m_pSupervisor->GetModifiableEcuData();

	ecuData->m_csDTC.Empty();

	if ((m_ucDTC[0] == 0) && (m_ucDTC[1] == 0) && (m_ucDTC[2] == 0))
		ecuData->m_csDTC = "No reported faults.";
	else
	{
		ecuData->m_csDTC = "The following faults are reported:\n";
		
		// Now print the fault-codes
		if (m_ucDTC[0] & 0x80) // This will never happen
		{ // 12 - Diagnostic Mode Enabled
			ecuData->m_csDTC += "12 - Diagnostic Mode Enabled";
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
		}

		if (m_ucDTC[1] & 0x80)
		{ // 25 - Mass Air Temperature (MAT) Sensor Circuit; High Temperature Indicated
			ecuData->m_csDTC += "25 - Mass Air Temperature (MAT) Sensor Circuit; High Temperature Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x40)
		{ // 26 - Quad-Driver (QDM) Circuit, Secondary Injectors
			ecuData->m_csDTC += "26 - Quad-Driver (QDM) Circuit, Secondary Injectors";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x20)
		{ // 31 - Baro Sensor Circuit; Signal Voltage Low or High
			ecuData->m_csDTC += "31 - Baro Sensor Circuit; Signal Voltage Low or High";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x10)
		{ // 33 - Manifold Absolute Pressure (MAP) Sensor Circuit; Signal Voltage High
			ecuData->m_csDTC += "33 - Manifold Absolute Pressure (MAP) Sensor Circuit; Signal Voltage High";
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
		}
		if (m_ucDTC[1] & 0x02)
		{ // 41 - Engine Speed Signal Missing
			ecuData->m_csDTC += "41 - Engine Speed Signal Missing";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x01)
		{ // 42 - Electronic Spark Timing (EST) Circuit
			ecuData->m_csDTC += "42 - Electronic Spark Timing (EST) Circuit";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[2] & 0x80)
		{ // 43 - Electronic Spark Control (ESC) Circuit
			ecuData->m_csDTC += "43 - Electronic Spark Control (ESC) Circuit";
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
		{ // 53 - Battery Voltage Too High
			ecuData->m_csDTC += "53 - Battery Voltage Too High";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x04)
		{ // 65 - Fuel Injector Circuit; Low Current
			ecuData->m_csDTC += "65 - Fuel Injector Circuit; Low Current";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[2] & 0x02)
		{
			ecuData->m_csDTC += "Byte 3;Bit 1 - Unknown error code";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x01)
		{
			ecuData->m_csDTC += "Byte 3;Bit 0 - Unknown error code";
			ecuData->m_csDTC += "\n";
		}
	}
}


