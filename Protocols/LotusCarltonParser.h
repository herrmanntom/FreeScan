// Parser.h: interface for the CLotusCarltonParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LotusCarltonPARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_LotusCarltonPARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseParser.h"
#include "BaseProtocol.h"

#define ECU_HEADER_LotusCarlton				0xf4

class CLotusCarltonParser : public CBaseParser
{
public:
	CLotusCarltonParser(CBaseProtocol* const pProtocol);
	virtual ~CLotusCarltonParser();

	void InitializeSupportedValues(CEcuData* const ecuData);
	int Parse(unsigned char*, int iLength);

private:
	unsigned char	m_ucDTC[5]; // Fault codes buffer

private:
	//Implementation
	void ParseADC(unsigned char* buffer, int len);
	void ParseAnalogues(unsigned char* buffer, int len);
	void ParseMode1_0(unsigned char* buffer, int len);
	void ParseMode2(unsigned char* buffer, int len);
	void ParseMode3(unsigned char* buffer, int len);
	void ParseMode4(unsigned char* buffer, int len);
	void ParseDTCs(void);// Parse the DTCs
};

#endif // !defined(AFX_LotusCarltonPARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
