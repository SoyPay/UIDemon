#pragma once


// TestTree 对话框

class TestTree : public CDialogEx
{
	DECLARE_DYNAMIC(TestTree)

public:
	TestTree(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TestTree();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
