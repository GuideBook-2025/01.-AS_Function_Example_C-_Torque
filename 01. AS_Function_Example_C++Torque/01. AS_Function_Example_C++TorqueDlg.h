
// 01. AS_Function_Example_C++TorqueDlg.h: 헤더 파일
//

#pragma once

#include "CRealTimeGraphCtrl.h"   // 그래프 클래스 헤더 포함

// CMy01ASFunctionExampleCTorqueDlg 대화 상자
class CMy01ASFunctionExampleCTorqueDlg : public CDialogEx
{
// 생성입니다.
public:
	CMy01ASFunctionExampleCTorqueDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY01_AS_FUNCTION_EXAMPLE_CTORQUE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	// ====== 여기부터 그래프 관련 멤버 ======
	CRealTimeGraphCtrl m_Graph;   // 전용 그래프 컨트롤
	double             m_dPhase;  // 테스트용 (sin 파형 위상)
	// =======================================

public:
	BOOL InitLibrary(void);
	BOOL AddAxisInfo(void);
	CString m_strTime;
	long m_lAxisCount;
	long m_lAxisNo;
	DWORD m_dwModuleID;
	void ControlInit(void);

	CComboBox m_ComboTorqueAxis;
	CComboBox m_ComboTorqueReadSet;
	CComboBox m_ComboEcatTorqueSet;
	CComboBox m_ComboTorqueLimitTarget;
	afx_msg void OnBnClickedBtnTorquReadApply();
	afx_msg void OnBnClickedBtnTorquelimitapply();
	CEdit m_EditPtorqueLimit;
	CEdit m_EditMtorqueLimit;
	afx_msg void OnBnClickedBtnEcatTorquereadapply();
	afx_msg void OnBnClickedBtnTorquestart();
	CComboBox m_ComboAccFiterSel;
	CComboBox m_ComboGainSel;
	CComboBox m_ComboSpdLoopSel;
	CEdit m_EditMaxTorque;
	CEdit m_EditMaxVel;
	afx_msg void OnBnClickedBtnTorquestop();
	CComboBox m_ComboStopMethod;
	afx_msg void OnBnClickedCheckServoon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DWORD m_dwAlarmOn;
	afx_msg void OnBnClickedBtnAlarmclear();
};
