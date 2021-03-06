#pragma once
#include "Resource.h"


// CRPCDlg 对话框

class CRPCDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRPCDlg)

public:
	CRPCDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRPCDlg();

// 对话框数据
	enum { IDD = IDD_RPC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnShowRecvData(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnShowSendData(WPARAM wparam,LPARAM lparam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnClear();

private:
	void UpdateEditContent(const CStringA& strNewData,const CString& strKeyWord);
};
#pragma once
