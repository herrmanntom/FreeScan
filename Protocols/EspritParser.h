// Parser.h: interface for the CEspritParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ESPRITPARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_ESPRITPARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseParser.h"
#include "BaseProtocol.h"

#define ECU_HEADER_ESPRIT					0xf0

class CEspritParser: public CBaseParser
{
public:
	CEspritParser(CBaseProtocol* const pProtocol);
	virtual ~CEspritParser();

	void InitializeSupportedValues(CEcuData* const ecuData);
	int Parse(unsigned char*, int iLength);

private:
	unsigned char	m_ucDTC[3]; // Fault codes buffer

private:
	BOOL ParseADC(unsigned char* buffer, int len);
	BOOL ParseAnalogues(unsigned char* buffer, int len);
	BOOL ParseMode1(unsigned char* buffer, int len);
	BOOL ParseMode2(unsigned char* buffer, int len);
	BOOL ParseMode3(unsigned char* buffer, int len);
	BOOL ParseMode4(unsigned char* buffer, int len);
	void ParseDTCs(CEcuData *const ecuData, unsigned char* buffer);// Parse the DTCs
};

#endif // !defined(AFX_ESPRITPARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
