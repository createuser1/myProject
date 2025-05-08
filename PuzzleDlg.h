
// PuzzleDlg.h: 헤더 파일
//

#pragma once


// CPuzzleDlg 대화 상자
class CPuzzleDlg : public CDialogEx
{
// 생성입니다.
public:
	CPuzzleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PUZZLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	CBitmap m_bm, m_hit;

	int click, start, end, m_Time;
	int m_game[5][5];
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_click;
	CString m_time;
	CString m_per;
	BOOL m_bhit;
	void EndingView();
	void ViewPic(int iCol, int iRow, int iNum);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void HintView();
	void DrawLine();
	void ClearView(int iCol, int iRow);
	BOOL FindEptyIndex(int iRow, int iCol, int& iEmptyRow, int& iEmptyCol);
	void MovePic(int iRow, int iCol, int iEmptyRos, int iEmptyCol);
	void RndPic();
	afx_msg void OnBnClickedButtonStart();
	BOOL IsEnd();
	afx_msg void OnBnClickedOk();
	BOOL PosToIndex(CPoint pnt, int& iRow, int& iCol);
	void SetPercent();
	void EndGame();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
};
