// SendRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SendRecord.h"
#include "afxdialogex.h"


// CSendRecord 对话框

IMPLEMENT_DYNAMIC(CSendRecord, CDialogEx)

CSendRecord::CSendRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendRecord::IDD, pParent)
{
	m_pBmp = NULL ;
	m_addr = _T("");
	m_pagecount = 0;
	m_curpage = 0;
	m_PoolList.clear();
	m_pagesize = 4;
}

CSendRecord::~CSendRecord()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CSendRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_listBox);
	DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
}


BEGIN_MESSAGE_MAP(CSendRecord, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_MESSAGE( WM_BN_CLICK, &CSendRecord::onBnCLick)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UP, &CSendRecord::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CSendRecord::OnBnClickedButtonNext)
END_MESSAGE_MAP()


// CSendRecord 消息处理程序
void CSendRecord::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CSendRecord::OnEraseBkgnd(CDC* pDC)
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


int CSendRecord::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid(IDB_BITMAP_SEND_BET) ;
	return 0;
}


void CSendRecord::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		CWnd *pst = GetDlgItem( IDC_LIST_BOX ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,0 , 33 , 875,130 ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_UP ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-140 , 130+33 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875 - 115, 130+33 , 40, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_NEXT ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-70 , 130+33 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_STATIC_COUNT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-50 ,130+36 , 50, 20  ,SWP_SHOWWINDOW ) ; 
		}
	}
}


BOOL CSendRecord::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogEx::Create(lpszTemplateName, pParentWnd);
	if ( bRes ) { 
		UpdateData(0);

		m_sCountpage.SetFont(90, _T("黑体"));				//设置显示字体和大小
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_sCountpage.SetWindowText(_T("共:"));

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

		GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(_T("共:0"));
	}
	return bRes ;
}

LRESULT CSendRecord::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	List_SendAppendData* pinf = m_listBox.GetAppendDataInfo((int)wParam);
	if ( NULL != pinf ) { 
		CString hash = pinf->pstr;
		//OpenBet(hash);
	}
	
	return 0;

}
void CSendRecord::Showlistbox(CString address)
{
	//// 查找数据库中是否存在此记录
	m_addr = address;
	m_curpage = 0;
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(_T(""));
	m_PoolList.clear();
	CString conditon;
	conditon.Format(_T("left_addr ='%s' and (actor = 0 or actor = 2) order by send_time desc") , address);
	//uistruct::P2PBETRECORDLIST  pPoolItem;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordList(conditon ,&m_PoolList ) ;
	m_pagecount = (m_PoolList.size()%m_pagesize)==0?(m_PoolList.size()/m_pagesize):(m_PoolList.size()/m_pagesize)+1;

	CString temp;
	temp.Format(_T("共:%d"),m_pagecount);
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	Invalidate();
	m_listBox.DeleteAllIndex();
	OnShowPagePool(1);
	//if (pPoolItem.size() != 0) ///此记录不存在,插入记录
	//{
	//	int i = 0;
	//	std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	//	for (const_it = pPoolItem.begin();const_it!=pPoolItem.end();const_it++)
	//	{
	//		CString sendaddr,acceptaddr;
	//		sendaddr.Format(_T("%s"),const_it->left_addr);
	//		acceptaddr.Format(_T("%s"),const_it->right_addr);

	//		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it->send_time);
	//		CString SendTime;
	//		SendTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

	//		CString dmoney,reward,result,guess;
	//		if (const_it->content[32] == 1)
	//		{
	//			result.Format(_T("%s"),"妹");
	//		}else
	//		{
	//			result.Format(_T("%s"),"哥");
	//		}
	//
	//		reward.Format(_T("%.4f"),const_it->amount);
	//		m_listBox.InsertStr(i,this->GetSafeHwnd());
	//		m_listBox.SetIndexInage(i , IDB_BITMAP_P2P_LISTBOX_BUT);
	//		m_listBox.SetIndexBackCol(i, 0, RGB(242,32,32));

	//		List_SendAppendData* pinf = m_listBox.GetAppendDataInfo(i);
	//		pinf->pBut0->EnableWindow(false);

	//		int rrrr = const_it->content[32];
	//		///说明开奖了
	//		if (const_it->state == 2 || const_it->state == 1)
	//		{
	//			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it->recv_time);
	//			CString strTime;
	//			strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

	//			if (const_it->guess_num == 1)
	//			{
	//				guess.Format(_T("%s"),"妹");
	//			}else
	//			{
	//				guess.Format(_T("%s"),"哥");
	//			}

	//			if (const_it->guess_num == const_it->content[32])
	//			{
	//				reward.Format(_T("-%.4f"),const_it->amount);
	//			}else
	//			{
	//				reward.Format(_T("+%.4f"),const_it->amount);
	//			}
	//		
	//			CString recaddr;
	//			recaddr.Format(_T("%s"),const_it->right_addr);
	//			if (const_it->state == 2)
	//			{
	//				m_listBox.SetIndexString(i ,sendaddr, acceptaddr,SendTime,strTime, result,guess, reward,_T("已开"),const_it->tx_hash);
	//			}else{
	//				if ((const_it->time_out + const_it->height)> theApp.blocktipheight && theApp.IsSyncBlock)
	//				{
	//					pinf->pBut0->EnableWindow(true);
	//					m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,strTime, result,_T("--"),reward, _T("开"),const_it->tx_hash);
	//				}else if(theApp.IsSyncBlock){
	//					reward.Format(_T("-%.4f"),const_it->amount);
	//					m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,strTime, result,guess,reward, _T("超时"),const_it->tx_hash);
	//				}else{
	//					pinf->pBut0->EnableWindow(true);
	//					m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,strTime, result,_T("--"),reward, _T("开"),const_it->tx_hash);
	//				}
	//				
	//			}
	//			

	//		}else
	//		{
	//			reward.Format(_T("%.4f"),const_it->amount);
	//			if (const_it->state == 0 &&(500 + const_it->height)> theApp.blocktipheight&& theApp.IsSyncBlock )
	//			{
	//				m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,_T("--"), result,_T("--"),reward,_T("未接"),const_it->tx_hash);
	//			}else if(theApp.IsSyncBlock){
	//				reward.Format(_T("-%.4f"),const_it->amount);
	//				m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,_T("--"), result,_T("--"),reward,_T("超时"),const_it->tx_hash);
	//			}else
	//			{
	//				m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,_T("--"), result,_T("--"),reward,_T("未接"),const_it->tx_hash);
	//			}
	//			
	//		}
	//		i++;
	//	}
	//}
}
void CSendRecord::OpenBet(CString txhash)
{
	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("同步未完成,不能发送交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	if (m_addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("请选择地址") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	CString conditon;
	conditon.Format(_T("tx_hash ='%s'") , txhash );
	uistruct::P2P_QUIZ_RECORD_t pPoolItem;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
	if (strlen(pPoolItem.tx_hash) == 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("数据库中无此记录") , _T("提示") , MB_ICONINFORMATION ) ;
	}

	CString strCommand1;
	strCommand1.Format(_T("%s %s"),_T("gethash") , pPoolItem.content );
	CStringA strShowData1 ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand1,strShowData1);
	int pos1 = strShowData1.Find("hash");
	if ( pos1 < 0 ) return ;

	Json::Reader reader1;  
	Json::Value root1; 
	if (!reader1.parse(strShowData1.GetString(), root1)) 
		return  ;

	CString strHash1 ;
	strHash1.Format(_T("%s") ,  root1["hash"].asCString() ) ;

	TRACE(_T("contect:%s\r\n"),pPoolItem.content);
	TRACE(_T("open:%s\r\n"),strHash1);

	CString randnumber,strShowData,txaccepthash;
	randnumber.Format(_T("%s"),pPoolItem.content);
	txaccepthash.Format(_T("%s"),pPoolItem.relate_hash);
	string sendhash = CSoyPayHelp::getInstance()->GetReverseHash(txhash.GetBuffer());
	string accepthash = CSoyPayHelp::getInstance()->GetReverseHash(txaccepthash.GetBuffer());
	
	string number;
	number.assign(pPoolItem.content,pPoolItem.content+sizeof(pPoolItem.content));
	string strContractData = m_P2PBetHelp.PacketP2PExposeContract(sendhash,number,accepthash,pPoolItem.time_out);

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid.GetBuffer(),m_addr.GetString(),strContractData,0,theApp.m_P2PBetCfg.OpenBetnFee,0);
	strShowData = _T("");
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	int pos = strShowData.Find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		CString strHash ;
		strHash.Format(_T("'%s'") , root["hash"].asCString() );
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜开奖成功!\n%s") , root["hash"].asCString() ) ;
		 strTip.Format( _T("恭喜开奖成功，请等待1-2分钟确认交易\n")) ;
	}else{
		strTip.Format( _T("开奖失败!") ) ;
	}

	//保存到数据库
	if ( bRes ) {

		//插入到交易记录数据库

		//// 查找数据库中是否存在此记录
		CString conditon;
		conditon.Format(_T("tx_hash ='%s'") , txhash );
		uistruct::P2P_QUIZ_RECORD_t pPoolItem;
		int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
		if (strlen(pPoolItem.tx_hash) == 0) ///此记录不存在,插入记录
		{
			CString strSourceData  , strW ;
			strSourceData.Format(_T("state = %d") , 5  ) ;
			strW.Format(_T("tx_hash = '%s'") , txhash ) ;

			uistruct::DATABASEINFO_t DatabaseInfo;
			DatabaseInfo.strSource = strSourceData.GetString();
			DatabaseInfo.strWhere = strW.GetString() ;
			DatabaseInfo.strTabName = _T("p2p_bet_record");
			CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
			string strtemp = DatabaseInfo.ToJson();
			CString pstr;
			pstr.Format(_T("%s"),strtemp.c_str());
			postmsg.SetData(pstr);
			theApp.m_MsgQueue.push(postmsg);


		}
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}

void  CSendRecord::OnShowPagePool(int page)
{
	if (page >m_pagecount || page == m_curpage || page <= 0)
	{
		return;
	}


	m_listBox.DeleteAllIndex();
	CString strpage;
	strpage.Format(_T("%d"),page);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage);

	bool flag = false;
	m_curpage = page;
	int index = (page-1)*m_pagesize;
	int count = (m_PoolList.size() -index)>=m_pagesize?m_pagesize:(m_PoolList.size() -index);

	int i = 0;
	std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	for (int k = index;k< (index+count);k++)
	{
		uistruct::P2P_QUIZ_RECORD_t const_it = m_PoolList.at(k);
		CString sendaddr,acceptaddr;
		sendaddr.Format(_T("%s"),const_it.left_addr);
		acceptaddr.Format(_T("%s"),const_it.right_addr);

		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it.send_time);
		CString SendTime;
		SendTime.Format("%02d-%02d %02d:%02d:%02d",curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

		CString dmoney,reward,result,guess;
		if (const_it.content[32] == 1)
		{
			result.Format(_T("%s"),"妹");
		}else
		{
			result.Format(_T("%s"),"哥");
		}

		reward.Format(_T("%.4f"),const_it.amount);
		m_listBox.InsertStr(i,this->GetSafeHwnd());
		m_listBox.SetIndexInage(i , IDB_BITMAP_P2PLIST);
		m_listBox.SetIndexBackCol(i, 1, RGB(242,32,32));
		m_listBox.SetIndexBackCol(i, 5, RGB(242,32,32));
		List_SendAppendData* pinf = m_listBox.GetAppendDataInfo(i);
		//pinf->pBut0->EnableWindow(false);

		int rrrr = const_it.content[32];
		///说明开奖了
		if (const_it.state == 2 || const_it.state == 1)
		{
			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it.recv_time);
			CString strTime;
			strTime.Format("%02d-%02d %02d:%02d:%02d",curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

			if (const_it.guess_num == 1)
			{
				guess.Format(_T("%s"),"妹");
			}else
			{
				guess.Format(_T("%s"),"哥");
			}

			if (const_it.guess_num == const_it.content[32])
			{
				m_listBox.SetIndexBackCol(i, 6, RGB(1,127,1));
				reward.Format(_T("-%.4f"),const_it.amount);
			}else
			{
				m_listBox.SetIndexBackCol(i, 6, RGB(242,32,32));
				reward.Format(_T("+%.4f"),const_it.amount);
			}

			CString recaddr;
			recaddr.Format(_T("%s"),const_it.right_addr);
			CString time = 0;
			curTime.wMinute = (curTime.wMinute+const_it.time_out)>60?(curTime.wMinute+const_it.time_out)-60:(curTime.wMinute+const_it.time_out);
			curTime.wHour = (curTime.wMinute+const_it.time_out)>60?curTime.wHour+1:curTime.wHour;
			time.Format("%02d:%02d:%02d",curTime.wHour, curTime.wMinute, curTime.wSecond);
			if (const_it.state == 2)
			{
				m_listBox.SetIndexString(i ,sendaddr, acceptaddr,SendTime,strTime, result,guess, reward,time,_T("已开"),const_it.tx_hash);
			}else{
				if ((const_it.time_out + const_it.height)> theApp.blocktipheight && theApp.IsSyncBlock)
				{
					//pinf->pBut0->EnableWindow(true);
					if (!flag)
					{
						CString strCond;
						uistruct::LISTADDR_t pAddr;
						strCond.Format(_T("reg_id ='%s'"),const_it.left_addr);
						theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &pAddr);
						double minfee = (theApp.m_P2PBetCfg.OpenBetnFee*1.0)/COIN;
						if (minfee > pAddr.fMoney)
						{
							::MessageBox(NULL ,_T("有些赌约未开奖,请先充值") , _T("提示") , MB_ICONINFORMATION ) ;
						}
						CString txhash;
						txhash.Format(_T("%s"),const_it.tx_hash);
						theApp.OpenBet(txhash);
						flag = true;
					}
					m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,strTime, result,_T("--"),reward,time, _T("待开"),const_it.tx_hash);
				}else if(theApp.IsSyncBlock && const_it.height != 0 &&(const_it.time_out + const_it.height)< theApp.blocktipheight){
					m_listBox.SetIndexBackCol(i, 6, RGB(1,127,1));
					reward.Format(_T("-%.4f"),const_it.amount);
					m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,strTime, result,guess,reward,time, _T("超时"),const_it.tx_hash);
				}else{
					m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,strTime, result,_T("--"),reward,time, _T("待开"),const_it.tx_hash);
				}

			}


		}else
		{
			reward.Format(_T("%.4f"),const_it.amount);
			if (const_it.state == 0 &&(500 + const_it.height)> theApp.blocktipheight&& theApp.IsSyncBlock )
			{
				m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,_T("--"), result,_T("--"),reward,_T(""),_T("未接"),const_it.tx_hash);
			}else if(theApp.IsSyncBlock&&const_it.height !=0 && (500 + const_it.height)< theApp.blocktipheight){
				reward.Format(_T("+%.4f"),const_it.amount);
				m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,_T("--"), result,_T("--"),reward,_T(""),_T("超时"),const_it.tx_hash);
			}else
			{
				m_listBox.SetIndexString(i , sendaddr, acceptaddr,SendTime,_T("--"), result,_T("--"),reward,_T(""),_T("未接"),const_it.tx_hash);
			}

		}
		i++;
	}
}
BOOL CSendRecord::PreTranslateMessage(MSG* pMsg)
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
					::MessageBox( this->GetSafeHwnd() ,_T("输入有误,请输入数字") , _T("提示") , MB_ICONINFORMATION ) ;
				}
				return TRUE;
			}
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSendRecord::OnBnClickedButtonUp()
{
	// TODO: 在此添加控件通知处理程序代码
	OnShowPagePool((m_curpage-1));
}


void CSendRecord::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	OnShowPagePool((m_curpage+1));
}
