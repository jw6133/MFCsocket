#pragma once

// CListenSocket 명령 대상

class CListenSocket : public CAsyncSocket
{
public:
	CListenSocket();
	virtual ~CListenSocket();
	virtual void OnAccept(int nErrorCode);
	CPtrList m_ptrClientSocketList;
	void CListenSocket::CloseClientSocket(CSocket* pClient);

	void CListenSocket::myWait(DWORD dwMillisecond);
};


