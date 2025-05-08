
// PuzzleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Puzzle.h"
#include "PuzzleDlg.h"
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
public:
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


// CPuzzleDlg 대화 상자



CPuzzleDlg::CPuzzleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PUZZLE_DIALOG, pParent)
	, m_click(_T(""))
	, m_time(_T(""))
	, m_per(_T(""))
	, click(0)
	, start(0)
	, end(0)
	, m_bhit(FALSE)
	, m_Time(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPuzzleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC1, m_click);
	DDX_Text(pDX, IDC_STATIC2, m_time);
	DDX_Text(pDX, IDC_STATIC3, m_per);
}

BEGIN_MESSAGE_MAP(CPuzzleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_START, &CPuzzleDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDOK, &CPuzzleDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPuzzleDlg 메시지 처리기
BOOL CPuzzleDlg::OnInitDialog()
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
	m_bm.LoadBitmap(IDB_BITMAP1);
	m_hit.LoadBitmap(IDB_BITMAP2);

	//설정한 아이콘에 5x5배열로 나눠 0-24 저장 각 칸이 움직일 오른쪽 아래 칸은 -1저장
	int k = 0;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			m_game[i][j] = k++;
	m_game[4][4] = -1;


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CPuzzleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPuzzleDlg::OnPaint()
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
	// game 종료, game 중
	if (!end)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				ViewPic(j, i, m_game[i][j]);
			}
		}
		DrawLine();
	}
	else
	{
		DrawLine();
		EndingView();
	}
	HintView();
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPuzzleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/// <summary>
/// game 종료, 전체 그림 보이기
/// </summary>

void CPuzzleDlg::EndingView()
{
	// TODO: 여기에 구현 코드 추가.
	CClientDC dc(this);
	CDC mem;
	mem.CreateCompatibleDC(&dc);
	CBitmap* old = mem.SelectObject(&m_bm);
	dc.StretchBlt(10, 10, 300, 300, &mem, 0, 0, 400,400,SRCCOPY);
	mem.SelectObject(old);
}


/// <summary>
/// 배열 값인 iNum에 해당하는 아이콘의 네모칸을 화면의 [iRow, iCol]에 출력하기
/// </summary>
/// <param name="iCol"></param>
/// <param name="iRow"></param>
/// <param name="iNum"></param>

void CPuzzleDlg::ViewPic(int iCol, int iRow, int iNum)
{
	// TODO: 여기에 구현 코드 추가.

	if (iNum == -1)
		return;
	int isize = 300 / 5;
	CClientDC dc(this);
	//그림 인덱스 값으로 2차원 배열의인덱스 계산하기

	int x = iNum % 5;
	int y = iNum / 5;

	CDC mem;
	mem.CreateCompatibleDC(&dc);
	CBitmap* old = mem.SelectObject(&m_bm);

	dc.StretchBlt(10 + (iCol * (isize+3)), 10 + (iRow * (isize + 3)), isize, isize, &mem, x * 80, y * 80, 80, 80, SRCCOPY);
	mem.SelectObject(old);
	//iNum인덱스의 값으로 icol, irow의 위치에 그리기만하고 인덱스 값을 icol, irow위치에 갱신 하진 않음
}


void CPuzzleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int lmax = 300 + 3 * 5;
	int emprow, empcol;
	BOOL b;

	// 힌트 그림 클릭시
	if ((point.x > 400 && point.x < 600) && (point.y > 24 && point.y < 259))
	{
		// hint그림 클릭 시 
		KillTimer(1);
		m_bhit = TRUE;
		HintView();
		SetTimer(1, 5000, NULL);
	}

	// 힌트그림 부분외에 다른 부분 클릭 시 함수종료
	if (point.x<10 || point.x>lmax || point.y<10 || point.y>lmax)
		return;

	point.x -= 10;
	point.y -= 10;
	int x =point.x/ 63;			// 배열 인덱스
	int y =point.y/ 63;

	// 선택한 인덱스에서 같은 열 혹은 같은 행에 빈칸이 있나 검사=>있다면 emprow, empcol로 반환
	b = FindEptyIndex(y, x, emprow, empcol);
	if (b)
	{
		MovePic(y, x, emprow, empcol);		// [emprow, empcol]부터 [y, x]의 네모칸을 [emprow, empcol]쪽으로 고대로 이동
		SetPercent();		// 네모칸 클릭 시마다 퍼센트 계산
		if (IsEnd())		// 끝났으면 
			EndGame();
	}

	if (start)	// 시작 버튼 눌렀으면
	{
		click++;		// 클릭할때마다 클릭 횟수 세기
		m_click.Format(_T("%d %s"), click, _T("번"));
		UpdateData(FALSE);
	}
}

/// <summary>
/// hint그림 보이기
/// </summary>

void CPuzzleDlg::HintView()
{
	// TODO: 여기에 구현 코드 추가.
	CClientDC dc(this);
	CDC mem;

	CBitmap* old;
	mem.CreateCompatibleDC(&dc);
	if (m_bhit)
		old = mem.SelectObject(&m_bm);
	else
		old = mem.SelectObject(&m_hit);
	dc.StretchBlt(400, 24, 200, 235, &mem, 0, 0, 300, 300, SRCCOPY);
	mem.SelectObject(old);
}

/// <summary>
/// 칸을 구분할 수 있도록 선 그리기
/// </summary>

void CPuzzleDlg::DrawLine()
{
	// TODO: 여기에 구현 코드 추가.
	CClientDC dc(this);
	int isize = 300 / 5;
	CPen pen, * old = NULL;
	pen.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_WINDOWFRAME));
	// 가로선 그리기
	for (int i = 0; i < 6; i++)
	{
		dc.MoveTo(8, 8 + i * 63);
		dc.LineTo(8+3*4+2*2+5*isize, 8 + i * 63);
	}
	// 세로선 그리기
	for (int i = 0; i < 6; i++)
	{
		dc.MoveTo(8 + i * 63, 8);
		dc.LineTo( 8 + i * 63, 8 + 3 * 4 + 2 * 2 + 5 * isize);
	}
	dc.SelectObject(old);
}

/// <summary>
/// 빈칸 색칠
/// </summary>
/// <param name="iCol"></param>
/// <param name="iRow"></param>

void CPuzzleDlg::ClearView(int iCol, int iRow)
{
	// TODO: 여기에 구현 코드 추가.
	CClientDC dc(this);
	int isize = 300 / 5;
	CPen pen, * pold = NULL;
	CBrush br, * bold = NULL;

	pen.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DFACE));
	br.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	pold = dc.SelectObject(&pen);
	bold = dc.SelectObject(&br);

	dc.Rectangle(10 + (iCol * (isize + 3)), 10 + (iRow * (isize + 3)), 10 + (iCol * (isize + 3)) + 60, 10 + (iRow * (isize + 3)) + 60);
	dc.SelectObject(pold);
	dc.SelectObject(bold);
}

/// <summary>
/// iRow, iCol의 같은 행 or 열에 빈칸 있는지 없는지
/// </summary>
/// <param name="iRow"></param>
/// <param name="iCol"></param>
/// <param name="iEmptyRow"></param>
/// <param name="iEmptyCol"></param>
/// <returns></returns>

BOOL CPuzzleDlg::FindEptyIndex(int iRow, int iCol, int& iEmptyRow, int& iEmptyCol)
{
	// TODO: 여기에 구현 코드 추가.
	// 세로검사
	for (int i = 0; i < 5; i++)
	{
		if (m_game[i][iCol] == -1)
		{
			iEmptyCol = iCol;
			iEmptyRow = i;
			return TRUE;
		}
	}
	// 가로 검사
	for (int i = 0; i < 5; i++)
	{
		if (m_game[iRow][i] == -1)
		{
			iEmptyCol = i;
			iEmptyRow = iRow;
			return TRUE;
		}
	}
	return FALSE;
}

/// <summary>
/// [emprow, empcol]부터 [y, x]의 네모칸을 [emprow, empcol]쪽으로 고대로 이동
/// </summary>
/// <param name="iRow"></param>
/// <param name="iCol"></param>
/// <param name="iEmptyRos"></param>
/// <param name="iEmptyCol"></param>

void CPuzzleDlg::MovePic(int iRow, int iCol, int iEmptyRos, int iEmptyCol)
{
	// TODO: 여기에 구현 코드 추가.
	int ver = iRow - iEmptyRos;
	int hor = iCol - iEmptyCol;
	// 사진과 빈칸을 바꾸고 바꾼 사진을 보여줌
	if (ver > 0)			// 왼쪽 빈칸 
	{
		for (int i = iEmptyRos; i < iRow; i++)
		{
			m_game[i][iCol] = m_game[i + 1][iCol];				// 아이콘 네모칸의 인덱스 값 한칸씩 이동
			ViewPic(iCol,i, m_game[i + 1][iCol]);			// 인덱스 값의 네모칸을 화면 위치로 아이콘 그림 한칸씩 이동
		}
	}
	if (ver < 0)			// 오른쪽 빈칸
	{
		for (int i = iEmptyRos; i > iRow; i--)
		{
			m_game[i][iCol] = m_game[i - 1][iCol];
			ViewPic(iCol, i,  m_game[i - 1][iCol]);
		}
	}
	if (hor > 0)		// 위 빈칸
	{
		for (int i = iEmptyCol; i < iCol; i++)
		{
			m_game[iRow][i] = m_game[iRow][i+1];
			ViewPic(i, iRow, m_game[iRow][i + 1]);
		}
	}
	if (hor < 0)		// 아래 빈칸
	{
		for (int i = iEmptyCol; i > iCol; i--)
		{
			m_game[iRow][i] = m_game[iRow][i - 1];
			ViewPic(i, iRow, m_game[iRow][i - 1]);
		}
	}
	// 클릭한 칸은 지우고 공백의 칸 보여줌
	m_game[iRow][iCol] = -1;
	ClearView(iCol, iRow);
}


/// <summary>
/// 빈칸을 찾아 랜덤으로 뽑은 iRow, iCol에서 빈칸으로 이동=>1000번 반복해 랜덤하게 섞음
/// </summary>

void CPuzzleDlg::RndPic()
{
	// TODO: 여기에 구현 코드 추가.
	int iRow, iCol, iEmptyRow, iEmptyCol;
	bool b;
	srand((int)time(NULL));

	for (int i = 0; i < 1000; i++)
	{
		iRow = rand() % 5;
		iCol = rand() % 5;

		b = FindEptyIndex(iRow, iCol, iEmptyRow, iEmptyCol);
		if (b)
			MovePic(iRow, iCol, iEmptyRow, iEmptyCol);			// iRow, iCol의 것을 빈칸으로 이동시킴
		if (i % 100 == 0)
			Sleep(10);
	}
}

/// <summary>
/// 시작 버튼 누를때 그림 섞고 set
/// </summary>

void CPuzzleDlg::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	start = 1;
	m_click.Format(_T("0"));
	DrawLine();
	RndPic();
	m_bhit = FALSE;
	SetPercent();
	Invalidate(TRUE);
	KillTimer(0);
	KillTimer(1);
	SetTimer(0, 1000, NULL);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			ViewPic(j, i, m_game[i][j]);
		}
	}
	end = 0;
}

/// <summary>
/// 다 맞췄나 검사
/// </summary>
/// <returns></returns>

BOOL CPuzzleDlg::IsEnd()
{
	// TODO: 여기에 구현 코드 추가.
	int i, j;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (i == 4 && j == 4)
			{
				if (m_game[i][j] != -1)
				{
					return FALSE;
				}
			}
			else if (m_game[i][j] != i * 5 + j)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}


void CPuzzleDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	IsEnd();
	CDialogEx::OnOK();
}


BOOL CPuzzleDlg::PosToIndex(CPoint pnt, int& iRow, int& iCol)
{
	// TODO: 여기에 구현 코드 추가.
	// 클릭한 좌표를 배열의 인덱스로 변경
	int i, j;

	for (i = 0; i < 5; i++)
	{
		if (pnt.x > 10 + i * 63 && pnt.x < 10 + (i + 1) * 63 - 3)
			break;
	}

	for (j = 0; j < 5; j++)
	{
		if (pnt.y > 10 + j * 63 && pnt.y < 10 + (j + 1) * 63 - 3)
			break;
	}
	if (i == 5 && j == 5)
		return FALSE;
	else
	{
		iRow = j;
		iCol = i;
		return TRUE;
	}
	return 0;
}

/// <summary>
/// 사용자가 이동시킨 결과의 인덱스 값이 올바른 인덱스값과 몇퍼 일치하는지 네모칸 누를때마다 계산
/// </summary>

void CPuzzleDlg::SetPercent()
{
	// TODO: 여기에 구현 코드 추가.
	int per = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (m_game[i][j] == i * 5 + j)		// i * 5 + j  =>올바른 인덱스값
				per++;
			if (i == 4 && j == 4)
			{
				if(m_game[i][j]==-1)
					per++;
			}
		}
	}
	int cent = (double)per / 25 * 100;
	m_per.Format(_T("%d %c"), cent,'%');
	UpdateData(FALSE);

}

/// <summary>
/// 다 맞췄으면 게임 종료
/// </summary>

void CPuzzleDlg::EndGame()
{
	// TODO: 여기에 구현 코드 추가.
	int i, j;
	start = 0;
	for (i = 0; i < 5; i++)
	{
		for (j = 0;j < 5; j++)
		{
			ClearView(i, j);
			Sleep(100);
		}
	}
	end = 1;
	KillTimer(0);
	KillTimer(1);
	Invalidate(TRUE);
}

/// <summary>
/// timer set
/// </summary>
/// <param name="nIDEvent"></param>

void CPuzzleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 0)
	{
		m_time.Format(_T("%d sec"), m_Time++);
		UpdateData(FALSE);
	}
	if (nIDEvent == 1)		// hint 그만 보이기
	{
		m_bhit = FALSE;
		HintView();
		KillTimer(1);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CPuzzleDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 타이머 죽이기
	KillTimer(0);
	KillTimer(1);

	CDialogEx::OnOK();
}
