// MainView.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "MFCDLL.h"
#include "MainView.h"
#include "afxdialogex.h"


// CMainView �Ի���

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


// CMainView ��Ϣ�������


BOOL CMainView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ctl_tab.AddPage("����8", &m_Page8, IDD_PAGE8);

	m_ctl_tab.Show();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
