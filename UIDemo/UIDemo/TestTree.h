#pragma once


// TestTree �Ի���

class TestTree : public CDialogEx
{
	DECLARE_DYNAMIC(TestTree)

public:
	TestTree(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TestTree();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};