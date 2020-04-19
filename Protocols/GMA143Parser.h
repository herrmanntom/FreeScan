// Parser.h: interface for the CGMA143Parser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMA143PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_GMA143PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseParser.h"
#include "BaseProtocol.h"

#define ECU_HEADER_GMA143					0xf0

class CGMA143Parser : public CBaseParser
{
public:
	CGMA143Parser(CBaseProtocol* const pProtocol);
	virtual ~CGMA143Parser();

	void InitializeSupportedValues(CEcuData* const ecuData);
	int Parse(unsigned char*, int iLength);

private:
	unsigned char	m_ucDTC[6]; // Fault codes buffer

private:
	void ParseADC(unsigned char* buffer, int len);
	void ParseAnalogues(unsigned char* buffer, int len);
	void ParseMode1(unsigned char* buffer, int len);
	void ParseMode2(unsigned char* buffer, int len);
	void ParseMode3(unsigned char* buffer, int len);
	void ParseMode4(unsigned char* buffer, int len);
	void ParseDTCs(CEcuData *const ecuData);// Parse the DTCs
};

#endif // !defined(AFX_GMA143PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
