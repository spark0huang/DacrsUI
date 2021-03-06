// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "DacrsUIDlg.h"
#include <afxinet.h>

// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialogBar)

CMainDlg::CMainDlg()
{
	m_pBmp = NULL ;
	m_url.clear();
	v_linkCtrl = NULL;
	v_linkCtrl1 = NULL;
	v_linkCtrl2= NULL;
	v_linkCtrl3= NULL;
	v_linkCtrlQQ= NULL;
	v_linkCtrlBlock= NULL;
}

CMainDlg::~CMainDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}

	if (v_linkCtrl != NULL)
	{
		delete v_linkCtrl;
		v_linkCtrl = NULL;
	}
	if (v_linkCtrl1 != NULL)
	{
		delete v_linkCtrl1;
		v_linkCtrl1 = NULL;
	}

	if (v_linkCtrl2 != NULL)
	{
		delete v_linkCtrl2;
		v_linkCtrl2 = NULL;
	}

	if (v_linkCtrl3 != NULL)
	{
		delete v_linkCtrl3;
		v_linkCtrl3 = NULL;
	}

	if (v_linkCtrlQQ != NULL)
	{
		delete v_linkCtrlQQ;
		v_linkCtrlQQ = NULL;
	}

	if (v_linkCtrlBlock != NULL)
	{
		delete v_linkCtrlBlock;
		v_linkCtrlBlock = NULL;
	}	
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALLTXDETAIL , m_rBtnAllTxdetail);
	DDX_Control(pDX, IDC_TX_JY2 , m_strTrading);
	DDX_Control(pDX, IDC_TX_JY3 , m_strTrading2);
	DDX_Control(pDX, IDC_TX_JY4 , m_strTrading3);
	DDX_Control(pDX, IDC_TX_JY5 , m_strTrading4);
	DDX_Control(pDX, IDC_TX_JY7 , m_strTrading5);

	DDX_Control(pDX, IDC_TX1 , m_strTx1);
	DDX_Control(pDX, IDC_TX2 , m_strTx2);
	DDX_Control(pDX, IDC_TX3 , m_strTx3);
	DDX_Control(pDX, IDC_TX4 , m_strTx4);
	DDX_Control(pDX, IDC_TX5 , m_strTx5);

	DDX_Control(pDX, IDC_STATIC_AMOUNT , m_strOver);
	DDX_Control(pDX, IDC_STATIC_NOTCOF , m_strOking);
	DDX_Control(pDX, IDC_STATIC_COUNT , m_strTranNum);
	//DDX_Control(pDX, IDC_STATIC_NOTCOF_RECV , m_strRecvOking);

	DDX_Control(pDX, IDC_STATIC_DES1 , m_strDesTx1);
	DDX_Control(pDX, IDC_STATIC_DES2 , m_strDesTx2);
	DDX_Control(pDX, IDC_STATIC_DES3 , m_strDesTx3);
	DDX_Control(pDX, IDC_STATIC_DES4 , m_strDesTx4);
	DDX_Control(pDX, IDC_STATIC_DES5 , m_strDesTx5);
    DDX_Control(pDX, IDC_STATIC_WALLET , m_sysWallet);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogBar)
	ON_BN_CLICKED(IDC_ALLTXDETAIL, &CMainDlg::OnBnClickedAlltxdetail)
	ON_MESSAGE(MSG_USER_MAIN_UI , &CMainDlg::OnShowListCtorl )
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//ON_BN_CLICKED(IDC_BUTTON_IMPORTWALLET, &CMainDlg::OnBnClickedButtonImportwallet)
	//ON_BN_CLICKED(IDC_BUTTON_DUMPWALLET, &CMainDlg::OnBnClickedButtonDumpwallet)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

void CMainDlg::SetBkBmpNid( UINT nBitmapIn ) 
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

void CMainDlg::OnBnClickedAlltxdetail()
{
	// TODO: 在此添加控件通知处理程序代码
	CDacrsUIDlg* pDlg = (CDacrsUIDlg*)GetParent();
	if ( NULL != pDlg ) {
		pDlg->ShowDialog(CTradDlg::IDD) ;
		pDlg->ShowStateTip(IDC_BUTTON_TRAD_INFO);
	}
}

void CMainDlg::OnnitCtrlText()
{

	ClearCtrlText();
	string strCommand,strShowData;
	strCommand  = "0";

	double dmoney =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_wallet_address") ,_T("money"), _T(" 1=1 "));

	if (-1 == dmoney)
	{
		m_strOver.SetWindowText(_T("0.0")) ;
	}else{
		string strMoney;
		strMoney = strprintf("%.3lf", dmoney);
		strMoney = CSoyPayHelp::getInstance()->DisPlayMoney(strMoney);
		m_strOver.SetWindowText(strMoney.c_str()) ;
	}

	string strCond;

	//add by lbf 2016-06-02
	//加入接收确认
	strCond = " confirm_height = 0 and state = 2";
	
	dmoney =  theApp.m_SqliteDeal.GetTableItemSum("t_transaction" , _T("money") , strCond) ;


	/*if (dmoney<0)
	{
		GetDlgItem(IDC_STATIC_NOTCOF_RECV)->SetWindowText(_T("0.0")) ;
	}else{
		string strMoney;
		strMoney = strprintf("%.3lf", dmoney);
		GetDlgItem(IDC_STATIC_NOTCOF_RECV)->SetWindowText(strMoney.c_str()) ;
	}*/

	//加入接收确认
	strCond=" confirm_height = 0 and state = 1";

	dmoney =  theApp.m_SqliteDeal.GetTableItemSum("t_transaction" , _T("money") , strCond) ;


	if (dmoney<0)
	{
		GetDlgItem(IDC_STATIC_NOTCOF)->SetWindowText(_T("0.0")) ;
	}else{
		string strMoney;
		strMoney = strprintf("%.3lf", dmoney);
		GetDlgItem(IDC_STATIC_NOTCOF)->SetWindowText(strMoney.c_str()) ;
	}
	


	int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction"), _T(" 1=1 ")) ;


	strCommand = strprintf("%d",nItem);
	m_strTranNum.SetWindowText(strCommand.c_str()) ;

	uistruct::TRANSRECORDLIST pTransaction;
	string strSource;
	strCond=" tx_type='COMMON_TX' order by confirmed_time desc limit 5";
	theApp.m_SqliteDeal.GetTransactionList(strCond,&pTransaction);


	int i = 1;
	strCommand=strprintf("IDC_TX%d",nItem);
	int item = IDC_TX1;
	int item1 = IDC_TX_JY2;

	int desitem = IDC_STATIC_DES1;
	string desStr ="";
	if (pTransaction.size() != 0  ) {
		int nSubIdx = 0 , i = 0 ;
		string strShowaddr ;
		std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
		for (const_it = pTransaction.begin() ; const_it != pTransaction.end()&&i<6 ; const_it++ ) {
			
			strSource = strprintf("%.4f",const_it->money);
			strShowaddr= strprintf("%s",const_it->addr.c_str());
			desStr = strprintf("%s",const_it->desaddr.c_str());
			
			if(item <=IDC_TX5)
			GetDlgItem(item)->SetWindowText(strShowaddr.c_str()) ;
			if(item1 <=IDC_TX_JY7)
			GetDlgItem(item1)->SetWindowText(strSource.c_str()) ;

			if (desitem <=IDC_STATIC_DES5)
			{
				GetDlgItem(desitem)->SetWindowText(desStr.c_str());
			}
			item++;
			item1++;
			desitem++;
		}
	}
}
void CMainDlg::SetCtrlText()
{

//	ClearCtrlText();
	CPostMsg postmsg;
	if (!theApp.m_UiManDlgQueue.pop(postmsg))
	{
		LogPrint("INFO","m_UiManDlgQueue Emptry\r\n");
		return ;
	}

	uistruct::MINDLG_T maindlg; 
	string strTemp = postmsg.GetData();
	maindlg.JsonToStruct(strTemp.c_str());


	string strCommand,strShowData;

	string strMoney = "";
	strMoney = strprintf("%s",maindlg.money.c_str());//)= _T(maindlg.money.c_str());
	strMoney = CSoyPayHelp::getInstance()->DisPlayMoney(strMoney);
	GetDlgItem(IDC_STATIC_AMOUNT)->SetWindowText(strMoney.c_str());
	//strMoney = _T(maindlg.unconfirmmoney.c_str());
	strMoney = strprintf("%s",maindlg.unconfirmmoney.c_str());
	strMoney = CSoyPayHelp::getInstance()->DisPlayMoney(strMoney);
	GetDlgItem(IDC_STATIC_NOTCOF)->SetWindowText(strMoney.c_str());

	/*strMoney = strprintf("%s",maindlg.unconfirmrecvmoney.c_str());
	strMoney = CSoyPayHelp::getInstance()->DisPlayMoney(strMoney);
	GetDlgItem(IDC_STATIC_NOTCOF_RECV)->SetWindowText(strMoney.c_str());*/


	m_strTranNum.SetWindowText(maindlg.itemcount.c_str()) ;

	string strDes = "";
	string addr1 = maindlg.addr1;
	uistruct::REVTRANSACTION_t temp;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		//if (temp.state == 1)
		//{
		//	strCommand.Format(_T("-%.8f"),temp.money*COIN);
		//	strShowData.Format(_T("%s "),temp.addr.c_str());
		//	m_strTrading.SetTextColor(RGB(255,0,0));
		//}else if (temp.state == 2)
		//{
		//	strCommand.Format(_T("+%.8f"),temp.money*COIN);
		//	strShowData.Format(_T("%s "),temp.desaddr.c_str());
		//	m_strTrading.SetTextColor(RGB(166,162,247));
		//}

		strCommand =strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes = strprintf("%s",temp.desaddr.c_str());
		
		GetDlgItem(IDC_STATIC_DES1)->SetWindowText(strDes.c_str()) ;
		GetDlgItem(IDC_TX1)->SetWindowText(strShowData.c_str()) ;
		GetDlgItem(IDC_TX_JY2)->SetWindowText(strCommand.c_str()) ;
	}
	
	addr1 = maindlg.addr2;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);

		strCommand = strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes = strprintf("%s",temp.desaddr.c_str());

		GetDlgItem(IDC_STATIC_DES2)->SetWindowText(strDes.c_str()) ;
		GetDlgItem(IDC_TX2)->SetWindowText(strShowData.c_str()) ;
		GetDlgItem(IDC_TX_JY3)->SetWindowText(strCommand.c_str()) ;
	}

	addr1 = maindlg.addr3;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);

		strCommand = strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes = strprintf("%s",temp.desaddr.c_str());

		GetDlgItem(IDC_STATIC_DES3)->SetWindowText(strDes.c_str()) ;
		GetDlgItem(IDC_TX3)->SetWindowText(strShowData.c_str()) ;
		GetDlgItem(IDC_TX_JY4)->SetWindowText(strCommand.c_str()) ;
		//GetDlgItem(IDC_STATIC_TARGET3)->ShowWindow(SW_HIDE);
	}
	addr1 = maindlg.addr4;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		strCommand = strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes =strprintf("%s",temp.desaddr.c_str());

		GetDlgItem(IDC_STATIC_DES4)->SetWindowText(strDes.c_str()) ;
		GetDlgItem(IDC_TX4)->SetWindowText(strShowData.c_str()) ;
		GetDlgItem(IDC_TX_JY5)->SetWindowText(strCommand.c_str()) ;
		//GetDlgItem(IDC_STATIC_TARGET4)->ShowWindow(SW_HIDE);
	}
	addr1 = maindlg.addr5;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		strCommand = strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes = strprintf("%s",temp.desaddr.c_str());

		GetDlgItem(IDC_STATIC_DES5)->SetWindowText(strDes.c_str()) ;
		GetDlgItem(IDC_TX5)->SetWindowText(strShowData.c_str()) ;
		GetDlgItem(IDC_TX_JY7)->SetWindowText(strCommand.c_str()) ;
	}
	Invalidate();
}
BOOL CMainDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes =  CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		m_rBtnAllTxdetail.LoadBitmaps(UiFun::GetBmpId(IDB_BITMAP_ALLTRADE1,IDB_BITMAP_ALLTRADE1EN,theApp.language()),UiFun::GetBmpId(IDB_BITMAP_ALLTRADE1,IDB_BITMAP_ALLTRADE1EN,theApp.language()),UiFun::GetBmpId(IDB_BITMAP_ALLTRADE1,IDB_BITMAP_ALLTRADE1EN,theApp.language()),UiFun::GetBmpId(IDB_BITMAP_ALLTRADE1,IDB_BITMAP_ALLTRADE1EN,theApp.language()));
		//m_rBtnImportWallet.LoadBitmaps(IDB_BITMAP_IMPORTWALLET,IDB_BITMAP_IMPORTWALLET,IDB_BITMAP_IMPORTWALLET,IDB_BITMAP_IMPORTWALLET);
		//m_rBtnDumpWallet.LoadBitmaps(IDB_BITMAP_DUMPWALLET,IDB_BITMAP_DUMPWALLET,IDB_BITMAP_DUMPWALLET,IDB_BITMAP_DUMPWALLET);

		UpdateData(0);
		//SetShowCtrol();
		OnnitCtrlText();
		GetUrlServer();
		onnitLinkText();
		m_strOver.SetFont(120, _T("黑体"));
		m_strOking.SetFont(120, _T("黑体"));
		//m_strRecvOking.SetFont(120, _T("黑体"));
		m_strTranNum.SetFont(120, _T("黑体"));

		m_strTrading.SetFont(95, _T("Calibri"));
		m_strTrading2.SetFont(95, _T("Calibri"));
		m_strTrading3.SetFont(95, _T("Calibri"));
		m_strTrading4.SetFont(95, _T("Calibri"));
		m_strTrading5.SetFont(95, _T("Calibri"));

		m_strTx1.SetFont(88, _T("Calibri"));
		m_strTx2.SetFont(88, _T("Calibri"));
		m_strTx3.SetFont(88, _T("Calibri"));
		m_strTx4.SetFont(88, _T("Calibri"));
		m_strTx5.SetFont(88, _T("Calibri"));

		m_strDesTx1.SetFont(88, _T("Calibri"));
		m_strDesTx2.SetFont(88, _T("Calibri"));
		m_strDesTx3.SetFont(88, _T("Calibri"));
		m_strDesTx4.SetFont(88, _T("Calibri")); 
		m_strDesTx5.SetFont(88, _T("Calibri")); 


		//加载指定位图资源 Bmp图片ID
		HBITMAP hBitmap; 
		hBitmap = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(UiFun::GetBmpId(IDB_BITMAP_WALLET,IDB_BITMAP_WALLETEN,theApp.language()))); 
		m_sysWallet.SetBitmap(hBitmap);

		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_MAIN_UI ) ;
	}
	return bRes ;
}
LRESULT CMainDlg::OnShowListCtorl( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	if (wParam == WM_UPWALLET)
	{
		GetDlgItem(IDC_STATIC_WALLET)->ShowWindow(SW_HIDE);
	}else{
		SetCtrlText();
	}
	
	return 0 ;
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//SetBkBmpNid( IDB_BITMAP_MAINUI_BJ ) ;
	SetBkBmpNid(UiFun::GetBmpId(IDB_BITMAP_MAINUI_BJ,IDB_BITMAP_MAINUI_BJEN,theApp.language()));
	m_strOver.SetFont(90, _T("Arial"));				//设置显示字体和大小
	m_strOking.SetFont(90, _T("Arial"));
	//m_strRecvOking.SetFont(90, _T("Arial"));
	m_strTranNum.SetFont(90, _T("Arial"));	
	if (v_linkCtrl == NULL)
	{
		v_linkCtrl = new CMFCLinkCtrl;
		v_linkCtrl->Create(_T(""), 
			WS_VISIBLE, 
			CRect(5, 5, 80, 50), 
			this, 
			IDC_MFCLINK1);
	}

	if (v_linkCtrl1 == NULL)
	{
		v_linkCtrl1 = new CMFCLinkCtrl;
		v_linkCtrl1->Create(_T(""), 
			WS_VISIBLE, 
			CRect(5, 5, 80, 50), 
			this, 
			IDC_MFCLINK3);
	}

	if (v_linkCtrl2 == NULL)
	{
		v_linkCtrl2 = new CMFCLinkCtrl;
		v_linkCtrl2->Create(_T(""), 
			WS_VISIBLE, 
			CRect(5, 5, 80, 50), 
			this, 
			IDC_MFCLINK4);
	}

	if (v_linkCtrl3 == NULL)
	{
		v_linkCtrl3 = new CMFCLinkCtrl;
		v_linkCtrl3->Create(_T(""), 
			WS_VISIBLE, 
			CRect(5, 5, 80, 50), 
			this, 
			IDC_MFCLINK5);
	}

	if (v_linkCtrlQQ == NULL)
	{
		v_linkCtrlQQ = new CMFCLinkCtrl;
		v_linkCtrlQQ->Create(_T(""), 
			WS_VISIBLE, 
			CRect(5, 5, 80, 50), 
			this, 
			IDC_MFCLINK_QQ);
	}

	if (v_linkCtrlBlock == NULL)
	{
		v_linkCtrlBlock = new CMFCLinkCtrl;
		v_linkCtrlBlock->Create(_T(""), 
			WS_VISIBLE, 
			CRect(5, 5, 80, 50), 
			this, 
			IDC_MFCLINK_BLOCK);
	}
	return 0;
}
bool CMainDlg::GetUrlServer()
{
	m_url.clear();
	CString url(_T("http://www.dacrs.com/dacrs/forumupdata.json"));    
	CInternetSession session;
	std::string strHtml;

	try
	{
		CHttpFile* pfile = (CHttpFile*)session.OpenURL(url,1,INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_RELOAD,NULL,0);

		DWORD dwStatusCode;    
		pfile->QueryInfoStatusCode(dwStatusCode);    
		if(dwStatusCode == HTTP_STATUS_OK)    
		{    
			char strBuff[1025] = {0};
			while ((pfile->Read((void*)strBuff, 1024)) > 0) 
			{ 
				strHtml += strBuff; 
			} 
		}
		else
		{
			return false;
		}

		pfile->Close();    
		delete pfile;    
		session.Close();
	}
	catch (CException* e)
	{
		e;//消除警告
		return false;
	}

	if (strHtml.empty())
	{
		CStdioFile myFile;
		CString strLine; 
		string strpath = theApp.str_InsPath;
		strpath +="\\dacrsUpdate.json";
		if(myFile.Open((LPCTSTR)(LPSTR)strpath.c_str(), CFile::modeRead))
		{
			while(myFile.ReadString(strLine))
			{
				strHtml +=strprintf("%s",strLine);
			}
			//读取
			myFile.Close();
		}else{
			return false;
		}
	}else{
			//创建
			CStdioFile  File;
			string strpath = theApp.str_InsPath;
			strpath +="\\dacrsUpdate.json";
			File.Open((LPCTSTR)(LPSTR)strpath.c_str(),CFile::modeWrite | CFile::modeCreate);  
			File.WriteString(strHtml.c_str());
			File.Close();
	}


	Json::Reader reader;  
	Json::Value root; 

	//if (reader.parse(strHtml, root)) 
	//{
	//	int index = root.size();
	//	for (int i = 0;i <index;i++)
	//	{
	//		Json::Value  msgroot = root[i];
	//		CString key = msgroot["msn"].asCString();
	//		CString valuetemp = msgroot["url"].asCString();
	//		m_url[key] = valuetemp;
	//	}
	//	//strVersion = root["version"].asString();
	//	//ShellExecuteW(NULL, L"open", _T("http://bbs.dspay.org/portal.php"), NULL, NULL, SW_SHOWNORMAL);
	//	return true;
	//}

	if (reader.parse(strHtml, root))
	{
		if (!root.isObject())
		{
			return false;
		}
		if (theApp.gsLanguage == 2)
		{
			Json::Value rooten = root["English"];
			if (rooten.isNull() && !rooten.isArray())
			{
				return false;
			}
			int index = rooten.size();
			for (int i = 0;i <index;i++)
			{
				Json::Value  msgroot = rooten[i];
				Json::Value value = msgroot["msn"];
				if (value.isNull())
				{
					return false;
				}
				CString key = msgroot["msn"].asCString();
				value = msgroot["url"];
				if (value.isNull())
				{
					return false;
				}
				CString valuetemp = msgroot["url"].asCString();
				m_url[key] = valuetemp;
			}
			return true;
		}else{
			Json::Value rootcn = root["Chinese"];
			if (rootcn.isNull()&& !rootcn.isArray())
			{
				return false;
			}
			int index = rootcn.size();
			for (int i = 0;i <index;i++)
			{
				Json::Value  msgroot = rootcn[i];
				Json::Value value = msgroot["msn"];
				if (value.isNull())
				{
					return false;
				}
				CString key = msgroot["msn"].asCString();
				value = msgroot["url"];
				if (value.isNull())
				{
					return false;
				}
				CString valuetemp = msgroot["url"].asCString();
				m_url[key] = valuetemp;
			}
				return true;
		}
	}
	return false;
}

void CMainDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogBar::OnPaint()
}


BOOL CMainDlg::OnEraseBkgnd(CDC* pDC)
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
void CMainDlg::ClearCtrlText()
{
	//SetShowCtrol();
	GetDlgItem(IDC_STATIC_AMOUNT)->SetWindowText(_T("")) ;

	GetDlgItem(IDC_STATIC_NOTCOF)->SetWindowText(_T("")) ;

	//GetDlgItem(IDC_STATIC_NOTCOF_RECV)->SetWindowTextA(_T(""));

	m_strTranNum.SetWindowText(_T("")) ;


	GetDlgItem(IDC_STATIC_DES1)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_DES2)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_STATIC_DES3)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_STATIC_DES4)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_STATIC_DES5)->SetWindowText(_T("")) ;

	GetDlgItem(IDC_TX1)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX2)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX3)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX4)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX5)->SetWindowText(_T("")) ;

	GetDlgItem(IDC_TX_JY2)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX_JY3)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX_JY4)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX_JY5)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX_JY7)->SetWindowText(_T("")) ;
}

void CMainDlg::onnitLinkText()
{
   v_linkCtrl->SetWindowText(_T(""));
   v_linkCtrl1->SetWindowText(_T(""));
   v_linkCtrl2->SetWindowText(_T(""));
   v_linkCtrl3->SetWindowText(_T(""));
   v_linkCtrlQQ->SetWindowText(UiFun::UI_LoadString("WALLET_MODULE" , "WALLET_OFFICIAL_CUSTOMER" , theApp.gsLanguage ));
   v_linkCtrlQQ->SetURL("http://wpa.qq.com/msgrd?v=3&uin=1578215488&site=qq&menu=yes");

   v_linkCtrlBlock->SetWindowText("Block chain");
   v_linkCtrlBlock->SetURL("http://block.dacrs.com/");
   int i = 1;
	map<CString,CString>::iterator it;
	for(it=m_url.begin();it!=m_url.end();++it)
	{
		if (i == 1)
		{
			v_linkCtrl->SetWindowText(it->first);
			v_linkCtrl->SetURL(it->second);
		}
		if (i == 2)
		{
			v_linkCtrl1->SetWindowText(it->first);
			v_linkCtrl1->SetURL(it->second);
		}
		if (i == 3)
		{
			v_linkCtrl2->SetWindowText(it->first);
			v_linkCtrl2->SetURL(it->second);
		}
		if (i == 4)
		{
			v_linkCtrl3->SetWindowText(it->first);
			v_linkCtrl3->SetURL(it->second);
			break;
		}
		i++;
	}
		
}


void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;

		CRect rc ;
		GetClientRect( rc ) ;


		//CWnd *pst = GetDlgItem( IDC_BUTTON_IMPORTWALLET ) ;
		//if ( NULL != pst ) {
		//	CRect rect ;
		//	pst->GetClientRect( rect ) ;
		//	pst->SetWindowPos( NULL , rc.Width()- 2*rect.Width()-10 , rc.Height()-rect.Height()-10 , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		//}
		//pst = GetDlgItem( IDC_BUTTON_DUMPWALLET ) ;
		//if ( NULL != pst ) {
		//	CRect rect ;
		//	pst->GetClientRect( rect ) ;
		//	pst->SetWindowPos( NULL ,rc.Width()- rect.Width() ,rc.Height()-rect.Height()-10  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		//}

		
			//
		//	
		CWnd *pst = GetDlgItem( IDC_STATIC_AMOUNT ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*15 ,(rc.Height()/100)*17+2  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_NOTCOF ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*15 ,(rc.Height()/100)*25+2  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		/*pst = GetDlgItem(IDC_STATIC_NOTCOF_RECV);
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*15 ,(rc.Height()/100)*42  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}*/

		pst = GetDlgItem( IDC_STATIC_COUNT ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*15 ,(rc.Height()/100)*34  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}
		 /////////////////////////////////////////////////////////////////////////////////////////
		pst = GetDlgItem( IDC_MFCLINK1 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*63 ,(rc.Height()/100)*15  , 294, 20  ,SWP_SHOWWINDOW ) ; 
		}


		pst = GetDlgItem( IDC_MFCLINK3 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*63 ,(rc.Height()/100)*23 , 294, 20  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_MFCLINK4 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*63 ,(rc.Height()/100)*30, 294, 20  ,SWP_SHOWWINDOW ) ; 
		}


		pst = GetDlgItem( IDC_MFCLINK5 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*63 ,(rc.Height()/100)*38, 294, 20 ,SWP_SHOWWINDOW ) ; 
		}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 
	////	 
	////	 
	////	 
		pst = GetDlgItem( IDC_TX1 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*4 ,(rc.Height()/100)*67  , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_DES1 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*35+4 ,(rc.Height()/100)*67  , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}
		//pst = GetDlgItem( IDC_STATIC_TARGET1 ) ;
		//if ( NULL != pst ) {
		//	CRect rect ;
		//	pst->GetClientRect( rect ) ;
		//	pst->SetWindowPos( NULL ,(rc.Width()/100)*33+4 ,(rc.Height()/100)*67  ,  (rc.Width()/100)*3, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		//}
		pst = GetDlgItem( IDC_TX_JY2 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*66 ,(rc.Height()/100)*67  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_TX2 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*4 ,(rc.Height()/100)*75 , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_DES2 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*35+4 ,(rc.Height()/100)*75  , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		//pst = GetDlgItem( IDC_STATIC_TARGET2) ;
		//if ( NULL != pst ) {
		//	CRect rect ;
		//	pst->GetClientRect( rect ) ;
		//	pst->SetWindowPos( NULL ,(rc.Width()/100)*33+4 ,(rc.Height()/100)*75  , (rc.Width()/100)*3, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		//}

		pst = GetDlgItem( IDC_TX_JY3 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*66 ,(rc.Height()/100)*75  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_TX3 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*4 ,(rc.Height()/100)*82 , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_DES3 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*35+4 ,(rc.Height()/100)*82  , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}


		pst = GetDlgItem( IDC_TX_JY4 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*66 ,(rc.Height()/100)*82  , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_TX4 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*4 ,(rc.Height()/100)*89 , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_DES4 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*35+4 ,(rc.Height()/100)*89  , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}


		pst = GetDlgItem( IDC_TX_JY5 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*66 ,(rc.Height()/100)*89 ,  (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_TX5 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*4 ,(rc.Height()/100)*97 , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_DES5 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*35+4 ,(rc.Height()/100)*97  , (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}


		pst = GetDlgItem( IDC_TX_JY7 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*66 ,(rc.Height()/100)*97 ,  (rc.Width()/100)*30, rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_MFCLINK_QQ ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,rc.Width()-180 ,rc.Height()-20-10   ,180, 20  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_MFCLINK_BLOCK ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,rc.Width()-180 ,rc.Height()-2*20-10   , 180, 20  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_ALLTXDETAIL ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*63 ,rc.Height()*9/10  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

	}
}
void CMainDlg::SetShowCtrol()
{
	/*GetDlgItem(IDC_STATIC_TARGET1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TARGET2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TARGET3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TARGET4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TARGET5)->ShowWindow(SW_HIDE);*/
}


