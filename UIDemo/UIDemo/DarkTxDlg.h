#pragma once


// CDarkTxDlg 对话框
#include "CommonStruct.h"
#include "JsonConfigHelp.h"
#include "BaseDlg.h"
class CDarkTxDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CDarkTxDlg)

public:
	CDarkTxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDarkTxDlg();

// 对话框数据
	enum { IDD = IDD_DARK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSend();
	LRESULT afx_msg OnShowRecvData(WPARAM wparam,LPARAM lparam);
	virtual BOOL OnInitDialog();

public:
	void CreateContractTx();
	void SignContract();
	void ShowStep(enumDarkTx emStep);
	void UpdateRegScriptSignAddr();
	int  GetCurStep();

private:
	void Init();
	bool CheckParam();
	void GetSignAddr();
	void HideAllControl();

private:
	CDarkTxHelp				m_DarkTxHelp;
	DARK_FIRST_CONTRACT		m_firstContract;
	DARK_NEXT_CONTRACT		m_nextContract;
	enumDarkTx				m_emStep;
	CString					m_strBuyerID;
	CString					m_strSellerID;
	CString					m_strRes;
	CString					m_strHash;
	CString					m_strBuyerSignAddr;
	CString					m_strSellerSignAddr;
	double					m_nMoney;
public:
	afx_msg void OnBnClickedBtnSign();
	afx_msg void OnClose();
};
