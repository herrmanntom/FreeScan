// Parser.h: interface for the CGM1989CorvetteParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GM1989CorvettePARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_GM1989CorvettePARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseParser.h"
#include "BaseProtocol.h"

#define ECU_HEADER_GM1989Corvette				0x80
#define ECU_CHATTER_HEADER_GM1989Corvette		0x80

class CGM1989CorvetteParser : public CBaseParser
{
public:
	CGM1989CorvetteParser(CBaseProtocol* const pProtocol);
	virtual ~CGM1989CorvetteParser();

	void InitializeSupportedValues(CEcuData* const ecuData);
	int Parse(unsigned char*, int iLength);

private:
	unsigned char	m_ucDTC[4]; // Fault codes buffer

private:
	//Implementation
	void ParseMode0A(unsigned char* buffer, int len);
	void ParseMode90(unsigned char* buffer, int len);
	void ParseMode1_0(unsigned char* buffer, int len);
	void ParseMode2(unsigned char* buffer, int len);
	void ParseMode3(unsigned char* buffer, int len);
	void ParseMode4(unsigned char* buffer, int len);
	void ParseMode7(unsigned char* buffer, int len);
	void ParseMode8(unsigned char* buffer, int len);
	void ParseMode9(unsigned char* buffer, int len);
	void ParseMode10(unsigned char* buffer, int len);
	void ParseDTCs(void);// Parse the DTCs
};

#endif // !defined(AFX_GM1989CorvettePARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
