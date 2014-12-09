#pragma once


// CBaseFuncionDlg 对话框

class CBaseFuncionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseFuncionDlg)

public:
	CBaseFuncionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaseFuncionDlg();

// 对话框数据
	enum { IDD = IDD_BASE_DLG };
	typedef enum 
	{
		emTransfer=1,
		emScript,
		emAccount
	}emType;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStartMine();
	afx_msg void OnBnClickedBtnStopMine();
	afx_msg LRESULT OnShowRecvData(WPARAM wparam,LPARAM lparam);
	virtual BOOL OnInitDialog();
	
	
private:
	void EnableAccountTx(BOOL bEnable);
	void EnableTransferTx(BOOL bEnable);
	void EnableScriptTx(BOOL bEnable);
	void UpdateSelectState(emType type);

private:
	unsigned char m_nType;
	CString m_strAccTx_Height;
	CString m_strAccTx_Fee;
	CString m_strAccTx_Acc;
	CString m_strScriptTx_Acc;
	CString m_strScriptTx_Fee;
	CString m_strScriptTx_Height;
	CString m_strScriptTx_Script;
public:
	afx_msg void OnBnClickedRadioTransfer();
	afx_msg void OnBnClickedRadioScript();
	afx_msg void OnBnClickedRadioAccount();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnScan();
	CString m_strFrom;
	CString m_strTo;
	int m_nMoney;
	int m_nHeight;
};
