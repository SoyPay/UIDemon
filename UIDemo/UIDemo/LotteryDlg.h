#pragma once


// CLotteryDlg 对话框
#include "CommonStruct.h"
class CLotteryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLotteryDlg)

public:
	CLotteryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLotteryDlg();

// 对话框数据
	enum { IDD = IDD_LOTTERY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	LRESULT afx_msg OnShowRecvData(WPARAM wparam,LPARAM lparam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSend();

private:
	void EnableBet(bool bEnable);
	void EnableDraw(bool bEnable);
	void EnableRegister(bool bEnable);
};
