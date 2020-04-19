// Parser.h: interface for the CGMA160Parser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMA160PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_GMA160PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseParser.h"
#include "BaseProtocol.h"

#define ECU_HEADER_GMA160				0xf0
#define ECU_CHATTER_HEADER_GMA160		0xf0

class CGMA160Parser : public CBaseParser
{
public:
	CGMA160Parser(CBaseProtocol* const pProtocol);
	virtual ~CGMA160Parser();

	void InitializeSupportedValues(CEcuData* const ecuData);
	int Parse(unsigned char*, int iLength);

private:
	unsigned char	m_ucDTC[3]; // Fault codes buffer

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
	void ParseDTCs(CEcuData *const ecuData);// Parse the DTCs
};

#endif // !defined(AFX_GMA160PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
