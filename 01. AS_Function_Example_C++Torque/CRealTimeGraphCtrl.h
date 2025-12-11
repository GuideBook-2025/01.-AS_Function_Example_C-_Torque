#pragma once

#include <vector>

class CRealTimeGraphCtrl : public CStatic
{
public:
    CRealTimeGraphCtrl();
    virtual ~CRealTimeGraphCtrl();

    // 초기화: 최대 포인트 개수, Y축 최소/최대
    void Initialize(int nMaxPoints, double dMin, double dMax);

    // 실시간 데이터 추가
    bool AddData(double dValue);

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
    // 내부 그리기용
    void DrawGraph(CDC* pDC);
    int  ValueToY(double dValue) const;
    void DrawGridAndAxis(CDC* pDC);
	void PreSubclassWindow();
    void RecreateBuffers(int cx, int cy, CDC* pRefDC);

private:
    // 클라이언트 영역 (OnSize에서만 갱신)
    CRect    m_rcClient;

    // 색상
    COLORREF m_clrBack;
    COLORREF m_clrLine;

    // 데이터 버퍼
    std::vector<double> m_vData;
    int      m_nMaxPoints;
    double   m_dMinValue;
    double   m_dMaxValue;

    // [A] 배경용 DC/Bitmap (격자, 축 포함)
    CDC      m_dcBack;
    CBitmap  m_bmpBack;
    CBitmap* m_pOldBackBmp = nullptr;
    bool     m_bBackReady = false;

    // [B] 최종 출력용 메모리 DC/Bitmap
    CDC      m_dcMem;
    CBitmap  m_bmpMem;
    CBitmap* m_pOldMemBmp = nullptr;
    bool     m_bMemReady = false;
};


