// P2PDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "P2PDlg.h"
#include "afxdialogex.h"
#include "Out.h"
#include "ReCharge.h"
#include "GuessNum.h"
#include "RpcCmd.h"
#include "CommonAddr.h"
#include "P2pTip.h"
#include "SetAutoSendp2p.h"

#define OUT_HEIGHT  60

// CP2PDlg 对话框

IMPLEMENT_DYNAMIC(CP2PDlg, CDialogBar)

CP2PDlg::CP2PDlg()
{
	m_pBmp = NULL ;
	m_seltab = 0;
	m_pagecount = 0;
	m_curpage = 0;
	m_PoolList.clear();
	m_pagesize = 5;
	v_linkCtrl = NULL;
	is_atuosendbet = false;   //// false  不自动发单 true 自动发单
	m_sendbethash = "";
	m_acceptmoney = 0.0;       /// 接单的百分比
	m_sendbetmoney =0;
}

CP2PDlg::~CP2PDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}

	if (v_linkCtrl != NULL)
	{
		delete v_linkCtrl;
		v_linkCtrl= NULL;
	}
}

void CP2PDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BALANCE , m_Balance);
	DDX_Control(pDX, IDC_STATIC_NOT_DRAW , m_NotDraw);
	DDX_Control(pDX, IDC_STATIC_DW , m_Dw);
	DDX_Control(pDX, IDC_STATIC_VS , m_VS);

	DDX_Control(pDX, IDC_BUTTON_WITHD , m_rBtnWithd);
	DDX_Control(pDX, IDC_BUTTON_RECH , m_rBtnRech);
	DDX_Control(pDX, IDC_BUTTON_MALE , m_rBtnMale);
	DDX_Control(pDX, IDC_BUTTON_WOMAN , m_rBtnWoman); 
	DDX_Control(pDX, IDC_COMBO_ADDRES, m_addrbook);
	DDX_Control(pDX, IDC_LIST_BONUS, m_BonusListBox);
	DDX_Control(pDX, IDC_BUTTON_REFRESH_1, m_rBtnRefresh1);
	DDX_Control(pDX, IDC_BUTTON_REFRESH_2, m_rBtnRefresh2);
	DDX_Control(pDX, IDC_TAB, m_tab);

	DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
	DDX_Control(pDX ,IDC_STATIC_MONEY ,m_money ) ;

	DDX_Control(pDX, IDC_WINERLOSER, m_rBtnWinerloser);
	DDX_Control(pDX, IDC_ONEWINER, m_rBtnAddrWinerloser);
	DDX_Control(pDX, IDC_CANCELORDE, m_rbCancelOrder);
	

	DDX_Control(pDX, IDC_BUTTON_SETADDR, m_rBtnSetCommonAddr);
	DDX_Control(pDX, IDC_SETAUTOBET, m_rbSetAtuoBet);
	DDX_Control(pDX, IDC_CANCELAUTOBET, m_rbCancelAtuoBet);
}


BEGIN_MESSAGE_MAP(CP2PDlg, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE( WM_BN_CLICK, &CP2PDlg::onBnCLick)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_ADDRES, &CP2PDlg::OnCbnSelchangeComboAddres)
	ON_MESSAGE(MSG_USER_P2P_UI , &CP2PDlg::OnShowListCtrol )
	ON_MESSAGE(MSG_USER_P2PADDRES , &CP2PDlg::OnUpAddressCombo )
	ON_BN_CLICKED(IDC_BUTTON_WITHD, &CP2PDlg::OnBnClickedButtonWithd)
	ON_BN_CLICKED(IDC_BUTTON_RECH, &CP2PDlg::OnBnClickedButtonRech)
	ON_BN_CLICKED(IDC_BUTTON_MALE, &CP2PDlg::OnBnClickedButtonMale)
	ON_BN_CLICKED(IDC_BUTTON_WOMAN, &CP2PDlg::OnBnClickedButtonWoman)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CP2PDlg::OnTcnSelchangeTab)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_2, &CP2PDlg::OnBnClickedButtonRefresh2)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_1, &CP2PDlg::OnBnClickedButtonRefresh1)
	ON_NOTIFY(NM_THEMECHANGED, IDC_LIST_BONUS, &CP2PDlg::OnNMThemeChangedListBonus)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CP2PDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CP2PDlg::OnBnClickedButtonNext)
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_LIST_BONUS, &CP2PDlg::OnLbnDblclkListBonus)
	ON_BN_CLICKED(IDC_CANCELORDE, &CP2PDlg::OnBnClickedCancelorde)
	ON_BN_CLICKED(IDC_BUTTON_SETADDR, &CP2PDlg::OnBnClickedButtonSetaddr)
	ON_BN_CLICKED(IDC_SETAUTOBET, &CP2PDlg::OnBnClickedSetautobet)
	ON_BN_CLICKED(IDC_CANCELAUTOBET, &CP2PDlg::OnBnClickedCancelautobet)
END_MESSAGE_MAP()


// CP2PDlg 消息处理程序
void CP2PDlg::SetBkBmpNid( UINT nBitmapIn ) 
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

int CP2PDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//SetBkBmpNid(IDB_BITMAP_P2P_BJ);IDB_BITMAP_P2P_BJEN
    SetBkBmpNid(UiFun::GetBmpId(IDB_BITMAP_P2P_BJ,IDB_BITMAP_P2P_BJEN,theApp.language()));
	if (v_linkCtrl == NULL)
	{
		v_linkCtrl = new CMFCLinkCtrl;
		v_linkCtrl->Create(_T(""), 
			WS_VISIBLE, 
			CRect(5, 5, 30, 20), 
			this, 
			IDC_MFCLINK1);
	}
	return 0;
}


BOOL CP2PDlg::OnEraseBkgnd(CDC* pDC)
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
		pDC-> StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return TRUE;
}


BOOL CP2PDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		m_rBtnMale.LoadBitmaps(IDB_BITMAP_SISTER_1,IDB_BITMAP_SISTER_3,IDB_BITMAP_SISTER_2,IDB_BITMAP_SISTER_1);
		m_rBtnWoman.LoadBitmaps(IDB_BITMAP_BOTHER_1,IDB_BITMAP_BOTHER_3,IDB_BITMAP_BOTHER_2,IDB_BITMAP_BOTHER_1);
		UpdateData(0);
		m_Balance.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_Balance.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_Balance.SetWindowText(_T(""));

		m_NotDraw.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_NotDraw.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_NotDraw.SetWindowText(_T(""));

		m_Dw.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_Dw.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_Dw.SetWindowText(_T("SMC"));

		m_VS.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_VS.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_VS.SetWindowText(_T(" VS"));

		m_sCountpage.SetFont(90, _T("黑体"));				//设置显示字体和大小
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_sCountpage.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_TOTAL" ,theApp.gsLanguage));

		m_rBtnWithd.SetBitmaps( IDB_BITMAP_P2PBUTTON_1 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_1 , RGB(255, 255, 255) );
		m_rBtnWithd.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnWithd.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS" ,theApp.gsLanguage)) ;
		m_rBtnWithd.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnWithd.SizeToContent();

		m_rBtnRech.SetBitmaps( IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 255) );
		m_rBtnRech.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRech.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_RECHARGE" ,theApp.gsLanguage)) ;
		m_rBtnRech.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnRech.SizeToContent();

		m_rBtnRefresh1.SetBitmaps( IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 255) );
		m_rBtnRefresh1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRefresh1.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_REFRESH" ,theApp.gsLanguage)) ;
		m_rBtnRefresh1.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnRefresh1.SizeToContent();

		m_rBtnRefresh2.SetBitmaps( IDB_BITMAP_BUTTON3 , RGB(255, 255, 0) , IDB_BITMAP_BUTTON3 , RGB(255, 255, 255) );
		m_rBtnRefresh2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRefresh2.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_REFRESH" ,theApp.gsLanguage)) ;
		m_rBtnRefresh2.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnRefresh2.SizeToContent();

		
		m_rBtnUp.SetBitmaps( IDB_BITMAP_UP , RGB(255, 255, 0) , IDB_BITMAP_UP , RGB(255, 255, 255) );
		m_rBtnUp.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnUp.SetWindowText("") ;
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnUp.SizeToContent();

		m_rBtnNext.SetBitmaps( IDB_BITMAP_NEXT , RGB(255, 255, 0) , IDB_BITMAP_NEXT , RGB(255, 255, 255) );
		m_rBtnNext.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNext.SetWindowText("") ;
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnNext.SizeToContent();

		m_rBtnWinerloser.SetBitmaps( IDB_BITMAP_WINERLOUSER , RGB(255, 255, 0) , IDB_BITMAP_WINERLOUSER , RGB(255, 255, 255) );
		m_rBtnWinerloser.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnWinerloser.SetWindowText("") ;
		m_rBtnWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(41, 57, 85));
		m_rBtnWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(41, 57, 85));
		m_rBtnWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(41, 57, 85));
		m_rBtnWinerloser.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(41, 57, 85));
		m_rBtnWinerloser.SizeToContent();

		m_rBtnAddrWinerloser.SetBitmaps( IDB_BITMAP_WINERLOUSER , RGB(255, 255, 0) , IDB_BITMAP_WINERLOUSER , RGB(255, 255, 255) );
		m_rBtnAddrWinerloser.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnAddrWinerloser.SetWindowText("") ;
		m_rBtnAddrWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(41, 57, 85));
		m_rBtnAddrWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(41, 57, 85));
		m_rBtnAddrWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(41, 57, 85));
		m_rBtnAddrWinerloser.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(41, 57, 85));
		m_rBtnAddrWinerloser.SizeToContent();

		m_rbCancelOrder.SetBitmaps( IDB_BITMAP_WINERLOUSER , RGB(255, 255, 0) , IDB_BITMAP_WINERLOUSER , RGB(255, 255, 255) );
		m_rbCancelOrder.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_CANCEL_ORDER" ,theApp.gsLanguage));
		m_rbCancelOrder.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rbCancelOrder.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(41, 57, 85));
		m_rbCancelOrder.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(41, 57, 85));
		m_rbCancelOrder.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(41, 57, 85));
		m_rbCancelOrder.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(41, 57, 85));
		m_rbCancelOrder.SizeToContent();

		m_rbSetAtuoBet.SetBitmaps( IDB_BITMAP_WINERLOUSER , RGB(255, 255, 0) , IDB_BITMAP_WINERLOUSER , RGB(255, 255, 255) );
		m_rbSetAtuoBet.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_AUTO_SENDBET" ,theApp.gsLanguage));
		m_rbSetAtuoBet.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rbSetAtuoBet.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(41, 57, 85));
		m_rbSetAtuoBet.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(41, 57, 85));
		m_rbSetAtuoBet.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(41, 57, 85));
		m_rbSetAtuoBet.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(41, 57, 85));
		m_rbSetAtuoBet.SizeToContent();

		m_rbCancelAtuoBet.SetBitmaps( IDB_BITMAP_WINERLOUSER , RGB(255, 255, 0) , IDB_BITMAP_WINERLOUSER , RGB(255, 255, 255) );
		m_rbCancelAtuoBet.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_CANCEL_SENDBET" ,theApp.gsLanguage));
		m_rbCancelAtuoBet.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rbCancelAtuoBet.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(41, 57, 85));
		m_rbCancelAtuoBet.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(41, 57, 85));
		m_rbCancelAtuoBet.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(41, 57, 85));
		m_rbCancelAtuoBet.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(41, 57, 85));
		m_rbCancelAtuoBet.SizeToContent();


		m_rBtnSetCommonAddr.SetBitmaps( IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 255) );
		m_rBtnSetCommonAddr.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnSetCommonAddr.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_COMM_ADDRESS" ,theApp.gsLanguage)) ;
		m_rBtnSetCommonAddr.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnSetCommonAddr.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnSetCommonAddr.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnSetCommonAddr.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnSetCommonAddr.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnSetCommonAddr.SizeToContent();

		m_money.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_money.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_money.SetWindowText(_T(""));
		

		m_tab.InsertItem(0,UiFun::UI_LoadString("P2P_MODULE" , "P2P_SINGLE_RECORD" ,theApp.gsLanguage));  //添加参数一选项卡 
		m_tab.InsertItem(1,UiFun::UI_LoadString("P2P_MODULE" , "P2P_SEND_RECORD" ,theApp.gsLanguage));  //添加参数二选项卡 

		CSize sz(80,30);
		m_tab.SetItemSize(sz);
		m_tab.SetMinTabWidth(80);
		m_tab.SetItemPadding(6);

		if ( !m_BetRecord.Create( (LPCTSTR)CBetRecord::IDD , &m_tab ))
			return -1;

		if ( !m_SendRecord.Create( (LPCTSTR)CSendRecord::IDD , &m_tab ))
			return -1;

		CRect rc;
		m_tab.GetClientRect(rc);
		rc.bottom -= 1;
		rc.left += 1;
		rc.top += 33;
		rc.right -= 2;
		m_BetRecord.MoveWindow(&rc);
		m_SendRecord.MoveWindow(&rc);
		m_pDialog.push_back(&m_BetRecord) ;
		m_pDialog.push_back(&m_SendRecord) ;
		OnSelectShowWin(0);

		AddListaddrDataBox();
		OnListPool();

		onShowLink();


		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_P2P_UI ) ;
		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_P2PADDRES ) ;

//		m_rbCancelAtuoBet.ShowWindow(SW_HIDE);
		/// 设置定时器刷新界面 一分钟
		//SetTimer(1,30000,NULL);
		SetTimer(1,40000,NULL);
	}
	return bRes ;
}
void CP2PDlg::OnSelectShowWin(int nCurSelTab)
{
	m_seltab = nCurSelTab;
	int nCurSelTabTmp = 0 ;
	std::vector<CDialog*>::iterator itDialog = m_pDialog.begin();
	for (;itDialog != m_pDialog.end();itDialog++)
	{  
		if(nCurSelTabTmp == nCurSelTab ){
			(*itDialog)->ShowWindow(SW_SHOW);
		}else{
			(*itDialog)->ShowWindow(SW_HIDE);
		}
		nCurSelTabTmp++ ;
	}
}

void CP2PDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {

		CRect rc ;
		GetClientRect( rc ) ;

		CWnd *pst = GetDlgItem( IDC_LIST_BONUS ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,455 , 85 , 432, 147  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_UP ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-129 , 232 + 5 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-115+11, 232 + 5 , 40, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_NEXT ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-70+11 , 232 + 5 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_STATIC_COUNT_PAGE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,875-50+11 , 232 + 8 , 50, 30  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_REFRESH_1 ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,800 , 20 ,  0 , 0 , SWP_NOSIZE ) ; 
		}
		pst = GetDlgItem( IDC_TAB ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,10 , 268 ,  878 , 218 , SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_REFRESH_2 ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,800 , 270 ,  0 , 0 , SWP_NOSIZE ) ; 
		}

		pst = GetDlgItem( IDC_WINERLOSER ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,550 , 270 , 0 ,0, SWP_NOSIZE ) ; 
		}

		pst = GetDlgItem( IDC_ONEWINER ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,670 , 270 , 0 ,0, SWP_NOSIZE ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_BALANCE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*13 , (rc.Height()/100)*8 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_STATIC_NOT_DRAW ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*13 , (rc.Height()/100)*14 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_BUTTON_WITHD ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*28 , (rc.Height()/100)*9 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_BUTTON_RECH ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*42 , (rc.Height()/100)*9 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_COMBO_ADDRES ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*12+3 , (rc.Height()/100)*20 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 

			CComboBox*   pCtl  = (CComboBox*)pst; 
			CWnd *p_edit = pCtl->GetDlgItem (0x3E9);
			p_edit->GetClientRect( rect );
			p_edit->SetWindowPos( NULL, (rc.Width()/100)*25, (rc.Height()/100)*25-3, rect.Width(), rect.Height(), SWP_SHOWWINDOW );
		    pCtl->SetFont (&theApp.m_fontBlackbody);
			p_edit->SetFont (&theApp.m_fontBlackbody);

			pCtl->SendMessage(CB_SETITEMHEIGHT,(WPARAM)-1,(LPARAM)20);   //改变控件本身的高度
			pCtl->SetItemHeight(0,15);									 //改变下拉列表每个Item的高度
		}
		
		pst = GetDlgItem( IDC_STATIC_MONEY ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*28, (rc.Height()/100)*20+2 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW );
		}
		pst = GetDlgItem( IDC_BUTTON_SETADDR ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*42, (rc.Height()/100)*20 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW );
		}

		pst = GetDlgItem( IDC_EDIT_MONEY ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*14 , (rc.Height()/100)*42,  rect.Width() , rect.Height() + 5 , SWP_SHOWWINDOW ); 
			pst->SetFont (&theApp.m_fontBlackbody);
		}
		
		pst = GetDlgItem( IDC_STATIC_DW ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*38 , (rc.Height()/100)*43,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_BUTTON_MALE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*17 , (rc.Height()/100)*52+2,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_BUTTON_WOMAN ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*33 ,  (rc.Height()/100)*52+2,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_MFCLINK1 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*51 ,  (rc.Height()/100)*59,  30 ,20 , SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_CANCELORDE ) ;
		int ctrlwith =0;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,440 , 270 ,rect.Width() ,rect.Height(), SWP_SHOWWINDOW ) ; 
			ctrlwith=rect.Width();
		}

		pst = GetDlgItem( IDC_SETAUTOBET ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,440-ctrlwith, 270 ,rect.Width() ,rect.Height(), SWP_SHOWWINDOW ) ; 
		}

		//pst = GetDlgItem( IDC_CANCELAUTOBET ) ;
		//if ( NULL != pst ) {
		//	CRect rect ;
		//	pst->GetClientRect( rect ) ;
		//	pst->SetWindowPos( NULL ,440-ctrlwith, 270 ,rect.Width() ,rect.Height(), SWP_SHOWWINDOW ) ; 
		//}
	}
}


void CP2PDlg::OnCbnSelchangeComboAddres()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return;
	}
	m_addrbook.GetLBText(sel,text);

	//m_addrbook.GetWindowText(text) ;
	if(text!=_T(""))
	{
		string strCommand,strShowData =_T("");

		string strCond;
		strCommand = strprintf("%s %s %s","getappaccinfo" , theApp.m_betScritptid ,text);
		Json::Value root;
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("UpdateAddressData rpccmd listaddr error");
			return;
		}
		strShowData = root.toStyledString();

		int pos = strShowData.find("FreeValues");
		INT64 nMoney = 0;
		if (pos >0)
		{
			nMoney = root["FreeValues"].asInt64() ;
		}
		double money = (nMoney*1.0/COIN);
		strShowData = strprintf("%.8f",money);
		strShowData = strShowData.substr(0,strShowData.length()-4);

		((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->SetWindowText(strShowData.c_str());

		uistruct::LISTADDR_t pAddr;
		string condon;
		condon = strprintf("reg_id = '%s'",text);
		theApp.m_SqliteDeal.GetWalletAddressItem(condon,&pAddr);
		condon = strprintf("%.3f",pAddr.fMoney);
		((CStatic*)GetDlgItem(IDC_STATIC_MONEY))->SetWindowText(condon.c_str());

		Invalidate();
	}

	QueryNotDrawBalance(text);          /// blockchanged 刷新为开奖的数据
	ShowListItem(0);
	ShowListItem(1);
	ShowAllBetWinAndLoss();
	ShowAddressBetWinAndLoss(text);
}


BOOL CP2PDlg::AddListaddrDataBox(){

	map<int,uistruct::COMMONLISTADDR_t> m_mapCommonAddrInfo;
	string conditon =strprintf("app_id ='%s'",theApp.m_betScritptid);
	theApp.m_SqliteDeal.GetCommonWalletAddressList(conditon, &m_mapCommonAddrInfo);

	//map<string,uistruct::LISTADDR_t> m_mapAddrInfo;
	//theApp.m_SqliteDeal.GetWalletAddressList(_T(" sign=1 "), &m_mapAddrInfo);

	if ( 0 == m_mapCommonAddrInfo.size() ) return FALSE ;

	//清除ComBox控件
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->ResetContent();
	//加载到ComBox控件
	int nItem = 0;
	std::map<int,uistruct::COMMONLISTADDR_t>::const_iterator const_it;
	for ( const_it = m_mapCommonAddrInfo.begin() ; const_it != m_mapCommonAddrInfo.end() ; const_it++ ) {

		((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->InsertString(nItem , const_it->second.reg_id.c_str() );
		nItem++;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->SetCurSel(0);

	CString address;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return FALSE;
	}
	m_addrbook.GetLBText(sel,address);
	OnCbnSelchangeComboAddres();
	return TRUE ;
}
LRESULT CP2PDlg::OnShowListCtrol( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	int type = (int)wParam;
	switch(type)
	{
	case WM_UP_ADDRESS:
		{
			OnCbnSelchangeComboAddres();
		}
		break;
	default:
		break;

	}
	return 0 ;
}
LRESULT CP2PDlg::OnUpAddressCombo( WPARAM wParam, LPARAM lParam ) 
{
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->ResetContent(); //清除ComBox控件
	map<int,uistruct::COMMONLISTADDR_t> m_mapCommonAddrInfo;
	string conditon =strprintf("app_id ='%s'",theApp.m_betScritptid);
	theApp.m_SqliteDeal.GetCommonWalletAddressList(conditon, &m_mapCommonAddrInfo);

	if ( 0 == m_mapCommonAddrInfo.size() ) return  0;
	   
	int nItem = 0;  //加载到ComBox控件
	std::map<int,uistruct::COMMONLISTADDR_t>::const_iterator const_it;
	for ( const_it = m_mapCommonAddrInfo.begin() ; const_it != m_mapCommonAddrInfo.end() ; const_it++ ) {

	  ((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->InsertString(nItem , const_it->second.reg_id.c_str() );
	  nItem++;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->SetCurSel(0);
	return 0 ;
}
void  CP2PDlg::QueryNotDrawBalance(CString addr)
{
	if (addr == _T(""))
	{
		return;
	}
	string strCond;
	strCond = strprintf(" (state != 2) and (left_addr ='%s' or right_addr = '%s') ",addr,addr);

	//double   nmoney =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_p2p_quiz") , _T("amount") , strCond) ;
	//CString srtShow =_T("");
	//if (nmoney <0)
	//{
	//	srtShow = _T("0.0");
	//}else{
	//	srtShow.Format(_T("%.3lf"),nmoney);
	//}
	//((CStatic*)GetDlgItem(IDC_STATIC_NOT_DRAW))->SetWindowText(srtShow);

	uistruct::P2PBETRECORDLIST pPoolList;
	theApp.m_SqliteDeal.GetP2PQuizRecordList(strCond,&pPoolList);

	double money = 0.0;
	std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	for ( const_it = pPoolList.begin() ; const_it != pPoolList.end() ; const_it++ ) {
		if (theApp.IsSyncBlock)
		{
			if ((const_it->state ==0 &&(500 + const_it->height)< theApp.blocktipheight) || const_it->state ==3
				|| ((const_it->state ==1 || const_it->state ==4) &&(const_it->time_out + const_it->height)< theApp.blocktipheight))
			{
				continue;
			}
		}
		if (const_it->actor == 0)
		{
			money += const_it->amount;
		}else if (const_it->actor ==1)
		{
			money += const_it->accept_amount;
		}
		 
	}

	string srtShow ="";
	srtShow = strprintf("%.3lf",money);

	((CStatic*)GetDlgItem(IDC_STATIC_NOT_DRAW))->SetWindowText(srtShow.c_str());

	Invalidate();

}

void CP2PDlg::OnBnClickedButtonWithd()
{
	if (strcmp(theApp.m_betScritptid.c_str(),theApp.m_neststcriptid.strNewScriptBetid.c_str()) == 0)
	{
		GetAppAccountSomeMoney();
		return;
	}

	// TODO: 在此添加控件通知处理程序代码

	if (!theApp.IsSyncBlock )
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_UNFINISHED_BUSINESS" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	if (!CheckBalance(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_NOTWIHTDRAW" ,theApp.gsLanguage)))
	{
		return;
	}
	if ( IDNO == UiFun::MessageBoxEx( UiFun::UI_LoadString("P2P_MODULE" , "P2P_CONFIRM_WITHDRAWALS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_YESNO|MFB_TIP ) )
		return;

	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	string strShowData ="";

	
	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	CString Money = _T("");
	GetDlgItem(IDC_STATIC_BALANCE)->GetWindowText(Money);

	double dmoney = strtod(Money,NULL);

	string strContractData = m_P2PBetHelp.GetAppAccountMoneyContract(addr.GetString(),1,1,REAL_MONEY(dmoney));

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_TIP" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash =  root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜提现成功!\n%s") , root["hash"].asCString() ) ;
		strTip = UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_SUCCESS" ,theApp.gsLanguage) ;
	}else{
		strTip = UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_FAIL" ,theApp.gsLanguage) ;
	}
	
	UiFun::MessageBoxEx(  strTip.c_str(),UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage),MFB_OK|MFB_TIP );
}


void CP2PDlg::OnBnClickedButtonRech()
{
	// TODO: 在此添加控件通知处理程序代码

	string strshow= UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_UPDATA" ,theApp.gsLanguage) ;
	if (!UiFun::IsCurrentAppId(theApp.m_betScritptid.c_str(),theApp.m_neststcriptid.strNewScriptBetid.c_str(),strshow))
	{
		return;
	}
	if (!theApp.IsSyncBlock )
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_UNFINISHED_BUSINESS" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{

		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}

	CReCharge outdlg;
	if ( IDOK != outdlg.DoModal()){
		return;
	}

	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	if (theApp.m_strAddress == _T(""))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_MONEY_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}

	string strShowData ="" ;


	string strCondition;
	strCondition = strprintf("reg_id = '%s'",addr);
	uistruct::LISTADDR_t pAddr;
	theApp.m_SqliteDeal.GetWalletAddressItem(strCondition,&pAddr);
	double sub = pAddr.fMoney - strtod(theApp.m_strAddress,NULL);
	if (sub<0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_SET_RECHANGEMONEY" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}
	if (sub <1.0)
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_TRDING_TIPS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}


	string strContractData = m_P2PBetHelp.GetReChangContract();

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_TIP_INSU" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,strTxFee,REAL_MONEY(strtod(theApp.m_strAddress,NULL) ));
	CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash = root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜充值成功!\n%s") , root["hash"].asCString() ) ;
		strTip = UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_RECHARGE_SUCCESS" ,theApp.gsLanguage) ;
	}else{
		strTip = UiFun::UI_LoadString("P2P_MODULE" , "P2P_RECHARGE_FAIL" ,theApp.gsLanguage);
	}
	
	UiFun::MessageBoxEx(strTip.c_str(), UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
}

void CP2PDlg::SendBet(int rewardnum)
{
	// TODO: 在此添加控件通知处理程序代码  PacketP2PSendContract

	if (!theApp.IsSyncBlock )
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_UNFINISHED_BUSINESS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}

	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;


	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;


	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}

	//CReCharge outdlg(NULL,UiFun::UI_LoadString("P2P_MODULE" , "P2P_SET_ACCEPTAMOUNT" ,theApp.gsLanguage),_T(""),"",strTxMoney);
	//if (outdlg.DoModal() == IDCANCEL)
	//{
	//	return;
	//}
	CString strDisplay;
	if (rewardnum == 1)
	{
		strDisplay.Format(_T("%s:%s"), UiFun::UI_LoadString("P2P_MODULE" , "P2P_SENDSIS_MONEY" ,theApp.gsLanguage) ,strTxMoney);
	}else{
		strDisplay.Format(_T("%s:%s"), UiFun::UI_LoadString("P2P_MODULE" , "P2P_SENDBRO_MONEY" ,theApp.gsLanguage) ,strTxMoney);
	}
	
	CP2pTip tipsdlg(NULL,strDisplay,strTxMoney);
	if (tipsdlg.DoModal() == IDCANCEL)
	{
		return;
	}
	double money =strtod(theApp.m_strAddress,NULL);
	double  acceptmoney= strtod(strTxMoney,NULL);
	
    INT64 accept64=(INT64)REAL_MONEY(acceptmoney);
	INT64 send64=(INT64)REAL_MONEY(money);

	//if (accept64 <=0 || accept64 >=(INT64)(send64*1.1) )
	//{
	//	UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_SET_ACCEPTAMOUNTRANGE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
	//	return;
	//}
	//// 查询地址是否激活
	CString strCond;

	char strTemp[34];
	memset(strTemp , 0 , 34 );
	memcpy(strTemp , UiFun::Rnd32() , 32 );
	strTemp[32] =rewardnum ;


	string temp(strTemp,strTemp+33);
	int aa = temp.length() ;
	string strCommand;
	strCommand = strprintf("%s %s","gethash" , strTemp );
	string strShowData ;
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("UpdateAddressData rpccmd listaddr error");
		return;
	}

	string  strHash = root["hash"].asString() ;
	//strHash.Format(_T("%s") ,  root["hash"].asCString() ) ;

	TRACE(_T("contect:%s\r\n"),strTemp);
	TRACE(_T("sendhash:%s"),strHash.c_str());
	string strContractData;
	string strRamdHash = CSoyPayHelp::getInstance()->GetReverseHash(strHash);


	CString nTemp;
	nTemp.Format(_T("%.8f"),money);
	strContractData = m_P2PBetHelp.PacketP2PSendContract((INT64)REAL_MONEY(strtod(nTemp,NULL)),OUT_HEIGHT ,strRamdHash ,(INT64)REAL_MONEY(acceptmoney));

	INT64 strTxFee = theApp.m_P2PBetCfg.SendBetFee;
	if (  strTxFee < 10000  ) {
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_TIP_INSU" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}
	
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,strTxFee,0);
	strShowData = "";
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader; 
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash = root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
		strTip.Format(UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_SEND_SUCCESS" ,theApp.gsLanguage)) ;
	}else{
		strTip.Format( UiFun::UI_LoadString("P2P_MODULE" , "P2P_SEND_BET_FAIL" ,theApp.gsLanguage) ) ;
	}

	//保存到数据库
	if ( bRes ) {
		uistruct::P2P_QUIZ_RECORD_t p2pbetrecord ;
		memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_QUIZ_RECORD_t));
		SYSTEMTIME curTime ;
		memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
		GetLocalTime( &curTime ) ;
		string strSendTime;
		strSendTime= strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		p2pbetrecord.send_time = UiFun::SystemTimeToTimet(curTime);
		p2pbetrecord.time_out  = OUT_HEIGHT ;
		p2pbetrecord.tx_hash = root["hash"].asString();
		p2pbetrecord.left_addr = strprintf("%s",addr);
		p2pbetrecord.amount = money ;
		memcpy(p2pbetrecord.content ,strTemp , sizeof(p2pbetrecord.content));

		p2pbetrecord.actor  = 0 ;
		p2pbetrecord.state  = 0 ;
		//插入到数据库
		string strSourceData;
		strSourceData = strprintf("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'" , \
			strSendTime.c_str() , _T("") , p2pbetrecord.time_out , \
			p2pbetrecord.tx_hash.c_str() ,  p2pbetrecord.left_addr.c_str() , p2pbetrecord.right_addr.c_str() ,p2pbetrecord.amount);

		strSourceData += strprintf(",'%s' ,'%d','%d','%d','%d','%s','%d'",p2pbetrecord.content ,p2pbetrecord.actor ,p2pbetrecord.confirmed ,p2pbetrecord.height ,p2pbetrecord.state ,\
			p2pbetrecord.relate_hash.c_str() ,p2pbetrecord.guess_num ) ;
		strSourceData += strprintf(" ,'%d'",p2pbetrecord.deleteflag);
		strSourceData += strprintf(" ,'%lf'",acceptmoney);
		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData;
		pDatabase.strTabName =  _T("t_p2p_quiz");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();

		postmsg.SetData(strTemp);
		theApp.m_MsgQueue.push(postmsg);
	}
	
	UiFun::MessageBoxEx(strTip, UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
}

void CP2PDlg::OnBnClickedButtonMale()
{
	// TODO: 在此添加控件通知处理程序代码

	string strshow=UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_UPDATA" ,theApp.gsLanguage) ;
	if (!UiFun::IsCurrentAppId(theApp.m_betScritptid.c_str(),theApp.m_neststcriptid.strNewScriptBetid.c_str(),strshow))
	{
		return;
	}

	if (!CheckBalance())
	{
		return;
	}

	CString strMoney;
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	double balance =strtod(strMoney,NULL);

	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_MONEY_NOT_NULL" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}

	if (strtod(strTxMoney,NULL) > balance)
	{
	
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_MONEY_MORE_BALANCE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}

	if (strtod(strTxMoney,NULL)<100)
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_MONEY_MORE_100" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}

	//CString strDisplay;
	//strDisplay.Format(_T("%s:%s"), UiFun::UI_LoadString("P2P_MODULE" , "P2P_SENDSIS_MONEY" ,theApp.gsLanguage) ,strTxMoney);
	//if (IDCANCEL == UiFun::MessageBoxEx(strDisplay , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  , MFB_OKCANCEL|MFB_TIP ) )
	//{
	//	return;
	//}
	SendBet(1);
	OnBnClickedButtonRefresh2();
}


void CP2PDlg::OnBnClickedButtonWoman()
{
	// TODO: 在此添加控件通知处理程序代码
	string strshow= UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_UPDATA" ,theApp.gsLanguage) ;
	if (!UiFun::IsCurrentAppId(theApp.m_betScritptid.c_str(),theApp.m_neststcriptid.strNewScriptBetid.c_str(),strshow))
	{
		return;
	}

	if (!CheckBalance())
	{
		return;
	}

	CString strMoney;
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	double balance =strtod(strMoney,NULL);

	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_MONEY_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}

	if (strtod(strTxMoney,NULL) > balance)
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_MONEY_MORE_BALANCE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}

	if (strtod(strTxMoney,NULL)<100)
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_MONEY_MORE_100" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}

	//CString strDisplay;
	////strDisplay.Format(UiFun::UI_LoadString("P2P_MODULE" , "P2P_SENDBRO_MONEY" ,theApp.gsLanguage), strTxMoney);
	//strDisplay.Format(_T("%s:%s"), UiFun::UI_LoadString("P2P_MODULE" , "P2P_SENDBRO_MONEY" ,theApp.gsLanguage) ,strTxMoney);
	//if (IDCANCEL == UiFun::MessageBoxEx(strDisplay ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  , MFB_OKCANCEL|MFB_TIP ) )
	//{
	//	return;
	//}
	SendBet(2);
	OnBnClickedButtonRefresh2();
}
void CP2PDlg::OnListPool()
{
	//m_PoolList.clear();
	m_curpage = 0;
	//theApp.m_SqliteDeal.GetP2PQuizPoolList(_T(" 1=1 order by total_amount desc"), &m_PoolList);
	m_pagecount = (m_PoolList.size()%m_pagesize)==0?(m_PoolList.size()/m_pagesize):(m_PoolList.size()/m_pagesize)+1;
	
	string temp;
	temp =strprintf("%s:%d",UiFun::UI_LoadString("COMM_MODULE" , "COMM_TOTAL" ,theApp.gsLanguage) ,m_pagecount);
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	Invalidate();
	m_BonusListBox.DeleteAllIndex();
	OnShowPagePool(1);
}
 LRESULT CP2PDlg::onBnCLick( WPARAM wParam, LPARAM lParam )
 {
	PoolBox_AppendData* pinf = m_BonusListBox.GetAppendDataInfo((int)wParam);
	if ( NULL != pinf ) { 
		CString hash = pinf->pstr;
		int index = ((m_curpage-1)*m_pagesize)+pinf->nItem;
		if (index > (int)m_PoolList.size())
		{
			UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_THIS_ALONE_NOT" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
			return 0;
		}
		uistruct::LISTP2POOL_T item = m_PoolList.at(index);
		//string conditon;
		//conditon = strprintf("hash='%s'",hash);
		//uistruct::LISTP2POOL_T pPoolList;
		//theApp.m_SqliteDeal.GetP2PQuizPoolItem(conditon, &pPoolList);
		//if (pPoolList.hash.length() != 0)
		{
			CString money,adddr;
			money = pinf->pstr1;
			//pinf->pSta2->GetWindowText(money);
			pinf->pSta1->GetWindowText(adddr);
			AcceptBet(item.hash.c_str(),item.nAcceptMoney,adddr,item.outheight,item.nPayMoney);
			OnBnClickedButtonRefresh2();
		}
		
	}
	return 0;

}
void CP2PDlg::AcceptBet(CString hash,INT64 money,CString sendaddr,int timeout,INT64 sendmoney)
 {
	 string strshow=UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_UPDATA" ,theApp.gsLanguage) ;
	 if (!UiFun::IsCurrentAppId(theApp.m_betScritptid.c_str(),theApp.m_neststcriptid.strNewScriptBetid.c_str(),strshow))
	 {
		 return;
	 }

	 if (!theApp.IsSyncBlock )
	 {
		 
		 UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_UNFINISHED_BUSINESS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		 return;
	 }
	 if (!CheckBalance())
	 {
		 return;
	 }
	 if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	 CString strTxMoney;
	 GetDlgItem(IDC_STATIC_BALANCE)->GetWindowText(strTxMoney) ;

	 double dmoney = (money*1.0)/COIN;
	 if (strtod(strTxMoney,NULL) < dmoney)
	 {
		 UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ORDERS_MORE_BALANCE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		 return ;
	 }

	 CString addr;
	 int sel = m_addrbook.GetCurSel();

	 if (sel < 0)
	 {
		 UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		 return ;
	 }
	 m_addrbook.GetLBText(sel,addr);

	 if (addr == _T(""))
	 {
		 
		 UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		 return;
	 }


	 CGuessNum guessdlg;
	 if (IDOK != guessdlg.DoModal())
	 {
		 return;
	 }
	 char guess = atoi(theApp.m_strAddress);

	 string strContractData,strHash;
	 strHash= CSoyPayHelp::getInstance()->GetReverseHash(hash.GetString());
	 strContractData = m_P2PBetHelp.PacketP2PAcceptContract(money ,strHash,guess);

	 if (  theApp.m_P2PBetCfg.AcceptBetnFee < 10000  ) {
		 return ;
	 }

	 string strShowData ="";
	 string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,theApp.m_P2PBetCfg.AcceptBetnFee,0);
	 CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	 if (strShowData == "")
	 {
		 return;
	 }
	 Json::Reader reader;  
	 Json::Value root; 
	 if (!reader.parse(strShowData, root)) 
		 return  ;
	 BOOL bRes = FALSE ;
	 string strTip;
	 int pos = strShowData.find("hash");

	 if ( pos >=0 ) {
		 //插入到交易记录数据库
		 string strHash= root["hash"].asString() ;
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	 }
	 if ( pos >=0 ) {
		 bRes = TRUE ;
		 //strTip.Format( _T("恭喜接赌成功!\n%s") , root["hash"].asCString() ) ;
		 strTip=UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_SEND_SUCCESS" ,theApp.gsLanguage);
	 }else{
		 strTip= UiFun::UI_LoadString("P2P_MODULE" , "P2P_THIS_ALONE_OK" ,theApp.gsLanguage) ;
	 }

	 //保存到数据库
	 if ( bRes ) {

		 //插入到交易记录数据库

		 //// 查找数据库中是否存在此记录
		 string conditon;
		 conditon = strprintf("tx_hash ='%s'", hash );
		 uistruct::P2P_QUIZ_RECORD_t pPoolItem;
		 int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
		 if (pPoolItem.tx_hash == "") ///此记录不存在,插入记录
		 {
			 uistruct::P2P_QUIZ_RECORD_t p2pbetrecord ;
			 memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_QUIZ_RECORD_t));

			 strData = strprintf("%s %s",_T("gettxdetail"),hash);
			 string strShowData ="";
			 if(!CSoyPayHelp::getInstance()->SendRpc(strData,root))
			 {
				 TRACE("AcceptBet rpccmd gettxdetail error");
				 return;
			 }
			 strShowData = root.toStyledString();
			 int pos = strShowData.find("confirmedtime");
			 if (strShowData == "" && pos <0)
			 {
				 return;
			 }

			 int confirtime =root["confirmedtime"].asInt();

			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(confirtime);
			 string strSendTime;
			 strSendTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			 p2pbetrecord.send_time = UiFun::SystemTimeToTimet(curTime);

			p2pbetrecord.tx_hash = strprintf("%s",hash );

			 p2pbetrecord.relate_hash=root["hash"].asString();
		

			 p2pbetrecord.right_addr = strprintf("%s",addr);
			 p2pbetrecord.left_addr = strprintf("%s",sendaddr);
			 p2pbetrecord.amount = (sendmoney*1.0)/COIN;
			 p2pbetrecord.accept_amount =(money*1.0)/COIN;

			 p2pbetrecord.state = 4;
			 p2pbetrecord.actor  = 1 ;
			 p2pbetrecord.guess_num = (int)guess ;
			 //插入到数据库
			 string strSourceData ;
			 strSourceData = strprintf("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'" , \
				 strSendTime.c_str() , _T("") , timeout , \
				 p2pbetrecord.tx_hash ,  p2pbetrecord.left_addr , p2pbetrecord.right_addr ,p2pbetrecord.amount);

			 strSourceData+=strprintf(",'%s' ,'%d','%d','%d','%d','%s','%d'",p2pbetrecord.content ,p2pbetrecord.actor ,p2pbetrecord.confirmed ,p2pbetrecord.height ,p2pbetrecord.state ,\
				 p2pbetrecord.relate_hash ,p2pbetrecord.guess_num ) ;

			 strSourceData += strprintf(" ,'%d','%lf'",p2pbetrecord.deleteflag, p2pbetrecord.accept_amount);

			 uistruct::DATABASEINFO_t   pDatabase;
			 pDatabase.strSource = strSourceData;
			 pDatabase.strTabName =  _T("t_p2p_quiz");
			 CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
			 string strTemp = pDatabase.ToJson();

			 postmsg.SetData(strTemp);
			 theApp.m_MsgQueue.push(postmsg);

		 }else{        ///更新记录
			 string txhash = root["hash"].asString();
			 string strSourceData  , strW ;
			 strSourceData = strprintf("actor = %d , relate_hash = '%s' ,right_addr ='%s',", 2 , txhash ,addr ) ;
			 strSourceData += strprintf(" guess_num = %d,state = %d", (int)guess,4);
			 strW = strprintf("tx_hash = '%s'" , hash ) ;

			 uistruct::DATABASEINFO_t DatabaseInfo;
			 DatabaseInfo.strSource = strSourceData;
			 DatabaseInfo.strWhere = strW ;
			 DatabaseInfo.strTabName = _T("t_p2p_quiz");
			 CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
			 string strTemp = DatabaseInfo.ToJson();

			 postmsg.SetData(strTemp);
			 theApp.m_MsgQueue.push(postmsg);

		 }
	 }
	
	 UiFun::MessageBoxEx(strTip.c_str() , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
 }
 bool CP2PDlg::CheckBalance(CString strshow)
 {
	 OnCbnSelchangeComboAddres();
	 CString strMoney;
	 ((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	 double money =strtod(strMoney,NULL);
	if (money == 0.0)
	{
		if (strshow == "")
		{
			
			UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ACCOUNT_RECHARGE" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		}else{
			UiFun::MessageBoxEx(strshow , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		}
		
		return false;
	}
	return true;
 }
 void CP2PDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 // TODO: 在此添加控件通知处理程序代码
	 OnSelectShowWin(m_tab.GetCurSel());
	 *pResult = 0;
 }
 void    CP2PDlg::ShowListItem(int seltab)
 {
	 CString addr;
	 int sel = m_addrbook.GetCurSel();
	 if(sel == -1)
		 return;
	 m_addrbook.GetLBText(sel,addr);

	 if (addr == _T(""))
	 {
		 return;
	 }
	 if (seltab == 0)
	 {
		 m_BetRecord.Showlistbox(addr);
	 }else if (seltab == 1)
	 {
		 m_SendRecord.Showlistbox(addr);
	 }

 }

 void  CP2PDlg::ShowAllBetWinAndLoss(){
	 uistruct::P2PBETRECORDLIST       m_P2pBetTxList;
	 theApp.m_SqliteDeal.GetP2PQuizRecordList(_T("1=1") ,&m_P2pBetTxList ) ;

	 double result = ComPuteBetWinAandLoser(m_P2pBetTxList);
	 string show;
	 show =strprintf("%s:%.4f",UiFun::UI_LoadString("P2P_MODULE" , "P2P_TOTAL_PROFIT_LOSS" ,theApp.gsLanguage),result);
	 m_rBtnWinerloser.SetWindowText(show.c_str());
	 m_rBtnWinerloser.Invalidate();
 }
 void   CP2PDlg::ShowAddressBetWinAndLoss(CString addr)
 {
	 uistruct::P2PBETRECORDLIST       m_P2pBetTxList;
	 string condtion;
	 condtion = strprintf("left_addr = '%s' or right_addr = '%s'",addr,addr);
	 theApp.m_SqliteDeal.GetP2PQuizRecordList(condtion ,&m_P2pBetTxList ) ;

	 double result = ComPuteAddrBetWinAandLoser(m_P2pBetTxList,addr);
	 string show;
	 show = strprintf("%s:%.4f",UiFun::UI_LoadString("P2P_MODULE" , "P2P_TOTAL_PROFIT" ,theApp.gsLanguage),result);
	 m_rBtnAddrWinerloser.SetWindowText(show.c_str());
	 m_rBtnAddrWinerloser.Invalidate();
 }
 double CP2PDlg::ComPuteBetWinAandLoser(uistruct::P2PBETRECORDLIST  m_P2pBetTxList)
 {

	 double winer = 0.0;
	 double loser = 0.0;

	 std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	 for (const_it = m_P2pBetTxList.begin() ; const_it != m_P2pBetTxList.end() ; const_it++ ) {
		if (const_it->actor == 0){         //// 发赌约身份者
			if (const_it->state == 2){       
				if (const_it->guess_num == const_it->content[32])
				{
					loser += const_it->amount;
				}else
				{
					winer += const_it->accept_amount;
				}
			}else if (const_it->state == 1){
				if(const_it->height != 0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight){
					loser += const_it->amount;
				}
			}
		}else if (const_it->actor == 1){ /// 接赌身份者
			if (const_it->state == 2)
			{
				int rewardnum = (int)const_it->content[32];
				if (const_it->guess_num == const_it->content[32])
				{
					winer +=const_it->amount; //const_it->amount;
				}else
				{
					loser += const_it->accept_amount;;//const_it->amount;
				}

			}else{
				if (const_it->height>0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight)
				{
					winer += const_it->amount;
				}
			}
		}	 
	 }
	double result = winer-loser;
	return result;
 }
 double CP2PDlg::ComPuteAddrBetWinAandLoser(uistruct::P2PBETRECORDLIST  m_P2pBetTxList,CString addr)
 {
	 double winer = 0.0;
	 double loser = 0.0;

	 std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	 for (const_it = m_P2pBetTxList.begin() ; const_it != m_P2pBetTxList.end() ; const_it++ ) {
		 if (const_it->actor == 0){         //// 发赌约身份者
			 if (const_it->state == 2){       
				 if (const_it->guess_num == const_it->content[32])
				 {
					 loser += const_it->amount;
				 }else
				 {
					 winer += const_it->accept_amount;
				 }
			 }else if (const_it->state == 1){
				 if(const_it->height != 0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight){
					 loser += const_it->amount;
				 }
			 }
		 }else if (const_it->actor == 1){ /// 接赌身份者
			 if (const_it->state == 2)
			 {
				 int rewardnum = (int)const_it->content[32];
				 if (const_it->guess_num == const_it->content[32])
				 {
					 winer +=const_it->amount;// const_it->amount;
				 }else
				 {
					 loser += const_it->accept_amount;//const_it->amount;
				 }

			 }else{
				 if (const_it->height>0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight)
				 {
					 winer += const_it->amount;
				 }
			 }
		 }else if(strcmp(const_it->left_addr.c_str(),const_it->right_addr.c_str()) != 0){
			 if (strcmp(const_it->left_addr.c_str(),addr) == 0)  /// 这个地址发赌约
			 {
				 if (const_it->state == 2){       
					 if (const_it->guess_num == const_it->content[32])
					 {
						 loser += const_it->amount;
					 }else
					 {
						 winer += const_it->accept_amount;
					 }
				 }else if (const_it->state == 1){/// 超时
					 if(const_it->height != 0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight){
						 loser += const_it->amount;
					 }
				 }
			 }else if (strcmp(const_it->right_addr.c_str(),addr) == 0)
			 {
				 if (const_it->state == 2)
				 {
					 int rewardnum = (int)const_it->content[32];
					 if (const_it->guess_num == const_it->content[32])
					 {
						 winer +=const_it->amount;// const_it->amount;
					 }else
					 {
						 loser += const_it->accept_amount;//const_it->amount;
					 }

				 }else{  /// 超时
					 if (const_it->height>0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight)
					 {
						 winer += const_it->amount;
					 }
				 }
			 }
		 }
	 }
		 
	 double result = winer-loser;
	 return result;
 }

 void CP2PDlg::OnBnClickedButtonRefresh2()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 ShowListItem(m_seltab);
	 ShowAllBetWinAndLoss();
 }


 void CP2PDlg::OnBnClickedButtonRefresh1()
 {
	 // TODO: 在此添加控件通知处理程序代码
	// OnListPool();
	 ReadP2pPoolFromDB();
 }


 void CP2PDlg::OnNMThemeChangedListBonus(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 // 该功能要求使用 Windows XP 或更高版本。
	 // 符号 _WIN32_WINNT 必须 >= 0x0501。
	 // TODO: 在此添加控件通知处理程序代码
	 *pResult = 0;
 }
 void  CP2PDlg::OnShowPagePool(int page)
 {
	 if (page >m_pagecount || page == m_curpage || page <= 0)
	 {
		 return;
	 }


	 m_BonusListBox.DeleteAllIndex();
	 string strpage;
	 strpage = strprintf("%d",page);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());
	 m_curpage = page;
	 int index = (page-1)*m_pagesize;
	 unsigned int count = (m_PoolList.size() -index)>=m_pagesize?m_pagesize:(m_PoolList.size() -index);
	 int i =0;

	 string strmoney;
	 string addr,money,acceptmoney;
	 string txhash, line;

	 string temp = "";

	 char buffer[1024] = {0};
	 for (unsigned int k = index;k< (index+count)&& k<m_PoolList.size();k++)
	 {
		 uistruct::LISTP2POOL_T const_it = m_PoolList.at(k);

		 double dmoney = (const_it.nAcceptMoney*1.0)/COIN;
		 money =strprintf("%.4f",dmoney);
	
		 strmoney =strprintf("%.8f",dmoney);

		 int returnward= (const_it.nPayMoney*1.0/const_it.nAcceptMoney)*100;
		 acceptmoney =strprintf("%d",returnward)+"%";
		 temp = const_it.hash.substr(0,6);
		 line = temp;

		 m_BonusListBox.InsertStr(i,this->GetSafeHwnd());
		 m_BonusListBox.SetIndexInage(i , IDB_BITMAP_P2P_LISTBOX_BUT);
		 m_BonusListBox.SetIndexString(i , line.c_str(),const_it.sendbetid.c_str(), UiFun::UI_LoadString("COMM_MODULE" , "COMM_RECIVED" ,theApp.gsLanguage), money.c_str(), acceptmoney.c_str(),strmoney.c_str());
		 i++;
	 }
 }

 BOOL CP2PDlg::PreTranslateMessage(MSG* pMsg)
 {
	 // TODO: 在此添加专用代码和/或调用基类
	 if(pMsg->message == WM_KEYDOWN)  
	 {   
		 if(pMsg->wParam == VK_RETURN)  
		 {  
			 if (GetDlgItem(IDC_EDIT_PAGE) == this->GetFocus())
			 {
				 CString num;
				 GetDlgItem(IDC_EDIT_PAGE)->GetWindowText(num);
				 if (IsAllDigtal(num))
				 {
					 OnShowPagePool(atoi(num));
				 }else
				 {
					  GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
					
					UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_INPUT_ERROR" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
				 }
				 return TRUE;
			 }
		 }  
	 }  
	 return CDialogBar::PreTranslateMessage(pMsg);
 }


 void CP2PDlg::OnBnClickedButtonUp()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 OnShowPagePool((m_curpage-1));
 }


 void CP2PDlg::OnBnClickedButtonNext()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 OnShowPagePool((m_curpage+1));
 }


 void CP2PDlg::OnTimer(UINT_PTR nIDEvent)
 {
	 // TODO: 在此添加消息处理程序代码和/或调用默认值
	 switch(nIDEvent)
	 {
	 case 1:
		 OnBnClickedButtonRefresh2();
		 OnBnClickedButtonRefresh1();
		AutoSendClinetBet();
		// AutoSendBet();
		 break;
	 default:
		 break;

	 }
	 CDialogBar::OnTimer(nIDEvent);
 }
 void  CP2PDlg::onShowLink()
 {
	 v_linkCtrl->SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_HELP" ,theApp.gsLanguage));
	 if (theApp.gsLanguage ==2)
	 {
		  v_linkCtrl->SetURL(theApp.helpurlen.c_str());
	 }else{
		 v_linkCtrl->SetURL(theApp.helpurlcn.c_str());
	 }
	
 }

 void CP2PDlg::OnLbnDblclkListBonus()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 int nSel=m_BonusListBox.GetCurSel(); 
	 int count = ((m_curpage -1)*m_pagesize) + nSel;
	 if (count <=(int)m_PoolList.size())
	 {
		 uistruct::LISTP2POOL_T const_it = m_PoolList.at(count);
		 string temp = UiFun::UI_LoadString("P2P_MODULE" , "P2P_QUIZ_TRAD_ID" ,theApp.gsLanguage)+":";
	    temp +=const_it.hash ;
		 UiFun::MessageBoxEx(temp.c_str() , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
	 }
 }
bool CP2PDlg::CheckBalance(double dmoney)
{
	OnCbnSelchangeComboAddres();
	CString strMoney;
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	double money =strtod(strMoney,NULL);
	if (money == 0.0)
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ACCOUNT_RECHARGE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return false;
	}

	if (dmoney >money)
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_MORE_BALANCE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return false;
	}
	return true;
}
void  CP2PDlg::GetAppAccountSomeMoney()
{

	if (!theApp.IsSyncBlock )
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_UNFINISHED_BUSINESS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}


	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;
	
	if (!CheckBalance(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_NOTWIHTDRAW" ,theApp.gsLanguage)))
	{
		return;
	}
	CString balance ="";
	
	GetDlgItem(IDC_STATIC_BALANCE)->GetWindowText(balance);
	CReCharge outdlg(NULL,UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),"",balance);
	if ( IDOK != outdlg.DoModal()){
		return;
	}

	if (theApp.m_strAddress == _T(""))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_MONEY_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}
	
	double money = strtod(theApp.m_strAddress,NULL);
	if (money <0.0000000000000001)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}
	if (!CheckBalance(money))
	{
		return;
	}
		string show = strprintf("%s:%lf",UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_MONEY_MM" ,theApp.gsLanguage),money);
	if ( IDNO == UiFun::MessageBoxEx( show.c_str() , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  , MFB_YESNO|MFB_TIP ) )
		return;

	string strShowData ="";


	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
	
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}


	string strContractData = m_P2PBetHelp.GetAppAccountSomeMoneyContract(addr.GetString(),3,1,REAL_MONEY(money));

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_TIP_INSU" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}


	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash =  root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜提现成功!\n%s") , root["hash"].asCString() ) ;
		strTip = UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_SUCCESS" ,theApp.gsLanguage) ;
	}else{
		strTip = UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_FAIL" ,theApp.gsLanguage) ;
	}
	
	UiFun::MessageBoxEx(strTip.c_str() , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
}
void  CP2PDlg::AutoSendBet()
{
	//if (m_PoolList.size() != 0)
	//{
	//	return;
	//}
	static int hght = 0 ;
	static int lastrandom = 0 ;

	if(hght==theApp.blocktipheight)
	{
		return ;
	}

	hght = theApp.blocktipheight;

	uistruct::P2PLIST PoolList;
	ReadP2pPoolFromCmd(PoolList);
	BOOL bsend1 = TRUE;
	BOOL bsend2 = TRUE;
	BOOL bsend3 = TRUE;
	vector<uistruct::LISTP2POOL_T>::const_iterator const_it = PoolList.begin();
	for(;const_it != PoolList.end();const_it++)
	{
		double dmoney = (const_it->nPayMoney*1.0)/COIN;
		if (dmoney >= 10000.0)
		{
			bsend1 = FALSE;
		}

		if (dmoney > 1000.0 && dmoney <= 3000.0)
		{
			bsend2 = FALSE;
		}

		if (dmoney >= 300.0 && dmoney <= 1000.0)
		{
			bsend3 = FALSE;
		}
	}

	//for(;const_it != PoolList.end();const_it++)
	//{
	//	double dmoney = (const_it->nPayMoney*1.0)/COIN;
	//	if (dmoney >= 300.0)
	//	{
	//		bsend1 = FALSE;
	//	}

	//	if (dmoney >= 1.0 && dmoney <= 100.0)
	//	{
	//		bsend2 = FALSE;
	//	}

	//	if (dmoney > 100.0 && dmoney < 300.0)
	//	{
	//		bsend3 = FALSE;
	//	}
	//}

	CTime t1( 1980, 3, 19, 22, 15, 0 );

	CTime t = CTime::GetCurrentTime();

	CTimeSpan span=t-t1; //计算当前系统时间与时间t1的间隔

	srand(span.GetTotalSeconds()+hght+lastrandom);

     lastrandom = rand();
	// 	SYSTEMTIME curTime ;
	// 	memset( &curTime , 0 , sizeof(curTime) ) ;
	// 	GetLocalTime( &curTime ) ;
	// 
	// 	srand(curTime.wMilliseconds);

	CString addraray[]={"141675-1","141675-2","141686-2"};
	
	CString MaxMongy[3]={"25000","18000","10000"};
	CString Max2Mongy[3]={"2400","1800","1100"};
	CString Max3Mongy[3]={"888","600","500"};


	//CString MaxMongy[3]={"310","330","350"};
	//CString Max2Mongy[3]={"110","160","180"};
	//CString Max3Mongy[3]={"20","40","50"};

	for (int i =0;i<3;i++)
	{
		CString addr =addraray[(rand()%3)];
		CString strTxMoney;
		if (i == 0)
		{
			if (!bsend1)
			{
				continue;
			}
			strTxMoney = MaxMongy[(rand()%3)];
		}
		if (i == 1)
		{
			if (!bsend2)
			{
				continue;
			}
			strTxMoney =  Max2Mongy[(rand()%3)];
		}
		if (i == 2)
		{
			if (!bsend3)
			{
				continue;
			}
			strTxMoney =  Max3Mongy[(rand()%3)];
		}
		CString strMoney;
		((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
		double balance =strtod(strMoney,NULL);

		if (strtod(strTxMoney,NULL) > balance)
		{
			//::MessageBox( this->GetSafeHwnd() ,_T("投注金额大于账户余额") , _T("提示") , MB_ICONINFORMATION ) ;
			continue ;
		}
		
		int rewardnum =(rand()%2+1);
		//// 查询地址是否激活
		CString strCond;

		char strTemp[34];
		memset(strTemp , 0 , 34 );
		memcpy(strTemp , UiFun::Rnd32() , 32 );
		strTemp[32] =rewardnum ;


		string temp(strTemp,strTemp+33);
		int aa = temp.length() ;
		string strCommand;
		strCommand = strprintf("%s %s","gethash" , strTemp );
		string strShowData ;

		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		int pos = strShowData.find("hash");
		if ( pos < 0 ) return ;

		Json::Reader reader;  
		Json::Value root; 
		if (!reader.parse(strShowData, root)) 
			return  ;

		string  strHash = root["hash"].asString() ;

		string strContractData;
		string strRamdHash = CSoyPayHelp::getInstance()->GetReverseHash(strHash);

		double money = strtod(strTxMoney,NULL);
		CString nTemp;
		nTemp.Format(_T("%.8f"),money);
		INT64 acceptmoney = (INT64)REAL_MONEY(strtod(nTemp,NULL))*1.05-1;
		strContractData = m_P2PBetHelp.PacketP2PSendContract((INT64)REAL_MONEY(strtod(nTemp,NULL)),OUT_HEIGHT ,strRamdHash ,acceptmoney);

		INT64 strTxFee = theApp.m_P2PBetCfg.SendBetFee;
		if (  strTxFee < 10000  ) {
			//::MessageBox( this->GetSafeHwnd() ,UiFun::UI_LoadString("P2P_MODULE" , "P2P_TIP_INSU" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MB_ICONINFORMATION ) ;
			UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_TIP_INSU" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
			return ;
		}

		string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,strTxFee,0);
		strShowData = "";
		CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);
		if (strShowData == "")
		{
			return;
		}
		if (!reader.parse(strShowData, root)) 
			return  ;
		BOOL bRes = FALSE ;
		CString strTip;
		pos = strShowData.find("hash");

		if ( pos >=0 ) {
			//插入到交易记录数据库
			string strHash = root["hash"].asString();
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
			postmsg.SetData(strHash);
			theApp.m_MsgQueue.push(postmsg);
		}

		if ( pos >=0 ) {
			bRes = TRUE ;
			//strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
			strTip.Format( UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_SEND_SUCCESS" ,theApp.gsLanguage)) ;
		}else{
			strTip.Format( UiFun::UI_LoadString("P2P_MODULE" , "P2P_SEND_BET_FAIL" ,theApp.gsLanguage) ) ;
		}

		//保存到数据库
		if ( bRes ) {
			uistruct::P2P_QUIZ_RECORD_t p2pbetrecord ;
			memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_QUIZ_RECORD_t));
			SYSTEMTIME curTime ;
			memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			GetLocalTime( &curTime ) ;
			string strSendTime;
			strSendTime= strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			p2pbetrecord.send_time = UiFun::SystemTimeToTimet(curTime);
			p2pbetrecord.time_out  = OUT_HEIGHT ;
			p2pbetrecord.tx_hash = root["hash"].asString();
			p2pbetrecord.left_addr = strprintf("%s",addr);
			p2pbetrecord.amount = strtod(strTxMoney,NULL) ;
			memcpy(p2pbetrecord.content ,strTemp , sizeof(p2pbetrecord.content));

			p2pbetrecord.actor  = 0 ;
			p2pbetrecord.state  = 0 ;
			//插入到数据库
			string strSourceData;
			strSourceData = strprintf("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'" , \
				strSendTime.c_str() , _T("") , p2pbetrecord.time_out , \
				p2pbetrecord.tx_hash.c_str() ,  p2pbetrecord.left_addr.c_str() , p2pbetrecord.right_addr.c_str() ,p2pbetrecord.amount);

			strSourceData += strprintf(",'%s' ,'%d','%d','%d','%d','%s','%d'",p2pbetrecord.content ,p2pbetrecord.actor ,p2pbetrecord.confirmed ,p2pbetrecord.height ,p2pbetrecord.state ,\
				p2pbetrecord.relate_hash.c_str() ,p2pbetrecord.guess_num ) ;

			strSourceData += strprintf(" ,'%d'",p2pbetrecord.deleteflag);
			strSourceData += strprintf(" ,'%lf'",(acceptmoney*1.0)/COIN);
			uistruct::DATABASEINFO_t   pDatabase;
			pDatabase.strSource = strSourceData;
			pDatabase.strTabName =  _T("t_p2p_quiz");
			CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
			string strTemp = pDatabase.ToJson();

			postmsg.SetData(strTemp);
			theApp.m_MsgQueue.push(postmsg);
		}
	}

}

BOOL CP2PDlg::AcceptBet(string hash,double dmoney,string sendaddr,int timeout,string addr)
{
	string strshow=UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_UPDATA" ,theApp.gsLanguage) ;;
	if (!UiFun::IsCurrentAppId(theApp.m_betScritptid.c_str(),theApp.m_neststcriptid.strNewScriptBetid.c_str(),strshow))
	{
		return FALSE;
	}

	if (!theApp.IsSyncBlock )
	{
		
		UiFun::MessageBoxEx( UiFun::UI_LoadString("P2P_MODULE" , "P2P_UNFINISHED_BUSINESS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return FALSE;
	}

	if (!CheckRegIDValid( theApp.m_betScritptid )) return FALSE;

	char guess = (rand()%2+1);;

	string strContractData,strHash;
	strHash= CSoyPayHelp::getInstance()->GetReverseHash(hash);
	strContractData = m_P2PBetHelp.PacketP2PAcceptContract((INT64)REAL_MONEY(dmoney) ,strHash,guess);

	if (  theApp.m_P2PBetCfg.AcceptBetnFee < 10000  ) {
		return FALSE;
	}

	string strShowData ="";
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr,strContractData,0,theApp.m_P2PBetCfg.AcceptBetnFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	if (strShowData == "")
	{
		return FALSE;
	}

	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData, root)) 
		return  FALSE;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash= root["hash"].asString() ;
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}
	if ( pos >=0 ) {
		bRes = TRUE ;
	}

	//保存到数据库
	if ( bRes ) {

		//插入到交易记录数据库

		//// 查找数据库中是否存在此记录
		string conditon;
		conditon = strprintf("tx_hash ='%s'", hash );
		uistruct::P2P_QUIZ_RECORD_t pPoolItem;
		int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
		if (pPoolItem.tx_hash == "") ///此记录不存在,插入记录
		{
			uistruct::P2P_QUIZ_RECORD_t p2pbetrecord ;
			memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_QUIZ_RECORD_t));

			strData = strprintf("%s %s",_T("gettxdetail"),hash);
			string strShowData ="";
			if(!CSoyPayHelp::getInstance()->SendRpc(strData,root))
			{
				TRACE("UpdateAddressData rpccmd listaddr error");
				return false;
			}
			strShowData = root.toStyledString();
			int pos = strShowData.find("confirmedtime");
			if (strShowData == "" && pos <0)
			{
				return FALSE;
			}

			int confirtime =root["confirmedtime"].asInt();

			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(confirtime);
			string strSendTime;
			strSendTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			p2pbetrecord.send_time = UiFun::SystemTimeToTimet(curTime);

			p2pbetrecord.tx_hash = strprintf("%s",hash );

			p2pbetrecord.relate_hash=root["hash"].asString();


			p2pbetrecord.right_addr = strprintf("%s",addr);
			p2pbetrecord.left_addr = strprintf("%s",sendaddr);
			p2pbetrecord.amount = dmoney;

			p2pbetrecord.state = 4;
			p2pbetrecord.actor  = 1 ;
			p2pbetrecord.guess_num = (int)guess ;
			//插入到数据库
			string strSourceData ;
			strSourceData = strprintf("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'" , \
				strSendTime.c_str() , _T("") , timeout , \
				p2pbetrecord.tx_hash ,  p2pbetrecord.left_addr , p2pbetrecord.right_addr ,p2pbetrecord.amount);

			strSourceData+=strprintf(",'%s' ,'%d','%d','%d','%d','%s','%d'",p2pbetrecord.content ,p2pbetrecord.actor ,p2pbetrecord.confirmed ,p2pbetrecord.height ,p2pbetrecord.state ,\
				p2pbetrecord.relate_hash ,p2pbetrecord.guess_num ) ;

			strSourceData += strprintf(" ,'%d'",p2pbetrecord.deleteflag);

			uistruct::DATABASEINFO_t   pDatabase;
			pDatabase.strSource = strSourceData;
			pDatabase.strTabName =  _T("t_p2p_quiz");
			CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
			string strTemp = pDatabase.ToJson();

			postmsg.SetData(strTemp);
			theApp.m_MsgQueue.push(postmsg);

		}else{        ///更新记录
			string txhash = root["hash"].asString();
			string strSourceData  , strW ;
			strSourceData = strprintf("actor = %d , relate_hash = '%s' ,right_addr ='%s',", 2 , txhash ,addr ) ;
			strSourceData += strprintf(" guess_num = %d,state = %d", (int)guess,4);
			strW = strprintf("tx_hash = '%s'" , hash ) ;

			uistruct::DATABASEINFO_t DatabaseInfo;
			DatabaseInfo.strSource = strSourceData;
			DatabaseInfo.strWhere = strW ;
			DatabaseInfo.strTabName = _T("t_p2p_quiz");
			CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
			string strTemp = DatabaseInfo.ToJson();

			postmsg.SetData(strTemp);
			theApp.m_MsgQueue.push(postmsg);

		}
	}

	return bRes;
	
}
void CP2PDlg::AKeyCancelTheOrder()
{
	string strCond;
	strCond = strprintf("(height+500) > %d and state = 0 and height !=0 and (actor = 0 or actor =2)",theApp.blocktipheight);
	uistruct::P2PBETRECORDLIST pPoolList;
	theApp.m_SqliteDeal.GetP2PQuizRecordList(strCond,&pPoolList);
	if (pPoolList.size() == 0)
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_NOT_CANCEL_ORDER" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}

	map<string,uistruct::LISTADDR_t> mapAddrInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" sign=1 "), &mapAddrInfo);
	if (mapAddrInfo.size() == 0)
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_NOT_ACTI_ADDRESS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}

	int betCount = pPoolList.size();
	int acceptaccount = 0;
	std::map<string,uistruct::LISTADDR_t>::const_iterator it;
	for ( it = mapAddrInfo.begin() ; it != mapAddrInfo.end() ; it++ ) {

		std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it = pPoolList.begin();
		for (;const_it != pPoolList.end();)
		{
			string strCommand,strShowData ="";

			string strCond;
			strCommand = strprintf("%s %s %s 0","getappaccinfo" , theApp.m_betScritptid ,it->second.RegID);
			Json::Value root; 
			if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
			{
				TRACE("UpdateAddressData rpccmd listaddr error");
				return;
			}
			strShowData = root.toStyledString();
			if (strShowData == "")
			{
				continue;;
			}

			int pos = strShowData.find("FreeValues");
			INT64 nMoney = 0;
			if (pos >0)
			{
				nMoney = root["FreeValues"].asInt64() ;
			}
			double balance = (nMoney*1.0/COIN);
			if (balance > const_it->amount)
			{
				if (!AcceptBet(const_it->tx_hash,const_it->accept_amount,const_it->left_addr,const_it->time_out,it->second.RegID))
				{
					acceptaccount++;
				}
				const_it = pPoolList.erase(const_it);
			}else{
				const_it++;
			}
		}
	}

	string strShow = "";
	if (acceptaccount != 0)
	{
		strShow = UiFun::UI_LoadString("P2P_MODULE" , "P2P_ORDER_NOT_QX" ,theApp.gsLanguage);
	}
	if (pPoolList.size() != 0)
	{
		if (strShow != "")
		{
			strShow += UiFun::UI_LoadString("P2P_MODULE" , "P2P_ORDER_NOT_CZZQX" ,theApp.gsLanguage);
		}else{
			strShow = UiFun::UI_LoadString("P2P_MODULE" , "P2P_ORDER_NOT_BFCZZQX" ,theApp.gsLanguage);
		}
	}
	if (strShow == "")
	{
		strShow = UiFun::UI_LoadString("P2P_MODULE" , "P2P_ORDER_OK" ,theApp.gsLanguage);
	}
	
	UiFun::MessageBoxEx(strShow.c_str()  , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );

}

void CP2PDlg::OnBnClickedCancelorde()
{
	// TODO: 在此添加控件通知处理程序代码
	AKeyCancelTheOrder();
}
void CP2PDlg::ReadP2pPoolFromDB()
{
	if (theApp.m_readQuizPool)
	{
		m_PoolList.clear();
		theApp.m_SqliteDeal.GetP2PQuizPoolList(_T(" state =0 order by award_rate desc,accetp_amount desc"), &m_PoolList);
		theApp.m_readQuizPool = false;
	}
	OnListPool();	
}
void CP2PDlg::ReadP2pPoolFromCmd(uistruct::P2PLIST &PoolList)
{
	Json::Reader reader;  
	Json::Value root; 
	Json::Value root1;
	if(theApp.m_betScritptid != _T(""))
	{
		int requiredCount = 100;
		int index = 1;
		while(TRUE)
		{
			string strCommand;
			strCommand = strprintf("%s %s %s %s 0",_T("getscriptvalidedata"),theApp.m_betScritptid,requiredCount,index);
			index++;

			string strShowData;
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			//m_AppID
			int pos = strShowData.find("key");
			if (pos < 0)
			{
				return;
			}
			if (!reader.parse(strShowData, root)) 
				return;
			int size = root.size();
			for ( int index =0; index < size; ++index )
			{
				string txhash = root[index]["key"].asString();
				txhash = txhash.substr(txhash.length()-64);
				string nValue = root[index]["value"].asString();
				uistruct::DBBET_DATA DBbet;
				memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
				std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);

				if (vTemp.size() <=0)
				{
					continue;
				}

				memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

				std::vector<unsigned char> txTemp = CSoyPayHelp::getInstance()->ParseHex(txhash);
				reverse(txTemp.begin(),txTemp.end());
				string newTxhash =  CSoyPayHelp::getInstance()->HexStr(txTemp);
				/// 查找数据库中此赌约是否正在接赌
				uistruct::P2P_QUIZ_RECORD_t  betrecord;
				string strCond;
				strCond =strprintf(" tx_hash = '%s' ", newTxhash.c_str());

				int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(strCond, &betrecord );

				if (nItem != 0 && betrecord.state == 4) ////此赌约正在接赌，只是在block中没有确认
				{
					continue;
				}

				if (DBbet.betstate == 0x00)
				{
					std::vector<unsigned char> vSendid;
					vSendid.assign(DBbet.sendbetid,DBbet.sendbetid+sizeof(DBbet.sendbetid));
					string regid = CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);

					uistruct::LISTP2POOL_T item;
					item.hash = newTxhash;
					item.nPayMoney = DBbet.money;
					item.sendbetid = regid;
					item.outheight = DBbet.hight;
					PoolList.push_back(item);
				}
			}
			if ((int)root.size() < requiredCount ||(int)root.size()>requiredCount )
			{
				break;
			}
		}
	}
}
void CP2PDlg::OnBnClickedButtonSetaddr()
{
	// TODO: 在此添加控件通知处理程序代码
	CCommonAddr addDlg(UI_SENDP2P_RECORD);
	addDlg.DoModal() ;
}


void CP2PDlg::OnBnClickedSetautobet()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetAutoSendp2p dlg;
	if (dlg.DoModal()==IDOK)
	{
		is_atuosendbet = true;   //// false  不自动发单 true 自动发单
		string mony,perenct;
		dlg.GetSendBetParam(mony,perenct);

		m_acceptmoney= (INT64)REAL_MONEY(strtod(mony.c_str(),NULL)) ;

		double dpecent = (atoi(perenct.c_str())*1.0)/100;
		m_sendbetmoney = (INT64)m_acceptmoney*dpecent;

		m_rbSetAtuoBet.ShowWindow(SW_HIDE);
		m_rbCancelAtuoBet.ShowWindow(SW_SHOW);

		CRect rect ;
		m_rbCancelAtuoBet.GetClientRect(rect);

		m_rbCancelAtuoBet.SetWindowPos(NULL ,350-13 , 270 ,rect.Width() , rect.Height() , SWP_SHOWWINDOW);
		string strShow ="自动发单已设置成功,请确保账户ID中有足够的金额";
		UiFun::MessageBoxEx(strShow.c_str()  , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
	}

}

void  CP2PDlg::AutoSendClinetBet()
{
	if (theApp.IsSyncBlock == 0 )
	{
		return;
	}
	if (!is_atuosendbet)
	{
		return;
	}
	int count = m_addrbook.GetCount();
	if (count ==0)
	{
		return;
	}
	string conditon;
	conditon = strprintf("tx_hash ='%s'", m_sendbethash );
	uistruct::P2P_QUIZ_RECORD_t pPoolItem;
	theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
	/// 第一次发单或者上一次发的单以及开奖了 或者超时了
	if (pPoolItem.tx_hash.length()!=0 && (pPoolItem.state==2||                                                                            ///已经开奖
		(pPoolItem.height!=0&&(pPoolItem.state==0|| pPoolItem.state == 4)&&pPoolItem.height+500<=theApp.blocktipheight)||  ///接单超时
		(pPoolItem.height!=0&&(pPoolItem.state==1|| pPoolItem.state == 5)&&pPoolItem.height+pPoolItem.time_out<=theApp.blocktipheight)))  /// 开奖超时
	{
		m_sendbethash ="";
	}
	if (m_sendbethash != "")
	{
		return;
	}
	static int lastrandom = 0 ;


	CTime t1( 1980, 3, 19, 22, 15, 0 );

	CTime t = CTime::GetCurrentTime();

	CTimeSpan span=t-t1; //计算当前系统时间与时间t1的间隔

	srand(span.GetTotalSeconds()+theApp.blocktipheight+lastrandom);

	lastrandom = rand()%count;
	CString addr;
	m_addrbook.GetLBText(lastrandom,addr);

	CString strMoney;
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	double balance =strtod(strMoney,NULL);
	double dsendmoney = (m_sendbetmoney*1.0)/COIN;
	if (dsendmoney > balance)
	{
		return ;
	}

	int rewardnum =(rand()%2+1);
	//// 查询地址是否激活
	CString strCond;

	char strTemp[34];
	memset(strTemp , 0 , 34 );
	memcpy(strTemp , UiFun::Rnd32() , 32 );
	strTemp[32] =rewardnum ;


	string temp(strTemp,strTemp+33);
	int aa = temp.length() ;
	string strCommand;
	strCommand = strprintf("%s %s","gethash" , strTemp );
	string strShowData ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
	int pos = strShowData.find("hash");
	if ( pos < 0 ) return ;

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) 
		return  ;

	string  strHash = root["hash"].asString() ;

	string strContractData;
	string strRamdHash = CSoyPayHelp::getInstance()->GetReverseHash(strHash);


	strContractData = m_P2PBetHelp.PacketP2PSendContract(m_sendbetmoney,OUT_HEIGHT ,strRamdHash ,m_acceptmoney);

	INT64 strTxFee = theApp.m_P2PBetCfg.SendBetFee;
	if (  strTxFee < 10000  ) {
		//::MessageBox( this->GetSafeHwnd() ,UiFun::UI_LoadString("P2P_MODULE" , "P2P_TIP_INSU" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MB_ICONINFORMATION ) ;
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_TIP_INSU" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,strTxFee,0);
	strShowData = "";
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);
	if (strShowData == "")
	{
		return;
	}
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash = root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
		strTip.Format( UiFun::UI_LoadString("P2P_MODULE" , "P2P_WITHDRAWALS_SEND_SUCCESS" ,theApp.gsLanguage)) ;
	}else{
		strTip.Format( UiFun::UI_LoadString("P2P_MODULE" , "P2P_SEND_BET_FAIL" ,theApp.gsLanguage) ) ;
	}

	//// 保存发单的hash
	m_sendbethash = root["hash"].asString();

	//保存到数据库
	if ( bRes ) {
		uistruct::P2P_QUIZ_RECORD_t p2pbetrecord ;
		memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_QUIZ_RECORD_t));
		SYSTEMTIME curTime ;
		memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
		GetLocalTime( &curTime ) ;
		string strSendTime;
		strSendTime= strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		p2pbetrecord.send_time = UiFun::SystemTimeToTimet(curTime);
		p2pbetrecord.time_out  = OUT_HEIGHT ;
		p2pbetrecord.tx_hash = root["hash"].asString();
		p2pbetrecord.left_addr = strprintf("%s",addr);
		p2pbetrecord.amount = dsendmoney ;
		memcpy(p2pbetrecord.content ,strTemp , sizeof(p2pbetrecord.content));

		p2pbetrecord.actor  = 0 ;
		p2pbetrecord.state  = 0 ;
		//插入到数据库
		string strSourceData;
		strSourceData = strprintf("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'" , \
			strSendTime.c_str() , _T("") , p2pbetrecord.time_out , \
			p2pbetrecord.tx_hash.c_str() ,  p2pbetrecord.left_addr.c_str() , p2pbetrecord.right_addr.c_str() ,p2pbetrecord.amount);

		strSourceData += strprintf(",'%s' ,'%d','%d','%d','%d','%s','%d'",p2pbetrecord.content ,p2pbetrecord.actor ,p2pbetrecord.confirmed ,p2pbetrecord.height ,p2pbetrecord.state ,\
			p2pbetrecord.relate_hash.c_str() ,p2pbetrecord.guess_num ) ;

		strSourceData += strprintf(" ,'%d'",p2pbetrecord.deleteflag);
		strSourceData += strprintf(" ,'%lf'",(m_acceptmoney*1.0)/COIN);
		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData;
		pDatabase.strTabName =  _T("t_p2p_quiz");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();

		postmsg.SetData(strTemp);
		theApp.m_MsgQueue.push(postmsg);
	}
}

void CP2PDlg::OnBnClickedCancelautobet()
{
	// TODO: 在此添加控件通知处理程序代码
	is_atuosendbet = false; 
	m_rbSetAtuoBet.ShowWindow(SW_SHOW);
	m_rbCancelAtuoBet.ShowWindow(SW_HIDE);
}
