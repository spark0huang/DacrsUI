// AddApp.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "AddApp.h"
#include "afxdialogex.h"


// CAddApp 对话框

IMPLEMENT_DYNAMIC(CAddApp, CDialogBar)

CAddApp::CAddApp()
{
	m_pBmp = NULL ;
	pView = NULL;
}

CAddApp::~CAddApp()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
	DeleteView();
}

void CAddApp::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CAddApp, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CAddApp 消息处理程序

void CAddApp::SetBkBmpNid( UINT nBitmapIn ) 
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
int CAddApp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//SetBkBmpNid(IDB_BITMAP_ADDAPP_BJ);
	SetBkBmpNid(UiFun::GetBmpId(IDB_BITMAP_ADDAPP_BJ,IDB_BITMAP_IPO_BJEN,theApp.language()));
	return 0;
}


BOOL CAddApp::OnEraseBkgnd(CDC* pDC)
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


BOOL CAddApp::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	//return bRes;
	if ( bRes ) {
		UpdateData(0);

		//用来连接组成文档的组件和他的数据的视图
		CCreateContext pContext;
		CWnd* pFrameWnd = this;
		pContext.m_pCurrentDoc = new CDlgDocument;
		pContext.m_pNewViewClass = RUNTIME_CLASS(CDlgView);
		//CDlgView *
		pView =(CDlgView *) ((CFrameWnd*)pFrameWnd)->CreateView(&pContext);
		ASSERT(pView);
		pView->m_nMapMode = MM_TEXT;

		pView->ShowWindow(SW_NORMAL);

		CRect rectWindow;
		GetWindowRect(rectWindow);

		if (theApp.m_pMainWnd != NULL)
		{
			CRect rect;
			theApp.m_pMainWnd->GetWindowRect(&rect);
			//theApp.m_pMainWnd->ClientToScreen(rect);
			rect.top =-2;
			rect.left = -2;
			rect.bottom -= (72+32);
			pView->MoveWindow(rect);
		}
	}
	return bRes ;
}
void CAddApp::DeleteView()
{
	if (pView != NULL)
	{
		delete pView;
		//pView->OnClose();
		//pView->DestroyWindow();
	}
}