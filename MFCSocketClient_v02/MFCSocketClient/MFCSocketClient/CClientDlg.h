#pragma once
#include "afxdialogex.h"
#include "ClientSocket.h"


// CClientDlg dialog

class CClientDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CClientDlg)

public:
	CClientDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CClientDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	CClientSocket m_ClientSocket;
public:
	afx_msg void OnBnClickedButton1();
	int Select_Data(int sock);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_staticTIMER;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	int m_iCount;
	afx_msg void OnBnClickedButtonMul();
};
