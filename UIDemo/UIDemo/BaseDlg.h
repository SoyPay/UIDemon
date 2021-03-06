#pragma once


// CBaseDlg 对话框
#include "JsonConfigHelp.h"

class CBaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseDlg)

public:
	CBaseDlg(CWnd* pParent = NULL);   // 标准构造函数
	CBaseDlg(UINT nIDD,CWnd *pParent);
	virtual ~CBaseDlg();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void ConfirmGetScriptID();
	void GetInfoToRegScript(CString& strAddr,int& nHeight,int& nFee,AUTHORIZATE& author);
	void GetScriptID();

public:
	AUTHORIZATE	m_author;
	string		m_strRegisterScriptHash;
	string		m_strScriptSignAddr;
	CString		m_strScript;
	int			m_nHeight;
	double		m_nTxFee;
};
