#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "DemoSocket.h"
#include "CommonStruct.h"
#include "RpcCmd.h"

// CWalletDlg 对话框
#define SOCKET_BUFFER 1024
class CWalletDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWalletDlg)

public:
	CWalletDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWalletDlg();
	void Init();
	void InitAccountInfoTree(const string& strAccountInfo);
	void InitTxInfoTree(const string& strAccountInfo);

// 对话框数据
	enum { IDD = IDD_WALLET_DLG };
	enum TxType {
		REWARD_TX = 1,    //!< reward tx
		REG_ACCT_TX = 2,  //!< tx that used to register account
		COMMON_TX = 3,    //!< transfer money from one account to another
		CONTRACT_TX = 4,  //!< contract tx
		FREEZE_TX = 5,    //!< freeze tx
		REG_SCRIPT_TX = 6,//!< register script or modify authorization
		NULL_TX,          //!< NULL_TX
	};
public:
	void Clearn(void)
	{
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_KEY);
	pCombox->Clear();
	}
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnSocketConnnect(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSocketClose(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSocketReceive(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnShowRecvData(WPARAM wparam,LPARAM lparam);

private:
	void ConnetSever();
	void FillFundToTree(const string& strFundType,HTREEITEM hRoot,const Json::Value& v);
	void FillAuthorToTree(HTREEITEM hItemRoot,const Json::Value& root);
	void LoadTxInfo(const Json::Value& txArray,HTREEITEM hItemRoot,BOOL bComfirmed = TRUE);
	CString FormatBlockTime(ULONGLONG nTime);
	DECLARE_MESSAGE_MAP()

public:	
	void Jiexi(Json::Value &obj, HTREEITEM hItemRoot);

	virtual BOOL OnInitDialog();

private:
	CDemoSocket m_MsgSocket;
	BOOL		m_bConnected;
	char*		m_szBuffer;
public:

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CTreeCtrl m_TreeAcc;
	CTreeCtrl m_TreeTx;
	map<string,int> m_mapTxType;
	afx_msg void OnBnClickedBtnFreshAccinfo();
	afx_msg void OnBnClickedBtnFreshTxInfo();
	afx_msg void OnCbnSelchangeComboKey();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeAcc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedStaticHeight();
};
