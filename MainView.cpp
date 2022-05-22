// MainView.cpp : 实现文件
//
#include "stdafx.h"
#include "MFCDLL.h"
#include "MainView.h"
#include "afxdialogex.h"


// CMainView 对话框

IMPLEMENT_DYNAMIC(CMainView, CDialog)

CMainView::CMainView(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAINVIEW, pParent)
{

}

CMainView::~CMainView()
{
}

void CMainView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAINTAB, m_ctl_tab);
}


BEGIN_MESSAGE_MAP(CMainView, CDialog)
END_MESSAGE_MAP()


// CMainView 消息处理程序


BOOL CMainView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ctl_tab.AddPage("测试8", &m_Page8, IDD_PAGE8);

	m_ctl_tab.Show();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
