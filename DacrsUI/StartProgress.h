#pragma once


// CStartProgress 对话框

class CStartProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CStartProgress)

public:
	CStartProgress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartProgress();

// 对话框数据
	enum { IDD = IDD_DIALOG_INIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void             LoadGifing( BOOL bState );
public:
	CStatic		     *m_ProgressWnd ;	//Progress窗口
public:
	int              m_nBmpIndex;
	CBitmap          m_ProgressBmp[4];  //信号图片
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
public:
	void    SwitchBmp(int nIndex);         
};
