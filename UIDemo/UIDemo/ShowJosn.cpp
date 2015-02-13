// ShowJosn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UIDemo.h"
#include "ShowJosn.h"
#include "afxdialogex.h"
#include <algorithm>
#include <string>
#include "RpcCmd.h"
#include "CommonStruct.h"
using namespace std;
// CShowJosn �Ի���

IMPLEMENT_DYNAMIC(CShowJosn, CDialogEx)

CShowJosn::CShowJosn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowJosn::IDD, pParent)
	, m_AppID(_T(""))
{

}

CShowJosn::~CShowJosn()
{
}

void CShowJosn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, mTree);
	DDX_Text(pDX, IDC_EDIT1, m_AppID);
}


BEGIN_MESSAGE_MAP(CShowJosn, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CShowJosn::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDOK, &CShowJosn::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CShowJosn::OnNMDblclkTree1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CShowJosn::OnBnClickedButton1)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ACCEPT_ACCEPT, &CShowJosn::OnAcceptAccept)
	ON_COMMAND(ID_ACCEPT_OPEN, &CShowJosn::OnAcceptOpen)
END_MESSAGE_MAP()


// CShowJosn ��Ϣ�������


void CShowJosn::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CShowJosn::Jiexi(Json::Value &obj, HTREEITEM hItemRoot)
{

	switch (obj.type())
	{
	case Json::objectValue: //����
		//IDictionary<string, object> iDic = (IDictionary<string, object>)obj;
		{

			Json::Value::Members members( obj.getMemberNames() );
			std::sort( members.begin(), members.end() );
			for ( Json::Value::Members::iterator it = members.begin();	
				it != members.end();
				++it )	
			{
				const std::string &name = *it;
				HTREEITEM varRoot = mTree.InsertItem(CString(name.c_str())+":",hItemRoot);
				Jiexi(obj[name],varRoot);
			}	

		}
		break;

	case Json::arrayValue: //����
		{
			int size = obj.size();
			for ( int index =0; index < size; ++index )
			{

				static char buffer[500];
				sprintf( buffer, "[%d]", index );
				string name(buffer);
				HTREEITEM varRoot = mTree.InsertItem(CString(buffer),hItemRoot);
				Jiexi(obj[index],varRoot);
			}

		}
		break;
	default:	
		if(TVI_ROOT != hItemRoot )
		{
			CString strContent = mTree.GetItemText(hItemRoot); 
			strContent += CString(obj.toStyledString().c_str()); 
			mTree.SetItemText(hItemRoot,strContent);
			break;
		}
	
	}
}




INT_PTR CShowJosn::DoModal()
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialogEx::DoModal();
}


void CShowJosn::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

}


BOOL CShowJosn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	mTree.DeleteAllItems();
	Jiexi(mRoot,TVI_ROOT);
	UpdateData(TRUE);
	mType = NO_OP;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CShowJosn::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPoint   pt; 
	GetCursorPos(&pt); 
	mTree.ScreenToClient(&pt); 
	UINT   uFlags   ; 
	HTREEITEM   hSel   =   mTree.HitTest(pt,&uFlags); 
	mTree.Select(hSel,TVGN_CARET); 
	if (uFlags  &  TVHT_ONITEMBUTTON) {  
		;
	} else {
		AfxMessageBox( mTree.GetItemText(mTree.GetSelectedItem()) );
	}
	*pResult = 0;
	*pResult = 0;
}


void CShowJosn::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( NULL != GetSafeHwnd() ) {
		CRect rc ;
		GetClientRect( rc ) ;
		CWnd * p_St = (CStatic*)GetDlgItem(IDC_TREE1);
		if ( NULL != p_St ) {
			p_St->SetWindowPos(NULL ,0 , 0 , rc.Width() , rc.Height() , SWP_SHOWWINDOW);
		}
	}

}


void CShowJosn::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strCommand;
	UpdateData(TRUE);
	strCommand.Format(_T("%s %s %s %s"),_T("getp2pbetdata"),m_AppID,_T("1000"),_T("1"));
	CStringA strShowData ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
	
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return ;

	mRoot= root;
	mTree.DeleteAllItems();
	Jiexi(root,TVI_ROOT);

}


void CShowJosn::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: �ڴ˴������Ϣ����������
	// TODO: Add your message handler code here

	CPoint   pt; 
	GetCursorPos(&pt); 
	mTree.ScreenToClient(&pt); 
	UINT   uFlags   ; 
	HTREEITEM   hSel1   =   mTree.HitTest(pt,&uFlags); 
	mTree.Select(hSel1,TVGN_CARET); 



	  CMenu menu;
	   HTREEITEM HTreeParentItem ;
     menu.LoadMenu(IDR_MENU_POP); //��ȡ��Դ
	 HTREEITEM hSel=mTree.GetSelectedItem();
	 if(hSel != NULL)
	  HTreeParentItem = mTree.GetParentItem(hSel);
	 CString str;
	 if(HTreeParentItem != NULL)
	 str = mTree.GetItemText(HTreeParentItem);
	 if(str == _T("first:"))
	 {
		 menu.EnableMenuItem(ID_ACCEPT_OPEN,MF_GRAYED);
		  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y,this);
	 }
	 else if (str == _T("second:"))
	 {
		 menu.EnableMenuItem(ID_ACCEPT_ACCEPT,MF_GRAYED);
		  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y,this);
	 }
	
	 
}


void CShowJosn::OnAcceptAccept()
{
	// TODO: �ڴ���������������
	//AfxMessageBox( mTree.GetItemText(mTree.GetSelectedItem()) );
	USES_CONVERSION; 
	HTREEITEM hSel=mTree.GetSelectedItem();
	CString hashstr =  mTree.GetItemText(hSel) ;
	HTREEITEM HTreeParentItem = mTree.GetParentItem(hSel);
	hashstr.Remove(':');
	Json::Value first = mRoot["first"];
    m_selectValue = first[W2A(hashstr)];
	mType = ACCEPT;

	CDialogEx::OnOK();
//	AfxMessageBox( mTree.GetItemText(HTreeParentItem) );
//	mTree.GetParent()
}


void CShowJosn::OnAcceptOpen()
{
	// TODO: �ڴ���������������
	USES_CONVERSION; 
	HTREEITEM hSel=mTree.GetSelectedItem();
	CString hashstr =  mTree.GetItemText(hSel) ;
	HTREEITEM HTreeParentItem = mTree.GetParentItem(hSel);
	CString str = mTree.GetItemText(HTreeParentItem);
	hashstr.Remove(':');
	Json::Value first = mRoot["second"];
	m_selectValue = first[W2A(hashstr)];
	mType = OPEN;
		CDialogEx::OnOK();
}


BOOL CShowJosn::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->wParam == VK_RETURN)
	{
	    OnBnClickedButton1();
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
