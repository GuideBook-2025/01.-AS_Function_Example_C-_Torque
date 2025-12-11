
// 01. AS_Function_Example_C++TorqueDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "01. AS_Function_Example_C++Torque.h"
#include "01. AS_Function_Example_C++TorqueDlg.h"
#include "afxdialogex.h"
#define UI_DATA_CHECK 10

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	TM_DISPLAY		100
#define	LIBTYPE			4000  //ECAT Lib : 5000  / 범용제품 : 4000 에 따라 선택	
// ++ =======================================================================
// >> AXL(AjineXtek Library) 관련 Header파일 포함시킵니다.
// ※ [CAUTION] 예제를 복사하여 다른 위치에서 실행시 참조경로 변경이 필요합니다.
// --------------------------------------------------------------------------
#include <C:\Program Files (x86)\EzSoftware UC\AXL(Library)\C, C++\AXL.h>
#include <C:\Program Files (x86)\EzSoftware UC\AXL(Library)\C, C++\AXM.h>
#include <C:\Program Files (x86)\EzSoftware UC\AXL(Library)\C, C++\AXDev.h>
#include <C:\Program Files (x86)\EzSoftware UC\AXL(Library)\C, C++\AXHS.h>
#include <C:\Program Files (x86)\EzSoftware UC\AXL(Library)\C, C++\AXHD.h>

// ++ =======================================================================
// >> AXL(AjineXtek Library) 관련 Library파일을 Link시킵니다.
// ※ [CAUTION] 예제를 복사하여 다른 위치에서 실행시 참조경로 변경이 필요합니다.
// --------------------------------------------------------------------------
#ifdef  _M_X64 
#pragma comment(lib, "C:/Program Files (x86)/EzSoftware UC/AXL(Library)/Library/64Bit/AXL.lib")
#else
#pragma comment(lib, "C:/Program Files (x86)/EzSoftware UC/AXL(Library)/Library/32Bit/AXL.lib")
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


// CMy01ASFunctionExampleCTorqueDlg 대화 상자



CMy01ASFunctionExampleCTorqueDlg::CMy01ASFunctionExampleCTorqueDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY01_AS_FUNCTION_EXAMPLE_CTORQUE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy01ASFunctionExampleCTorqueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MoveAxis, m_ComboTorqueAxis);
	DDX_Control(pDX, IDC_COMBO_TorqueReadSet, m_ComboTorqueReadSet);
	DDX_Control(pDX, IDC_COMBO_ECAT_TorqueReadSet, m_ComboEcatTorqueSet);
	DDX_Control(pDX, IDC_COMBO_TARGET, m_ComboTorqueLimitTarget);
}

BEGIN_MESSAGE_MAP(CMy01ASFunctionExampleCTorqueDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TorqueReadApply, &CMy01ASFunctionExampleCTorqueDlg::OnBnClickedBtnTorquReadApply)
END_MESSAGE_MAP()


// CMy01ASFunctionExampleCTorqueDlg 메시지 처리기

BOOL CMy01ASFunctionExampleCTorqueDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

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
	InitLibrary();
	AxmInfoGetAxisCount(&m_lAxisCount);
	if (m_lAxisCount > 1)
	{
		AddAxisInfo();
		ControlInit();
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMy01ASFunctionExampleCTorqueDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy01ASFunctionExampleCTorqueDlg::OnPaint()
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
HCURSOR CMy01ASFunctionExampleCTorqueDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// ==============================================================================================================================================================
// >> InitLibrary() : 라이브러리 초기화하는 함수.
//  - AXL(AjineXtek Library)을 사용가능하게 합니다. 
//  - AxlOpen        : 라이브러리 내부 파라미터 및 H/W 설정 파라미터의 초기화 후 라이브러리를 사용 가능하도록 준비하는 함수
//  - AxlOpenNoReset : 라이브러리 내부 파라미터 및 H/W 설정 파라미터의 초기화 과정 없이 라이브러리를 사용 가능하도록 준비하는 함수
// ==============================================================================================================================================================
BOOL CMy01ASFunctionExampleCTorqueDlg::InitLibrary(void)
{
	//++ AXL(AjineXtek Library)을 사용가능하게 하고 장착된 보드들을 초기화합니다.
	DWORD dwReturn;

	dwReturn = AxlOpen(7);
	SYSTEMTIME Time;	GetLocalTime(&Time);
	m_strTime.Format("%02d:%02d:%02d:%03d -> ", Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
	if (dwReturn != AXT_RT_SUCCESS)
	{
		AfxMessageBox("AxlOpen Fail...");
		return FALSE;
	}
	return TRUE;
}

// ==============================================================================================================================================================
// >> AddAxisInfo() : 시스템에 장착된 모션축들을 ComboBox에 등록하는 함수.
//  - 한 개이상의 축이 장착되면 검색된 축들의 정보를 읽어 ComboBox에 등록하여 사용자가 선택 제어할 수 있도록 합니다.
// ==============================================================================================================================================================
BOOL CMy01ASFunctionExampleCTorqueDlg::AddAxisInfo(void)
{
	CString	strData;
	DWORD	dwModuleID;

	//++ 유효한 전체 모션축수를 반환합니다.
	AxmInfoGetAxisCount(&m_lAxisCount);
	if (m_lAxisCount < 1)	return FALSE;

	m_lAxisNo = 0;
	//++ 지정한 축의 정보를 반환합니다.
	// [INFO] 여러개의 정보를 읽는 함수 사용시 불필요한 정보는 NULL(0)을 입력하면 됩니다.
	AxmInfoGetAxis(m_lAxisNo, NULL, NULL, &m_dwModuleID);

	// 유효한 전체 모션축수의 정보를 읽어 ComboBox에 등록합니다.
	for (int i = 0; i < m_lAxisCount; i++)
	{
		//++ 지정한 축의 정보를 반환합니다.
		// [INFO] 여러개의 정보를 읽는 함수 사용시 불필요한 정보는 NULL(0)을 입력하면 됩니다.
		AxmInfoGetAxis(i, NULL, NULL, &dwModuleID);
		switch (dwModuleID)
		{
		case AXT_SMC_R1V04A4:			strData.Format("%02ld-[RTEX-A4N]", i);		break;
		case AXT_SMC_R1V04A5:			strData.Format("%02ld-[RTEX-A5N]", i);		break;
		case AXT_SMC_R1V04A6:			strData.Format("%02ld-[RTEX-A6N]", i);		break;
		case AXT_SMC_R1V04:				strData.Format("%02ld-(RTEX-PM)", i);		break;
		case AXT_SMC_R1V04PM2Q:			strData.Format("%02ld-(RTEX-PM2Q)", i);		break;
		case AXT_SMC_R1V04PM2QE:		strData.Format("%02ld-(RTEX-PM2QE)", i);	break;
		case AXT_SMC_R1V04MLIISV:		strData.Format("%02ld-[MLII-SGDV]", i);		break;
		case AXT_SMC_R1V04MLIIPM:		strData.Format("%02ld-(MLII-PM)", i);		break;
		case AXT_SMC_R1V04MLIICL:		strData.Format("%02ld-[MLII-CSDL]", i);		break;
		case AXT_SMC_R1V04MLIICR:		strData.Format("%02ld-[MLII-CSDH]", i);		break;
		case AXT_SMC_R1V04SIIIHMIV:		strData.Format("%02ld-(SIIIH-MRJ4)", i);	break;
		case AXT_SMC_R1V04SIIIHMIV_R:	strData.Format("%02ld-(SIIIH-MRJ4)", i);	break;
		case AXT_SMC_R1V04PM2QSIIIH:	strData.Format("%02ld-(SIIIH-PM2Q)", i);	break;
		case AXT_SMC_R1V04PM4QSIIIH:	strData.Format("%02ld-(SIIIH-PM4Q)", i);	break;
		case AXT_SMC_R1V04MLIIIS7S:		strData.Format("%02ld-(MLIII-SGD7S)", i);	break;
		case AXT_SMC_R1V04MLIIIS7W:		strData.Format("%02ld-(MLIII-SGD7W)", i);	break;
		case AXT_SMC_R1V04MLIIIPM:		strData.Format("%02ld-(MLIII-PM)", i);	break;
#if LIBTYPE == 5000
		case AXT_ECAT_MOTION:
		{
			char szModuleName[50];
			AxmInfoGetAxisEx(i, NULL, szModuleName, NULL);
			strData.Format("%02ld-[ECAT-%s]", i, szModuleName);
			break;
		}
		default:	strData.Format("%02ld-[Unknown]", i);
#elif LIBTYPE == 4000  default:strData.Format("%02ld-[Unknown]",i);
#endif
		}
		m_ComboTorqueAxis.AddString(strData);
	}
	return TRUE;
}

// ==============================================================================================================================================================
// >> ControlInit() : 컨트롤들을 초기화 및 정보 갱신용 타이머 동작 하는 함수
//  - 컨트롤들을 초기화 하고, 정보 갱신용 타이머를 50msec 주기로 동작시킵니다.
// ==============================================================================================================================================================
void CMy01ASFunctionExampleCTorqueDlg::ControlInit(void)
{
	m_ComboTorqueAxis.SetCurSel(0);

	switch (m_dwModuleID)
	{
	case AXT_SMC_R1V04A5:
		// (RTEX, A5Nx, A6Nx, dwSelMon : 0 ~ 4) ==
		//     [0] : Command Torque(0.1%)
		//     [1] : Regenerative load ratio(0.1%)
		//     [2] : Overload ratio(0.1%)
		//     [3] : Inertia ratio(%)
		//     [4] : Actual speed(rpm)
		m_ComboTorqueReadSet.AddString("0 : Command Torque(0.1%)");
		m_ComboTorqueReadSet.AddString("1 : Regenerative load ratio(0.1%)");
		m_ComboTorqueReadSet.AddString("2 : Overload ratio(0.1%)");
		m_ComboTorqueReadSet.AddString("3 : Inertia ratio(%)");
		m_ComboTorqueReadSet.AddString("4 : Actual speed(rpm)");
		m_ComboTorqueReadSet.SetCurSel(0);
		break;
	case AXT_SMC_R1V04A6:
		// (RTEX, A5Nx, A6Nx, dwSelMon : 0 ~ 4) ==
		//     [0] : Command Torque(0.1%)
		//     [1] : Regenerative load ratio(0.1%)
		//     [2] : Overload ratio(0.1%)
		//     [3] : Inertia ratio(%)
		//     [4] : Actual speed(rpm)
		m_ComboTorqueReadSet.AddString("0 : Command Torque(0.1%)");
		m_ComboTorqueReadSet.AddString("1 : Regenerative load ratio(0.1%)");
		m_ComboTorqueReadSet.AddString("2 : Overload ratio(0.1%)");
		m_ComboTorqueReadSet.AddString("3 : Inertia ratio(%)");
		m_ComboTorqueReadSet.AddString("4 : Actual speed(rpm)");
		m_ComboTorqueReadSet.SetCurSel(0);
		break;
	case AXT_SMC_R1V04MLIISV:
		// 지정 축의 부하율을 읽을 수 있도록 설정 합니다.(MLII : Sigma-5, SIIIH : MR_J4_xxB, RTEX : A5N, A6N 전용)
		// (MLII, Sigma-5, dwSelMon : 0 ~ 2) ==
		//     [0] : Accumulated load ratio(%)
		//     [1] : Regenerative load ratio(%)
		//     [2] : Reference Torque load ratio
		//     [3] : Motor rotation speed (rpm)
		m_ComboTorqueReadSet.AddString("0 : Accumlated load ratio(%)");
		m_ComboTorqueReadSet.AddString("1 : Regenerative load ratio(%)");
		m_ComboTorqueReadSet.AddString("2 : Reference Torque load ratio");
		m_ComboTorqueReadSet.AddString("3 : Motor rotation speed (rpm)");
		m_ComboTorqueReadSet.SetCurSel(0);
		break;
	case AXT_SMC_R1V04SIIIHMIV:
		// (SIIIH, MR_J4_xxB, dwSelMon : 0 ~ 4) ==
		//     [0] : Assumed load inertia ratio(0.1times)
		//     [1] : Regeneration load factor(%)
		//     [2] : Effective load factor(%)
		//     [3] : Peak load factor(%)
		//     [4] : Current feedback(0.1%)	
		//     [5] : Speed feedback(rpm)	
		m_ComboTorqueReadSet.AddString("0 : Assumed load inertia ratio(0.1times)");
		m_ComboTorqueReadSet.AddString("1 : Regeneration load factor(%)");
		m_ComboTorqueReadSet.AddString("2 : Effective load factor(%)");
		m_ComboTorqueReadSet.AddString("3 : Peak load factor(%)");
		m_ComboTorqueReadSet.AddString("4 : Current feedback(0.1%)");
		m_ComboTorqueReadSet.AddString("5 : Speed feedback(rpm)");
		m_ComboTorqueReadSet.SetCurSel(0);
		break;
	case AXT_SMC_R1V04SIIIHMIV_R:
		// (SIIIH, MR_J4_xxB, dwSelMon : 0 ~ 4) ==
		//     [0] : Assumed load inertia ratio(0.1times)
		//     [1] : Regeneration load factor(%)
		//     [2] : Effective load factor(%)
		//     [3] : Peak load factor(%)
		//     [4] : Current feedback(0.1%)	
		//     [5] : Speed feedback(rpm)	
		m_ComboTorqueReadSet.AddString("0 : Assumed load inertia ratio(0.1times)");
		m_ComboTorqueReadSet.AddString("1 : Regeneration load factor(%)");
		m_ComboTorqueReadSet.AddString("2 : Effective load factor(%)");
		m_ComboTorqueReadSet.AddString("3 : Peak load factor(%)");
		m_ComboTorqueReadSet.AddString("4 : Current feedback(0.1%)");
		m_ComboTorqueReadSet.AddString("5 : Speed feedback(rpm)");
		m_ComboTorqueReadSet.SetCurSel(0);
		break;
	}

	m_ComboEcatTorqueSet.AddString("00 : Actual Torque(0.1%)");
	m_ComboEcatTorqueSet.SetCurSel(0);

	m_ComboTorqueLimitTarget.AddString("00 : 목표 위치 기준");
	m_ComboTorqueLimitTarget.AddString("01 : 실제 위치 기준");
	m_ComboTorqueLimitTarget.SetCurSel(0);

	SetDlgItemText(IDC_EDIT_POS0, "POS: 0");
	SetDlgItemText(IDC_EDIT_POS1, "POS:1");
	SetDlgItemText(IDC_EDIT_POS2, "POS:2");
	SetDlgItemText(IDC_EDIT_POS3, "POS:3");
	SetDlgItemText(IDC_EDIT_POS4, "POS:4");
	SetDlgItemText(IDC_EDIT_POS5, "POS:5");
	SetDlgItemText(IDC_EDIT_POS6, "POS:6");
	SetDlgItemText(IDC_EDIT_POS7, "POS:7");

	SetDlgItemText(IDC_EDIT_PTORQUELIMIT0, "PPOS:0");
	SetDlgItemText(IDC_EDIT_PTORQUELIMIT1, "PPOS:1");
	SetDlgItemText(IDC_EDIT_PTORQUELIMIT2, "PPOS:2");
	SetDlgItemText(IDC_EDIT_PTORQUELIMIT3, "PPOS:3");
	SetDlgItemText(IDC_EDIT_PTORQUELIMIT4, "PPOS:4");
	SetDlgItemText(IDC_EDIT_PTORQUELIMIT5, "PPOS:5");
	SetDlgItemText(IDC_EDIT_PTORQUELIMIT6, "PPOS:6");
	SetDlgItemText(IDC_EDIT_PTORQUELIMIT7, "PPOS:7");

	SetDlgItemText(IDC_EDIT_MTORQUELIMIT0, "MPOS:0");
	SetDlgItemText(IDC_EDIT_MTORQUELIMIT1, "MPOS:1");
	SetDlgItemText(IDC_EDIT_MTORQUELIMIT2, "MPOS:2");
	SetDlgItemText(IDC_EDIT_MTORQUELIMIT3, "MPOS:3");
	SetDlgItemText(IDC_EDIT_MTORQUELIMIT4, "MPOS:4");
	SetDlgItemText(IDC_EDIT_MTORQUELIMIT5, "MPOS:5");
	SetDlgItemText(IDC_EDIT_MTORQUELIMIT6, "MPOS:6");
	SetDlgItemText(IDC_EDIT_MTORQUELIMIT7, "MPOS:7");

}


void CMy01ASFunctionExampleCTorqueDlg::OnBnClickedBtnTorquReadApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
