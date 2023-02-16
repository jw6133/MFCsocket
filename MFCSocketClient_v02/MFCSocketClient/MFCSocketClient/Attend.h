#pragma once
#include "afxdialogex.h"
#include "ClientSocket.h"


// Attend dialog

class Attend : public CDialogEx
{
	DECLARE_DYNAMIC(Attend)

public:
	Attend(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Attend();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		int Select_Data(int sock);
	void socketFunc(CString dml);
	void OnBnClickedButton1();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	CClientSocket m_ClientSocket;
	afx_msg void OnBnClickedButton4();
};
