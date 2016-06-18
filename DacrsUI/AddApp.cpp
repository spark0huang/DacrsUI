// AddApp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "AddApp.h"
#include "afxdialogex.h"


// CAddApp �Ի���

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


// CAddApp ��Ϣ��������

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

	// TODO:  �ڴ�������ר�õĴ�������
	//SetBkBmpNid(IDB_BITMAP_ADDAPP_BJ);
	SetBkBmpNid(UiFun::GetBmpId(IDB_BITMAP_ADDAPP_BJ,IDB_BITMAP_IPO_BJEN,theApp.language()));
	return 0;
}


BOOL CAddApp::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
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
	// TODO: �ڴ�����ר�ô����/����û���

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	//return bRes;
	if ( bRes ) {
		UpdateData(0);

		//������������ĵ���������������ݵ���ͼ
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