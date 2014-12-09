#pragma once
#include "afxwin.h"


// CSesureDlg 对话框
#include "CommonStruct.h"
class CSesureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSesureDlg)

public:
	CSesureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSesureDlg();
	void ShowStep(unsigned char nStep);
// 对话框数据
	enum { IDD = IDD_SESURE1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnShowRecvData(WPARAM wparam,LPARAM lparam);

private:
	void GetArRegID(vector<string>& vRegID);
	void HideAllControl();
	void FillRegIDToCombox();
	void UpdateSignAddr(const CString &strFullRegID);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnSend();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	int GetCurStep();
	void CreateScriptTx(bool bRigsterScript);
	void CreateContractTx();
	

private:
	map<CString,CString>m_mapAccount;
	vector<string>		m_vArRegID;
	enumSesureTrade		m_emStep;
	FIRST_CONTRACT		m_firstContract;
	NEXT_CONTRACT		m_secondContract;
	NEXT_CONTRACT		m_thirdContract;
	ARBIT_RES_CONTRACT  m_arContract;
	CString				m_strSignAddr;
	CString				m_strBuyer;
	CString				m_strScript;
	CString				m_strSeller;
	CString				m_strHash;
	string				m_strUserDefineData;
	int					m_nPay;
	int					m_nDeposit;
	int					m_nArFee;
	int					m_nMaxFine;
	int					m_nFine;
	int					m_nHeight;
	int					m_nTxFee;

public:
	CComboBox m_comboxAr;
	CListBox m_listAr;
	afx_msg void OnCbnSelchangeComboAr();

};
