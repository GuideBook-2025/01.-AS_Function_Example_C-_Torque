
// 01. AS_Function_Example_C++TorqueDlg.h: 헤더 파일
//

#pragma once


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

public:
	BOOL InitLibrary(void);
	BOOL AddAxisInfo(void);
	CString m_strTime;
	long m_lAxisCount;
	long m_lAxisNo;
	DWORD m_dwModuleID;


	CComboBox m_ComboTorqueAxis;
};
