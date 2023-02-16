
// T_SERVER01Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "T_SERVER01.h"
#include "T_SERVER01Dlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTSERVER01Dlg 대화 상자



CTSERVER01Dlg::CTSERVER01Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_T_SERVER01_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTSERVER01Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST1, m_list_log_1);
	//  DDX_Control(pDX, IDC_LIST2, m_list_client_log);
	//  DDX_Control(pDX, IDC_LIST3, m_list_log_3);
	DDX_Control(pDX, IDC_LIST3, m_list_log_1);
	DDX_Control(pDX, IDC_LIST4, m_list_client_log);
}

BEGIN_MESSAGE_MAP(CTSERVER01Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CTSERVER01Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_START, &CTSERVER01Dlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CTSERVER01Dlg::OnBnClickedBtnExit)
	ON_LBN_SELCHANGE(IDC_LIST3, &CTSERVER01Dlg::OnLbnSelchangeList3)
	//ON_EN_CHANGE(IDC_EDIT1, &CTSERVER01Dlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CTSERVER01Dlg 메시지 처리기

BOOL CTSERVER01Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보" 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//포트 기본 값 설정
	SetDlgItemText(IDC_EDIT_PORT, "11051");

	//DB관련 ODBC 정보 추가 
	SetDlgItemText(IDC_EDIT_DB_DSN, "KHY_DSN");
	SetDlgItemText(IDC_EDIT_DB_USER, "khy");
	SetDlgItemText(IDC_EDIT_DB_PWD, "khy1229*");

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTSERVER01Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTSERVER01Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTSERVER01Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTSERVER01Dlg::OnBnClickedButton2()
{
	AfxMessageBox("hello");
	//AfxMessageBox(_T("hello"));
}

//server 시작 
void CTSERVER01Dlg::OnBnClickedBtnStart()
{
	//AfxMessageBox("시작합니다");
	CString sPORT = " ";
	CString sLOG = " ";
	GetDlgItemText(IDC_EDIT_PORT, sPORT);
	
	sLOG.Format("포트번호 [%d] 시작합니다.", _ttoi(sPORT));
	m_list_log_1.AddString(sLOG);

	sLOG.Format("%s", "Client 로그입니다");
	m_list_client_log.AddString(sLOG);
	
	//listen 시작 
	if (m_ListenSocket.Create(_ttoi(sPORT), SOCK_STREAM)) {
		if (!m_ListenSocket.Listen()) {
			m_list_log_1.AddString("listen 실패");
			AfxMessageBox("listen 실패");
		}
		else {
			m_list_log_1.AddString("listen 성공");
		}
	}
	else {
		m_list_log_1.AddString("소캣 서버 생성 실패");
		AfxMessageBox("소캣 서버 생성 실패");
	}
}


void CTSERVER01Dlg::OnBnClickedBtnExit()
{
	AfxMessageBox("종료합니다");
}


void CTSERVER01Dlg::OnLbnSelchangeList3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

/*
void CTSERVER01Dlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
*/