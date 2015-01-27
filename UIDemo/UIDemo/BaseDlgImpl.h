#pragma once
#include "JsonConfigHelp.h"
class CBaseDlgImpl
{
public:
	CBaseDlgImpl(void);
	~CBaseDlgImpl(void);

private:
	void GetScriptID();
	bool HasGetScriptID();

public:
	void ConfirmGetScriptID();

protected:
	CString				m_strScript;
	string				m_strRegisterScriptHash;
};

