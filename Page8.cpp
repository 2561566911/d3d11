// Page8.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDLL.h"
#include "Page8.h"
#include "afxdialogex.h"
#include "Cheat.h"
#include "./Detours-master/include/detours.h"

// CPage8 对话框

IMPLEMENT_DYNAMIC(CPage8, CDialog)

CPage8::CPage8(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PAGE8, pParent)
{

}

CPage8::~CPage8()
{
}

void CPage8::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage8, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage8::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage8::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage8::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK1, &CPage8::OnBnClickedCheck1)
	//ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CPage8 消息处理程序
CCheat g_cheat;


//键盘事件
//BOOL CPage8::PreTranslateMessage(MSG* pMsg)
//{
//	if (pMsg->message == WM_KEYDOWN)
//	{
//		pMsg->lParam = 0;
//		if (pMsg->wParam == VK_CAPITAL)//大写键
//		{
//			if (pMsg->lParam = 0)
//				g_cheat.Initialize();//开启 cmd窗口
//			if (pMsg->lParam = 1)
//				g_cheat.Release();//关闭 cmd窗口
//
//			pMsg->lParam = 1;
//		}
//	}
//	return CPage8::PreTranslateMessage(pMsg);
//}
//按钮一
void CPage8::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//开启 cmd窗口
	g_cheat.Initialize();
}

//按钮二
void CPage8::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//关闭 cmd窗口
	g_cheat.Release();
}


////////////////////////////////////////////
//按钮三
void CPage8::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	::MessageBox(NULL, "1111111", "", MB_OK);
}


//单选框函数定义
int (WINAPI*Real_MessageBox)(
	HWND hWnd
	, LPCTSTR lpText
	, LPCTSTR lpCaption
	, UINT uType
	) = MessageBox;

//单选框函数定义
int WINAPI My_MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	////

	////
	lpText = "勾上了";

	return Real_MessageBox(hWnd, lpText, lpCaption, uType);
}

//单选框事件
void CPage8::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourAttach(&(PVOID&)Real_MessageBox,My_MessageBox);

		DetourTransactionCommit();
	}
	else
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourDetach(&(PVOID&)Real_MessageBox, My_MessageBox);

		DetourTransactionCommit();
	}
}



