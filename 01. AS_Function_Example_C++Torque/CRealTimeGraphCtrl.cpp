#include "pch.h"
#include "CRealTimeGraphCtrl.h"
#include <cmath>

BEGIN_MESSAGE_MAP(CRealTimeGraphCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CRealTimeGraphCtrl::CRealTimeGraphCtrl()
	: m_clrBack(RGB(0, 0, 0))     // 배경색
	, m_clrLine(RGB(255, 0, 0))   // 그래프 선색
	, m_nMaxPoints(0)
	, m_dMinValue(-1.0)
	, m_dMaxValue(1.0)

	// [A] 백버퍼 초기 상태
	, m_pOldBackBmp(nullptr)
	, m_bBackReady(false)

	// [B] 메모리 버퍼 초기 상태
	, m_pOldMemBmp(nullptr)
	, m_bMemReady(false)
{
	m_rcClient.SetRectEmpty();
}

CRealTimeGraphCtrl::~CRealTimeGraphCtrl()
{
	// DC(m_dcMem)가 실제 HDC를 보유하고 있는지 확인
	if (m_dcMem.GetSafeHdc())
	{
		// dc는 반드시 원복 후 삭제
		if (m_pOldMemBmp != nullptr)
		{
			m_dcMem.SelectObject(m_pOldMemBmp);
			m_pOldMemBmp = nullptr;   // 안전하게 초기화
		}

		// DC 삭제 (CreateCompatibleDC로 생성된 DC의 해제)
		m_dcMem.DeleteDC();
	}

	// 비트맵 삭제
	if (m_bmpMem.GetSafeHandle())
	{
		m_bmpMem.DeleteObject();
	}
}

void CRealTimeGraphCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();

	CRect rc;
	GetClientRect(&rc);
	if (rc.Width() > 0 && rc.Height() > 0)
	{
		CClientDC dc(this);
		RecreateBuffers(rc.Width(), rc.Height(), &dc);
	}
}

void CRealTimeGraphCtrl::RecreateBuffers(int cx, int cy, CDC* pRefDC)
{

	m_rcClient.SetRect(0, 0, cx, cy);
	CClientDC dc(this);

	// ++====================================================================
	// >> 기존 DC/Bitmap 정리
	// ======================================================================

	// ===========================================
	// [A] 배경 전용 버퍼
	// dc 제거
	if (m_dcBack.GetSafeHdc())
	{
		if (m_pOldBackBmp)
		{
			// 기존 비트맵을 다시 DC에 연결하여 상태 복원
			m_dcBack.SelectObject(m_pOldBackBmp);

			// 포인터 초기화 (재사용 방지)
			m_pOldBackBmp = nullptr;
		}

		// 백버퍼 제거
		m_dcBack.DeleteDC();
	}

	// 비트맵 제거
	if (m_bmpBack.GetSafeHandle())
		m_bmpBack.DeleteObject();

	// ===========================================
	// [B] 메인 그리기용 세트 (그래프/최종 이미지 버퍼)

	// dc 제거
	if (m_dcMem.GetSafeHdc())
	{
		if (m_pOldMemBmp)
		{
			m_dcMem.SelectObject(m_pOldMemBmp);
			m_pOldMemBmp = nullptr;
		}
		m_dcMem.DeleteDC();
	}

	// 비트맵 제거
	if (m_bmpMem.GetSafeHandle())
		m_bmpMem.DeleteObject();

	m_bBackReady = false;
	m_bMemReady = false;

	// ++====================================================================
	// >> 배경 DC/Bitmap 생성
	// ======================================================================
	// [A] 배경 DC/Bitmap 생성
	if (m_dcBack.CreateCompatibleDC(&dc))
	{
		if (m_bmpBack.CreateCompatibleBitmap(&dc, cx, cy))
		{
			m_pOldBackBmp = m_dcBack.SelectObject(&m_bmpBack);
			m_bBackReady = true;

			// 여기서 배경 한 번만 그림 (검은 배경 + 격자/축 등)
			// └─ OnPaint에서 다시 그리지 않음
			m_dcBack.FillSolidRect(&m_rcClient, m_clrBack);

			// 필요 시: 격자/축 그리기
			DrawGridAndAxis(&m_dcBack);
		}
	}

	// [B] 메모리 DC/Bitmap 생성
	if (m_dcMem.CreateCompatibleDC(&dc))
	{
		if (m_bmpMem.CreateCompatibleBitmap(&dc, cx, cy))
		{
			m_pOldMemBmp = m_dcMem.SelectObject(&m_bmpMem);
			m_bMemReady = true;
		}
	}
}



void CRealTimeGraphCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
	if (cx <= 0 || cy <= 0)
		return;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CClientDC dc(this);
	RecreateBuffers(cx, cy, &dc);
}

void CRealTimeGraphCtrl::DrawGridAndAxis(CDC* pDC)
{
	// 예시: 연한 회색 격자
	CPen pen(PS_SOLID, 1, RGB(60, 60, 60));
	CPen* pOldPen = pDC->SelectObject(&pen);

	const int stepX = 50;
	const int stepY = 50;

	for (int x = m_rcClient.left; x < m_rcClient.right; x += stepX)
		pDC->MoveTo(x, m_rcClient.top), pDC->LineTo(x, m_rcClient.bottom);

	for (int y = m_rcClient.top; y < m_rcClient.bottom; y += stepY)
		pDC->MoveTo(m_rcClient.left, y), pDC->LineTo(m_rcClient.right, y);

	pDC->SelectObject(pOldPen);
}


void CRealTimeGraphCtrl::OnPaint()
{
	CPaintDC dc(this); 
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	// 클라이언트 영역이 비정상적일 경우
	if (!m_bBackReady || !m_bMemReady || m_rcClient.IsRectEmpty())
	{
		// 준비가 안 되어 있으면 최소한의 처리
		dc.FillSolidRect(&m_rcClient, m_clrBack);
		return;
	}

	// (1) 배경 비트맵을 메모리 DC로 복사
	m_dcMem.BitBlt(
		0, 0,
		m_rcClient.Width(),
		m_rcClient.Height(),
		&m_dcBack,
		0, 0,
		SRCCOPY
	);

	// (2) 새 그래프 선만 m_dcMem에 그림
	DrawGraph(&m_dcMem);

	// (3) m_dcMem을 실제 화면으로 복사
	dc.BitBlt(
		0, 0,
		m_rcClient.Width(),
		m_rcClient.Height(),
		&m_dcMem,
		0, 0,
		SRCCOPY
	);
}

void CRealTimeGraphCtrl::Initialize(int nMaxPoints, double dMin, double dMax)
{
	// 그래프가 저장할 최대 데이터 개수를 설정
	// 예: 1000이면 최대 1000개의 시계열 데이터만 보관
	m_nMaxPoints = nMaxPoints;

	// 그래프의 Y축 최소값 설정
	m_dMinValue = dMin;

	// 그래프의 Y축 최대값 설정
	m_dMaxValue = dMax;

	// 기존 그래프 데이터를 모두 초기화
	m_vData.clear();

	// 현재 컨트롤의 클라이언트 영역(Rect)을 얻음
	// OnSize가 호출되기 전 초기 1회만 필요한 값
	GetClientRect(&m_rcClient);
}

bool CRealTimeGraphCtrl::AddData(double dValue)
{
	// 그래프가 저장할 수 있는 최대 데이터 개수가 유효한지 검사
	// (0 이하라면 데이터 추가 자체를 수행할 수 없음)
	if (m_nMaxPoints <= 0)
		return false;

	// 현재 데이터 개수가 최대 개수에 도달한 경우,
	// 가장 오래된 데이터(맨 앞)를 제거하여 공간 확보
	if (static_cast<int>(m_vData.size()) >= m_nMaxPoints)
		m_vData.erase(m_vData.begin());

	// 새로운 데이터를 벡터의 끝에 추가
	m_vData.push_back(dValue);

	// 화면 갱신 요청
	//  - 여기서는 단순히 다시 그려달라고 Only Invalidate
	//  - 실제 크기 계산 등은 OnSize에서만 수행하여 불필요한 비용을 제거
	Invalidate(FALSE);

	return true;
}

BOOL CRealTimeGraphCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 깜빡임 방지: 배경은 OnPaint에서 메모리 DC로만 처리
	return TRUE;
}







// ++====================================================================
// >> ValueToY(): 스케일링 (정규화 함수).
// ======================================================================
int CRealTimeGraphCtrl::ValueToY(double dValue) const
{
	// 최소/최대 값이 같다면 스케일링 불가능 → 그래프의 바닥(y = bottom)으로 처리
	if (m_dMaxValue == m_dMinValue)
		return m_rcClient.bottom;

	// ------------------------------------------------------------
	// (1) 입력값 dValue를 0~1 사이의 비율로 정규화
	//
	//   dValue = m_dMinValue  → t = 0.0
	//   dValue = m_dMaxValue  → t = 1.0
	//   그 사이 값은 0~1로 선형 매핑
	// ------------------------------------------------------------
	double t = (dValue - m_dMinValue) / (m_dMaxValue - m_dMinValue);

	// t 범위 클램프(안전성 확보)
	//  그래프 값이 범위를 벗어나더라도 y좌표는 화면 안에서 유지
	//t = max(0.0, min(1.0, t));

	// 클라이언트 높이
	int height = m_rcClient.Height();
	if (height <= 1)
		return m_rcClient.bottom;

	// ------------------------------------------------------------
	// (2) 정규화된 t(0~1)를 실제 화면 y좌표로 변환
	//
	//   화면 좌표계는 y가 아래로 증가함.
	//     t = 0 → 그래프 최소값 → 화면의 bottom
	//     t = 1 → 그래프 최대값 → 화면의 top
	//
	//   따라서 bottom - (t * height)로 변환해야
	//   "값이 커질수록 위로 올라가는 그래프"가 된다.
	// ------------------------------------------------------------
	int y = m_rcClient.bottom - static_cast<int>(t * (height - 1));

	return y;
}


// ++====================================================================
// >> DrawGraph(): 그래프를 출력하는 함수이다.
// ======================================================================
void CRealTimeGraphCtrl::DrawGraph(CDC* pDC)
{
	// 현재 누적된 데이터 개수
	const int nCount = static_cast<int>(m_vData.size());

	// 2점 미만이면 선을 그릴 수 없으므로 종료
	if (nCount < 2)
		return;

	// 클라이언트 영역의 폭
	int width = m_rcClient.Width();

	// 폭이 1 이하라면 정상적인 그리기 불가
	if (width <= 1)
		return;

	// ------------------------------------------------------------
	// x 좌표 간격 계산
	//
	// 예: 픽셀 폭이 300이고 데이터가 100개면,
	//     각 데이터 간 X 간격 = (300 - 1) / (100 - 1)
	//
	// width-1 / (nCount-1) : 좌→우 균일한 간격 배치
	// ------------------------------------------------------------
	double xStep = (nCount > 1)
		? static_cast<double>(width - 1) / (nCount - 1)
		: 0.0;

	// 그래프 선 색상을 위한 펜 생성
	CPen pen(PS_SOLID, 1, m_clrLine);
	CPen* pOldPen = pDC->SelectObject(&pen);

	// ------------------------------------------------------------
	// 첫 번째 점 이동
	// ------------------------------------------------------------
	int x = m_rcClient.left;
	int y = ValueToY(m_vData[0]);   // 실제 데이터 값을 Y 좌표로 변환
	pDC->MoveTo(x, y);

	// ------------------------------------------------------------
	// 나머지 데이터 점들을 순차적으로 연결 (LineTo)
	// ------------------------------------------------------------
	for (int i = 1; i < nCount; ++i)
	{
		x = m_rcClient.left + static_cast<int>(xStep * i);
		y = ValueToY(m_vData[i]);
		pDC->LineTo(x, y);   // 선 그리기
	}

	// 사용한 펜 원복
	pDC->SelectObject(pOldPen);
}


