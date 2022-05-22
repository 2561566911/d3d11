#pragma once
#include "Page8.h"
#include "afxcmn.h"
#include "TabSheet.h"
// CMainView 对话框


class CMainView : public CDialog
{
	DECLARE_DYNAMIC(CMainView)

public:
	CMainView(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMainView();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	
	CPage8 m_Page8;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabSheet m_ctl_tab;
};
