
#include "BaseParser.h"

CBaseParser::CBaseParser(CBaseProtocol* const pBaseProtocol): m_pBaseProtocol(pBaseProtocol) {
}

CBaseParser::~CBaseParser() {
}

void CBaseParser::WriteStatus(const CString csText) {
	m_pBaseProtocol->WriteStatus(csText);
}

void CBaseParser::WriteASCII(const unsigned char* const buffer, const int ilength) {
	m_pBaseProtocol->WriteASCII(buffer, ilength);
}

void CBaseParser::UpdateDialog(void) {
	m_pBaseProtocol->UpdateDialog();
}

CEcuData* const CBaseParser::GetModifiableEcuData(void) {
	return m_pBaseProtocol->GetModifiableEcuData();
}
