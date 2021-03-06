// GuessNum.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "GuessNum.h"
#include "afxdialogex.h"


// CGuessNum 对话框

IMPLEMENT_DYNAMIC(CGuessNum, CDialogBase)

CGuessNum::CGuessNum(CWnd* pParent /*=NULL*/)
	: CDialogBase(CGuessNum::IDD, pParent)
{

}

CGuessNum::~CGuessNum()
{

}

void CGuessNum::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SHOW, m_Text);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_rBtnClose);	
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CGuessNum, CDialogBase)
	ON_BN_CLICKED(IDOK, &CGuessNum::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGuessNum::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CGuessNum::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CGuessNum 消息处理程序


void CGuessNum::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_strAddress=_T("");
	char guess = 0x00;
	guess = ((CButton *)GetDlgItem(IDC_RADIO_MAN))->GetCheck();
	theApp.m_strAddress.Format(_T("%d"),((int)(guess+1)));
	CDialogBase::OnOK();
}


void CGuessNum::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_strAddress=_T("");
	CDialogBase::OnCancel();
}


BOOL CGuessNum::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_Text.SetFont(100, _T("微软雅黑"));				//设置显示字体和大小
	m_Text.SetTextColor(RGB(255,255,255));	    //字体颜色
	m_Text.SetWindowText(UiFun::UI_LoadString("GUESS_DILOG" , "GUESS_NUMBER" ,theApp.gsLanguage)) ;
	m_Text.SetWindowPos( NULL , 3 , 3 , 200, 20  ,SWP_SHOWWINDOW ) ; 

	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	//m_rBtnClose.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnClose.SizeToContent();
	m_rBtnClose.SetWindowPos(NULL ,290 , 0 , 0 , 0 , SWP_NOSIZE); 


	CRect rect ;
	m_rBtnClose.GetClientRect(rect);

	RECT ret;
	GetWindowRect(&ret);
	m_rBtnClose.SetWindowPos(NULL ,(ret.right-ret.left)-rect.Width() , 2 , 0 , 0 , SWP_NOSIZE); 

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255));
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_CONFIRM" ,theApp.gsLanguage)) ;
	//m_rBtnOk.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_CANCEL" ,theApp.gsLanguage)) ;
	//m_rBtnCancel.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();
	
	GetDlgItem(IDC_RADIO_MAN)->SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_BROTHER" ,theApp.gsLanguage));
	GetDlgItem(IDC_RADIO_WOMAN)->SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_SISTER" ,theApp.gsLanguage));
	GetDlgItem(IDC_STATIC)->SetWindowText(UiFun::UI_LoadString("GUESS_DILOG" , "GUESS_NUMBER01" ,theApp.gsLanguage));
	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO_MAN);
	radio->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CGuessNum::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDCANCEL);
}
