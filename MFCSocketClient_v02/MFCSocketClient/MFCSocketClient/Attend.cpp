// Attend.cpp : implementation file
//

#include "pch.h"
#include "MFCSocketClient.h"
#include "afxdialogex.h"
#include "Attend.h"
#include "MFCSocketClientDoc.h"
#include "DAO.h"
#define PORT 11051

// Attend dialog

IMPLEMENT_DYNAMIC(Attend, CDialogEx)

Attend::Attend(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

Attend::~Attend()
{
}

void Attend::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Attend, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Attend::OnBnClickedButton1)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// Attend message handlers

int Attend::Select_Data(int sock)
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

void Attend::socketFunc(CString dml) {
	CString str;
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

	csDML = dml;

	GetDlgItemText(IDC_EDIT_STUID, csU_ID);
	GetDlgItemText(IDC_EDIT_NAME, csU_NAME);
	GetDlgItemText(IDC_EDIT_TIME, csU_ED_TIME);
	GetDlgItemText(IDC_EDIT_STAT, csU_STATE);
	GetDlgItemText(IDC_EDIT_SUB, csU_EDU);

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

	str = csCMD + csDML + csRT + csU_ID + csU_NAME + csU_ST_TIME + csU_ED_TIME + csU_STATE.Left(2) + csU_EDU + csEND;

	m_ClientSocket.SetWnd(this->m_hWnd);
	m_ClientSocket.Create();
	if (m_ClientSocket.Connect(_T("211.177.42.242"), PORT) == FALSE) {
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		//PostQuitMessage(0);
		return;
	}

	m_ClientSocket.Send(str, str.GetLength(), 0);
	memset(szRCVBUF, 0x00, sizeof(szRCVBUF));
	for (int ii = 0; ii < 3; ii++) //3초 동안 데이터가 오는지 확인
	{
		int ret = Select_Data(m_ClientSocket);
		if (ret == 1)
		{
			int rlen = recv(m_ClientSocket, szRCVBUF, sizeof(szRCVBUF), 0);

			//SetDlgItemText(IDC_EDIT_ATTNUM, szRCVBUF);

			m_ClientSocket.ShutDown(1);
			m_ClientSocket.Close();
			return;
		}
	}
	m_ClientSocket.ShutDown(1);
	m_ClientSocket.Close();
}

void Attend::OnBnClickedButton1()
{
	CString str = "IN";
	
	GetDlgItemText(IDC_EDIT_STAT, str);
	
	socketFunc(str.Right(2));
}


void Attend::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	CString str;
	CString csCMD = "LOGIN";
	CString csDML = "UP";
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

	SetDlgItemText(IDC_EDIT_STUID, csU_ID);
	SetDlgItemText(IDC_EDIT_NAME, csU_NAME);
	SetDlgItemText(IDC_EDIT_TIME, csU_ED_TIME);
	SetDlgItemText(IDC_EDIT_STAT, csU_STATE);
	SetDlgItemText(IDC_EDIT_SUB, csU_EDU);

}


void Attend::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
}
