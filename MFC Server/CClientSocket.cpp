// CClientSocket.cpp: 구현 파일
//

#include "pch.h"
#include "T_SERVER01.h"
#include "CClientSocket.h"
#include "T_SERVER01Dlg.h"

// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

//소캣을 받아옴 
void CClientSocket::SetListenSocket(CAsyncSocket* pSocket) { //Listen 시작 
	CString str;
	CString	strDestAddr; //목적지 주소
	UINT unPort; //포트번호
	CTSERVER01Dlg* pMain = (CTSERVER01Dlg*)AfxGetMainWnd(); // CSocketServerDlg의 핸들을 가져옴	
	S_CLOSE_CHECK = 0;
	m_plistenSocket = pSocket;

	//현재 프로그램이 실행 되는 IP랑 PORT를 출력 
	GetPeerName(strDestAddr, unPort);
	str.Format("CLIENNT IP(%s) PORT(%d) ", strDestAddr, unPort);

	pMain->m_list_log_1.AddString(str);
	pMain->GetDlgItemTextA(IDC_EDIT_DB_DSN, csDB_DNS);
	pMain->GetDlgItemTextA(IDC_EDIT_DB_USER, csDB_USER);
	pMain->GetDlgItemTextA(IDC_EDIT_DB_PWD, csDB_PWD);

	str.Format("DB정보 :  DSN(%s) USER(%s) PWD(%s)", csDB_DNS, csDB_USER, csDB_PWD);
	pMain->m_list_log_1.AddString(str);

	Log_to_file(1, str);

}


// CClientSocket 멤버 함수

//클라이언트가 서버로부터 데이터를 보내게 되면 자동으로 호출
void CClientSocket::OnReceive(int nErrorCode) //데이터가 들어옴 
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CString str;
	CString	strDestAddr;
	UINT unPort;

	struct att_STR	MST_STR;
	struct att_STR_LIST	MST_STR_LIST;

	char	CMD[5 + 1];  //LOGIN	
	char	DML[2 + 1]; // IN,UP,DE,LS
	char	RT[4 + 1]; //결과  - 0000 :성공, 9999: 실패
	char	U_ID[9 + 1]; //학번
	char	U_ST_TIME[12 + 1]; //시작시간
	char	U_ED_TIME[12 + 1]; //종료시간
	char	U_STATE[2 + 1]; //종료시간
	char	U_NAME[20 + 1]; //이름
	char	U_EDU[2 + 1]; //과목

	char szRecvBuff[1024]; ZeroMemory(szRecvBuff, sizeof(szRecvBuff));
	char szSendBuff[1024]; ZeroMemory(szSendBuff, sizeof(szSendBuff));
	char rheaer[30]; ZeroMemory(rheaer, sizeof(rheaer));
	int nLenth = Receive(szRecvBuff, sizeof(szRecvBuff)); //데이터 받는 receive 함수
	int nSendLen = 0;
	int nRC = 0;

	//연결된 클라이언트의 IP 주소와 포트번호 알아내기
	GetPeerName(strDestAddr, unPort);
	//데이터를 수신하였다면 받은 메시지를 리스트에 출력 
	CTSERVER01Dlg* pMain = (CTSERVER01Dlg*)AfxGetMainWnd(); 

	
	int nLEN_MST_STR = sizeof(MST_STR);
	int nLEN_MST_STR_LIST = sizeof(MST_STR_LIST);

//실제 데이터 수신 
	if (nLenth > 0)
	{
		str.Format("CLIENT의 IP(%s) PORT(%d)\n RECV한 내용: %s", strDestAddr, unPort, szRecvBuff);
		pMain->m_list_log_1.AddString(str);

		Log_to_file(1, str); //D드라이브 실행파일이 있는곳 위치에서 날짜별 log확인가능 

		if (strlen(szRecvBuff) == nLEN_MST_STR || strlen(szRecvBuff) == nLEN_MST_STR_LIST)
		{
			//정상
		}
		else
		{//내가 정의한 전문 길이와 다를경우, send로 error 보냄 
			memset(szSendBuff, 0x00, sizeof(szSendBuff));
			strcpy_s(szSendBuff, sizeof(szSendBuff), "ERROR"); //error보내기
			nSendLen = Send(szSendBuff, strlen(szSendBuff));
			if (nLenth > 0)
			{
				str.Format("데이터 길이 오류! IP: %s\n PORT:%d\n SEND내용: %s\n", strDestAddr, unPort, szSendBuff);
				pMain->m_list_log_1.AddString(str);
				Log_to_file(1, str);
			}
	// 연결된 모든 클라이언트에게 해당 메시지 에코 
			CListenSocket* pServerSocket = (CListenSocket*)m_plistenSocket;
			pServerSocket->CloseClientSocket(this);
			return;
		}

		//전문확인하기  전문을 복사
		memset((char*)&MST_STR, 0x00, sizeof(MST_STR));
		memcpy((char*)&MST_STR, szRecvBuff, sizeof(MST_STR));
		//receive받은 데이터를 전문 구조체에 복사 


		//전문 변수값 초기화 
		memset(CMD, 0x00, sizeof(CMD)); //변수초기화
		memset(DML, 0x00, sizeof(DML));
		memset(RT, 0x00, sizeof(RT));
		memset(U_ID, 0x00, sizeof(U_ID));
		memset(U_ST_TIME, 0x00, sizeof(U_ST_TIME));
		memset(U_ED_TIME, 0x00, sizeof(U_ED_TIME));
		memset(U_STATE, 0x00, sizeof(U_STATE));
		memset(U_NAME, 0x00, sizeof(U_NAME));
		memset(U_EDU, 0x00, sizeof(U_EDU));
		//전문 별로 분리 
		memcpy(CMD, MST_STR.CMD, sizeof(MST_STR.CMD));
		memcpy(DML, MST_STR.DML, sizeof(MST_STR.DML));
		memcpy(RT, MST_STR.RT, sizeof(MST_STR.RT));
		memcpy(U_ID, MST_STR.U_ID, sizeof(MST_STR.U_ID));
		memcpy(U_ST_TIME, MST_STR.U_ST_TIME, sizeof(MST_STR.U_ST_TIME));
		memcpy(U_ED_TIME, MST_STR.U_ED_TIME, sizeof(MST_STR.U_ED_TIME));
		memcpy(U_STATE, MST_STR.U_STATE, sizeof(MST_STR.U_STATE));
		memcpy(U_NAME, MST_STR.U_NAME, sizeof(MST_STR.U_NAME));
		memcpy(U_EDU, MST_STR.U_EDU, sizeof(MST_STR.U_EDU));
		//DB에 넣기 전에 공백제거 
		CutBlank(CMD);  
		CutBlank(DML);  
		CutBlank(RT);  
		CutBlank(U_ID);  
		CutBlank(U_ST_TIME);  
		CutBlank(U_ED_TIME);  
		CutBlank(U_STATE);  
		CutBlank(U_NAME);  
		CutBlank(U_EDU);  

		str.Format("%s", "전문확인");
		pMain->m_list_log_1.AddString(str);


		str.Format("CMD[%s]", CMD);
		pMain->m_list_log_1.AddString(str);

		str.Format("DML[%s]", DML);
		pMain->m_list_log_1.AddString(str);

		str.Format("RT[%s]", RT);
		pMain->m_list_log_1.AddString(str);

		str.Format("U_ID[%s]", U_ID);
		pMain->m_list_log_1.AddString(str);


		str.Format("U_NAME[%s]", U_NAME);
		pMain->m_list_log_1.AddString(str);

		str.Format("U_ST_TIME[%s]", U_ST_TIME);
		pMain->m_list_log_1.AddString(str);

		str.Format("U_ED_TIME[%s]", U_ED_TIME);
		pMain->m_list_log_1.AddString(str);

		str.Format("U_STATE[%s]", U_STATE);
		pMain->m_list_log_1.AddString(str); 

			str.Format("U_EDU[%s]", U_EDU);
		pMain->m_list_log_1.AddString(str);

		
		ODBC_SET(1, csDB_DNS, csDB_USER, csDB_PWD);  //DB정보
		//DB에 입력 
		if (m_bODBCIs == true)
		{
			memset(m_szSQLSentence, 0x00, sizeof(m_szSQLSentence));
			if (memcmp(DML, "IN", 2) == 0)
			{
				sprintf_s(m_szSQLSentence, sizeof(m_szSQLSentence), "INSERT INTO  ATT_TBL (AT_ID, AT_NAME,AT_ST_TIME,AT_ED_TIME,AT_EDU, AT_STATE   ) values ( '%s','%s' ,'%s','%s','%s','%s') ", U_ID, U_NAME, U_ST_TIME, U_ED_TIME, U_EDU, U_STATE);
				pMain->m_list_log_1.AddString(m_szSQLSentence);
				nRC = MSSQL_DML_EXEC();
				if (nRC == 1)
				{
					memcpy(MST_STR.RT, "1000", sizeof(MST_STR.RT));  //성공
					memcpy(szSendBuff, (char*)&MST_STR, sizeof(MST_STR));
				}
				else
				{
					memcpy(MST_STR.RT, "9999", sizeof(MST_STR.RT)); //실패
					memcpy(szSendBuff, (char*)&MST_STR, sizeof(MST_STR));
				}
			}
			else if (memcmp(DML, "UP", 2) == 0) //수정 - 학번 과 시작시간 , 과목
			{
				sprintf_s(m_szSQLSentence, sizeof(m_szSQLSentence), "UPDATE ATT_TBL SET AT_STATE ='%s' WHERE AT_ID='%s' and AT_ST_TIME='%s'  and AT_EDU='%s'  ", U_STATE, U_ID, U_ST_TIME, U_EDU);
				pMain->m_list_log_1.AddString(m_szSQLSentence);
				nRC = MSSQL_DML_EXEC();
				if (nRC == 1)
				{
					memcpy(MST_STR.RT, "1000", sizeof(MST_STR.RT));  //성공
					memcpy(szSendBuff, (char*)&MST_STR, sizeof(MST_STR));
				}
				else
				{
					memcpy(MST_STR.RT, "9999", sizeof(MST_STR.RT)); //실패
					memcpy(szSendBuff, (char*)&MST_STR, sizeof(MST_STR));
				}
			}
			else if (memcmp(DML, "DE", 2) == 0) //삭제- 학번 과 시작시간 , 과목
			{
				sprintf_s(m_szSQLSentence, sizeof(m_szSQLSentence), "DELETE FROM ATT_TBL WHERE AT_ID='%s' and AT_ST_TIME='%s' and AT_EDU='%s'  ", U_ID, U_ST_TIME, U_EDU);
				pMain->m_list_log_1.AddString(m_szSQLSentence);
				nRC = MSSQL_DML_EXEC();
				if (nRC == 1)
				{
					memcpy(MST_STR.RT, "1000", sizeof(MST_STR.RT));  //성공
					memcpy(szSendBuff, (char*)&MST_STR, sizeof(MST_STR));
				}
				else
				{
					memcpy(MST_STR.RT, "9999", sizeof(MST_STR.RT)); //실패
					memcpy(szSendBuff, (char*)&MST_STR, sizeof(MST_STR));
				}
			}
			else if (memcmp(DML, "WA", 2) == 0) //test 5  초동안 아무것도 안하고 대기
			{
				pMain->m_list_log_1.AddString("### 5초동안 대기 ### ");
				myWait(1000 * 1);
				myWait(1000 * 1);
				myWait(1000 * 1);
				myWait(1000 * 1);

			}
			else if (memcmp(DML, "SE", 2) == 0)  //조회 - 학번 과 시작시간  --최근 5개
			{
				memset((char*)&MST_STR_LIST, 0x00, sizeof(MST_STR_LIST));
				memset((char*)&MST_STR_LIST, 0x20, sizeof(MST_STR_LIST));

				ZeroMemory(m_szSQLSentence, sizeof(m_szSQLSentence));
				if (memcmp(U_EDU, "99", 2) == 0)  //학번 
				{

					sprintf_s(m_szSQLSentence, sizeof(m_szSQLSentence), " SELECT TOP 5 AT_ID,AT_NAME,AT_ST_TIME,AT_ED_TIME ,AT_STATE ,  AT_EDU FROM ATT_TBL WHERE AT_ID = '%s' ORDER BY AT_ST_TIME ",
						U_ID);

				}
				else
				{

					sprintf_s(m_szSQLSentence, sizeof(m_szSQLSentence), " SELECT TOP 5 AT_ID,AT_NAME,AT_ST_TIME,AT_ED_TIME ,AT_STATE ,  AT_EDU FROM ATT_TBL WHERE  AT_EDU ='%s'  ORDER BY AT_ST_TIME ",
						 U_EDU);
				}


				char	R_U_ID[9 + 1]; //학번
				char	R_U_ST_TIME[12 + 1]; //시작시간
				char	R_U_ED_TIME[12 + 1]; //종료시간
				char	R_U_STATE[2 + 1]; //종료시간
				char	R_U_NAME[20 + 1]; //이름 한글 처리 때문에 맨 뒤로 뺌. 
				char	R_U_EDU[2 + 1]; //과목
				SQLINTEGER n_1, n_2, n_3, n_4, n_5, n_6;

				int nCnt = 0;
				BOOL m_errCheck = true;

				memset(R_U_ID, 0x00, sizeof(R_U_ID));
				memset(R_U_ST_TIME, 0x00, sizeof(R_U_ST_TIME));
				memset(R_U_ED_TIME, 0x00, sizeof(R_U_ED_TIME));
				memset(R_U_STATE, 0x00, sizeof(R_U_STATE));
				memset(R_U_NAME, 0x00, sizeof(R_U_NAME));
				memset(R_U_EDU, 0x00, sizeof(R_U_EDU));

				pMain->m_list_log_1.AddString(m_szSQLSentence);

				if (SQLPrepare(m_hSTMT, (SQLTCHAR*)m_szSQLSentence, _tcslen(m_szSQLSentence)) == SQL_ERROR)
				{
					SQL_ERROR_LOG(" SQLPrepare() Error", m_szSQLSentence);
					m_errCheck = false;
				}
				else
				{
					SQLBindCol(m_hSTMT, 1, SQL_C_CHAR, R_U_ID, sizeof(R_U_ID), &n_1);
					SQLBindCol(m_hSTMT, 2, SQL_C_CHAR, R_U_NAME, sizeof(R_U_NAME), &n_2);
					SQLBindCol(m_hSTMT, 3, SQL_C_CHAR, R_U_ST_TIME, sizeof(R_U_ST_TIME), &n_3);
					SQLBindCol(m_hSTMT, 4, SQL_C_CHAR, R_U_ED_TIME, sizeof(R_U_ED_TIME), &n_4);
					SQLBindCol(m_hSTMT, 5, SQL_C_CHAR, R_U_STATE, sizeof(R_U_STATE), &n_5);
					SQLBindCol(m_hSTMT, 6, SQL_C_CHAR, R_U_EDU, sizeof(R_U_EDU), &n_6);
				}

				if (SQLExecute(m_hSTMT) == SQL_ERROR)
				{
					SQL_ERROR_LOG(" SQLExecute() Error", m_szSQLSentence);
					SQLFreeStmt(m_hSTMT, SQL_CLOSE);
					m_errCheck = false;
				}
				else
				{
					nCnt = 0;
					while (1)
					{
						m_RetCode = SQLFetch(m_hSTMT);
						if (m_RetCode == SQL_ERROR)
						{
							SQL_ERROR_LOG(" SQLFetch() Error", m_szSQLSentence);
							SQLFreeStmt(m_hSTMT, SQL_CLOSE);
							m_errCheck = false;
							break;
						}
						else if (m_RetCode == SQL_NO_DATA_FOUND) //Data값을 찾지 못할경우 
						{
							SQLFreeStmt(m_hSTMT, SQL_CLOSE);
							m_errCheck = true;
							break;
						}
						else
						{
							//여기서 부터는 무조건 1부터 시작한다.
							//사유: 헷갈리니까 
							nCnt = nCnt + 1;
							m_errCheck = true;

							CutBlank(R_U_ID);
							CutBlank(R_U_NAME);
							CutBlank(R_U_ST_TIME);
							CutBlank(R_U_ED_TIME);
							CutBlank(R_U_STATE);
							CutBlank(R_U_EDU);

							if (nCnt == 1)
							{
								memcpy(MST_STR_LIST.CMD, "LIST1", sizeof(MST_STR_LIST.CMD));
								memcpy(MST_STR_LIST.DML, "SE", sizeof(MST_STR_LIST.DML));
								memcpy(MST_STR_LIST.RT, "0000", sizeof(MST_STR_LIST.RT));

								memcpy(MST_STR_LIST.U_ID_1, R_U_ID, strlen(R_U_ID));
								memcpy(MST_STR_LIST.U_NAME_1, R_U_NAME, strlen(R_U_NAME));
								memcpy(MST_STR_LIST.U_ST_TIME_1, R_U_ST_TIME, strlen(R_U_ST_TIME));
								memcpy(MST_STR_LIST.U_ED_TIME_1, R_U_ED_TIME, strlen(R_U_ED_TIME));
								memcpy(MST_STR_LIST.U_STATE_1, R_U_STATE, strlen(R_U_STATE));
								memcpy(MST_STR_LIST.U_EDU_1, R_U_EDU, strlen(R_U_EDU));
							}
							else if (nCnt == 2)
							{
								memcpy(MST_STR_LIST.U_ID_2, R_U_ID, strlen(R_U_ID));
								memcpy(MST_STR_LIST.U_NAME_2, R_U_NAME, strlen(R_U_NAME));
								memcpy(MST_STR_LIST.U_ST_TIME_2, R_U_ST_TIME, strlen(R_U_ST_TIME));
								memcpy(MST_STR_LIST.U_ED_TIME_2, R_U_ED_TIME, strlen(R_U_ED_TIME));
								memcpy(MST_STR_LIST.U_STATE_2, R_U_STATE, strlen(R_U_STATE));
								memcpy(MST_STR_LIST.U_EDU_2, R_U_EDU, strlen(R_U_EDU));
							}
							else if (nCnt == 3)
							{
								memcpy(MST_STR_LIST.U_ID_3, R_U_ID, strlen(R_U_ID));
								memcpy(MST_STR_LIST.U_NAME_3, R_U_NAME, strlen(R_U_NAME));
								memcpy(MST_STR_LIST.U_ST_TIME_3, R_U_ST_TIME, strlen(R_U_ST_TIME));
								memcpy(MST_STR_LIST.U_ED_TIME_3, R_U_ED_TIME, strlen(R_U_ED_TIME));
								memcpy(MST_STR_LIST.U_STATE_3, R_U_STATE, strlen(R_U_STATE));
								memcpy(MST_STR_LIST.U_EDU_3, R_U_EDU, strlen(R_U_EDU));
							}
							else if (nCnt == 4)
							{
								memcpy(MST_STR_LIST.U_ID_4, R_U_ID, strlen(R_U_ID));
								memcpy(MST_STR_LIST.U_NAME_4, R_U_NAME, strlen(R_U_NAME));
								memcpy(MST_STR_LIST.U_ST_TIME_4, R_U_ST_TIME, strlen(R_U_ST_TIME));
								memcpy(MST_STR_LIST.U_ED_TIME_4, R_U_ED_TIME, strlen(R_U_ED_TIME));
								memcpy(MST_STR_LIST.U_STATE_4, R_U_STATE, strlen(R_U_STATE));
								memcpy(MST_STR_LIST.U_EDU_4, R_U_EDU, strlen(R_U_EDU));
							}
							else if (nCnt == 5)
							{
								memcpy(MST_STR_LIST.U_ID_5, R_U_ID, strlen(R_U_ID));
								memcpy(MST_STR_LIST.U_NAME_5, R_U_NAME, strlen(R_U_NAME));
								memcpy(MST_STR_LIST.U_ST_TIME_5, R_U_ST_TIME, strlen(R_U_ST_TIME));
								memcpy(MST_STR_LIST.U_ED_TIME_5, R_U_ED_TIME, strlen(R_U_ED_TIME));
								memcpy(MST_STR_LIST.U_STATE_5, R_U_STATE, strlen(R_U_STATE));
								memcpy(MST_STR_LIST.U_EDU_5, R_U_EDU, strlen(R_U_EDU));
							}
							memset(R_U_ID, 0x00, sizeof(R_U_ID));
							memset(R_U_ST_TIME, 0x00, sizeof(R_U_ST_TIME));
							memset(R_U_ED_TIME, 0x00, sizeof(R_U_ED_TIME));
							memset(R_U_STATE, 0x00, sizeof(R_U_STATE));
							memset(R_U_NAME, 0x00, sizeof(R_U_NAME));
							memset(R_U_EDU, 0x00, sizeof(R_U_EDU));
						}
					}//while 

					if (m_errCheck == true)
					{
						MST_STR_LIST.U_END[0] = '#';
						memcpy(szSendBuff, (char*)&MST_STR_LIST, sizeof(MST_STR_LIST));
					}

				}
			}//여기까지 SE 
		}///데이터가 있을때만

		if (strlen(szSendBuff) >= 4)
		{

		}
		else
		{
			if (memcmp(DML, "IN", 2) == 0 && memcmp(DML, "UP", 2) == 0 && memcmp(DML, "DE", 2) == 0)
			{
				memcpy(MST_STR.RT, "9999", sizeof(MST_STR.RT)); //실패
				memcpy(szSendBuff, (char*)&MST_STR, sizeof(MST_STR));
			}
			else if (memcmp(DML, "SE", 2) == 0)
			{
				memcpy(MST_STR_LIST.CMD, "LIST1", sizeof(MST_STR_LIST.CMD));
				memcpy(MST_STR_LIST.DML, "SE", sizeof(MST_STR_LIST.DML));
				memcpy(MST_STR_LIST.RT, "9999", sizeof(MST_STR_LIST.RT)); //실패
				MST_STR_LIST.U_END[0] = '#';
				memcpy(szSendBuff, (char*)&MST_STR_LIST, sizeof(MST_STR_LIST));
			}
			else
			{
				memset(szSendBuff, 0x00, sizeof(szSendBuff));
				strcpy_s(szSendBuff, sizeof(szSendBuff), "ERROR");

			}
		}

		if (S_CLOSE_CHECK == 1)
		{
			str.Format("IP(%s) PORT(%d)의 CLIENT SEND 연결종료.", strDestAddr, unPort);
			pMain->m_list_log_1.AddString(str);

		}
		else
		{


			nSendLen = Send(szSendBuff, strlen(szSendBuff), MSG_DONTROUTE);
			if (nLenth > 0)
			{
				str.Format("CLIENT: IP(%s) PORT(%d)\n SEND한 내용(%s)", strDestAddr, unPort, szSendBuff);
				pMain->m_list_log_1.AddString(str);

				Log_to_file(1, str);
			}
		}

	}
	else
	{
		str.Format("CLIENT: IP(%s) PORT(%d) RECV한 내용(%s)", strDestAddr, unPort, "없음");
		pMain->m_list_log_1.AddString(str);
	}

	ODBC_FREE(); //종료
	CListenSocket* pServerSocket = (CListenSocket*)m_plistenSocket;
	pServerSocket->CloseClientSocket(this);
	CSocket::OnReceive(nErrorCode);
}



void CClientSocket::OnClose(int nErrorCode)
{
	S_CLOSE_CHECK = 1;
}

void CClientSocket::Log_to_file(int logidx, LPCTSTR szFormat, ...)
{
	SYSTEMTIME st;
	CString strFileName;
	CString strMsg = "";
	CString strLogData = "";
	CTSERVER01Dlg* pMain = (CTSERVER01Dlg*)AfxGetMainWnd(); // CSocketServerDlg의 핸들을 가져옴	
	CString strVIEW = "";

	GetLocalTime(&st);

	va_list argptr;
	va_start(argptr, szFormat);
	int nBuffLen = _vsctprintf(szFormat, argptr) + 1; // _vscprintf doesn't count terminating '\0'
	LPTSTR pszBuffer = new TCHAR[nBuffLen];
	_vstprintf_s(pszBuffer, nBuffLen, szFormat, argptr);
	va_end(argptr);

	strMsg.Format("%02d:%02d:%02d-", st.wHour, st.wMinute, st.wSecond);

	strMsg += pszBuffer;
	strMsg += _T("\n");

	strFileName.Format("%04d%02d%02d.log", st.wYear, st.wMonth, st.wDay);

	FILE* pFile = _tfsopen(strFileName, _T("at"), _SH_DENYNO);
	if (pFile == NULL)
	{
		OutputDebugString(_T("log file open error!"));
		return;
	}

	fwrite(strMsg, _tcslen(strMsg), 1, pFile);
	fclose(pFile);

	delete[] pszBuffer;


	strVIEW.Format("%s", strMsg);
	pMain->m_list_log_1.AddString(strVIEW);
	return;
}

void CClientSocket::CutBlank(TCHAR* szString)
{
	int nLen = _tcslen(szString) - 1;

	for (int i = nLen; i >= 0; i--)
	{
		if (szString[i] != 0x0d && szString[i] != 0x0a && szString[i] != ' ' && szString[i] != '\t')
			break;
		szString[i] = '\0';
	}
}


int CClientSocket::ODBC_SET(int logIDX, LPCTSTR strODBCName, LPCTSTR strUser, LPCTSTR strPassword)
{
	m_LOGIDX = logIDX;
	SQLUINTEGER uIntVal = 10;
	m_RetCode = SQLAllocEnv(&m_hENV); 	//ODBC 환경활당
	if (m_RetCode == SQL_SUCCESS)
	{
		m_RetCode = SQLAllocConnect(m_hENV, &m_hDBC); //Memory활당
		if (m_RetCode == SQL_SUCCESS)
		{
			SQLSetConnectAttr(m_hDBC, SQL_ATTR_AUTOCOMMIT, (void*)SQL_AUTOCOMMIT_ON, SQL_NTS);

			SQLSetConnectAttr(m_hDBC, SQL_ATTR_LOGIN_TIMEOUT, static_cast<SQLPOINTER>(&uIntVal), static_cast<SQLINTEGER>(sizeof(uIntVal)));;
			m_RetCode = SQLConnect(m_hDBC, (UCHAR*)strODBCName, SQL_NTS, (UCHAR*)strUser, SQL_NTS, (UCHAR*)strPassword, SQL_NTS); //Connect
			if (m_RetCode == SQL_SUCCESS || m_RetCode == SQL_SUCCESS_WITH_INFO)
			{
				m_RetCode = SQLAllocStmt(m_hDBC, &m_hSTMT);
				if (m_RetCode == SQL_SUCCESS)
				{
					Log_to_file(m_LOGIDX, _T("ODBC_SET Success Server[%s] UserName[%s]"), strODBCName, strUser);

					m_bODBCIs = TRUE;
					return(1); //성공적으로 연결
				}
				else
				{
					SQL_ERROR_LOG(_T("SQLAllocStmt"), "Conenct Error");
					SQLFreeStmt(m_hSTMT, SQL_DROP);
				}
			}
			else
			{
				SQL_ERROR_LOG(_T("SQLConnect"), "Conenct Error");
				SQLDisconnect(m_hDBC);
			}
		}
		else
		{
			SQL_ERROR_LOG(_T("SQLAllocConnect"), "Conenct Error");
			SQLFreeConnect(m_hDBC);
		}
	}
	else
	{
		SQL_ERROR_LOG(_T("SQLAllocEnv"), "Conenct Error");
		SQLFreeEnv(m_hENV);
	}

	return(-1);
}

//DB연결 해제
void CClientSocket::ODBC_FREE()
{
	try
	{
		CTSERVER01Dlg* pMain = (CTSERVER01Dlg*)AfxGetMainWnd();  // CSocketServerDlg의 핸들을 가져옴	
		CString strVIEW = "";
		strVIEW.Format("%s", "ODBC FREE");
		pMain->m_list_log_1.AddString(strVIEW);

		SQLDisconnect(m_hSTMT);

		SQLFreeHandle(SQL_HANDLE_DBC, m_hDBC);
		SQLFreeHandle(SQL_HANDLE_ENV, m_hENV);
	}
	catch (...)
	{

	}
}

void CClientSocket::SQL_ERROR_LOG(TCHAR* szLogMsg, TCHAR* szSQL)
{
	char szState[10]; memset(szState, 0x00, sizeof(szState));
	char szMsg[255]; memset(szMsg, 0x00, sizeof(szMsg));
	char szErrorMsg[300]; memset(szErrorMsg, 0x00, sizeof(szErrorMsg));
	SDWORD sdwNative;
	SWORD swMsgLen;

	SQLError(m_hENV, m_hDBC, m_hSTMT, (UCHAR*)szState, &sdwNative, (UCHAR*)szMsg, sizeof(szMsg), &swMsgLen);
	if (strlen(szMsg) == 0 || strlen(szState) == 0 || m_hENV == NULL || m_hDBC == NULL || m_hSTMT == NULL)
	{
		if (strlen(szMsg) > 2)
			wsprintf(szErrorMsg, "Error [%s][%s]", szLogMsg, szMsg);
		else
			wsprintf(szErrorMsg, "Error [%s]", szLogMsg);
	}

	//주의 wsprintf 사용, sprintf을 쓰면 Error
	char szTMPMSG[4038];

	memset(szTMPMSG, 0x00, sizeof(szTMPMSG));
	wsprintf(szTMPMSG, "SQLERROR [%s][%s]", szSQL, szErrorMsg);

	Log_to_file(m_LOGIDX, szTMPMSG);  //SQL Log는 파일에 직접 쓴다. 
	ODBC_FREE();

}


int CClientSocket::MSSQL_DML_EXEC()
{
	if (m_hSTMT != NULL)
	{
		SQLFreeStmt(m_hSTMT, SQL_DROP);
		m_hSTMT = NULL;
	}

	m_RetCode = SQLAllocStmt(m_hDBC, &m_hSTMT);
	if (m_RetCode != SQL_SUCCESS)
	{
		SQL_ERROR_LOG("DB 오류 1 .", m_szSQLSentence);
		m_hSTMT = NULL;
		return -1;
	}
	SQLINTEGER m_RetCode2 = SQLExecDirect(m_hSTMT, (SQLTCHAR*)m_szSQLSentence, SQL_NTS);
	if (m_RetCode2 == SQL_ERROR)
	{
		SQL_ERROR_LOG("DB 오류 2 ", m_szSQLSentence);
		SQLFreeStmt(m_hSTMT, SQL_CLOSE);
		return -1;
	}

	SQLFreeStmt(m_hSTMT, SQL_CLOSE);

	return 1;
}

void CClientSocket::myWait(DWORD dwMillisecond)
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