// MFCDLL.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MFCDLL.h"
#include "MainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�������еĵ�һ�����:
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CMFCDLLApp

BEGIN_MESSAGE_MAP(CMFCDLLApp, CWinApp)
END_MESSAGE_MAP()


// CMFCDLLApp ����

CMFCDLLApp::CMFCDLLApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ�� CMFCDLLApp ����

CMFCDLLApp theApp;

HANDLE g_hUIThread;
HANDLE g_hFreeThread;

// CMFCDLLApp ��ʼ��

void UIThread(void* Param)
{
	//��Դ�л�
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HINSTANCE hResOld = AfxGetResourceHandle();//����ɵľ��
	AfxSetResourceHandle(theApp.m_hInstance);

	//��������
	CMainView* MainDlg = new CMainView();
	MainDlg->DoModal();

	//�رմ��ڻ�ִ��
	delete MainDlg;
	MainDlg = NULL;
	g_hFreeThread = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(FreeLibrary), theApp.m_hInstance, 0, 0);
	CloseHandle(g_hUIThread);
	CloseHandle(g_hFreeThread);


	//��Դ�л���ȥ
	AfxSetResourceHandle(hResOld);
}

BOOL CMFCDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	//�����߳�
	g_hUIThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UIThread, NULL, 0, 0);

	return TRUE;
}
