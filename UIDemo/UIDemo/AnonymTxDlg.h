#pragma once


// CAnonymTxDlg 对话框
#include "CommonStruct.h"
#include "BaseDlg.h"
class CAnonymTxDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CAnonymTxDlg)

public:
	CAnonymTxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnonymTxDlg();

// 对话框数据
	enum { IDD = IDD_ANONYM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	LRESULT afx_msg OnShowRecvData(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSend();
	void CreateContractTx();
	void UpdateRegScriptSignAddr();
	void Show();
	
private:
	void Init();
	void GetSignAddr();


private:
	CAnonymTxHelp	m_AnonymTxHelp;
	ANONYM_CONTRACT	m_anonymContract;
	CString			m_strSend;
	CString			m_strRecv1;
	CString			m_strRecv2;
	CString			m_strRes;
	string			m_strSignAddrSend;
	string			m_strBinFilePath;
	int				m_nMoney;
};
