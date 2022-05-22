// MFCDLL.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MFCDLL.h"
#include "MainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CMFCDLLApp

BEGIN_MESSAGE_MAP(CMFCDLLApp, CWinApp)
END_MESSAGE_MAP()


// CMFCDLLApp 构造

CMFCDLLApp::CMFCDLLApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMFCDLLApp 对象

CMFCDLLApp theApp;

HANDLE g_hUIThread;
HANDLE g_hFreeThread;

// CMFCDLLApp 初始化

void UIThread(void* Param)
{
	//资源切换
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HINSTANCE hResOld = AfxGetResourceHandle();//保存旧的句柄
	AfxSetResourceHandle(theApp.m_hInstance);

	//创建界面
	CMainView* MainDlg = new CMainView();
	MainDlg->DoModal();

	//关闭窗口会执行
	delete MainDlg;
	MainDlg = NULL;
	g_hFreeThread = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(FreeLibrary), theApp.m_hInstance, 0, 0);
	CloseHandle(g_hUIThread);
	CloseHandle(g_hFreeThread);


	//资源切换回去
	AfxSetResourceHandle(hResOld);
}

BOOL CMFCDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	//开新线程
	g_hUIThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UIThread, NULL, 0, 0);

	return TRUE;
}
