#pragma once


// CBaseFunctionExDlg 对话框
#include "BaseDlg.h"
class CBaseFunctionExDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseFunctionExDlg)

public:
	CBaseFunctionExDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaseFunctionExDlg();

// 对话框数据
	enum { IDD = IDD_BASEEX_DLG };
	typedef enum 
	{
		emRegister = 1,
		emModify
	}emOption;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnShowRecvData(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnScan();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();


public:
	void Show();
	void Init(map<emBusinessType,CBaseDlg*>&& mapBasePtr);
	CString GetScriptID();

private:
	void UpdateOption(emOption emTyle);
	void GetScriptIDByRPC();
	void ShowRegScriptData();
	void ShowRegScriptData(const CString& strFileName);

private:
	emBusinessType					m_emScriptType;
	emOption						m_emType;
	CString							m_strScriptID;
	CString							m_strModifyAddr;
	CString							m_strScriptTx_Path;
	CString							m_strUserData;
	CString							m_strRegisterScriptHash;

	int 							m_nValidTime;
	int 							m_nMaxPerTime;
	int								m_nMaxTotal;
	int								m_nMaxPerDay;

	double							m_nModifyFee;
	map<emBusinessType,CBaseDlg*>	m_mapBasePtr;
	AUTHORIZATE						m_author;

public:
	afx_msg void OnBnClickedRadioRegister();
	afx_msg void OnBnClickedRadioModify();
	CString m_strRegisterAddr;
	double m_nRegisterFee;
	int m_nRegisterHeight;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
