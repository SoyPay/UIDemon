#pragma once


// CUpdateVerDlg 对话框

class CUpdateVerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateVerDlg)

public:
	CUpdateVerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpdateVerDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	//CStatic m_LinkStatic;
public:
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult);
};
