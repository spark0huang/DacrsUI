
// DacrsUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"
#include "Out.h"
#include "afxdialogex.h"
#include "ChangeDPI.h"
#include "SetAppFee.h"
#include "EncryptWallet.h"
#include "ChangPassWord.h"
#include "Reminderdlg.h"
#include "SetAppId.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CDacrsUIDlg �Ի���




CDacrsUIDlg::CDacrsUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDacrsUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	p_CurSelDlg = NULL  ;
	m_pTitleBar = NULL  ;
	m_pStatusBar = NULL ;
	m_pMainDlg  = NULL  ;
	m_pSendDlg  = NULL  ;
	m_pReceiveDlg  = NULL  ;
	m_pTradDlg  = NULL  ;
	m_pP2PDlg  = NULL  ;
	m_pMortgageTardDlg  = NULL  ;
	m_pAddApp  = NULL  ;
	dlgType = 0;
	m_pOutGifDlg =  NULL ;
	m_pRPCDlg = NULL;
	m_pIpoCoinDlg = NULL;
	m_BalloonTip = NULL;
}

void CDacrsUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDacrsUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_MESSAGE(MSG_TITLE_LBUTTON_DOWN, &CDacrsUIDlg::OnBeginMoveWnd)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_UI, &CDacrsUIDlg::OnBnClickedButtonMainUI)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CDacrsUIDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_RECE, &CDacrsUIDlg::OnBnClickedButtonRece)
	ON_BN_CLICKED(IDC_BUTTON_TRAD_INFO, &CDacrsUIDlg::OnBnClickedButtonTrad)
	ON_BN_CLICKED(IDC_BUTTON_P2P, &CDacrsUIDlg::OnBnClickedButtonP2P)
	ON_BN_CLICKED(IDC_BUTTON_MORTGAGE, &CDacrsUIDlg::OnBnClickedButtonMortgage)
	ON_BN_CLICKED(IDC_BUTTON_IPO, &CDacrsUIDlg::OnBnClickedButtonDeals)  
	ON_BN_CLICKED(IDC_BUTTON_ADDAPP, &CDacrsUIDlg::OnBnClickedButtonAddApp)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDacrsUIDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CDacrsUIDlg::OnBnClickedButtonMin)
	ON_WM_TIMER()
	ON_COMMAND(ID__BAKWALLET, &CDacrsUIDlg::BakWallet)
	ON_COMMAND(ID__SET, &CDacrsUIDlg::SetAppFee)
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_MESSAGE(MSG_USER_CLOSEPROCESS,OnCloseOpenProcess)
	ON_COMMAND(ID__ENCRYPTWALLET, &CDacrsUIDlg::encryptwallet)
	ON_COMMAND(ID_CHANGEPASSWORD, &CDacrsUIDlg::walletpassphrasechange)
	ON_COMMAND(ID__LOCK, &CDacrsUIDlg::LockWallet)
	ON_COMMAND(ID__EXPORTPRIVEKEY, &CDacrsUIDlg::ExportPriveKey)
	ON_COMMAND(ID__IMPORTPRIVEKEY, &CDacrsUIDlg::ImportPrvieKey)
	ON_COMMAND(ID__SETAPPID, &CDacrsUIDlg::SetAppID)
	ON_COMMAND(ID__SETDEFAULT, &CDacrsUIDlg::RestoreDefault)
	ON_UPDATE_COMMAND_UI(ID__SET, &CDacrsUIDlg::OnUpdataState)
	ON_COMMAND(WM_CLOSEAPP,&CDacrsUIDlg::OnCloseApp)

	ON_COMMAND(ID_SENDBET,&CDacrsUIDlg::OnSendBetExportHistory)
	ON_COMMAND(ID_ACCEPTBET,&CDacrsUIDlg::OnAcceptBetExportHistory)
	ON_COMMAND(ID_SENDREDPAKET,&CDacrsUIDlg::OnSendRedPacketExportHistory)
	ON_COMMAND(ID_GRABREDPACKE,&CDacrsUIDlg::OnGrabRedPacketExportHistory)


	ON_MESSAGE(WM_POPUPBAR,OnPopupBar)
	ON_WM_ACTIVATE()

	ON_COMMAND(ID_CNL,&CDacrsUIDlg::OnChinese)
	ON_COMMAND(ID_ENL,&CDacrsUIDlg::OnEnglish)
	ON_COMMAND(ID__CLOSEPOP,&CDacrsUIDlg::OnPopTips)
	ON_UPDATE_COMMAND_UI(ID_ENL, &CDacrsUIDlg::OnUpdateEn)
	ON_UPDATE_COMMAND_UI(ID_CNL, &CDacrsUIDlg::OnUpdateCn)
	ON_UPDATE_COMMAND_UI(ID__CLOSEPOP, &CDacrsUIDlg::OnUpdatePopTips)
END_MESSAGE_MAP()


// CDacrsUIDlg ��Ϣ��������

BOOL CDacrsUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	SetWindowText("���ܷ�");

	// TODO: �ڴ����Ӷ���ĳ�ʼ������
	if ( ((CDacrsUIApp*)AfxGetApp())->pSplashThread != NULL)
	{
		//((CDacrsUIApp*)AfxGetApp())->pSplashThread->HideSplash();   
	    ((CDacrsUIApp*)AfxGetApp())->pSplashThread->PostThreadMessage(WM_QUIT,NULL,NULL);
	}

	CDC* dc = GetDC();
	int dpiX = GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSX);
	int dpiY = GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSY);
	SetDPI(dpiX, dpiY);
	ReleaseDC(dc);

	ToTray() ;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDacrsUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}else if (nID==SC_MINIMIZE) 
	{	
		ToTray(); //��С�������̵ĺ��� )
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDacrsUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDacrsUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CDacrsUIDlg::OnBeginMoveWnd(WPARAM wParam, LPARAM lParam)
{
	PostMessage(WM_NCLBUTTONDOWN,wParam,lParam);
	return 1 ;
}
void CDacrsUIDlg::OnInitMunu()
{
	newMenu.LoadMenu(IDR_MENU1);
	CMenu *pPopup=newMenu.GetSubMenu(0);
	pPopup->AppendMenu(MF_STRING,WM_CLOSEAPP,"  "+UiFun::UI_LoadString("DACRSU" , "DACRSU_CLOSE" ,theApp.gsLanguage));
	CMenu *pPopupChild=pPopup->GetSubMenu(11);
	if (theApp.language() == 1)
	{
		pPopupChild->CheckMenuItem(ID_CNL, MF_BYCOMMAND|MF_CHECKED);
		if (!theApp.m_poptips)
		{
			pPopup->CheckMenuItem(ID__CLOSEPOP, MF_BYCOMMAND|MF_CHECKED);
		}else{
			pPopup->CheckMenuItem(ID__CLOSEPOP, MF_BYCOMMAND|MF_USECHECKBITMAPS );
		}
	}else{
		pPopup->ModifyMenu(0, MF_BYPOSITION | MF_STRING, ID_RPC_CMD, UiFun::UI_LoadString("MENU" , "MENU_RPCCOMMAND" ,theApp.gsLanguage));
		pPopup->ModifyMenu(1, MF_BYPOSITION | MF_STRING, ID__ENCRYPTWALLET, UiFun::UI_LoadString("MENU" , "MENU_PASSWORD" ,theApp.gsLanguage));
		pPopup->ModifyMenu(2, MF_BYPOSITION | MF_STRING, ID_CHANGEPASSWORD,UiFun::UI_LoadString("MENU" , "MENU_MODIFYPASSWORD" ,theApp.gsLanguage));
		pPopup->ModifyMenu(3, MF_BYPOSITION | MF_STRING, ID__LOCK, UiFun::UI_LoadString("MENU" , "MENU_LOCK" ,theApp.gsLanguage));
		pPopup->ModifyMenu(4, MF_BYPOSITION | MF_STRING, ID__BAKWALLET,UiFun::UI_LoadString("MENU" , "MENU_BACK" ,theApp.gsLanguage));
		pPopup->ModifyMenu(5, MF_BYPOSITION | MF_STRING, ID__EXPORTPRIVEKEY, UiFun::UI_LoadString("MENU" , "MENU_EXPORT" ,theApp.gsLanguage));
		pPopup->ModifyMenu(6, MF_BYPOSITION | MF_STRING, ID__IMPORTPRIVEKEY, UiFun::UI_LoadString("MENU" , "MENU_IMPORT" ,theApp.gsLanguage));
		pPopup->ModifyMenu(7, MF_BYPOSITION | MF_STRING, ID__SETAPPID, UiFun::UI_LoadString("MENU" , "MENU_SETAPPID" ,theApp.gsLanguage));
		pPopup->ModifyMenu(8, MF_BYPOSITION | MF_STRING, ID__SET, UiFun::UI_LoadString("MENU" , "MENU_SET" ,theApp.gsLanguage));
		pPopup->ModifyMenu(9, MF_BYPOSITION | MF_STRING, ID__SETDEFAULT, UiFun::UI_LoadString("MENU" , "MENU_RESTORY" ,theApp.gsLanguage));
		pPopup->ModifyMenu(10, MF_BYPOSITION | MF_STRING, ID_RECORD, UiFun::UI_LoadString("MENU" , "MENU_HISTORYRECORD" ,theApp.gsLanguage));
		pPopup->ModifyMenu(11, MF_BYPOSITION | MF_STRING, ID__LANGUAGE, UiFun::UI_LoadString("MENU" , "MENU_LANGUAGE" ,theApp.gsLanguage));
		pPopup->ModifyMenu(12, MF_BYPOSITION | MF_STRING, ID__CLOSEPOP, UiFun::UI_LoadString("MENU" , "MENU_CLOSEPOP" ,theApp.gsLanguage));
		//pPopup->CheckMenuItem(ID__CLOSEPOP, MF_BYCOMMAND|MF_CHECKED);
		if (!theApp.m_poptips)
		{
			pPopup->CheckMenuItem(ID__CLOSEPOP, MF_BYCOMMAND|MF_CHECKED);
		}else{
			pPopup->CheckMenuItem(ID__CLOSEPOP, MF_BYCOMMAND|MF_USECHECKBITMAPS );
		}

		CMenu *pPopupChild=pPopup->GetSubMenu(10);
		pPopupChild->ModifyMenu(0, MF_BYPOSITION | MF_STRING, ID_SENDBET, UiFun::UI_LoadString("MENU" , "MENU_SENDBET" ,theApp.gsLanguage));
		pPopupChild->ModifyMenu(1, MF_BYPOSITION | MF_STRING, ID_ACCEPTBET, UiFun::UI_LoadString("MENU" , "MENU_ACCEPTET" ,theApp.gsLanguage));
		pPopupChild->ModifyMenu(2, MF_BYPOSITION | MF_STRING, ID_SENDREDPAKET, UiFun::UI_LoadString("MENU" , "MENU_SENDRED" ,theApp.gsLanguage));
		pPopupChild->ModifyMenu(3, MF_BYPOSITION | MF_STRING, ID_GRABREDPACKE, UiFun::UI_LoadString("MENU" , "MENU_GRABRED" ,theApp.gsLanguage));

		CMenu *pPopuplChild=pPopup->GetSubMenu(11);
		pPopuplChild->ModifyMenu(ID_CNL, MF_BYCOMMAND, ID_CNL, UiFun::UI_LoadString("MENU" , "MENU_CNL" ,theApp.gsLanguage));
		pPopuplChild->ModifyMenu(ID_ENL, MF_BYCOMMAND, ID_ENL, UiFun::UI_LoadString("MENU" , "MENU_ENL" ,theApp.gsLanguage));
		pPopuplChild->CheckMenuItem(ID_ENL, MF_BYCOMMAND|MF_CHECKED);
	}
}
int CDacrsUIDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ�������ר�õĴ�������
	SetWindowPos(NULL, 0, 0, 900/*1024*/, 600/*768*/, 0);
	//OnInitMunu();

	//SetWindowLong(m_hWnd,   GWL_STYLE,GetWindowLong(m_hWnd,   GWL_STYLE)   &   
	//	(~(WS_CAPTION   |   WS_BORDER)));  
	CRect rect ;
	GetClientRect( rect ) ;

	//���EXE�ļ�����·���Ƿ�������Ļ��߿ո�
	//CheckPathValid( theApp.str_InsPath );

	//��ʼ��RPC Cmd
	InitialRpcCmd();

	LoadListDataInfo();//�������ݿ���Ϣ

	if ( NULL == m_pRPCDlg ) {
		m_pRPCDlg = new CRPCDlg;
		m_pRPCDlg->Create(CRPCDlg::IDD,this);
		m_pRPCDlg->ShowWindow(SW_HIDE);
	}
	//TOP
	if( NULL == m_pTitleBar ){
		m_pTitleBar = new CIndTitleBar ;
		m_pTitleBar->Create(this, IDD_DIALOGBAR_TOOL,CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,1) ;
		m_pTitleBar->SetWindowPos(NULL , 0 , 0  , rect.Width() ,72 , SWP_SHOWWINDOW) ;
		m_pTitleBar->ShowWindow(SW_SHOW) ;
	}
	
	//TRACE("wwwwwCIndTitleBar\r\n");
	//BOTTOM
	if( NULL == m_pStatusBar ){
		m_pStatusBar = new CProgStatusBar ;
		m_pStatusBar->Create(this, IDD_DIALOG_STATUS,CBRS_ALIGN_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,2) ;
		m_pStatusBar->SetWindowPos(NULL , 0 , rect.Height() - 32  , rect.Width() ,32 , SWP_SHOWWINDOW) ;
		m_pStatusBar->ShowWindow(SW_SHOW) ;
	}
	
	//TRACE("wwwwwCProgStatusBar\r\n");
	if( NULL == m_pMainDlg ){
		m_pMainDlg = new CMainDlg ;
		m_pMainDlg->Create(this, CMainDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,3) ;
		m_pMainDlg->ShowWindow(SW_HIDE) ;
	}
	
	//TRACE("wwwwwCMainDlg\r\n");
	if( NULL == m_pSendDlg ){
		m_pSendDlg = new CSendDlg ;
		m_pSendDlg->Create(this, CSendDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,4) ;
		m_pSendDlg->ShowWindow(SW_HIDE) ;
	}
	
	//TRACE("wwwwwCSendDlg\r\n");
	//
	if( NULL == m_pReceiveDlg ){
		m_pReceiveDlg = new CReceiveDlg ;
		m_pReceiveDlg->Create(this, CReceiveDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,5) ;
		m_pReceiveDlg->ShowWindow(SW_HIDE) ;
	}
	
	//TRACE("wwwwwCReceiveDlg\r\n");
	if( NULL == m_pTradDlg ){
		m_pTradDlg = new CTradDlg ;
		m_pTradDlg->Create(this, CTradDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,6) ;
		m_pTradDlg->ShowWindow(SW_HIDE) ;
	}

	if( NULL == m_pP2PDlg ){
		m_pP2PDlg = new CP2PDlg ;
		m_pP2PDlg->Create(this, CP2PDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,7) ;
		m_pP2PDlg->ShowWindow(SW_HIDE) ;
	}
		
	if( NULL == m_pMortgageTardDlg ){
		m_pMortgageTardDlg = new CMortgageTardDlg ;
		m_pMortgageTardDlg->Create(this, CMortgageTardDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,8) ;
		m_pMortgageTardDlg->ShowWindow(SW_HIDE) ;
	}
	//if( NULL == m_pIpoDlg ){
	//	m_pIpoDlg = new CIpoDlg ;
	//	m_pIpoDlg->Create(this, CIpoDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,9) ;
	//	m_pIpoDlg->ShowWindow(SW_HIDE) ;
	//}

	if( NULL == m_pIpoCoinDlg ){
		m_pIpoCoinDlg = new CIpoCoin ;
		m_pIpoCoinDlg->Create(this, CIpoCoin::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,11) ;
		m_pIpoCoinDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pAddApp ){
		m_pAddApp = new CAddApp ;
		m_pAddApp->Create(this, CAddApp::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,10) ;
		m_pAddApp->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pOutGifDlg ){
		m_pOutGifDlg = new COutGifDlg ;
		m_pOutGifDlg->Create(COutGifDlg::IDD , this) ;
		m_pOutGifDlg->ShowWindow(SW_HIDE) ;
	}
	//���뵽m_dlgMap
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMainDlg::IDD , m_pMainDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CSendDlg::IDD , m_pSendDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CReceiveDlg::IDD , m_pReceiveDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CTradDlg::IDD , m_pTradDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CP2PDlg::IDD , m_pP2PDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMortgageTardDlg::IDD , m_pMortgageTardDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CIpoCoin::IDD , m_pIpoCoinDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CAddApp::IDD , m_pAddApp)) ;

	//��ʾ������
	ShowDialog(CMainDlg::IDD) ;
	theApp.m_dlgCreatfinsh = TRUE;

	
	//SetTimer( 0x11 , 30000 , NULL);   //�����
	SetTimer( 0x11 , 15000 , NULL); 
	SetTimer( 0x12 , 15000 , NULL ) ; 
	return 0;
}
void CDacrsUIDlg::ShowDialog(UINT dlgid) 
{
	CRect rc , rt;
	GetClientRect(&rc);
	std::map< UINT , CDialogBar * >::iterator it ;
	for( it = m_dlgMap.begin() ; it != m_dlgMap.end() ; it++ ) {
		if ( NULL != it->second ) {
			if ( dlgid == it->first ) {
				(it->second)->SetWindowPos(NULL , rc.left , 72  , rc.Width() ,rc.Height() - 72- 32 , SWP_SHOWWINDOW) ;
				(it->second)->ShowWindow( SW_SHOW ) ;
			}else{
				(it->second)->ShowWindow( SW_HIDE ) ;
			}
		}
	}
	p_CurSelDlg = m_dlgMap[dlgid] ;  //��ǰѡ��Ի���ָ��
	dlgType = dlgid;

	//CPostMsg postkmsg(MSG_USER_UPDATA_UI,0);
	//theApp.m_MsgQueue.push(postkmsg); 
}
void CDacrsUIDlg::ShowStateTip(UINT nButtonID)
{
	if ( NULL == m_pTitleBar  ) return ;
	m_pTitleBar->MobileTip(nButtonID , FALSE );
}
void CDacrsUIDlg::DestroyDlg()
{
	if ( NULL != m_pTitleBar ) {
		delete m_pTitleBar ;
		m_pTitleBar = NULL ;
	}
	if ( NULL != m_pStatusBar ) {
		delete m_pStatusBar ;
		m_pStatusBar = NULL ;
	}
	if ( NULL != m_pMainDlg ) {
		delete m_pMainDlg ;
		m_pMainDlg = NULL ;
	}
	if ( NULL != m_pSendDlg ) {
		delete m_pSendDlg ;
		m_pSendDlg = NULL ;
	}
	if ( NULL != m_pReceiveDlg ) {
		delete m_pReceiveDlg ;
		m_pReceiveDlg = NULL ;
	}
	if ( NULL != m_pTradDlg ) {
		delete m_pTradDlg ;
		m_pTradDlg = NULL ;
	}
	if ( NULL != m_pP2PDlg ) {
		delete m_pP2PDlg ;
		m_pP2PDlg = NULL ;
	}
	if ( NULL != m_pMortgageTardDlg ) {
		delete m_pMortgageTardDlg ;
		m_pMortgageTardDlg = NULL ;
	}
	if ( NULL != m_pIpoCoinDlg ) {
		delete m_pIpoCoinDlg ;
		m_pIpoCoinDlg = NULL ;
	}
	if ( NULL != m_pAddApp ) {
		delete m_pAddApp ;
		m_pAddApp = NULL ;
	}
	if ( NULL != m_pRPCDlg ) {
		delete m_pRPCDlg ;
		m_pRPCDlg = NULL ;
	}
}
//������
void CDacrsUIDlg::OnBnClickedButtonMainUI()
{
	ShowDialog(CMainDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_MAIN_UI);
}
//����
void CDacrsUIDlg::OnBnClickedButtonSend()
{
	ShowDialog(CSendDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_SEND);
}
//����
void CDacrsUIDlg::OnBnClickedButtonRece()
{
	ShowDialog(CReceiveDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_RECE);
}
//��������
void CDacrsUIDlg::OnBnClickedButtonTrad()
{
	ShowDialog(CTradDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_TRAD_INFO);
}
//P2P��ɫ��
void CDacrsUIDlg::OnBnClickedButtonP2P()
{
	ShowDialog(CP2PDlg::IDD) ;
	//// ˢ�¶�Լ��
	m_pP2PDlg->OnBnClickedButtonRefresh1();
	ShowStateTip(IDC_BUTTON_P2P);
}
//��Ѻ����
void CDacrsUIDlg::OnBnClickedButtonMortgage()
{
	ShowDialog(CMortgageTardDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_MORTGAGE);
}
//IPO���
void CDacrsUIDlg::OnBnClickedButtonDeals()
{
	ShowDialog(CIpoCoin::IDD) ;
	ShowStateTip(IDC_BUTTON_IPO);
}
//����Ӧ��
void CDacrsUIDlg::OnBnClickedButtonAddApp()
{
	ShowDialog(CAddApp::IDD) ;
	ShowStateTip(IDC_BUTTON_ADDAPP);
}

void CDacrsUIDlg::OnBnClickedButtonMin()
{
	//ShowWindow(SW_SHOWMINIMIZED);
	ToTray();
}

void CDacrsUIDlg::InitialRpcCmd()
{
	//ProductHttpHead(theApp.str_InsPath ,m_strServerCfgFileName,m_rpcport,m_sendPreHeadstr,m_sendendHeadstr);
	CSoyPayHelp::getInstance()->InitialRpcCmd(theApp.m_severip,theApp.m_sendPreHeadstr,theApp.m_sendendHeadstr,theApp.m_rpcport);
}
void    CDacrsUIDlg::SyncAddrInfo()
{
	string strCommand;
	strCommand = strprintf("%s","listaddr");
	string strShowData ="";

	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("SyncAddrInfo rpccmd listaddr error");
		return;
	}
	
	map<string,uistruct::LISTADDR_t> pListInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 "), (&pListInfo));
	
	/// ���ݿ���û�еĵ�ַҪ����
	map<string,int> SListInfo;
	
	for(unsigned int i = 0; i < root.size(); ++i){
		//address
		uistruct::LISTADDR_t listaddr;
		string address = root[i]["addr"].asString();	
		SListInfo[address] = i;

		//memset(&listaddr , 0 , sizeof(uistruct::LISTADDR_t));
		//address
		
		listaddr.address = root[i]["addr"].asString(); ;
		//RegID

		listaddr.RegID = root[i]["regid"].asString() ;
		//���
		double fmoney = 0.0 ;  
		fmoney = root[i]["balance"].asDouble(); 
		listaddr.fMoney = fmoney ;
		//�Ƿ�֧�����ڿ�
		listaddr.nColdDig = root[i]["haveminerkey"].asBool() ;

		//�Ƿ�ע��GetLength();
		if ( 3 <= listaddr.RegID.length() ) {
			listaddr.bSign    = 1 ;
		}else{
			listaddr.bSign   = 0 ;
		}
		
		string strCond;
		strCond=strprintf(" address = '%s' ", listaddr.address.c_str());
		uistruct::LISTADDR_t addrsql;
		int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;
	
		if (addrsql.address.length() == 0 )
		{
			string strData;
			strData= strprintf("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'" , listaddr.address.c_str() ,listaddr.RegID.c_str() ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign,listaddr.Label.c_str() ) ;
			if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_wallet_address") ,strData ))
			{
				TRACE("INSERT t_wallet_address data failed!\n");
			}
		}else{
			if (listaddr.fMoney != addrsql.fMoney || listaddr.bSign != addrsql.bSign)
			{
				string strSourceData,strWhere;
				strSourceData =strprintf("reg_id = '%s', money = %.8f ,cold_dig =%d, sign =%d" ,listaddr.RegID.c_str() ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign ) ;
				strWhere=strprintf("address = '%s'" , listaddr.address.c_str()  ) ;
				if ( !theApp.m_SqliteDeal.UpdateTableItem(_T("t_wallet_address") , strSourceData , strWhere ) ){
					TRACE("UPDATE t_wallet_address data failed!\n");
				}
			}
		}
	}
	
	////// �޳����ݿ���Ǯ��û�еĵ�ַ
	map<string,uistruct::LISTADDR_t>::const_iterator it;
	for (it= pListInfo.begin();it != pListInfo.end();it++)
	{
		if (SListInfo.count(it->first) <= 0)
		{
			string strCond;
			strCond = strprintf(" address='%s' ", it->first.c_str());
			int item = theApp.m_SqliteDeal.DeleteTableItem(_T("t_wallet_address"), strCond);
		}
	}

}
void  CDacrsUIDlg::LoadListDataInfo()
{
	//������������,û�еı�����

	SyncAddrInfo();
	//theApp.m_SqliteDeal.UpdataAllTableData();
	
}
void CDacrsUIDlg::CloseThread()
{
	DWORD exc;
	theApp.m_msgAutoDelete= true;
	theApp.m_blockAutoDelete = true;

	closesocket(theApp.m_blockSock);

	while( ::GetExitCodeThread( theApp.m_msgThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_msgThread );
	while( ::GetExitCodeThread( theApp.m_hProcessNoUiMsgThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hProcessNoUiMsgThread );
	while( ::GetExitCodeThread( theApp.m_hblockThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hblockThread);

	while( ::GetExitCodeThread( theApp.m_hProcessBet , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hProcessBet);

	while( ::GetExitCodeThread( theApp.m_hProcessUpadata , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hProcessUpadata);
	//delete theApp.pSplashThread;
}

void  CDacrsUIDlg::StopSever()
{
	string strCommand;
	strCommand = strprintf("%s",_T("stop"));
	string strSendData;
	string strret = _T("Dacrsd server stopping");

	SYSTEMTIME curTime ;
	memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
	GetLocalTime( &curTime ) ;
	int RecivetxTimestart =0;
	RecivetxTimestart=(int) UiFun::SystemTimeToTimet(curTime);
	bool nRecStopCmd = false;
	while(TRUE){
		if(!nRecStopCmd) 
		{
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);
		}
		if (strSendData.find(strret) >=0)
		{
			nRecStopCmd = true;
		}
		GetLocalTime( &curTime ) ;
		int RecivetxTimeLast =0;
		RecivetxTimeLast= (int)UiFun::SystemTimeToTimet(curTime);
		if(!theApp.m_bServerState)
		{
			return;
		}
		if ((RecivetxTimeLast - RecivetxTimestart) > 5)
		{
			return;
		}
		Sleep(5);
	}

}
void CDacrsUIDlg::CloseApp()
{
	if (!theApp.m_passlock)
	{
		EndDialog(IDD_DIALOG_SETWALLETENTRP);
	}
	DeleteTray() ;
	EndWaitCursor();
//	PostMessage( WM_QUIT ) ; 
	PostMessage( WM_CLOSE ); 	
	//DestroyWindow();
	Sleep(200);
}

bool  CDacrsUIDlg::IsP2pBetFinsh()
{

	/// ���ڷ���Լ״̬
	string strCond;
	strCond = strprintf("(height+500) > %d and (state = 0 or state = 4) and (actor = 0 or actor =2)",theApp.blocktipheight);
	uistruct::P2PBETRECORDLIST pPoolList;
	theApp.m_SqliteDeal.GetP2PQuizRecordList(strCond,&pPoolList);
	// ���ڽӶ�״̬
	uistruct::P2PBETRECORDLIST pPoolList1;
	strCond= strprintf("(height +time_out)> %d and (state = 1 or state = 5)  and (actor = 0 or actor =2)",theApp.blocktipheight);
	theApp.m_SqliteDeal.GetP2PQuizRecordList(strCond,&pPoolList1);
	if (pPoolList.size() != 0 || pPoolList1.size() != 0)
	{
		return false;
	}
	return true;
}
void  CDacrsUIDlg::ClosWallet()
{
	if (theApp.m_reminder == 0 || theApp.m_reminder == 2)
	{
		CReminderdlg remindgl(this,theApp.m_reminder);
		remindgl.DoModal();
	}else if (theApp.m_reminder == 1)  ///��С��
	{
		ToTray();                              /// ��С��
	}else if (theApp.m_reminder == 3){        /// �رճ���
		OnCloseWriteAppFee();
		BeginWaitCursor();
		if ( NULL != m_pOutGifDlg ) {
			CRect rc;
			GetWindowRect(&rc);	
			m_pOutGifDlg->LoadGifing(TRUE);
			m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
			m_pOutGifDlg->ShowWindow(SW_SHOW) ;
		}
		//::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 ) ;
		SetTimer( 0x10 , 2000 , NULL ) ; 
	}
	//COut outdlg;
	//if ( IDOK == outdlg.DoModal()){
	//	LogPrint("INFO","Close app start \n");
	//	BeginWaitCursor();
	//	if ( NULL != m_pOutGifDlg ) {
	//		CRect rc;
	//		GetWindowRect(&rc);	
	//		m_pOutGifDlg->LoadGifing(TRUE);
	//		m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
	//		m_pOutGifDlg->ShowWindow(SW_SHOW) ;
	//	}
	//	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 ) ;
	//	SetTimer( 0x10 , 2000 , NULL ) ; 
	//}else{
	//	;
	//}
}
void CDacrsUIDlg::OnBnClickedButtonClose()
{
	if (!IsP2pBetFinsh())
	{
		CString strDisplay = UiFun::UI_LoadString("DACRSU" , "DACRSU_CLOSTIP" ,theApp.gsLanguage);
		//strDisplay.Format(_T("��������Щ����δ�ӶĻ򿪽�,�ر�ϵͳ�ѽӶĳ�ʱδ����\r\n�Զ�����,�Ƿ�ر�"));
		if (IDYES == UiFun::MessageBoxEx(strDisplay , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MFB_YESNO|MFB_TIP ) )
		{
			ClosWallet();
		}
	}else{
		ClosWallet();
	}
	//COut outdlg;
	//if ( IDOK == outdlg.DoModal()){
	//	BeginWaitCursor();
	//	if ( NULL != m_pOutGifDlg ) {
	//		CRect rc;
	//		GetWindowRect(&rc);	
	//		m_pOutGifDlg->LoadGifing(TRUE);
	//		m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
	//		m_pOutGifDlg->ShowWindow(SW_SHOW) ;
	//	}
	//	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 ) ;
	//	SetTimer( 0x10 , 2000 , NULL ) ; 
	//}else{
	//	;
	//}
}

void CDacrsUIDlg::Close() 
{
	//::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 );
	SetTimer( 0x10 , 3000 , NULL); 
}
void CDacrsUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	if( 0x10 == nIDEvent ) {
		KillTimer(0x10);
		StopSever();

		DestroyDlg();
		LogPrint("INFO","OnTimer start \n");
		if ( NULL != m_pOutGifDlg ) {
			m_pOutGifDlg->ShowWindow(SW_HIDE) ;
			m_pOutGifDlg->LoadGifing(FALSE);
			delete m_pOutGifDlg ;
			m_pOutGifDlg = NULL ;
		}
		theApp.m_bOutApp = TRUE ;
		CloseThread();
		CloseApp();
	}else if (0x11  == nIDEvent)
	{
		CPostMsg Postmsg ;
		if (m_BalloonTip!= NULL && CBalloonTip::nBalloonInstances == 0 && m_barpoomesg.pop(Postmsg))
		{
			string message = Postmsg.GetData();
			 ::SendMessage(theApp.m_pMainWnd->m_hWnd,WM_POPUPBAR,0,(LPARAM)message.c_str());	
		}
	}else if (0x12  == nIDEvent) /// ��������������Ƿ�������û������
	{
		NOTIFYICONDATA nid; 
		memset(&nid,0,sizeof(NOTIFYICONDATA));
		nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
		nid.hWnd=this->m_hWnd; 
		nid.uID=IDR_MAINFRAME; 
		nid.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE ; //NIF_MESSAGE|
		nid.uCallbackMessage=WM_SHOWTASK;//�Զ������Ϣ���� 
		nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
		memset(nid.szTip,0,128);
		strcpy_s(nid.szTip,"dacrs"); //��Ϣ��ʾ�� 
		if ( !Shell_NotifyIcon( NIM_MODIFY, &nid ) )  
			   Shell_NotifyIcon( NIM_ADD, &nid );  

	}
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CDacrsUIDlg::PreTranslateMessage(MSG* pMsg)  
{  
  
	if(pMsg->message == WM_KEYDOWN) 
	{  
  
		switch(pMsg->wParam) 
		{  
  
		case VK_RETURN: //�س�   
			return TRUE;  
  
		case VK_ESCAPE: //ESC  
			return TRUE;  
  
		}  
  
	}  
	return CDialog::PreTranslateMessage(pMsg);  
}  

void CDacrsUIDlg::OnOk()
{
}

LRESULT CDacrsUIDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ�����ר�ô����/����û���
	
	switch(message) 
	{
	case WM_COMMAND:
		{
			if ( ID_RPC_CMD == LOWORD(wParam) ) {
				CRect rcWindow;
				GetWindowRect(&rcWindow);
				if ( NULL != m_pRPCDlg ){
					m_pRPCDlg->MoveWindow(rcWindow.right,rcWindow.top,300,rcWindow.Height()-8);
					m_pRPCDlg->ShowWindow(SW_SHOW);
				}
			}
		}
		break;
	default:
		break ;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
bool CDacrsUIDlg::GetFileName(CString &fileName,CString strexe )
{
	int spcace = fileName.Find(" ");
	if (spcace >=0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_PATHNOTNULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return false;
	}
	int pos = fileName.Find(".",0);
	if (pos <0)
	{
		fileName.AppendFormat(strexe);
	}
	pos = fileName.Find(".",0);
	fileName = fileName.Left(pos);
	fileName.AppendFormat(strexe);
	if(PathFileExistsA(fileName)){
		CString strDisplay;
		strDisplay=UiFun::UI_LoadString("DACRSU" , "DACRSU_INSTEAD" ,theApp.gsLanguage) ;
		if (IDOK == UiFun::MessageBoxEx(strDisplay , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MFB_OKCANCEL|MFB_TIP ) )
		{
			return TRUE;
		}
	}
	
	return TRUE;
}
void CDacrsUIDlg::BakWallet()
{
	// TODO: �ڴ�����������������
	
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,_T("*.dat||"));
	if (IDOK == dlg.DoModal())
	{
		CString strPath = dlg.GetPathName();
		if (!GetFileName(strPath,_T(".dat")))
		{
			return;
		}

		//strPath.AppendFormat(_T(".dat"));
		string strCommand;
		strCommand = strprintf("%s %s",_T("backupwallet"),strPath);
		string strSendData;
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);

		CString strShowData;
		strShowData.Format(_T("%s:%s"),UiFun::UI_LoadString("DACRSU" , "DACRSU_BACK" ,theApp.gsLanguage),strPath);
		UiFun::MessageBoxEx(strShowData, UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
	}

}

void CDacrsUIDlg::ToTray() 
{ 
	
	if(m_BalloonTip != NULL&& m_BalloonTip->nBalloonInstances==1)
	{
		CBalloonTip::Hide(m_BalloonTip);
	}
	NOTIFYICONDATA nid; 
	memset(&nid,0,sizeof(NOTIFYICONDATA));
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this->m_hWnd; 
	nid.uID=IDR_MAINFRAME; 
	nid.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE ; //NIF_MESSAGE|
	nid.uCallbackMessage=WM_SHOWTASK;//�Զ������Ϣ���� 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	memset(nid.szTip,0,128);
	strcpy_s(nid.szTip,"dacrs"); //��Ϣ��ʾ�� 
	Shell_NotifyIcon(NIM_ADD,&nid); //������������ͼ�� 
	ShowWindow(SW_HIDE); //���������� 
}

LRESULT CDacrsUIDlg::OnCloseOpenProcess(WPARAM wParam,LPARAM lParam)
{
	delete m_pAddApp;
	m_pAddApp = NULL;
	return 0;
}
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ 
LRESULT CDacrsUIDlg::OnShowTask(WPARAM wParam,LPARAM lParam) 
{ 
	if(wParam!=IDR_MAINFRAME) 
		return 1; 
	switch(lParam) 
	{ 
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա� 
		{ 
			LPPOINT lpoint=new tagPOINT; 
			::GetCursorPos(lpoint);//�õ����λ�� 
			//CMenu menu; 
			//menu.CreatePopupMenu();//����һ������ʽ�˵� 

			//menu.AppendMenu(MF_STRING,WM_DESTROY,"�ر�"); //���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ������أ�������������� 
			//menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); //ȷ������ʽ�˵���λ�� 
			//HMENU hmenu=menu.Detach(); 
			//menu.DestroyMenu(); //��Դ���� 

			//CMenu newMenu;
			CMenu *pPopup=newMenu.GetSubMenu(0);
			if (theApp.HaveLocked)
			{
				pPopup->EnableMenuItem(ID__ENCRYPTWALLET,MF_GRAYED);
			}
			pPopup->EnableMenuItem(ID__SET,MF_GRAYED);
			
			//��ʾ�Ҽ��˵��������ര��ӵ�С�
			pPopup->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
			delete lpoint; 
		} break; 
	case WM_LBUTTONDBLCLK: //˫������Ĵ��� 
		{ 
			this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
			//DeleteTray(); 
		} break; 
	case  WM_LBUTTONDOWN:
		{
			if (IsWindowVisible())
			{
				this->ShowWindow(SW_HIDE);//�򵥵���ʾ���������¶�
			}else{
				SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);  
				this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
				SetWindowPos(&this->wndNoTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);

				if(m_BalloonTip != NULL&& m_BalloonTip->nBalloonInstances==1)
				{
					CBalloonTip::Hide(m_BalloonTip);
				}
			}
	case WM_MOUSEFIRST:
		{
			int b =5;
		}
	}
	default: break; 
	} 
	return 0; 
} 
void CDacrsUIDlg::OnCloseApp() 
{
	ClosWalletWind();
}
void CDacrsUIDlg::DeleteTray() 
{ 
	NOTIFYICONDATA nid; 
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this->m_hWnd; 
	nid.uID=IDR_MAINFRAME; 
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
	nid.uCallbackMessage=WM_SHOWTASK; //�Զ������Ϣ���� 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy_s(nid.szTip,"dacrs"); //��Ϣ��ʾ��Ϊ���ƻ��������ѡ� 
	Shell_NotifyIcon(NIM_DELETE,&nid); //��������ɾ��ͼ�� 
} 
void CDacrsUIDlg::SetAppFee()
{
	CSetAppFee setappdlg;
	if (setappdlg.DoModal() ==IDOK)
	{
		return;
	}
	return;
}
void CDacrsUIDlg::encryptwallet()
{
	if (theApp.HaveLocked)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_HAVE" ,theApp.gsLanguage)  , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}
	CEncryptWallet enwalletdlg;
	if (enwalletdlg.DoModal() ==IDOK)
	{
		return;
	}
	return;
}
void CDacrsUIDlg::walletpassphrasechange(){

	CChangPassWord changwalletdlg;
	if (changwalletdlg.DoModal() ==IDOK)
	{
		return;
	}
	return;
}
void CDacrsUIDlg:: LockWallet()
{
	if (!theApp.HaveLocked)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_NOTLOCK" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	string strCommand;
	strCommand = strprintf("%s",_T("walletlock"));

	Json::Value root;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("UpdateAddressData rpccmd listaddr error");
		return;
	}


	bool isEntryp = root["walletlock"].asBool();
	if (!isEntryp)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_LOCKERROR" ,theApp.gsLanguage)  , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

}
void CDacrsUIDlg::WriteExportWalletAndBookAddr(CString fileName)
{
	string fiename =strprintf("%s",fileName);
	string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(fiename);
	if (strFile == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 

	if (!reader.parse(strFile, root)) 
		return;

	int pos = strFile.find("walletaddr");
	if ( pos>=0)
	{
		Json::Value addrValue = root["walletaddr"]; 
		for(int i =0;i<(int)addrValue.size();i++){
			Json::Value obj = addrValue[i];

			CString addr = obj["addr"].asCString();
			CString label =obj["label"].asCString();
			CString regid = obj["reg_id"].asCString();
			double money = obj["money"].asDouble();
			int cold = obj["cold_dig"].asInt();
			int sig = obj["sign"].asInt();

			string conditon = _T("");
			conditon = strprintf("address = '%s'",addr);
			uistruct::LISTADDR_t pAddr;
			theApp.m_SqliteDeal.GetWalletAddressItem(conditon,&pAddr);
			if (pAddr.address.length() == 0)
			{
				string strSourceData= "";
				strSourceData = strprintf("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'" , addr ,regid ,money ,cold ,sig,label ) ;
				if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_wallet_address") ,strSourceData ) )
				{
					TRACE("Insert t_wallet_address error!\n");
				}

			}else{
				string strSourceData,strWhere;
				strSourceData = strprintf("label = '%s'" ,label) ;
				strWhere= strprintf("address = '%s'" , addr ) ;
				if ( !theApp.m_SqliteDeal.UpdateTableItem(_T("t_wallet_address") , strSourceData , strWhere ) ){
					TRACE(_T("Update t_wallet_address failed!") );
				}
			}
		}
	}
	 pos = strFile.find("wbookaddr");
	if (pos >=0)
	{
		Json::Value addrValue = root["bookaddr"]; 
		for(int i =0;i<(int)addrValue.size();i++){
			Json::Value obj = addrValue[i];
			CString addr = obj["addr"].asCString();
			CString label =obj["label"].asCString();
			string conditon = _T("");
			conditon = strprintf("address = '%s'",addr);
			uistruct::ADDRBOOK_t  pAddr;
			theApp.m_SqliteDeal.GetAddressBookItem(conditon,&pAddr);
			if (pAddr.address.length() == 0)
			{
				string strSourceData= "";
				strSourceData= strprintf("'%s' , '%s' "  ,label, addr) ;
				if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_address_book") ,strSourceData ) )
				{
					TRACE("Insert t_wallet_address error!\n");
				}

			}else{
				string strSourceData,strWhere;
				strSourceData=strprintf("Label = '%s'",label) ;
				strWhere= strprintf("address = '%s'", addr ) ;
				if ( !theApp.m_SqliteDeal.UpdateTableItem(_T("t_address_book") , strSourceData , strWhere ) ){
					TRACE(_T("Update t_wallet_address failed!") );
				}
			}
		}
	}
}

void CDacrsUIDlg::AddImportWalletAndBookAddr(CString fileName)
{
	string fiename =strprintf("%s",fileName);
	string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(fiename);
	if (strFile == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 

	if (!reader.parse(strFile, root)) 
		return;
	/// �Լ�Ǯ����ַ����  walletaddr
	Json::Value walletaddr; 
	map<string,uistruct::LISTADDR_t> pListInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 "), (&pListInfo));
	if (pListInfo.size() != 0)
	{
		Json::Value Array;
		Json::Value itemValue;
		map<string,uistruct::LISTADDR_t>::iterator item = pListInfo.begin();
		for (;item != pListInfo.end();item++)
		{
			if (item->second.Label.length() != 0)
			{
				itemValue["addr"]=item->second.address;
				itemValue["label"] = item->second.Label;
				itemValue["reg_id"] = item->second.RegID;
				itemValue["money"] = item->second.fMoney;
				itemValue["cold_dig"] = item->second.nColdDig;
				itemValue["sign"] = item->second.bSign;
				Array.append(itemValue);
			}
		}
	root["walletaddr"] = Array;
	}

	map<string,uistruct::ADDRBOOK_t> pAddrBookMap;
	theApp.m_SqliteDeal.GetAddressBookList(_T(" 1=1 "),(&pAddrBookMap));

	if (pAddrBookMap.size() != 0)
	{
		Json::Value Array;
		Json::Value itemValue;
		map<string,uistruct::ADDRBOOK_t>::iterator item = pAddrBookMap.begin();
		for (;item != pAddrBookMap.end();item++)
		{
			//if (item->second.label != _T(""))
			{
				itemValue["addr"]=item->second.address;
				itemValue["label"] = item->second.label;
				Array.append(itemValue);
			}
		}
		root["wbookaddr"] = Array;
	}

	/// ���浽�ļ�
	CStdioFile  File;
	File.Open(fileName,CFile::modeWrite | CFile::modeCreate); 
	string strfile = root.toStyledString();
	File.WriteString(strfile.c_str());
	File.Close();

}
void CDacrsUIDlg:: ExportPriveKey()
{
	if (theApp.IsLockWallet())
	{
		return ;
	}
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,_T("*.smc||"));
	if (IDOK == dlg.DoModal())
	{
		CString strPath = dlg.GetPathName();
		if (!GetFileName(strPath,_T(".smc")))
		{
			return;
		}
		//strPath.AppendFormat(_T(".smc"));
		string strCommand;
		strCommand = strprintf("%s %s","dumpwallet",strPath);
		string strSendData;
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);
		CString strShowData;
		strShowData.Format(_T("%s:%s"),UiFun::UI_LoadString("DACRSU" , "DACRSU_EXPORT" ,theApp.gsLanguage),strPath);
		AddImportWalletAndBookAddr(strPath);
		UiFun::MessageBoxEx(strShowData , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
	}
}
void CDacrsUIDlg:: ImportPrvieKey()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	// TODO: �ڴ�����������������
	if (theApp.IsLockWallet())
	{
		return ;
	}
	OPENFILENAME ofn;
	char szFile[MAX_PATH];
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = TEXT('\0'); 
	ofn.nMaxFile = sizeof(szFile); 
	//CString strFormat;
	//strFormat.Format("%s(*.smc)\0*.smc\0",UiFun::UI_LoadString("DACRSU" , "DACRSU_FILE" ,theApp.gsLanguage));
	//strFormat.AppendFormat("%s(*.*)\0*.*\0\0",UiFun::UI_LoadString("DACRSU" , "DACRSU_ALLFILE" ,theApp.gsLanguage));

	//ofn.lpstrFilter =  "�ı��ļ�(*.smc)\0*.smc\0�����ļ�(*.*)\0*.*\0\0"; 
	ofn.lpstrFilter =  "All Files(*.smc)\0*.smc\0All Files(*.*)\0*.*\0\0"; 
	ofn.nFilterIndex = 1; 
	ofn.lpstrFileTitle = NULL; 
	ofn.nMaxFileTitle = 0; 
	ofn.lpstrInitialDir = NULL;	ofn.hwndOwner = m_hWnd; 
	ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (::GetOpenFileName(&ofn))
	{
			CString strPath = ofn.lpstrFile;
			string strCommand;
			strCommand = strprintf("%s %s",_T("importwallet"),strPath);
			Json::Value root;
	
			if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
			{
				TRACE("UpdateAddressData rpccmd listaddr error");
				return;
			}
			
			int size = root["imorpt key size"].asInt();
			if (size > 0)
			{
				WriteExportWalletAndBookAddr(strPath);
				UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_EXPORTTIP" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
				//ClosWallet();
				//((CDacrsUIDlg*)(this->GetParent()))->Close();
				ClosWalletWind();
			}else{
				UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_EXPORTREEOR" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
			}
	}
}

void CDacrsUIDlg::OnUpdataState(CCmdUI *pCmdUI)
{
	// TODO: �ڴ�������������û����洦���������
//	pCmdUI->Enable(FALSE); 
}

void  CDacrsUIDlg::ClosWalletWind()
{
	   OnCloseWriteAppFee();
		BeginWaitCursor();
		if ( NULL != m_pOutGifDlg ) {
			CRect rc;
			GetWindowRect(&rc);	
			m_pOutGifDlg->LoadGifing(TRUE);
			m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
			m_pOutGifDlg->ShowWindow(SW_SHOW) ;
		}
		//::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 ) ;
		SetTimer( 0x10 , 2000 , NULL ) ; 
}
void CDacrsUIDlg::SetAppID()
{
	CSetAppId dlg;
	if (dlg.DoModal() == IDOK)
	{
		CPostMsg p2ppmsg(MSG_USER_P2P_UI,WM_UP_ADDRESS);
		theApp.m_MsgQueue.push(p2ppmsg);

		CPostMsg redmsg(MSG_USER_REDPACKET_UI,WM_UP_ADDRESS);
		theApp.m_MsgQueue.push(redmsg);
		return;
	}
	return;
}
void CDacrsUIDlg::RestoreDefault()
{
	if (PathFileExistsA(theApp.str_InsPath.c_str()))
	{
		string configpath = "";
		configpath = strprintf("%s",theApp.str_InsPath);
		configpath+= strprintf("\\%s","dacrsclient.conf");
		string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		if (strFile == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile, root)) 
			return;

		int pos =strFile.find("closeconf");
		if (pos>=0)
		{
			Json::Value setclose = root["closeconf"];
			ASSERT(!setclose.isNull());
			setclose["colse"]= 0;
			setclose["tip"]= true;
			root["closeconf"]=setclose;
		}
		pos =strFile.find("script");
		if (pos>=0)
		{
			Json::Value setscriptid = root["script"];
			ASSERT(!setscriptid.isNull());
			setscriptid["betscript"]= theApp.m_neststcriptid.strNewScriptBetid;
			setscriptid["iposcript"]= theApp.m_neststcriptid.strNewipoScritptid;
			setscriptid["redpacketscript"]= theApp.m_neststcriptid.strNewSrcriptRedPacektid;
			root["script"]=setscriptid;
		}
		
		pos =strFile.find("p2pbet");
		if (pos>=0)
		{
			Json::Value setbet = root["p2pbet"];
			ASSERT(!setbet.isNull());
			setbet["SendBetFee"]= 63806;
			setbet["AcceptBetnFee"]= 89407;
			setbet["OpenBetnFee"]= 84457;
			setbet["GetAppAmountnFee"]= 84457;
			setbet["GetRechangeFee"]= 32587;
			root["p2pbet"]=setbet;
		}

		pos =strFile.find("redpacket");
		if (pos>=0)
		{
			Json::Value setred = root["redpacket"];
			ASSERT(!setred.isNull());
			setred["sendredcommFee"]= 1800000;
			setred["acceptredcommFee"]= 180000;
			setred["sendredspecalFee"]= 480000;
			setred["acceptredspecalFee"]= 960000;
			root["redpacket"]=setred;
		}

		CStdioFile  File;
		string strpath = theApp.str_InsPath;
		strpath+="\\dacrsclient.conf";
		File.Open((LPCTSTR)(LPSTR)strpath.c_str(),CFile::modeWrite | CFile::modeCreate); 
		string strfile = root.toStyledString();
		File.WriteString(strfile.c_str());
		File.Close();
	}

	theApp.ParseUIConfigFile(theApp.str_InsPath);

	CPostMsg p2ppmsg(MSG_USER_P2P_UI,WM_UP_ADDRESS);
	theApp.m_MsgQueue.push(p2ppmsg);

	CPostMsg redmsg(MSG_USER_REDPACKET_UI,WM_UP_ADDRESS);
	theApp.m_MsgQueue.push(redmsg);
	UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_SETDEAFLUT" ,theApp.gsLanguage) ,  UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
}

LRESULT CDacrsUIDlg::OnPopupBar(WPARAM wParam,LPARAM lParam) 
{
	///�����˲���ʾ��Ϣ
	if (!theApp.m_poptips)
	{
		return 0;
	}
	char* message = (char*)(lParam);
	string strmessage =strprintf("%s",message);

	if (m_BalloonTip!= NULL && CBalloonTip::nBalloonInstances != 0)
	{
		CPostMsg Postmsg ;
		Postmsg.SetData(strmessage);
		m_barpoomesg.push(Postmsg);
		return 0;
	}


	RECT ret;
	GetWindowRect(&ret);


	LOGFONT lf;
	::ZeroMemory (&lf, sizeof (lf));
	lf.lfHeight = 11;
	lf.lfWeight = FW_BOLD;
	lf.lfUnderline = FALSE;
	strcpy((char*)lf.lfFaceName, "����");

	//// �Ի���û�б����ز���Ҫ����ǰ��
	if (IsWindowVisible() && this->IsTopParentActive())
	{
		m_BalloonTip =CBalloonTip::Show(
			CPoint(ret.right -90, ret.bottom-10),         // Point on the screen where the tip will be shown
			CSize(270, 150),          // Size of the total rectangle encompassing the balloon 
			_T(message), // Message to be shown in the balloon
			lf,                               // LOGFONT structure for font properties 
			15,                 // Time in seconds to show the balloon
			TRUE              // TRUE  == Balloon is up(Balloon Tip is down) 
			// FALSE ==  Balloon is down(Balloon Tip is up)
			);
	}else{
		int x=GetSystemMetrics(SM_CXSCREEN); //�õ�x����
			int y=GetSystemMetrics(SM_CYSCREEN);//�õ�y����
		m_BalloonTip =CBalloonTip::Show(
			CPoint(x-90, y-10),         // Point on the screen where the tip will be shown
			CSize(270, 150),          // Size of the total rectangle encompassing the balloon 
			_T(message), // Message to be shown in the balloon
			lf,                               // LOGFONT structure for font properties 
			15,                 // Time in seconds to show the balloon
			TRUE              // TRUE  == Balloon is up(Balloon Tip is down) 
			// FALSE ==  Balloon is down(Balloon Tip is up)
			);
	}

	return 0;
}


void CDacrsUIDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{

	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	if (nState == WA_INACTIVE )
	{
		if (m_BalloonTip != NULL && m_BalloonTip->nBalloonInstances==1)
		{
			CBalloonTip::Hide(m_BalloonTip);
		}
	}
	

	// TODO: �ڴ˴�������Ϣ�����������
}

/// ����رյ�ʱ�򽫷��ñ����������ļ���
void CDacrsUIDlg::OnCloseWriteAppFee()
{
	if (PathFileExistsA(theApp.str_InsPath.c_str()))
	{
		string configpath = "";
		configpath = strprintf("%s",theApp.str_InsPath);
		configpath+= strprintf("\\%s","dacrsclient.conf");
		string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		if (strFile == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile, root)) 
			return;

		int pos =strFile.find("p2pbet");
		if (pos>=0)
		{
			Json::Value setbet = root["p2pbet"];
			ASSERT(!setbet.isNull());
			setbet["SendBetFee"]= theApp.m_P2PBetCfg.SendBetFee;
			setbet["AcceptBetnFee"]= theApp.m_P2PBetCfg.AcceptBetnFee;
			setbet["OpenBetnFee"]= theApp.m_P2PBetCfg.OpenBetnFee;
			setbet["GetAppAmountnFee"]= theApp.m_P2PBetCfg.GetAppAmountnFee;
			setbet["GetRechangeFee"]= theApp.m_P2PBetCfg.GetRechangeFee;
			root["p2pbet"]=setbet;
		}else{
			Json::Value setbet ;
			setbet["SendBetFee"]= theApp.m_P2PBetCfg.SendBetFee;
			setbet["AcceptBetnFee"]= theApp.m_P2PBetCfg.AcceptBetnFee;
			setbet["OpenBetnFee"]= theApp.m_P2PBetCfg.OpenBetnFee;
			setbet["GetAppAmountnFee"]= theApp.m_P2PBetCfg.GetAppAmountnFee;
			setbet["GetRechangeFee"]= theApp.m_P2PBetCfg.GetRechangeFee;
			root["p2pbet"]=setbet;
		}

		pos =strFile.find("redpacket");
		if (pos>=0)
		{
			Json::Value setred = root["redpacket"];
			ASSERT(!setred.isNull());
			setred["sendredcommFee"]= theApp.m_RedPacketCfg.SendRedPacketCommFee;
			setred["acceptredcommFee"]= theApp.m_RedPacketCfg.AcceptRedPacketCommFee;
			setred["sendredspecalFee"]= theApp.m_RedPacketCfg.SendRedPacketSpecailFee;
			setred["acceptredspecalFee"]= theApp.m_RedPacketCfg.AcceptRedPacketSpecailFee;
			root["redpacket"]=setred;
		}else{
			Json::Value setred ;
			setred["sendredcommFee"]= theApp.m_RedPacketCfg.SendRedPacketCommFee;
			setred["acceptredcommFee"]= theApp.m_RedPacketCfg.AcceptRedPacketCommFee;
			setred["sendredspecalFee"]= theApp.m_RedPacketCfg.SendRedPacketSpecailFee;
			setred["acceptredspecalFee"]= theApp.m_RedPacketCfg.AcceptRedPacketSpecailFee;
			root["redpacket"]=setred;
		}

		CStdioFile  File;
		string strpath = theApp.str_InsPath;
		strpath+="\\dacrsclient.conf";
		if (File.Open((LPCTSTR)(LPSTR)strpath.c_str(),CFile::modeWrite | CFile::modeCreate))
		{
			string strfile = root.toStyledString();
			File.WriteString(strfile.c_str());
			File.Close();
		} 

	}
}


void CDacrsUIDlg::OnSendBetExportHistory()
{
	if (m_pP2PDlg != NULL)
	{
		m_pP2PDlg->m_SendRecord.ExportSendBetRecordToexel();
	}
	
}
void CDacrsUIDlg::OnAcceptBetExportHistory(){
	if (m_pP2PDlg != NULL)
	{
		m_pP2PDlg->m_BetRecord.OExportAcceptBetToexel();
	}
	
}
void CDacrsUIDlg::OnSendRedPacketExportHistory()
{
	if (m_pMortgageTardDlg != NULL)
	{
		m_pMortgageTardDlg->m_SendRecord.ExportSendRedPacketToexel();
	}
	
}
void CDacrsUIDlg::OnGrabRedPacketExportHistory()
{
	if (m_pMortgageTardDlg != NULL)
	{
		m_pMortgageTardDlg->m_BetRecord.ExportAcceptRedPacektToexel();
	}

}

void CDacrsUIDlg::OnChinese()
{
	/// Ӣ�Ľ���
	if (theApp.gsLanguage == 2)
	{
		//theApp.gsLanguage = 1;
		UiFun::Setlanguage(theApp.gsLanguage-1);
		if (UiFun::MessageBoxEx(UiFun::UI_LoadString("SETAPPID" , "SETAPPFEE_LANGAGE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP ) == IDOK)
		{
			((CDacrsUIDlg*)(theApp.m_pMainWnd))->ClosWalletWind();
			theApp.gsLanguage = 1;
		}
		// UiFun::MessageBoxEx(_T("�޸����Ի�������������Ǯ��") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		//((CDacrsUIDlg*)(theApp.m_pMainWnd))->ClosWalletWind();

	}
}
void CDacrsUIDlg::OnEnglish()
{
	/// ���Ľ���
	if (theApp.gsLanguage == 1)
	{

		UiFun::Setlanguage(theApp.gsLanguage+1);
		if (UiFun::MessageBoxEx(UiFun::UI_LoadString("SETAPPID" , "SETAPPFEE_LANGAGE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP ) == IDOK)
		{
			((CDacrsUIDlg*)(theApp.m_pMainWnd))->ClosWalletWind();
			theApp.gsLanguage = 2;
		}
		//UiFun::MessageBoxEx(_T("�޸����Ի�������������Ǯ��") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		//((CDacrsUIDlg*)(theApp.m_pMainWnd))->ClosWalletWind();
	}
}



void CDacrsUIDlg::OnUpdateEn(CCmdUI *pCmdUI)
{
	// TODO: �ڴ�������������û����洦���������
	CMenu *pPopup=newMenu.GetSubMenu(0);
	CMenu *pPopupChild=pPopup->GetSubMenu(11);
	//if (pPopupChild->GetMenuState(ID_CNL,MF_CHECKED) == MF_CHECKED)
	{
		pPopupChild->CheckMenuItem(ID_CNL, MF_BYCOMMAND|MF_USECHECKBITMAPS );
	}

	pPopupChild->CheckMenuItem(ID_ENL, MF_BYCOMMAND|MF_CHECKED);
}


void CDacrsUIDlg::OnUpdateCn(CCmdUI *pCmdUI)
{
	// TODO: �ڴ�������������û����洦���������
	CMenu *pPopup=newMenu.GetSubMenu(0);
	CMenu *pPopupChild=pPopup->GetSubMenu(11);
	//if (pPopupChild->GetMenuState(ID_ENL,MF_CHECKED) == MF_CHECKED)
	{
		pPopupChild->CheckMenuItem(ID_ENL, MF_BYCOMMAND|MF_USECHECKBITMAPS );
	}

	pPopupChild->CheckMenuItem(ID_CNL, MF_BYCOMMAND|MF_CHECKED);
}


void CDacrsUIDlg::OnPopTips()
{
	theApp.m_poptips = !theApp.m_poptips ;
	UiFun::WriteClosConfig(theApp.m_poptips,theApp.str_InsPath);
	CMenu *pPopup=newMenu.GetSubMenu(0);
	if (!theApp.m_poptips)
	{
		pPopup->CheckMenuItem(ID__CLOSEPOP, MF_BYCOMMAND|MF_CHECKED);
	}else{
		pPopup->CheckMenuItem(ID__CLOSEPOP, MF_BYCOMMAND|MF_USECHECKBITMAPS );
	}
}
void CDacrsUIDlg::OnUpdatePopTips(CCmdUI *pCmdUI)
{
	CMenu *pPopup=newMenu.GetSubMenu(0);
	if (!theApp.m_poptips)
	{
		pPopup->CheckMenuItem(ID__CLOSEPOP, MF_BYCOMMAND|MF_CHECKED);
	}else{
		pPopup->CheckMenuItem(ID__CLOSEPOP, MF_BYCOMMAND|MF_USECHECKBITMAPS );
	}
}