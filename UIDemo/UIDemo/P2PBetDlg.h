#pragma once


// CP2PBetDlg 对话框
#include "CommonStruct.h"
#include "BaseDlg.h"
class CP2PBetDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CP2PBetDlg)

public:
	CP2PBetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CP2PBetDlg();

// 对话框数据
	enum { IDD = IDD_P2PBET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	LRESULT afx_msg OnShowRecvData(WPARAM wparam,LPARAM lparam);
	afx_msg void OnBnClickedBtnSend();
	DECLARE_MESSAGE_MAP()

public:
	int GetCurStep();
	void CreateContractTx();
	void ShowStep(enumP2PBet emStep);
	void UpdateRegScriptSignAddr();

private:
	void Init();
	bool CheckParam();
	void CreateRandomData();
	void EnableP2PSend(bool bEnable);
	void EnableP2PAccept(bool bEnable);	
	void EnableP2PAExpose(bool bEnable);
	void EnableP2PBExpose(bool bEnable);
	void FillBetRandomData(const string& strData);
	string GetRandomHash(const CString& strRandomHash);

public:
	double			m_nSendMoney;
	double			m_nAcceptMoney;
	enumP2PBet		m_emStep;
	unsigned char	m_szRandomA[5];
	unsigned char	m_szRandomB[5];
	string			m_strRandomAHash;
	CString			m_strSendHash;
	CString			m_strSendSignAddr;
	CString			m_strAcceptSignAddr;
	string			m_strBinFilePath;
	SEND_DATA		m_sendContract;
	ACCEPT_DATA		m_acceptContract;
	OPEN_DATA		m_openAContract;
	OPEN_DATA		m_openBContract;
	CP2PBetHelp		m_P2PBetHelp;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString m_strOpenAddr;
};
