// AcceptRedPacketRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "AcceptRedPacketRecord.h"
#include "afxdialogex.h"
#include "RedPacketList.h"
#include "DacrsUIDlg.h"
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorksheet.h"
#include "CWorkbooks.h"
#include "CWorksheets.h"
// CAcceptRedPacketRecord 对话框

IMPLEMENT_DYNAMIC(CAcceptRedPacketRecord, CDialogEx)

CAcceptRedPacketRecord::CAcceptRedPacketRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAcceptRedPacketRecord::IDD, pParent)
{
	m_pBmp = NULL ;
	m_pagecount = 0;
	m_curpage = 0;
	m_AcceptRedPacketList.clear();
	m_pagesize = 4;
}

CAcceptRedPacketRecord::~CAcceptRedPacketRecord()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CAcceptRedPacketRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_ListBox);
	DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
}


BEGIN_MESSAGE_MAP(CAcceptRedPacketRecord, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UP, &CAcceptRedPacketRecord::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CAcceptRedPacketRecord::OnBnClickedButtonNext)
	ON_LBN_DBLCLK(IDC_LIST_BOX, &CAcceptRedPacketRecord::OnLbnDblclkListBox)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAcceptRedPacketRecord 消息处理程序

void CAcceptRedPacketRecord::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CAcceptRedPacketRecord::OnEraseBkgnd(CDC* pDC)
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


int CAcceptRedPacketRecord::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//SetBkBmpNid(IDB_BITMAP_GRBREDPACKET);
	SetBkBmpNid(UiFun::GetBmpId(IDB_BITMAP_GRBREDPACKET,IDB_BITMAP_GRBREDPACKETEN,theApp.language()));
	return 0;
}


BOOL CAcceptRedPacketRecord::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogEx::Create(lpszTemplateName, pParentWnd);
	if ( bRes ){
		UpdateData(0);

		m_sCountpage.SetFont(90, _T("黑体"));				//设置显示字体和大小
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_sCountpage.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_TOTAL" ,theApp.gsLanguage));

		m_rBtnUp.SetBitmaps( IDB_BITMAP_RED_UP , RGB(255, 255, 0) , IDB_BITMAP_RED_UP , RGB(255, 255, 255) );
		m_rBtnUp.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnUp.SetWindowText("") ;
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnUp.SizeToContent();

		m_rBtnNext.SetBitmaps( IDB_BITMAP_RED_NEX , RGB(255, 255, 0) , IDB_BITMAP_RED_NEX , RGB(255, 255, 255) );
		m_rBtnNext.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNext.SetWindowText("") ;
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnNext.SizeToContent();

		GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_TOTAL" ,theApp.gsLanguage)+"0");
	}
	/// listbox 背景颜色
	//m_hbrush = CreateSolidBrush(RGB(255,252,247));RGB(255,236,229)
	m_hbrush = CreateSolidBrush(RGB(255,236,229));
	return bRes;
}


void CAcceptRedPacketRecord::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		CWnd *pst = GetDlgItem( IDC_LIST_BOX ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,0 , 25 , 875, 120  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_UP ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-140 , 120+25 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875 - 115, 120+25 , 40, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_NEXT ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-70 , 120+25 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_STATIC_COUNT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-50 ,120+28 , 50, 20  ,SWP_SHOWWINDOW ) ; 
		}

	}
}
void CAcceptRedPacketRecord::Showlistbox(CString address)
{
	//// 查找数据库中是否存在此记录
	//m_ListBox.DeleteAllIndex();
	m_AcceptRedPacketList.clear();
	m_curpage = 0;
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(_T(""));
	string conditon;
	conditon =strprintf("grab_acct_id ='%s' order by grab_time desc",address);

	int nItem =  theApp.m_SqliteDeal.GetRedPacketGrabRecordList(conditon.c_str() ,&m_AcceptRedPacketList ) ;

	m_pagecount = (m_AcceptRedPacketList.size()%m_pagesize)==0?(m_AcceptRedPacketList.size()/m_pagesize):(m_AcceptRedPacketList.size()/m_pagesize)+1;

	string temp;
	temp =strprintf("%s%d",UiFun::UI_LoadString("P2P_MODULE" , "P2P_TOTAL" ,theApp.gsLanguage),m_pagecount);

	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	Invalidate();
	m_ListBox.DeleteAllIndex();
	OnShowPagePool(1);
}


void  CAcceptRedPacketRecord::OnShowPagePool(int page)
{
	if (page >m_pagecount || page == m_curpage || page <= 0)
	{
		return;
	}


	m_ListBox.DeleteAllIndex();

	string strpage = strprintf("%d",page);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());

	m_curpage = page;
	int index = (page-1)*m_pagesize;
	unsigned int count = (m_AcceptRedPacketList.size() -index)>=m_pagesize?m_pagesize:(m_AcceptRedPacketList.size() -index);

	string amount,address,luckeMoney,strnum,type,luckeValue,operate,sendTime;
	int i = 0;
	std::vector<uistruct::REDPACKETGRAB_t>::const_iterator const_it;
	for (unsigned int k = index;k< (index+count) && k < m_AcceptRedPacketList.size();k++)
	{
		uistruct::REDPACKETGRAB_t const_it = m_AcceptRedPacketList.at(k);
		
		amount =strprintf("%.4f",const_it.total_amount);

		if (const_it.packet_type == 1)
		{
			//type="普通";
			type=UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_COMMTYPE" ,theApp.gsLanguage);
		}else if (const_it.packet_type == 2)
		{
			//type="接龙";
			type=UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_COMMTYPEL" ,theApp.gsLanguage);

		}

		if (const_it.grab_time == 0)
		{
			//luckeMoney = _T("--");
			luckeMoney="--";
			sendTime="--";

		}else{
			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it.grab_time);
			sendTime= strprintf("%02d-%02d %02d:%02d:%02d",curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			luckeMoney =strprintf("%.4f",const_it.lucky_amount);
		}

		if (const_it.lucky_fortune == 1)
		{
			//luckeValue="普通";
			luckeValue=UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_COMMTYPE" ,theApp.gsLanguage);
		}else if (const_it.lucky_fortune == 2)
		{
			//luckeValue="运气王";
			luckeValue=UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_LUCKLY" ,theApp.gsLanguage);
		}else
		{
			luckeValue="--";
		}
		strnum =strprintf("%d",const_it.total_num);
		m_ListBox.InsertStr(i,this->GetSafeHwnd());
		m_ListBox.SetIndexString(i , const_it.send_acc_id.c_str(),type.c_str(),sendTime.c_str(),amount.c_str(), strnum.c_str(),luckeValue.c_str(),luckeMoney.c_str());
	
		i++;
	}
}
BOOL CAcceptRedPacketRecord::PreTranslateMessage(MSG* pMsg)
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
					UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_INPUT_ERROR" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
				}
				return TRUE;
			}
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CAcceptRedPacketRecord::OnBnClickedButtonUp()
{
	// TODO: 在此添加控件通知处理程序代码
	OnShowPagePool((m_curpage-1));
}


void CAcceptRedPacketRecord::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	OnShowPagePool((m_curpage+1));
}


void CAcceptRedPacketRecord::OnLbnDblclkListBox()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel=m_ListBox.GetCurSel(); 
	int count = ((m_curpage -1)*m_pagesize) + nSel;
	uistruct::REDPACKETGRAB_t const_it = m_AcceptRedPacketList.at(count);

	if (const_it.send_hash != "")
	{
		theApp.m_strAddress.Format(_T("%s") ,const_it.send_hash.c_str() ) ;
	}else
	{		
		theApp.m_strAddress =_T("");
	}
	CRedPacketList dlg;
	dlg.DoModal();	
}


HBRUSH CAcceptRedPacketRecord::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (nCtlColor== CTLCOLOR_LISTBOX)
	{
		return m_hbrush; 
	} 
	else
	{
		return  CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CAcceptRedPacketRecord::GetExportCol(map<int,string> &item,uistruct::REDPACKETGRAB_t const_it)
{
	string sendTime,amount,Sendaddr,address,luckeMoney,strnum,type,luckeValue,operate;

	amount =strprintf("%.4f",const_it.total_amount);

	if (const_it.packet_type == 1)
	{
		//type="普通";
		type=UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_COMMTYPE" ,theApp.gsLanguage);
	}else if (const_it.packet_type == 2)
	{
		//type="接龙";
		type=UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_COMMTYPEL" ,theApp.gsLanguage);
	}

	if (const_it.grab_time == 0)
	{
		//luckeMoney = _T("--");
		luckeMoney="--";
		sendTime="--";

	}else{
		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it.grab_time);
		sendTime =strprintf("%02d-%02d %02d:%02d:%02d",curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		luckeMoney =strprintf("%.4f",const_it.lucky_amount);
	}

	if (const_it.lucky_fortune == 1)
	{
		//luckeValue="普通";
		luckeValue=UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_COMMTYPE" ,theApp.gsLanguage);
	}else if (const_it.lucky_fortune == 2)
	{
		//luckeValue="运气王";
		luckeValue =UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_LUCKLY" ,theApp.gsLanguage);
	}else
	{
		luckeValue="--";
	}

	strnum =strprintf("%d",const_it.total_num);

	int i = 0;
	item[i++]=const_it.grab_hash;
	item[i++]=const_it.grab_acct_id;
	item[i++]=const_it.send_acc_id;
	item[i++]=type;
	item[i++]=sendTime;
	item[i++]=amount;
	item[i++]=strnum;
	item[i++]=luckeValue;
	item[i++]=luckeMoney;
}

void CAcceptRedPacketRecord::ExportAcceptRedPacektToexel()
{
	// TODO: 在此添加控件通知处理程序代码
	string conditon;
	conditon ="where 1=1 order by grab_time desc";
	uistruct::REDPACKETGRABLIST  AcceptRedPacketList;
	int nItem =  theApp.m_SqliteDeal.GetRedPacketGrabRecordList(conditon.c_str() ,&AcceptRedPacketList ) ;
	if (AcceptRedPacketList.size() == 0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_NO_RECORDS_EXPORT" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	CFileDialog dlg(FALSE,NULL,UiFun::UI_LoadString("MENU" , "MENU_GRABRED" ,theApp.gsLanguage),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_FILE" ,theApp.gsLanguage)+" (*.xls)|*.xls||");
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	CString strFile = dlg.GetPathName();
	if (!((CDacrsUIDlg*)(theApp.m_pMainWnd))->GetFileName(strFile,_T(".xls")))
	{
		return;
	}

	struct LISTCol {
		string		name ;
		UINT		size ;
	} listheadr[9]  = {
		{UiFun::UI_LoadString("TITLEBAR_MODULE" , "TITLEBAR_MORTGAGE" ,theApp.gsLanguage)+" hash" ,  70},
		{UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_GRABER" ,theApp.gsLanguage) ,    30},
		{UiFun::UI_LoadString("P2P_MODULE" , "P2P_PROMOTER" ,theApp.gsLanguage) ,    30},
		{UiFun::UI_LoadString("MORTTARD_MODULE" , "Type" ,theApp.gsLanguage) ,    10},
		{UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_GRABTIME" ,theApp.gsLanguage)  ,  20}, 
		{UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_TOTALMONEY" ,theApp.gsLanguage) ,30},
		{UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_COUNT" ,theApp.gsLanguage) ,10},
		{UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_LUCKVALUE" ,theApp.gsLanguage) ,10},
		{UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_GRABMONEY" ,theApp.gsLanguage) , 30}
	};

	COleVariant

		covTrue((short)TRUE),

		covFalse((short)FALSE),

		covOptional((long)DISP_E_PARAMNOTFOUND,   VT_ERROR);

	CApplication   app;

	CWorkbooks   books;

	CWorkbook   book;

	CWorksheets   sheets;

	CWorksheet   sheet;

	CRange   range;

	CFont0   font;



	if (!app.CreateDispatch(_T("Excel.Application")))

	{

		UiFun::MessageBoxEx(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_NOT_OFFICE" ,theApp.gsLanguage)  , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP ) ;
		return;

	}



	//Get   a   new   workbook.

	books = app.get_Workbooks();

	book = books.Add(covOptional);



	sheets = book.get_Worksheets();

	sheet = sheets.get_Item(COleVariant((short)1));



	////////////////////////////////////CListCtrl控件report风格//////////////////////////////////////////////////////////

	//CHeaderCtrl   *pmyHeaderCtrl;

	//pmyHeaderCtrl = m_listCtrl.GetHeaderCtrl();//此句取得CListCtrl控件的列表頭



	int   iRow,iCol;

	int   m_cols   =   9;

	int   m_rows = AcceptRedPacketList.size();

	HDITEM   hdi;

	TCHAR     lpBuffer[256];

	bool       fFound   =   false;



	hdi.mask   =   HDI_TEXT;

	hdi.pszText   =   lpBuffer;

	hdi.cchTextMax   =   256;

	CString   colname;

	CString strTemp;

	for(iCol=0;   iCol <m_cols;   iCol++)//将列表的标题头写入EXCEL

	{

		UiFun::GetCellName(1 ,iCol + 1, colname);

		range   =   sheet.get_Range(COleVariant(colname),COleVariant(colname));

		//pmyHeaderCtrl-> GetItem(iCol,   &hdi);

		range.put_Value2(COleVariant(listheadr[iCol].name.c_str()));

		int   nWidth   = listheadr[iCol].size;  //m_listCtrl.GetColumnWidth(iCol)/6;

		//得到第iCol+1列  

		range.AttachDispatch(range.get_Item(_variant_t((long)(iCol+1)),vtMissing).pdispVal,true);  

		//设置列宽 

		range.put_ColumnWidth(_variant_t((long)nWidth));

	}

	range   =   sheet.get_Range(COleVariant( _T("A1 ")),   COleVariant(colname));

	range.put_RowHeight(_variant_t((long)50));//设置行的高度

	font = range.get_Font();

	font.put_Bold(covTrue);

	range.put_VerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108



	COleSafeArray   saRet;

	DWORD   numElements[]={m_rows,m_cols};       //5x2   element   array

	saRet.Create(VT_BSTR,   2,   numElements);

	range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional);

	range = range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));

	long   index[2];

	range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional);

	range   =   range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));


	int iLine = 0;
	iRow   =   1;
	iCol   =   1;

	vector<uistruct::REDPACKETGRAB_t>::const_iterator pitem = AcceptRedPacketList.begin();
	for(;pitem != AcceptRedPacketList.end();pitem++,iRow++)
	{
		map<int,string> item;
		GetExportCol(item,*pitem);
		for   (   iCol   =   1;   iCol   <=   m_cols;   iCol++)  

		{

			index[0]=iRow-1;

			index[1]=iCol-1;
			string strTemp =  item[iCol-1];
			CString   szTemp = strTemp.c_str();

			BSTR   bstr   =   szTemp.AllocSysString();

			saRet.PutElement(index,bstr);

			SysFreeString(bstr);

		}
	}


	range.put_Value2(COleVariant(saRet));


	saRet.Detach();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	book.SaveCopyAs(COleVariant(strFile));

	//       cellinterior.ReleaseDispatch();

	book.put_Saved(true);

	book.ReleaseDispatch();  

	books.ReleaseDispatch();  

	app.Quit();

	app.ReleaseDispatch();
}
