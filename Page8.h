#pragma once


// CPage8 �Ի���

class CPage8 : public CDialog
{
	DECLARE_DYNAMIC(CPage8)

public:
	CPage8(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage8();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	/*virtual BOOL PreTranslateMessage(MSG* pMsg);*/
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedCheck1();
};
