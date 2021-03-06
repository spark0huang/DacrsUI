#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "XProgressCtrl.h"
// CProgStatusBar 对话框

class CProgStatusBar : public CDialogBar
{
	DECLARE_DYNAMIC(CProgStatusBar)

public:
	CProgStatusBar();   // 标准构造函数
	virtual ~CProgStatusBar();

	// 对话框数据
	enum { IDD = IDD_DIALOG_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic		     *m_ProgressWnd;	//Progress窗口
	CStaticTrans     m_strNeting;		//网络同步 
	CStaticTrans     m_strHeight;		//高度 
	CStaticTrans     m_strVersion;      //版本
public:
	HBITMAP		     m_pBmp; 
	CBitmap          m_Sigbmp[4];  //信号图片
	CBitmap          m_Lockbmp[2];  //锁的图片
	int              m_nSigIndex;
	int              m_nLockIndex;
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	void             LoadGifing( BOOL bState );
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
public:
	afx_msg LRESULT OnShowProgressCtrl(  WPARAM wParam, LPARAM lParam ) ;
	int  ShowProgressCtrl();
	void ShowLockCtrl();
	void OnIniLockParam();
	void ShowNetCount();
	string GetUIVersion();
public:
	BOOL        m_bProgressType;
	int         m_ProgressMax  ;
//	CProgressCtrl m_progress;
	CXProgressCtrl m_progress;
	afx_msg void OnPaint();
	CRect  m_bmpsig;
	bool   m_walletui;
	bool   m_prosshiden;    /// false 出现进度条 true 不显示进度条
	CRect  m_bmplock;
	CString netStr;
	int  m_connectCount;
public:
	void SetAppStepfee(int fuelrate);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

};
