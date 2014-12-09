#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "DemoSocket.h"
// CWalletDlg 对话框
#define SOCKET_BUFFER 1024
class CWalletDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWalletDlg)

public:
	CWalletDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWalletDlg();
	void Init();

// 对话框数据
	enum { IDD = IDD_WALLET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnSocketConnnect(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSocketClose(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSocketReceive(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnShowRecvData(WPARAM wparam,LPARAM lparam);

private:
	void ConnetSever();
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_txList;
	virtual BOOL OnInitDialog();

private:
	CDemoSocket m_MsgSocket;
	BOOL		m_bConnected;
	char*		m_szBuffer;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
