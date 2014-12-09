
// TestDlg.h : 头文件
//

#pragma once

#include "WalletDlg.h"
#include "BusinessDlg.h"
#include "RPCDlg.h"
#include "BaseFuncionDlg.h"
#include "DemoSocket.h"
#include "CommonStruct.h"
#define BUFFER_SIZE	4*1024
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
	afx_msg void OnBnClickedCancel();
	afx_msg void OnMenuWallet();
	afx_msg void OnMenuTrade();
	afx_msg void OnMenuBase();
	afx_msg void OnClose();
	afx_msg void OnRpcCommand();
	afx_msg LRESULT OnSendRPCData(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSocketConnnect(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSocketClose(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSocketReceive(WPARAM wparam,LPARAM lparam);

public:
	void OnRecive();
	void OnConnnect();
	void Close();
	void GetAccountInfo(map<CString,CString>& mapAccountInfo);

private:
	void Send();
	void StartSeverProcess();
	void CloseProcess(const string& exename);
	void ConnetSever();
	CStringA GetResultStr(const CStringA& message);

private:
	CRPCDlg*				m_pRPCDlg;
	CWalletDlg*				m_pWalletDlg;
	CBusinessDlg*			m_pBusinessDlg;
	CBaseFuncionDlg*		m_pBaseFunctionDlg;
	CDemoSocket				m_socket;
	PROCESS_INFORMATION		sever_pi; 
	CStringA				m_sendPreHeadstr;
	CStringA				m_sendendHeadstr;
	CStringA				m_rpcport;
	string					m_recivestr;
	string					m_sendCommand;
	BOOL					m_bConnected;
	char*					m_szRecvBuffer;
	emSendDataType			m_emSendType;
	map<CString,CString>	m_mapAccount;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTest();
};
