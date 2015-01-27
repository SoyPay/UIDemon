// TestTree.cpp : 实现文件
//

#include "stdafx.h"
#include "UIDemo.h"
#include "TestTree.h"
#include "afxdialogex.h"


// TestTree 对话框

IMPLEMENT_DYNAMIC(TestTree, CDialogEx)

TestTree::TestTree(CWnd* pParent /*=NULL*/)
	: CDialogEx(TestTree::IDD, pParent)
{

}

TestTree::~TestTree()
{
}

void TestTree::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TestTree, CDialogEx)
END_MESSAGE_MAP()


// TestTree 消息处理程序
