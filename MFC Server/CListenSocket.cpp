// CListenSocket.cpp: 구현 파일
//

#include "pch.h"
#include "T_SERVER01.h"
#include "CListenSocket.h"
#include "CClientSocket.h"
#include "T_SERVER01Dlg.h"

// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket 멤버 함수


void CListenSocket::OnAccept(int nErrorCode) //사용자 추가 
{
	//클라이언트 소캣 객체를 관리하는 리스트 
	CClientSocket* pClient = new CClientSocket;
	CString str;
	CString strDestAddr;
	UINT unPort;

	//받은 소캣 저장 
	if (Accept(*pClient)) {
		//ClientSocket객체의 주소를 CChildSocket객체에 알려주기 위한 함수를 호출 
		pClient->SetListenSocket(this); //CChildSocket 클래스에 사용자가 정의한 함수 
		m_ptrClientSocketList.AddTail(pClient);
		//클라이언트가 접속 목록을 리스트에 출력 
		CTSERVER01Dlg* pMain = (CTSERVER01Dlg*)AfxGetMainWnd(); //CTSERVER01Dlg에서 핸들 값을 가져옴 
		//Client 리스트에 추가
		pClient->GetPeerName(strDestAddr, unPort);
		str.Format("%d - 접속IP (%s) PORT (%d)", (int)m_ptrClientSocketList.Find(pClient), strDestAddr, unPort);
		pMain->m_list_client_log.AddString(str);
		//AfxMessageBox("succes");
	}
		CAsyncSocket::OnAccept(nErrorCode);
}

void CListenSocket::CloseClientSocket(CSocket* pClient) {

	CString str;
	CString	strDestAddr;
	UINT unPort;


	POSITION pos;
	pos = m_ptrClientSocketList.Find(pClient);
	CTSERVER01Dlg* pMain = (CTSERVER01Dlg*)AfxGetMainWnd(); // CSocketServerDlg의 핸들을 가져옴

	if (pos != NULL)
	{
		if (pClient != NULL)
		{

			pClient->GetPeerName(strDestAddr, unPort);

			str.Format("CListenSocket clinet종료 POS:%d\n IP:%s\n PORT:%d\n ", (int)m_ptrClientSocketList.Find(pClient), strDestAddr, unPort);
			pMain->m_list_log_1.AddString(str);

			myWait(1000); //1초

			pClient->ShutDown();
			pClient->Close();

			CString csSocketL = "";
			csSocketL.Format("%d", (int)m_ptrClientSocketList.Find(pClient));

			int idx = pMain->m_list_client_log.FindString(-1, csSocketL); //맨처음부터...
			if (idx >= 0)
			{
				pMain->m_list_client_log.DeleteString(idx);
				str.Format("client삭제 IP: %s \n PORT:%d\n  ", strDestAddr, unPort);
				pMain->m_list_log_1.AddString(str);
			}
			else
			{
				str.Format("데이터가 없어서 client 삭제 불가능 IP:%s PORT:%d", strDestAddr, unPort);
				pMain->m_list_log_1.AddString(str);
			}

		}
		else
		{
			str.Format("실패/ CListenSocket 종료 핸들[%d] 값이 없음", (int)pClient);
			pMain->m_list_log_1.AddString(str);
		}

		m_ptrClientSocketList.RemoveAt(pos);
		delete pClient;
	}
	else
	{
		str.Format("실패/ CListenSocket clinet 종료 핸들 [%d] 값이 없음", (int)pClient);
		pMain->m_list_log_1.AddString(str);
	}
}

//대기
void CListenSocket::myWait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}