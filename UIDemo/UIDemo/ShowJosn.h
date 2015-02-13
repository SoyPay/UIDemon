#pragma once
#include "afxcmn.h"
#include "json\reader.h"


enum AcceptType{
	ACCEPT,
	OPEN,
	NO_OP
};

// CShowJosn �Ի���

class CShowJosn : public CDialogEx
{
	DECLARE_DYNAMIC(CShowJosn)

public:
	CShowJosn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowJosn();

// �Ի�������
	enum { IDD = IDD_DIALOG_JOSN_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl mTree;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);

private:
	Json::Value mRoot; 
public:

	Json::Value m_selectValue;
	AcceptType mType;
public:
	void SetShowJosn(const Json::Value &root)
	{
	 mRoot= root;
	}

	void Jiexi(Json::Value &obj, HTREEITEM hItemRoot);
	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString m_AppID;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnAcceptAccept();
	afx_msg void OnAcceptOpen();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
