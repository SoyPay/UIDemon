// TestTree.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UIDemo.h"
#include "TestTree.h"
#include "afxdialogex.h"


// TestTree �Ի���

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


// TestTree ��Ϣ�������
