
// 01. AS_Function_Example_C++TorqueDlg.h: 헤더 파일
//

#pragma once

#include "CRealTimeGraphCtrl.h"   // 그래프 클래스 헤더 포함
enum TorqueFlag
{
	TORQUE_NONE = 0,
	TORQUE_LOAD_RATIO = 1 << 0,
	TORQUE_LIMIT = 1 << 1,
	TORQUE_ECAT_LOAD_RATIO = 1 << 2,
	TORQUE_ECAT_LIMIT = 1 << 3,
	TORQUE_DONE = 1 <<4,
	TORQUE_START_TEST = 1 << 5,
	TORQUE_TEST_PLUS = 1 << 6
};

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
	void			   SetGraphIntialize();
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	void InitAxis();
	void InMotionCheck();
	void MoveStop();
	void PositionMove(double dPos, double dVel, double dAcc, double dDec);
	BOOL CheckTorqueSetup();  
	DWORD m_TorqueSetupFlags;
	void RunCompressionTest();
	void TorqueStart(bool bDirection);
	bool CheckTorqueMin();
	afx_msg void OnBnClickedBtnTorquetest();
	afx_msg void OnBnClickedCheckServoon2();
	void WaitWithMessagePump(int axis);


};
