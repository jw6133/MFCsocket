
// T_SERVER01Dlg.h: 헤더 파일
//

#pragma once
#include "CListenSocket.h"

// CTSERVER01Dlg 대화 상자
class CTSERVER01Dlg : public CDialogEx
{
// 생성입니다.
public:
	CTSERVER01Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CListenSocket m_ListenSocket;//소캣생성
// 대화 상자 데이터
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_T_SERVER01_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnExit();
//	CListCtrl m_list_log_1;
//	CListCtrl m_list_client_log;
	afx_msg void OnLbnSelchangeList3();
//	CListBox m_list_log_3;
	CListBox m_list_log_1;
	CListBox m_list_client_log;
	afx_msg void OnEnChangeEdit1();
};
