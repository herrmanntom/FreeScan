#pragma once

#include "BaseDefines.h"

#include "BaseProtocol.h"

class CBaseParser : public CObject
{
private:
	CBaseProtocol* const m_pBaseProtocol;

protected:
	CBaseParser(CBaseProtocol* const m_pBaseProtocol);
	virtual ~CBaseParser();

	void WriteStatus(const CString csText);
	void WriteASCII(const unsigned char* const buffer, const int ilength);

	void UpdateDialog(void);

	CEcuData* const GetModifiableEcuData(void);

public:
	virtual void InitializeSupportedValues(CEcuData* const ecuData) = 0;
	virtual int Parse(unsigned char*, int iLength) = 0;
};