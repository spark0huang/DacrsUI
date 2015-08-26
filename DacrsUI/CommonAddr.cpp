// CommonAddr.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "CommonAddr.h"
#include "afxdialogex.h"


// CCommonAddr 对话框

IMPLEMENT_DYNAMIC(CCommonAddr, CDialogEx)

CCommonAddr::CCommonAddr(UiType uType , CWnd* pParent /*=NULL*/)
	: CDialogBase(CCommonAddr::IDD, pParent)
{
	m_uAddrType = uType ;
}

CCommonAddr::~CCommonAddr()
{
}

void CCommonAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_ButClose);
	DDX_Control(pDX, IDC_LIST_ALLADDR, m_AllAddrlistBox);
	DDX_Control(pDX, IDC_LIST_COMMONADDR, m_CommonAddrListBox);
}


BEGIN_MESSAGE_MAP(CCommonAddr, CDialogBase)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CCommonAddr::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCommonAddr::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CCommonAddr::OnBnClickedButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_ALLADDR, &CCommonAddr::OnLbnSelchangeListAlladdr)
	ON_LBN_SELCHANGE(IDC_LIST_COMMONADDR, &CCommonAddr::OnLbnSelchangeListCommonaddr)
	ON_BN_CLICKED(IDC_BUTTON_ADDROK, &CCommonAddr::OnBnClickedButtonAddrok)
END_MESSAGE_MAP()


// CCommonAddr 消息处理程序


void CCommonAddr::OnSize(UINT nType, int cx, int cy)
{
	CDialogBase::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


BOOL CCommonAddr::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);

	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);

	m_ButClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_ButClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_ButClose.SetWindowText("") ;
	m_ButClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_ButClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_ButClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_ButClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_ButClose.SizeToContent();

	RECT ret;
	GetWindowRect(&ret);
	CRect titleRect ;
	m_ButClose.GetClientRect(titleRect);
	m_ButClose.SetWindowPos(NULL ,(ret.right-ret.left)-titleRect.Width() , 2 , 0 , 0 , SWP_NOSIZE); 

	//GetDlgItem(IDC_LIST_ALLADDR)->SetWindowPos(NULL ,2 , 30 , (ret.left + ret.right)/6  , (ret.top + ret.bottom) - 165 , SWP_SHOWWINDOW); 
	//GetDlgItem(IDC_LIST_COMMONADDR)->SetWindowPos(NULL , (ret.left + ret.right)/6 + 42 , 30 , (ret.left + ret.right)/6  , (ret.top + ret.bottom) - 165  , SWP_SHOWWINDOW); 

	AddListaddrDataBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CCommonAddr::AddListaddrDataBox()
{
	m_mapAddrInfo.clear(); //全部地址
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" sign=1 "), &m_mapAddrInfo);

	if ( 0 == m_mapAddrInfo.size() ) return FALSE ;

	m_mapCommonAddrInfo.clear();//常用地址
	switch(m_uAddrType)
	{
	case UI_SENDP2P_RECORD:
		{
			theApp.m_SqliteDeal.GetCommonWalletAddressList(_T(" betid=1 "), &m_mapCommonAddrInfo);
		}
		break;
	}

	m_AllAddrlistBox.ResetContent(); //清除全部地址ComBox控件
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it; //加载到全部地址ComBox控件
	for ( const_it = m_mapAddrInfo.begin() ; const_it != m_mapAddrInfo.end() ; const_it++ ) {
		int pos = m_AllAddrlistBox.AddString( const_it->second.RegID.c_str() ) ;
		m_AllAddrlistBox.SetItemData( pos , (DWORD_PTR)(&const_it->second) ) ;
	}

	//更新常用地址
	UpdataCommonAddrList(&m_mapCommonAddrInfo);
}
void CCommonAddr::UpdataCommonAddrList(map<string,uistruct::COMMONLISTADDR_t> *pListInfo)
{
	if ( NULL == pListInfo ) return ;
	//常用地址
	m_CommonAddrListBox.ResetContent(); //清除全部地址ComBox控件
	std::map<string,uistruct::COMMONLISTADDR_t>::const_iterator const_it; //加载到全部地址ComBox控件
	for ( const_it = pListInfo->begin() ; const_it != pListInfo->end() ; const_it++ ) {
		int pos = m_CommonAddrListBox.AddString( const_it->second.RegID.c_str() ) ;
		m_CommonAddrListBox.SetItemData( pos , (DWORD_PTR)(&const_it->second) ) ;
	}
}
void CCommonAddr::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDOK);
	CDialogBase::OnClose();
}

void CCommonAddr::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int curSel = m_AllAddrlistBox.GetCurSel() ;
	uistruct::LISTADDR_t * pAddrList = (uistruct::LISTADDR_t*)m_AllAddrlistBox.GetItemData( curSel ) ;
	if ( NULL == pAddrList ) return ;

	string strSourceData , strCond;
	m_mapCommonAddrInfo.clear();//常用地址
	strCond =strprintf(" address = '%s' ", pAddrList->address.c_str());
	uistruct::COMMONLISTADDR_t addrsql;
	int item = theApp.m_SqliteDeal.GetCommonWalletAddressItem(strCond, &addrsql) ;

	if (addrsql.address.length() == 0 ){  //插入
	    switch(m_uAddrType)
		{
		case UI_SENDP2P_RECORD:
			{
				strSourceData=strprintf("'%s' , '%s' , '%d' " , pAddrList->address.c_str() ,pAddrList->RegID.c_str() , 1 ) ;
				if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_common_address") ,strSourceData ) ) {
					  TRACE("Insert t_common_address error!\n");
				}
				theApp.m_SqliteDeal.GetCommonWalletAddressList(_T(" betid=1 "), &m_mapCommonAddrInfo);
			}
			break;
		}
		//更新到常用地址
	    UpdataCommonAddrList(&m_mapCommonAddrInfo);
		switch(m_uAddrType)
		{
		case UI_SENDP2P_RECORD:
			{
				CPostMsg postp2pmsg(MSG_USER_P2PADDRES,0);
				theApp.m_MsgQueue.push(postp2pmsg);
			}
			break;
		}

	}else{  //更新
	    UiFun::MessageBoxEx(_T("该地址以存在!") , _T("提示") ,MFB_OK|MFB_TIP );
	}
}

void CCommonAddr::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int curSel = m_CommonAddrListBox.GetCurSel() ;
	uistruct::COMMONLISTADDR_t * pAddrList = (uistruct::COMMONLISTADDR_t*)m_CommonAddrListBox.GetItemData( curSel ) ;
	if ( NULL == pAddrList ) return ;

	string strSourceData ;

	strSourceData = strprintf(" address='%s' ", pAddrList->address.c_str());
	int item = theApp.m_SqliteDeal.DeleteTableItem(_T("t_common_address"), strSourceData);

	m_mapCommonAddrInfo.clear();//常用地址
	switch(m_uAddrType)
	{
	case UI_SENDP2P_RECORD:
		{
			theApp.m_SqliteDeal.GetCommonWalletAddressList(_T(" betid=1 "), &m_mapCommonAddrInfo);
		}
		break;
	}
	//更新到常用地址
	UpdataCommonAddrList(&m_mapCommonAddrInfo);
	switch(m_uAddrType)
	{
	case UI_SENDP2P_RECORD:
		{
			CPostMsg postp2pmsg(MSG_USER_P2PADDRES,0);
			theApp.m_MsgQueue.push(postp2pmsg);
		}
		break;
	}
}


void CCommonAddr::OnLbnSelchangeListAlladdr()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
}


void CCommonAddr::OnLbnSelchangeListCommonaddr()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
}


void CCommonAddr::OnBnClickedButtonAddrok()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDOK);
}
