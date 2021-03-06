#pragma once


// CBusinessDlg 对话框
#include "SesureDlg.h"
#include "DarkTxDlg.h"
#include "AnonymTxDlg.h"
#include "P2PBetDlg.h"
#include "LotteryDlg.h"

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
	afx_msg void OnBnClickedTrade1();
	afx_msg void OnBnClickedBtnTrade2();
	afx_msg void OnBnClickedBtnTrade3();
	afx_msg void OnBnClickedBtnTrade4();
	afx_msg void OnBnClickedBtnDark1();
	afx_msg LRESULT OnShowRecvData(WPARAM wparam,LPARAM lparam);
	afx_msg void OnBnClickedBtnDark2();
	afx_msg void OnBnClickedRadioSesure();
	afx_msg void OnBnClickedRadioDark();

public:
	map<emBusinessType,CBaseDlg*> GetBaseDlgPtrMap();
	void UpdateRegScriptSignAddr();

private:
	void Init();
	void EnableAnonymTx(bool bEnable);
	void EnableSesureTx(bool bEnable);
	void EnableDarkTx(bool bEnable);
	void EnableP2PBet(bool bEnable);
	void EnableLottery(bool bEnable);
	void EnableStock(bool bEnable);
	void UpdateSelectState(emBusinessType type);

private:
	emBusinessType		m_emType;
	CSesureDlg*			m_pSesureDlg;
	CDarkTxDlg*			m_pDarkTxDlg;
	CAnonymTxDlg*		m_pAnonymTxDlg;
	CP2PBetDlg*			m_pP2PBetDlg;
	CLotteryDlg*		m_pLotteryDlg;
public:
	afx_msg void OnBnClickedBtnAnonym();
	afx_msg void OnBnClickedRadioAnonym();
	afx_msg void OnBnClickedBtnSendBet();
	afx_msg void OnBnClickedBtnAcceptBet();
	afx_msg void OnBnClickedBtnAexpose();
	afx_msg void OnBnClickedRadioP2pBet();
	afx_msg void OnBnClickedRadioLottery();
	afx_msg void OnBnClickedBtnBexpose();
	afx_msg void OnBnClickedBtnLottertRegister();
	afx_msg void OnBnClickedBtnLotteryBet();
	afx_msg void OnBnClickedBtnLottertDraw();
	afx_msg void OnBnClickedBtnDark3();
	afx_msg void OnNMClickSyslinkSesure(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkDark(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkBet(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnP2pPool();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnNMClickSyslinkBet2(NMHDR *pNMHDR, LRESULT *pResult);
};
