#pragma once
#include "afxwin.h"


// CSesureDlg 对话框
#include "CommonStruct.h"
#include "BaseDlg.h"
class CSesureDlg : public CBaseDlg
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
	void Init();
	bool ChechParams();
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
	void UpdateRegScriptSignAddr();
	void CreateContractTx();

private:
	CSesureTradeHelp	m_SesureHelp;
	map<CString,CString>m_mapAccount;
	vector<string>		m_vArRegID;
	enumSesureTrade		m_emStep;
	FIRST_CONTRACT		m_firstContract;
	NEXT_CONTRACT		m_secondContract;
	NEXT_CONTRACT		m_thirdContract;
	ARBIT_RES_CONTRACT  m_arContract;
	CString				m_strSignAddr;
	CString				m_strBuyer;
	CString				m_strSeller;
	CString				m_strHash;
	string				m_strUserDefineData;
	string				m_strBinPath;
	double				m_nPay;
	double				m_nDeposit;
	double				m_nArFee;
	double				m_nMaxFine;
	double				m_nFine;

public:
	CComboBox m_comboxAr;
	CListBox m_listAr;
	afx_msg void OnCbnSelchangeComboAr();

	CString m_strAr;
};
