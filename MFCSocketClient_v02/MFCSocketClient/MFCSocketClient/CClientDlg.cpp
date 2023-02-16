// CClientDlg.cpp : implementation file
//

#include "pch.h"
#include "MFCSocketClient.h"
#include "CClientDlg.h"
#include "afxdialogex.h"
#include "DAO.h"
#define PORT 11051

// CClientDlg dialog

IMPLEMENT_DYNAMIC(CClientDlg, CDialogEx)

CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT, pParent)
{

}

CClientDlg::~CClientDlg()
{
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CClientDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MUL, &CClientDlg::OnBnClickedButtonMul)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_iCount = 0;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CClientDlg::OnBnClickedButton1()
{
	CString str;
	str ="LOGININ0000123456789KKMD20220912110020220912115001KR#";
	CString csCMD = "LOGIN";
	CString csDML = "IN";
	CString csRT = "0000";
	CString csU_ID = "123456789";
	CString csU_NAME = "KKMD";
	CString csU_ST_TIME = "202209121100";
	CString csU_ED_TIME = "202209121150";
	CString csU_STATE = "01";
	CString csU_EDU = "KR";
	CString csEND = "#";
	char szRCVBUF[1024];
	int rc;

	DAO app;

	str = app.getMsg();

	//	CMD	5
	//	DML	2
	//	RT	4
	//	U_ID	9
	//	U_NAME	20
	//	U_ST_TIME	12
	//	U_ED_TIME	12
	//	U_STATE	2
	//	U_EDU	2
	//	U_END	1

	GetDlgItemText(IDC_EDIT_STUID, csU_ID);
	GetDlgItemText(IDC_EDIT_NAME, csU_NAME);
	
	csCMD.Format("%-5s", csCMD);
	csDML.Format("%-2s", csDML);
	csRT.Format("%-4s", csRT);
	csU_ID.Format("%-9s", csU_ID);
	csU_NAME.Format("%-20s", csU_NAME);
	csU_ST_TIME.Format("%-12s", csU_ST_TIME);
	csU_ED_TIME.Format("%-12s", csU_ED_TIME);
	csU_STATE.Format("%-2s", csU_STATE);
	csU_EDU.Format("%-2s", csU_EDU);
	csEND.Format("%-1s", csEND);

	str = csCMD + csDML + csRT + csU_ID + csU_NAME + csU_ST_TIME + csU_ED_TIME + csU_STATE + csU_EDU + csEND;

	//빈자리는 공백 채우기(%-5s: 5자리까지 채우고 없으면 공백)  

	m_ClientSocket.SetWnd(this->m_hWnd);
	m_ClientSocket.Create();
	if (m_ClientSocket.Connect(_T("211.177.42.242"), PORT) == FALSE) {
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		//PostQuitMessage(0);
		return;
	}


	//GetDlgItemText(IDC_EDIT_ATTNUM, str);
	m_ClientSocket.Send(str, str.GetLength(), 0);
	memset(szRCVBUF, 0x00, sizeof(szRCVBUF));
	for (int ii = 0; ii < 3; ii++) //3초 동안 데이터가 오는지 확인
	{
		int ret = Select_Data(m_ClientSocket);
		if (ret == 1)
		{
			int rlen = recv(m_ClientSocket, szRCVBUF, sizeof(szRCVBUF), 0);
			CString msgCStr = csU_NAME + "출석되었습니다.";
			MessageBox(msgCStr, "알림", MB_ICONINFORMATION);
			
			m_ClientSocket.ShutDown(1);
			m_ClientSocket.Close();
			return;
		}
	}
	m_ClientSocket.ShutDown(1);
	m_ClientSocket.Close();
}

int CClientDlg::Select_Data(int sock)
{
	int	s;
	struct	timeval timeout;
	fd_set	readmask;

	FD_ZERO(&readmask);
	FD_SET(sock, &readmask);

	timeout.tv_sec = 1;		/* Seconds	  */
	timeout.tv_usec = 0;	/* Micro Seconds (1000000) 1초 입니다.*/

	s = select(sock + 1, &readmask, (fd_set*)0, (fd_set*)0, &timeout);
	if (s < 0)
	{
		return(-1);		 /* Data Check Error */
	}
	if (FD_ISSET(sock, &readmask))
	{
		return(1);		/* Data Arrival     */
	}
	return(0);			/* No Data Arrival  */
}


void CClientDlg::OnBnClickedCancel()
{
	if (m_ClientSocket != NULL)
	{
		m_ClientSocket.ShutDown(1);
		m_ClientSocket.Close();
	}
	CDialogEx::OnCancel();
}


void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString sTxt;
	switch (nIDEvent) {
	case 1:
		sTxt.Format(_T("%d"), m_iCount++);
		m_staticTIMER.SetWindowTextA(sTxt);
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CClientDlg::OnBnClickedButton2()
{
	SetTimer(1, 1000, NULL);
}


void CClientDlg::OnBnClickedButton3()
{
	KillTimer(1);
}


void CClientDlg::OnBnClickedButtonMul()
{
	m_ClientSocket.SetWnd(this->m_hWnd);
	m_ClientSocket.Create();
	if (m_ClientSocket.Connect(_T("211.177.42.242"), PORT) == FALSE) {
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		//PostQuitMessage(0);
		return;
	}

	Sleep(20000);

	m_ClientSocket.ShutDown(1);
	m_ClientSocket.Close();
}
