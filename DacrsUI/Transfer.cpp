// Transfer.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "Transfer.h"
#include "afxdialogex.h"


// CTransfer 对话框

IMPLEMENT_DYNAMIC(CTransfer, CDialogEx)

CTransfer::CTransfer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTransfer::IDD, pParent)
{

}

CTransfer::~CTransfer()
{
}

void CTransfer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTransfer, CDialogEx)
	ON_BN_CLICKED(IDC_SENDTRNSFER, &CTransfer::OnBnClickedSendtrnsfer)
	ON_CBN_SELCHANGE(IDC_COMBO_ADDR_OUT, &CTransfer::OnCbnSelchangeCombo1)
	ON_MESSAGE(MSG_USER_TRANSER_UI , &CTransfer::OnShowListaddrData  )
END_MESSAGE_MAP()


// CTransfer 消息处理程序


void CTransfer::OnBnClickedSendtrnsfer()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pListaddrInfo.size() == 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("地址不存在") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	uistruct::LISTADDR_t *pListAddr = (uistruct::LISTADDR_t*)(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetItemData(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetCurSel())) ;
	if ( NULL != pListAddr ) {
		CString strCommand , strMaddress , strMoney;
		GetDlgItem(IDC_EDIT_DESADDRESS)->GetWindowTextA(strMoney);

		GetDlgItem(IDC_EDIT_MONEY)->GetWindowTextA(strMoney);

		strCommand.Format(_T("%s %s %s %lld"),_T("sendtoaddress") ,pListAddr->address ,strMaddress ,REAL_MONEY(atof(strMoney)) );
		CStringA strShowData ;

		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strShowData.GetString(), root)) 
		{
			::MessageBox( this->GetSafeHwnd() ,strShowData , _T("提示") , MB_ICONINFORMATION ) ;
			return  ;
		}
		BOOL bRes = FALSE ;
		CString strGettxdetail;
		int pos = strShowData.Find("hash");
		if ( pos >=0 ) {
			//插入到数据库
			CString strHash,strHash1 ;
			strHash.Format(_T("'%s'") , root["hash"].asCString() );
			strHash1.Format(_T("'%s'") , root["hash"].asCString() );
			theApp.cs_SqlData.Lock();
			int nItem =  theApp.m_SqliteDeal.FindDB(_T("revtransaction") , strHash1 ,_T("hash") ) ;
			theApp.cs_SqlData.Unlock();
	
			if ( 0 == nItem ) {

				CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
				postmsg.SetData(strHash);
				theApp.m_MsgQueue.push(postmsg);
			}
		}

		CString strData;
		if ( pos >=0 ) {
			strData.Format( _T("转账成功\n%s") , root["hash"].asCString() ) ;
		}else{
			strData.Format( _T("转账失败!") ) ;
		}
		::MessageBox( this->GetSafeHwnd() ,strData , _T("提示") , MB_ICONINFORMATION ) ;
	}
}


void CTransfer::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pListaddrInfo.size() == 0)
	{
		return;
	}
	uistruct::LISTADDR_t *pListAddr = (uistruct::LISTADDR_t*)(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetItemData(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetCurSel())) ;
	if ( NULL != pListAddr ) {
		double money = CSoyPayHelp::getInstance()->GetAccountBalance(pListAddr->address);
		CString strshow;
		strshow.Format(_T("余额:%.8f"),money);
		((CStatic*)GetDlgItem(IDC_STATIC_XM))->SetWindowText(strshow);
	}
}
BOOL CTransfer::AddListaddrDataBox(){

	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.GetListaddrData(&m_pListaddrInfo);
	theApp.cs_SqlData.Unlock();

	if ( 0 == m_pListaddrInfo.size() ) return FALSE ;
	
	//清除ComBox控件
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->ResetContent();
	//加载到ComBox控件
	int nItem = 0;
	std::vector<uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_pListaddrInfo.begin() ; const_it != m_pListaddrInfo.end() ; const_it++ ) {
		((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->InsertString(nItem , const_it->address );
		((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->SetItemData(nItem, (DWORD_PTR)&(*const_it));
		nItem++;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->SetCurSel(0);

	uistruct::LISTADDR_t *pListAddr = (uistruct::LISTADDR_t*)(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetItemData(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetCurSel())) ;
	if ( NULL != pListAddr ) {
		double money = CSoyPayHelp::getInstance()->GetAccountBalance(pListAddr->address);
		CString strshow;
		strshow.Format(_T("余额:%.8f"),money);
		((CStatic*)GetDlgItem(IDC_STATIC_XM))->SetWindowText(strshow);
	}
	return TRUE ;
}

LRESULT CTransfer::OnShowListaddrData( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	switch (wParam)
	{
	case WM_UP_ADDRESS:
		{
			AddListaddrDataBox();
		}
		break;
	}
	return 0 ;
}