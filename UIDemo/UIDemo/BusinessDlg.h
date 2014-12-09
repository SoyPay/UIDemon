#pragma once


// CBusinessDlg 对话框
#include "SesureDlg.h"
class CBusinessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBusinessDlg)

public:
	CBusinessDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBusinessDlg();

// 对话框数据
	enum { IDD = IDD_BUSINESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	void Init();
public:
	afx_msg void OnBnClickedTrade1();

private:
	CSesureDlg*	m_pSesureDlg;
public:
	afx_msg void OnBnClickedBtnTrade2();
	afx_msg void OnBnClickedBtnTrade3();
	afx_msg void OnBnClickedBtnTrade4();
	afx_msg LRESULT OnShowRecvData(WPARAM wparam,LPARAM lparam);
};
