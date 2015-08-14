// AddrBook.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "AddrBook.h"
#include "afxdialogex.h"

#include "NewSendAddr.h"
// CAddrBook 对话框

IMPLEMENT_DYNAMIC(CAddrBook, CDialogEx)

CAddrBook::CAddrBook(CWnd* pParent /*=NULL*/  , CString strTip /*=_T("")*/)
	: CDialogEx(CAddrBook::IDD, pParent)
{
	m_strTip = strTip ;
	m_pBmp = NULL ;

	hitRow = -1;
	hitCol = -1;
	m_prehittex = _T("");
}

CAddrBook::~CAddrBook()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CAddrBook::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SHOW, m_Text);
	DDX_Control(pDX, IDC_LIST, m_listCtrl);
	DDX_Control(pDX, IDC_BUTTON_GB, m_rBtnClose);
	DDX_Control(pDX, IDC_BUTTON_ADDADDRBOOK, m_rBtnAdd);
	DDX_Control(pDX, IDC_BUTTON_DELEITEM, m_rBtnDelete);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CAddrBook, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADDADDRBOOK, &CAddrBook::OnBnClickedButtonAddaddrbook)
	ON_BN_CLICKED(IDOK, &CAddrBook::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DELEITEM, &CAddrBook::OnBnClickedButtonDeleitem)
	//ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CAddrBook::OnNMDblclkList1)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_GB, &CAddrBook::OnBnClickedButtonGb)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CAddrBook::OnNMDblclkList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CAddrBook::OnNMClickList)
END_MESSAGE_MAP()


// CAddrBook 消息处理程序


void CAddrBook::OnBnClickedButtonAddaddrbook()
{
	// TODO: 在此添加控件通知处理程序代码
	CNewSendAddr addsendaddr;
	string strShowData;
	INT_PTR nResponse = addsendaddr.DoModal();;
	if (nResponse == IDOK)
	{
		uistruct::ADDRBOOK_t addr;
		addsendaddr.GetAddrbook(addr);
		int count = m_listCtrl.GetItemCount();
		int nSubIdx = 0;

		m_listCtrl.InsertItem( count ,addr.label.c_str()) ;

		m_listCtrl.SetItemText(count , ++nSubIdx , addr.address.c_str() ) ;

		//// 插入数据库
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRBOOK);
		string temp =addr.ToJson();
		postmsg.SetData(temp);
		theApp.m_MsgQueue.push(postmsg);
	}

}

BOOL CAddrBook::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogEx::Create(lpszTemplateName, pParentWnd);
	if ( bRes ) {
		//UpdateData(FALSE);

		
	}
	return bRes ;
}

BOOL CAddrBook::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Text.SetFont(110, _T("微软雅黑"));				//设置显示字体和大小
	m_Text.SetTextColor(RGB(255,255,255));	    //字体颜色
	m_Text.SetWindowText(_T("地址簿管理")) ;
	m_Text.SetWindowPos( NULL , 5 , 8 , 200, 25  ,SWP_SHOWWINDOW ) ; 

	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	m_rBtnClose.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnClose.SizeToContent();
	m_rBtnClose.SetWindowPos(NULL ,780-26 , 0 , 0 , 0 , SWP_NOSIZE); 

	struct LISTCol {
		string		name ;
		UINT		size ;
	} listcol[3]  = {
		{"标签" ,      200},
		{"地址" ,      558}
	};
	m_listCtrl.SetBkColor(RGB(240,240,240));       
	m_listCtrl.SetRowHeigt(23);               
	m_listCtrl.SetHeaderHeight(1.5);         
	m_listCtrl.SetHeaderFontHW(15,0);
	m_listCtrl.SetHeaderBKColor(222,224,218,0); 
	//m_listCtrl.SetHeaderBKColor(240,240,240,0);
	m_listCtrl.SetHeaderTextColor(RGB(0,0,0)); //设置头部字体颜色
	m_listCtrl.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i < 2 ; i++  ) {
		m_listCtrl.InsertColumn(i,listcol[i].name.c_str(),LVCFMT_CENTER,listcol[i].size);
	}
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );
	m_listCtrl.SetWindowPos( NULL , 0 , 40 , 780, 314 - 100  ,SWP_SHOWWINDOW ) ; 

	m_rBtnAdd.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
	m_rBtnAdd.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnAdd.SetWindowText("添加地址") ;
	m_rBtnAdd.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnAdd.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnAdd.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnAdd.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnAdd.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnAdd.SizeToContent();
	m_rBtnAdd.SetWindowPos(NULL ,80 , 314 - 100 + 40 + 8, 0 , 0 , SWP_NOSIZE); 

	m_rBtnDelete.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
	m_rBtnDelete.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnDelete.SetWindowText("删除地址") ;
	m_rBtnDelete.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnDelete.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnDelete.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnDelete.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnDelete.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnDelete.SizeToContent();
	m_rBtnDelete.SetWindowPos(NULL ,240 , 314 - 100 + 40 + 8, 0 , 0 , SWP_NOSIZE); 

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText("确 定") ;
	m_rBtnOk.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();
	m_rBtnOk.SetWindowPos(NULL ,600 , 314 - 100 + 40 + 8, 0 , 0 , SWP_NOSIZE); 

	SetBkBmpNid( IDB_BITMAP_ADD_BOOK_BJ ) ;
	m_fontGrid.CreatePointFont(100,_T("新宋体"));

	LoadAddrBook();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CAddrBook::SetBkBmpNid( UINT nBitmapIn ) 
{
	if( NULL != m_pBmp ) {
		::DeleteObject( m_pBmp ) ;
		m_pBmp = NULL ;
	}
	m_pBmp	= NULL ;
	HINSTANCE	hInstResource = NULL;	
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);
	if( NULL != hInstResource ) {
		m_pBmp = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);
	}
}
bool CAddrBook::LoadAddrBook()
{
	theApp.m_SqliteDeal.GetAddressBookList(_T(" 1=1 "), &m_mapAddrInfo);

	if ( 0 == m_mapAddrInfo.size() ) return FALSE ;

	m_listCtrl.DeleteAllItems();

	int nSubIdx = 0 , i = 0 ;
	string strShowData = _T("");
	std::map<string,uistruct::ADDRBOOK_t>::const_iterator const_it;
	for ( const_it = m_mapAddrInfo.begin() ; const_it != m_mapAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;
		uistruct::ADDRBOOK_t address = const_it->second;
	
		strShowData=strprintf("%s" ,address.label);
		m_listCtrl.InsertItem(i,address.label.c_str());

		strShowData=strprintf("%s" ,address.address);
		m_listCtrl.SetItemText(i , ++nSubIdx , address.address.c_str() ) ;
		i++;
	}

	return TRUE ;
}

void CAddrBook::GetAddrbook(uistruct::ADDRBOOK_t &addr)
{
	addr = m_selectAddr;
}

void CAddrBook::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
		CString Label =m_listCtrl.GetItemText(nRow, 0);
		CString addr =m_listCtrl.GetItemText(nRow, 1);
		m_selectAddr.label = Label;
		m_selectAddr.address = addr;
		}else{
			StrShow.Format(_T("请选择地址!\n"));
			UiFun::MessageBoxEx(StrShow, _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}
	CDialogEx::OnOK();
}


void CAddrBook::OnBnClickedButtonDeleitem()
{
	// TODO: 在此添加控件通知处理程序代码
		//EndDialog(IDOK);
		string StrShow;
		POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
		if ( pos ) {
			if ( IDYES == ::MessageBox( this->GetSafeHwnd() ,_T("是否要删除此收款地址") , _T("提示") , MB_YESNO|MB_ICONINFORMATION ) ){
				int nRow = m_listCtrl.GetNextSelectedItem(pos) ;

				//// 删除此条数据库
				CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_UP_DELETERBOOK);
				uistruct::ADDRBOOK_t addr;
				CString Leble = m_listCtrl.GetItemText(nRow, 0); 
				CString address = m_listCtrl.GetItemText(nRow, 1); 
				addr.address = address;
				addr.label = Leble;
				string temp =addr.ToJson();
				postmsg.SetData(temp);
				theApp.m_MsgQueue.push(postmsg);

				//// 从控件中删除
				m_listCtrl.DeleteItem(nRow);
			}
		}else{
			//StrShow.Format(_T("请选择地址!\n"));
			StrShow = "请选择地址!\n";
			UiFun::MessageBoxEx(StrShow.c_str() , _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}
}




BOOL CAddrBook::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect   rect; 
	GetClientRect(&rect); 

	if(m_pBmp   !=   NULL) { 
		BITMAP   bm; 
		CDC   dcMem; 
		::GetObject(m_pBmp,sizeof(BITMAP),   (LPVOID)&bm); 
		dcMem.CreateCompatibleDC(NULL); 
		HBITMAP     pOldBitmap   =(HBITMAP   )   dcMem.SelectObject(m_pBmp); 
		pDC->StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return TRUE;
}


HBRUSH CAddrBook::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	/*if (nCtlColor == CTLCOLOR_STATIC)
	{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&m_fontGrid);
	hbr = (HBRUSH)CreateSolidBrush(RGB(240,240,240));
	}*/
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CAddrBook::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	
}


LRESULT CAddrBook::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT nResult = CDialog::OnNcHitTest(point);
	return nResult  == HTCLIENT ? HTCAPTION : nResult;//鼠标的坐标在客户区么？在的话就把它当成标题栏

	return CDialogEx::OnNcHitTest(point);
}


void CAddrBook::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);//发送一个移动的消息
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CAddrBook::OnBnClickedButtonGb()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDOK);
}


void CAddrBook::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	LVHITTESTINFO info;
	info.pt  =  pNMItemActivate -> ptAction;

	if (m_listCtrl.SubItemHitTest( & info)  !=   - 1  )
	{
		hitRow  =  info.iItem;
		hitCol  =  info.iSubItem;
		if ( hitCol == 1)
		{
			int nRow = pNMItemActivate->iItem;
			CString Label =m_listCtrl.GetItemText(nRow, 0);
			CString addr =m_listCtrl.GetItemText(nRow, 1);
			m_selectAddr.label = Label;
			m_selectAddr.address = addr;
			CDialogEx::OnOK();
			*pResult = 0;
			return;
		}
		if (editItem.m_hWnd  ==  NULL ) // editItem为一输入框控件， 
		{
			RECT rect;
			rect.left  =   0 ;
			rect.top  =   0 ;
			rect.bottom  =   15 ;
			rect.right  =   200 ;
			editItem .Create(WS_CHILD  |  ES_LEFT  |  WS_BORDER  |  ES_AUTOHSCROLL  |  ES_WANTRETURN  |  ES_MULTILINE, rect,  this ,  101 );
			editItem.SetFont( this -> GetFont(), FALSE);
		}
		/// 保存修改之前的字符串
		editItem.GetWindowText(m_prehittex);
		CRect rect;
		m_listCtrl.GetSubItemRect(info.iItem, info.iSubItem, LVIR_BOUNDS, rect);
		rect.top  +=   40 ;
		rect.left  +=   2 ;
		rect.right  -=   556 ;
		rect.bottom  +=   40 ;

		editItem.SetWindowText(m_listCtrl.GetItemText( info .iItem,  info .iSubItem));
		editItem.MoveWindow( & rect, TRUE);
		editItem.ShowWindow( 1 );
		editItem.SetSel(-1);
		editItem.SetFocus();
	}
	*pResult = 0;
}


void CAddrBook::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (editItem.m_hWnd  !=  NULL)
	{
		if( hitCol!= 0)
			{
				hitCol  =  hitRow  =   - 1 ;
			*pResult = 0;
			}
		editItem.ShowWindow( 0 );
		if (hitRow  !=   - 1 )
		{
			CString text;
			editItem.GetWindowText(text);
			m_listCtrl.SetItemText(hitRow, hitCol, text);
			if (text != m_prehittex)
			{
				CString addr = _T("");
				addr = m_listCtrl.GetItemText(hitRow,1);
				string strSourceData  , strW ;
				strSourceData = strprintf("Label = '%s'" , text  ) ;
				strW= strprintf("address = '%s'" , addr ) ;

				uistruct::DATABASEINFO_t DatabaseInfo;
				DatabaseInfo.strSource = strSourceData;
				DatabaseInfo.strWhere = strW ;
				DatabaseInfo.strTabName = _T("t_address_book");
				CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
				string strtemp = DatabaseInfo.ToJson();
	
				postmsg.SetData(strtemp.c_str());
				theApp.m_MsgQueue.push(postmsg);
			}
		}
	}
	hitCol  =  hitRow  =   - 1 ;
	*pResult = 0;
}
