#pragma once
#include "Page8.h"
#include "afxcmn.h"
#include "TabSheet.h"
// CMainView �Ի���


class CMainView : public CDialog
{
	DECLARE_DYNAMIC(CMainView)

public:
	CMainView(CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CMainView();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	
	CPage8 m_Page8;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabSheet m_ctl_tab;
};
