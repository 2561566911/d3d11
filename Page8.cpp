// Page8.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCDLL.h"
#include "Page8.h"
#include "afxdialogex.h"
#include "Cheat.h"
#include "./Detours-master/include/detours.h"

// CPage8 �Ի���

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


// CPage8 ��Ϣ�������
CCheat g_cheat;


//�����¼�
//BOOL CPage8::PreTranslateMessage(MSG* pMsg)
//{
//	if (pMsg->message == WM_KEYDOWN)
//	{
//		pMsg->lParam = 0;
//		if (pMsg->wParam == VK_CAPITAL)//��д��
//		{
//			if (pMsg->lParam = 0)
//				g_cheat.Initialize();//���� cmd����
//			if (pMsg->lParam = 1)
//				g_cheat.Release();//�ر� cmd����
//
//			pMsg->lParam = 1;
//		}
//	}
//	return CPage8::PreTranslateMessage(pMsg);
//}
//��ťһ
void CPage8::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//���� cmd����
	g_cheat.Initialize();
}

//��ť��
void CPage8::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//�ر� cmd����
	g_cheat.Release();
}


////////////////////////////////////////////
//��ť��
void CPage8::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::MessageBox(NULL, "1111111", "", MB_OK);
}


//��ѡ��������
int (WINAPI*Real_MessageBox)(
	HWND hWnd
	, LPCTSTR lpText
	, LPCTSTR lpCaption
	, UINT uType
	) = MessageBox;

//��ѡ��������
int WINAPI My_MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	////

	////
	lpText = "������";

	return Real_MessageBox(hWnd, lpText, lpCaption, uType);
}

//��ѡ���¼�
void CPage8::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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



