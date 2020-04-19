// Parser.h: interface for the CGM1992PontiacParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GM1992PontiacPARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_GM1992PontiacPARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseParser.h"
#include "BaseProtocol.h"

#define ECU_HEADER_GM1992Pontiac				0xf4
#define ECU_CHATTER_HEADER_GM1992Pontiac		0xf0

class CGM1992PontiacParser : public CBaseParser
{
public:
	CGM1992PontiacParser(CBaseProtocol* const pProtocol);
	virtual ~CGM1992PontiacParser();

	void InitializeSupportedValues(CEcuData* const ecuData);
	int Parse(unsigned char*, int iLength);

private:
	unsigned char	m_ucDTC[4]; // Fault codes buffer

private:
	//Implementation
	BOOL ParseModeF0(unsigned char* buffer, int len);
	BOOL ParseMode05(unsigned char* buffer, int len);
	BOOL ParseMode0A(unsigned char* buffer, int len);
	BOOL ParseMode1_0(unsigned char* buffer, int len);
	BOOL ParseMode2(unsigned char* buffer, int len);
	BOOL ParseMode3(unsigned char* buffer, int len);
	BOOL ParseMode4(unsigned char* buffer, int len);
	BOOL ParseMode7(unsigned char* buffer, int len);
	BOOL ParseMode8(unsigned char* buffer, int len);
	BOOL ParseMode9(unsigned char* buffer, int len);
	BOOL ParseMode10(unsigned char* buffer, int len);
	void ParseDTCs(CEcuData *const ecuData);// Parse the DTCs
};

#endif // !defined(AFX_GM1992PontiacPARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
