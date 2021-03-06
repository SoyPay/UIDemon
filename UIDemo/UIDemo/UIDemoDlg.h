
// TestDlg.h : 头文件
//

#pragma once

#include "WalletDlg.h"
#include "BusinessDlg.h"
#include "RPCDlg.h"
#include "BaseFuncionDlg.h"
#include "BaseFunctionExDlg.h"
#include "DemoSocket.h"
#include "CommonStruct.h"
#include "JsonConfigHelp.h"
#include "RpcCmd.h"
#define BUFFER_SIZE	40*1024

// CTestDlg 对话框
class CTestDlg : public CDialogEx
{
// 构造
public:
	CTestDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CTestDlg();

// 对话框数据
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HMENU m_hMenu;


	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnMenuWallet();
	afx_msg void OnMenuTrade();
	afx_msg void OnMenuBase();
	afx_msg void OnMenuBaseEx();
	afx_msg void OnMenuAboutSmartCoin();
	afx_msg void OnMenuImportWallet();
	afx_msg void OnChinse();
	afx_msg void OnEnglish();
	afx_msg void OnMenuDumpWallet();
	afx_msg void OnClose();
	afx_msg void OnRpcCommand();
	afx_msg LRESULT OnSendRPCData(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSocketConnnect(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSocketClose(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSocketReceive(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnUpdateHeight(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnGetAccountInfo(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnListTx(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnGetAccList(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnGetBetRandomData(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnUpdate(WPARAM wparam,LPARAM lparam);
	
	
public:

	
	void OnRecive();
	void OnConnnect();
	void Close();
	void GetAccountInfo(map<CString,CString>& mapAccountInfo);
	bool GetVersionFromServer(string& strVersion);
	CString GetScriptID();
	CString GetAddrByRegID(const CString& strFullRegID);

private:
	void Send();
	void CheckPathValid(const CStringA& strDir);
	void LoadLanguage(const CStringA& strDir);
	void CheckUpdate();
	void StartSeverProcess(const CStringA& strdir);
	void CloseProcess(const string& exename);
	void ConnetSever();
	void ParseUIConfigFile(const string& strExeDir);
	bool GetValueFormCfgFile(const string& strData,const string& strKey,string& strValue);
	CStringA GetResultStr(const CStringA& message);
	CStringA GetExePath();
	CString GetExePath2();

private:
	CRPCDlg*				m_pRPCDlg;
	CWalletDlg*				m_pWalletDlg;
	CBusinessDlg*			m_pBusinessDlg;
	CBaseFuncionDlg*		m_pBaseFunctionDlg;
	CBaseFunctionExDlg*		m_pBaseFunctionExDlg;
	CDemoSocket				m_socket;
	PROCESS_INFORMATION		sever_pi; 
	CStringA				m_sendPreHeadstr;
	CStringA				m_sendendHeadstr;
	CStringA				m_rpcport;
	string					m_recivestr;
	string					m_sendCommand;
	string					m_strServerCfgFileName;
	BOOL					m_bConnected;
	BOOL					m_bStartServer;
	int 					m_IsChinese;
	char*					m_szRecvBuffer;
	emSendDataType			m_emSendType;
	map<CString,CString>	m_mapAccount;
	int						m_nHeight;
};
