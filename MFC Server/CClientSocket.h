#pragma once

// CClientSocket 명령 대상

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void CClientSocket::SetListenSocket(CAsyncSocket* pSocket);
	CAsyncSocket* m_plistenSocket;
	void CClientSocket::Log_to_file(int logidx, LPCTSTR szFormat, ...); //날짜별로 파일 로그 저장 
	void CClientSocket::CutBlank(TCHAR* szString); //공백제거 함수

	//DB연결
	CHAR m_szSQLSentence[2048];

	RETCODE m_RetCode;
	SQLHSTMT m_hSTMT;
	SQLHDBC m_hDBC;
	SQLHENV m_hENV;

	int m_LOGIDX;
	BOOL m_bODBCIs;

	CString  csDB_DNS;
	CString  csDB_USER;
	CString  csDB_PWD;

	int ODBC_SET(int logIDX, LPCTSTR strODBCName, LPCTSTR strUser, LPCTSTR strPassword);
	void ODBC_FREE();
	void SQL_ERROR_LOG(TCHAR* szLogMsg, TCHAR* szSQL);
	int MSSQL_DML_EXEC();
	void CClientSocket::myWait(DWORD dwMillisecond);

	int S_CLOSE_CHECK;
};

